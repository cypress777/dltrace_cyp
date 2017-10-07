#include "trace.hpp"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <cassert>
#include <algorithm>
#include "funcmsgsutil.hpp"

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
            execv(m_testFileName.c_str(), m_argv);
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
        addEntryFuncMsgs();
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
        FuncMsgsUtil::loadEntryFuncMsgs(m_testFileName, m_funcMsgs,m_breakPoints);
        enableBreakPoints();
        addEntryFuncNames();        
    }

    void Trace::addLibEntryFuncMsgs() {
        cout << "add lib entry breakpoints." << endl;
        assert(!m_processes.empty());
        auto &process = m_processes.front();
        for(auto libName : m_libFileNames) {
            auto offset = process.getLibOffset(libName);
            cout << offset << endl;
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

    const std::list<Process>& Trace::getProcesses() const {
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

    bool Trace::isAnyProcessSingleSteping() {
        for(auto it : m_processes) {
            if(it.isSingleStep())
                return true;
        }
        return false;
    }

    Event Trace::traceEvent() {
        cout << "trace event" << endl;
        if(!m_initialized)
            initTrace();

        TimeEx frontTime, backTime;
        int status;
        auto pid = waitpid(-1, &status, __WALL);
        frontTime.getCurrentTime();
        if(pid < 0) {
            cout << "no process to trace" << endl;
            Process tmp = pid;
            return Event(frontTime, tmp, Event::EVENTTYPE::COMPLETE);
        }
        auto pProcess = find(m_processes.begin(), m_processes.end(), Process(pid));
        assert(pProcess != m_processes.end());
        if(WIFEXITED(status)) {
            m_processes.erase(pProcess);
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::EXITED);
        }
        if(WIFSIGNALED(status)) {
            m_processes.erase(pProcess);
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::SIGNALED);          
        }
        uint64_t rip = pProcess->getRegister(RIP);
        if(rip < 0) {
            int errnoSave = errno;
            if(!pProcess->isStopped()) {

                cout << ERROR_START "process not stopped, is it terminating?" ERROR_END << endl;
                pProcess->continueToRun();  
                backTime.getCurrentTime();
                m_delay += backTime - frontTime;
                return Event(frontTime, *pProcess, Event::EVENTTYPE::NONE);
            }
            errno = errnoSave;
            if(errno != 0) {
                cout << ERROR_START "get rip error." ERROR_END<< endl;
            }
        }

        if(WIFSTOPPED(status)) {
            auto what = status >> 16;
            if(what == PTRACE_EVENT_CLONE) {
                long newPid;
                getPtraceEventMessage(*pProcess, &newPid);
                cout << "clone : pid:" << dec << newPid << endl;
                Event event(frontTime, *pProcess, Event::EVENTTYPE::CLONE);
                event.setNewPid(newPid);
                Process newProcess(newPid);
                m_processes.push_back(newProcess);  
                backTime.getCurrentTime();
                m_delay += backTime - frontTime;
                return event;
            }
        }

        if(!WIFSTOPPED(status)) {
            //should never happen.
            cout <<ERROR_START "should never happen. what happened!" ERROR_END<< endl;
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::WTHP);
        }

        auto stopSig = WSTOPSIG(status);
        cout << "pid:" << dec << pid << "rip:" << hex << rip << endl;
        rip -= 1;        

        if(stopSig == SIGSTOP) {
            if(pProcess->isSingleStep()) {
                cout <<ERROR_START "singlestep but sigstop." ERROR_END<< endl;
                exit(0);
            }
            cout << "stop sig" << endl;
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::PROCESSSTOP);
        }

        auto &callStack = pProcess->getCallStack();        

        if((stopSig == SIGSEGV || stopSig == SIGILL) && 
            (m_entryFuncNames.find(rip) != m_entryFuncNames.end() ||
            (!callStack.empty() && callStack.top().getAddr() == rip))
        )
            stopSig = SIGTRAP;

        if(pProcess->isSingleStep()) {
            pProcess->afterSingleStep();
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::SINGLESTEP);
        }

        if(stopSig != SIGTRAP) {
            //TODO:
            cout <<ERROR_START "unhandled sig:" << dec << stopSig<<ERROR_END <<endl;
            exit(0);
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::SIGNALED);
        }

        

        /*breakpoint event*/

        Event event(frontTime, *pProcess, Event::EVENTTYPE::BREAKPOINT);
        //see description in event.hpp to find why use stack here.
        stack<FuncMsg> funcMsgStack;

        //is this a func-in trap
        if(m_entryFuncNames.find(rip) != m_entryFuncNames.end()) {
            /* func-in trap */
            FuncMsgIndex funcInIndex;
            FuncMsg funcMsg;

            //set func-in msg for event.
            funcInIndex.setAddr(rip);
            funcInIndex.setFuncName(m_entryFuncNames[rip]);
            assert(m_funcMsgs.find(funcInIndex) != m_funcMsgs.end());
            funcMsg = m_funcMsgs[funcInIndex];
            funcMsgStack.push(funcMsg);

            cout << "IN:func name:" << funcInIndex.getFuncName()<<" addr:"  << funcInIndex.getAddr() << endl;

            /*
                start to load lib entries function messages when stopped at 
                function's entry whoes name is main.
                we think it is true that the libs in program is all loaded 
                when program enter function whoes name is main.
                is that so?
            */
            if(funcMsg.getFuncName() == "main") {
                if(!m_libFileNames.empty())
                    addLibEntryFuncMsgs();
            }

            /*
                push func-end-message's index into process's callstack.
            */
            
            FuncMsgIndex funcEndIndex;
            FuncMsg funcEndMsg;
            BreakPointSptr pBreakPoint;
            unsigned long endAddr;

            endAddr = pProcess->getData(pProcess->getRegister(RSP));
            funcEndIndex.setAddr(endAddr);
            funcEndIndex.setFuncName(funcInIndex.getFuncName());
            
            callStack.push(funcEndIndex);

            /*
                make sure funcEndIndex can refer to a func message in map.

                if message is exist, just ignore.
                else, create a message and insert it to map.
            */
            do {
                if(m_funcMsgs.find(funcEndIndex) != m_funcMsgs.end()) {
                    //message is exist.
                    break;
                }

                /* message is not exist. */

                //get breakpoint.
                if(m_breakPoints.find(endAddr) != m_breakPoints.end()) {
                    //breakpoint is exist.
                    pBreakPoint = m_breakPoints[endAddr];
                }
                else {
                    //breakpoint is not exist, just create and insert it to map
                    pBreakPoint = make_shared<BreakPoint>(endAddr);
                    m_breakPoints[endAddr] = pBreakPoint;
                }

                cout << "enable at ret :" << funcEndIndex.getFuncName() << endl;
                pBreakPoint->enable(pProcess->getPid());
                
                //create message and insert it to map
                funcEndMsg.setBreakPoint(pBreakPoint);
                funcEndMsg.setFuncName(funcEndIndex.getFuncName());
                funcEndMsg.setType(FuncMsg::FUNCMSGTYPE::FUNCOUT);
                m_funcMsgs[funcEndIndex] = funcEndMsg;
            }while(0);
            
        }else {
            
            if(!callStack.empty() && callStack.top().getAddr() == rip) {
                //func-end trap.
                while(!callStack.empty()) {
                    FuncMsgIndex index = callStack.top();
                    cout << index.getFuncName() << endl;
                    if(index.getAddr() != rip)
                        break;
                    funcMsgStack.push(m_funcMsgs[index]);
                    callStack.pop();
                }
            }else {
                cout <<ERROR_START "what happened ,can not find breakpoint with rip" ERROR_END<< endl;
                exit(0);
            }
        }
        event.setFuncMsgs(move(funcMsgStack));
        stopAllProcess();
        backTime.getCurrentTime();
        m_delay += backTime - frontTime;
        return event;
    }

}