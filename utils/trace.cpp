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
        //enable function's entry breakpoints in user's program.
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
        /*
         * 1.set tracee and tracer;
         * 2.initiate functions' entry breakpoints in user's programs;
         *   note that: library entry breakpoints would be initiate after main function is loaded;
         *              all function's out breakpoints would be enabled until we stopped at it's entry.
         */
        if(!m_initialized)
            initTrace();

        //funciton run time = back - front - delay;
        TimeEx frontTime, backTime;

        /*
         * By defination, receiving any SIGTRAP indecates a process stopped at function entry/end breakpoint.
         * However sometimes tracee will receive a SIGEGV or SIGILL at breakpoint instead.
         */
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
            //process exited
            m_processes.erase(pProcess);
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::EXITED);
        }
        if(WIFSIGNALED(status)) {
            //process exited by signal
            m_processes.erase(pProcess);
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::SIGNALED);          
        }
        uint64_t rip = pProcess->getRegister(RIP);
        if(rip < 0) {
            int errnoSave = errno;
            if(!pProcess->isStopped()) {
                //process should be runing but stopped by unknown reason, restart it.
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
                //a new process is cloned
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
                //see more in breakpoint event handler
                cout <<ERROR_START "singlestep but sigstop." ERROR_END<< endl;
                exit(0);
            }
            cout << "stop sig" << endl;
            backTime.getCurrentTime();
            m_delay += backTime - frontTime;
            return Event(frontTime, *pProcess, Event::EVENTTYPE::PROCESSSTOP);
        }

        auto &callStack = pProcess->getCallStack();        

        // make sure we don't miss any breakpoint
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

        /*
         * finally the event could be nothing but breakpoint event
         */
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
             *  Start to enable library breakpoints and load their messages when process is stopped
             *  at "main", considering all library function should be loaded and linked ready now.
             *  Is that so?
             */
            if(funcMsg.getFuncName() == "main") {
                if(!m_libFileNames.empty())
                    addLibEntryFuncMsgs();
            }

            //  push func-end breakpoint's index into process's callstack.
            FuncMsgIndex funcEndIndex;
            FuncMsg funcEndMsg;
            BreakPointSptr pBreakPoint;
            unsigned long endAddr;

            endAddr = pProcess->getData(pProcess->getRegister(RSP));
            funcEndIndex.setAddr(endAddr);
            funcEndIndex.setFuncName(funcInIndex.getFuncName());
            
            callStack.push(funcEndIndex);

            /*
             *  Make sure funcEndIndex refers to a valid funEndMessage.
             *  If the index refers to a message in fun-end message map, then good;
             *  else, create a new message and insert it to map.
             */
            do {
                if(m_funcMsgs.find(funcEndIndex) != m_funcMsgs.end()) {
                    //fun-end message exists.
                    break;
                }

                /* fun-end message does not exist. */

                //get fun-end breakpoint
                if(m_breakPoints.find(endAddr) != m_breakPoints.end()) {
                    //breakpoint already exists.
                    pBreakPoint = m_breakPoints[endAddr];
                }
                else {
                    //It's a new end breakpoint, insert it to breakpoint map
                    pBreakPoint = make_shared<BreakPoint>(endAddr);
                    m_breakPoints[endAddr] = pBreakPoint;
                }

                cout << "enable at ret :" << funcEndIndex.getFuncName() << endl;
                pBreakPoint->enable(pProcess->getPid());
                
                //create fun-end message and insert it to message map
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