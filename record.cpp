#include "record.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
#include <sys/reg.h>
using namespace std;

namespace dltrace {

    Record::Record(int argc, char **argv) : m_trace(nullptr) {
        parseOptions(argc, argv);
        m_trace = new Trace(m_testFileName, argv, m_libFileNames);
        NEWFAILEDMSG(m_trace)
    }
    
    Record::~Record() {
        if(m_trace != nullptr)
            delete m_trace;
    }

    void Record::submitEvent(Event *event) {
        m_eventQueue.push(event);
    }

    void Record::handleEvent(Event *event) {
        auto leader = event->getProcess()->getLeader();
        auto trace = event->getTrace();

        auto retEvent = event->doEvent();      
        
        if(retEvent != nullptr) {
            submitEvent(retEvent);
        }

        auto memOpProcess = leader->getMemOperatingProcess();
        auto breakPoint = leader->getSingleStepBreakPoint();
        auto state = leader->m_memOpState;

        switch(state) {
            case Process::MEMOPSTATE::NONE:
                break;
            case Process::MEMOPSTATE::DISABLING_BREAKPOINT:
                cout << ERROR_START << dec << memOpProcess->getPid() << " disabling" << ERROR_END << endl;            
                if(trace->isEveryOneStopped()) {
                    breakPoint->disable(memOpProcess->getPid());
                    leader->m_memOpState = Process::MEMOPSTATE::DISABLED_BREAKPOINT;
                    goto disabled_breakpoint;
                }
                break;
            disabled_breakpoint:
            case Process::MEMOPSTATE::DISABLED_BREAKPOINT:
                cout << ERROR_START << dec << memOpProcess->getPid() << " disabled" << ERROR_END << endl;            
                if(memOpProcess->m_isSigStopped) {
                    if(!memOpProcess->m_waitingForSigStop)
                        memOpProcess->continueToRun();                    
                }
                else {
                    leader->m_memOpState = Process::MEMOPSTATE::SINGLESTEPPING;                        
                    goto singlestepping;
                }
                break;
            singlestepping:
            case Process::MEMOPSTATE::SINGLESTEPPING:
                cout << ERROR_START << dec << memOpProcess->getPid() << " singlestepping" << ERROR_END << endl;            
                memOpProcess->setRegister(RIP, breakPoint->getAddr());
                if(memOpProcess->singleStep())
                    leader->m_memOpState = Process::MEMOPSTATE::SINGLESTEPPED;
                break;
            case Process::MEMOPSTATE::SINGLESTEPPED:
                break;
        }
    }

    void Record::handleEventInQueue() {
        while(!m_eventQueue.empty()) {
            auto it = m_eventQueue.front();
            if(it->doEvent() != nullptr) {
                break;
            }
            m_eventQueue.pop();
        }
    }

    const char* Record::getTestFileName() const{
        return m_testFileName;
    }

    const vector<string>& Record::getLibFileNames() const{
        return m_libFileNames;
    }

    void Record::parseOptions(int argc, char **argv) {
        cout << "parse options." << endl;
        if(argc == 0) {
            cout << "should print help." << endl;
            exit(0);
        }
        
        int i = 0;
        if(strcmp(argv[i], "-l") == 0) {
            i = 1;
            m_flags |= TRACELIB;
        }

        for(; i < argc - 1; i++) 
            m_libFileNames.push_back(argv[i]);

        m_testFileName = argv[i];
        m_argv = argv + i;
    }

    void Record::startWork() {
        cout << "start work." << endl;
        do {
            handleEventInQueue();
            auto event = m_trace->traceEvent();
            if(event == nullptr) {
                cout << "trace complete." << endl;
                break;
            }
            handleEvent(event);
        } while(true);
    }    

}