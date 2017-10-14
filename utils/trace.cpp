#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <cassert>
#include <algorithm>
#include "trace.hpp"
#include "funcmsgsutil.hpp"
#include "../event/breakpointevent.hpp"
#include "../event/cloneevent.hpp"
#include "../event/exitedevent.hpp"
#include "../event/newprocessevent.hpp"
#include "../event/signaledevent.hpp"
#include "../event/signalexitedevent.hpp"
#include "../event/singlesteppedevent.hpp"
#include "../event/ignoreevent.hpp"

using namespace std;

namespace dltrace {

    Trace::Trace(string testFileName, char * const *argv, std::vector<std::string> &libFileNames)
        :m_initialized(false), m_testFileName(testFileName), m_argv(argv), m_libFileNames(libFileNames) {}    

    void Trace::initTrace() {
        cout << "init trace." << endl;
        pid_t pid = fork();
        Process process(pid);

        if(pid == 0) {
            process.traceMe();
            execvpe(m_testFileName.c_str(), m_argv, nullptr);
            cout <<ERROR_START  << "execv failed." <<ERROR_END<< endl;
            exit(1);
        }

        if(pid < 0) {
            cout <<ERROR_START "fork failed." ERROR_END << endl;
            exit(1);
        }

        if(waitpid(pid, nullptr, __WALL) != pid) {
            cout <<ERROR_START "waitpid failed" ERROR_END << endl;
            exit(1);
        }
        cout << "first wait." << endl;
        setTraceOptions(process);
        m_processes.push_back(process);
        {
            auto tmp = m_processes.begin();
            tmp->setLeader(&*tmp);
        }
        addEntryFuncMsgs();
        m_delay.getCurrentTime();
        process.continueToRun();
        m_initialized = true;
    }

    void Trace::setTraceOptions(const Process &process) {
        ptrace(PTRACE_SETOPTIONS, process.getPid(), 0, PTRACE_O_TRACECLONE);
    }

    void Trace::getPtraceEventMessage(const Process& process, long *pData) {
        ptrace(PTRACE_GETEVENTMSG, process.getPid(), NULL, (void*)pData);
    }

    void Trace::addEntryFuncMsgs() {
        cout << "add entry breakpoints." << endl;
        assert(!m_processes.empty());
        auto &process = m_processes.front();
        size_t offset = process.getExecOffset(m_testFileName);
        FuncMsgsUtil::loadEntryFuncMsgs(m_testFileName, m_funcMsgs,m_breakPoints, offset);
        enableBreakPoints();
        addEntryFuncNames();
    }

    void Trace::addLibEntryFuncMsgs() {
        cout << "add lib entry breakpoints." << endl;
        assert(!m_processes.empty());
        auto &process = m_processes.front();
        for(auto libName : m_libFileNames) {
            auto offset = process.getLibOffset(libName);
            cout << ERROR_START << "OFFSET :" << hex << offset << ERROR_END << endl;
            FuncMsgsUtil::loadEntryFuncMsgs(libName, m_funcMsgs,m_breakPoints, offset);
            enableBreakPoints();
        }
        addEntryFuncNames();
    }

    void Trace::enableBreakPoints() const{
        cout << "enable breakpoints." << endl;
        assert(!m_processes.empty());
        auto &process = m_processes.front();
        for(auto funcMsg : m_funcMsgs) {
            if(funcMsg.second.getType() == FuncMsg::FUNCMSGTYPE::FUNCIN)
                funcMsg.second.getBreakPoint()->enable(process.getPid());
        }
    }

    void Trace::addEntryFuncNames() {
        for(auto it : m_funcMsgs) {
            if(it.second.getType() == FuncMsg::FUNCMSGTYPE::FUNCIN) {
                m_entryFuncNames[it.first.getAddr()] = it.first.getFuncName();
            }
        }
    }    

    std::list<Process>& Trace::getProcesses() {
        return m_processes;
    }    

    void Trace::addDelay(TimeEx time) {
        m_delay += time;
    }    

    void Trace::continueAllProcess() {
        for(auto &process : m_processes) {
            process.continueToRun();
        }
    }

    void Trace::stopAllProcess() {
        for(auto &process : m_processes) {
            process.stopProcess();
        }
    }

    bool Trace::isEveryOneStopped() {
        for(auto it : m_processes) {
            if(!it.isStopped())
                return false;
        }
        return true;
    }

    Event* Trace::traceEvent() {
        if(!m_initialized)
            initTrace();

        TimeEx frontTime, backTime;
        int status;
        auto pid = waitpid(-1, &status, __WALL);
        frontTime.getCurrentTime();
        if(pid == 0) {
            cout << ERROR_START << "pid = 0" << ERROR_END << endl;
            return nullptr;
        }
        if(pid < 0) {
            cout << "no process to trace" << endl;
            //trace complete.
            return nullptr;
        }

        auto pProcess = find(m_processes.begin(), m_processes.end(), Process(pid));
        assert(pProcess != m_processes.end());
        auto leader = pProcess->getLeader();

        if(pProcess->m_isNew) {
            //new process event.
            return new NewProcessEvent(frontTime, &*pProcess, this);
        }

        if(WIFEXITED(status)) {
            //exited.
            return new ExitedEvent(frontTime, &*pProcess, this);
        }
        if(WIFSIGNALED(status)) {
            //signal exited.
            return new SignalExitedEvent(frontTime, &*pProcess, this);
        }

        //maybe process not stopped.
        uint64_t rip = pProcess->getRegister(RIP);
        if(rip < 0) {
            int errnoSave = errno;
            if(!pProcess->isStopped()) {
                cout << ERROR_START "process not stopped, is it terminating?" ERROR_END << endl;
                pProcess->continueToRun();  
                backTime.getCurrentTime();
                m_delay += backTime - frontTime;
                //should continue process.
                return new IgnoreEvent(frontTime, &*pProcess, this);
            }
            errno = errnoSave;
            if(errno != 0) {
                cout << ERROR_START "get rip error." ERROR_END<< endl;
            }
        }

        if(WIFSTOPPED(status)) {
            auto what = status >> 16;
            if(what == PTRACE_EVENT_CLONE) {
                return new CloneEvent(frontTime, &*pProcess, this);
            }
        }

        if(!WIFSTOPPED(status)) {
            //should never happen.
            return new IgnoreEvent(frontTime, &*pProcess, this);
        }

        auto stopSig = WSTOPSIG(status);
        cout << "pid:" << dec << pid << "rip:" << hex << rip << endl;
        rip -= 1;

        auto &callStack = pProcess->getCallStack();        

        if((stopSig == SIGSEGV || stopSig == SIGILL) && 
            (m_entryFuncNames.find(rip) != m_entryFuncNames.end() ||
            (!callStack.empty() && callStack.top().getAddr() == rip))
        )
            stopSig = SIGTRAP;

        if(stopSig != SIGTRAP) {
            //signal event
            cout << dec << stopSig << endl;
            return new SignaledEvent(frontTime, &*pProcess, this, stopSig);
        }

        if(leader->getMemOperatingProcess() == &*pProcess) {
            //singlestep event.
            return new SingleSteppedEvent(frontTime, &*pProcess, this);
        }

        /*breakpoint event*/
        return new BreakPointEvent(frontTime, &*pProcess, this);
    }
}
