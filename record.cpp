#include "record.hpp"
#include <iostream>
#include <cassert>
#include <cstring>
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

    void Record::submitEvent(Event &event) {
        m_eventQueue.push(event);
    }

    /*
     * Basically we gonna handle events in queue one by one, and then PTRACE_CONT them all at once
     *
     * Handling a breakpoint event requires disable bp, singlestep, and then enable bp. Note that we can
     * make sure singlestep is successed only after a SIGTRAP is received. So do-event set
     * m_issinglestep = true then breaks the loop after singlestep and the tracer waits for SIGTRAP in trace-event.
     *
     * Aftrer receiving SIGTRAP in trace-event, we will enable breakpoint and set issinglestep = false in trace-event.
     * And submit a SINGLESTEP event to queue.
     */
    void Record::handleEvent() {
        assert(m_eventQueue.empty());
        cout<<m_eventQueue.size()<<" "<<m_trace->getProcesses().size()<<endl;

        //singlestep is successed only after SIGTRAP is received.
        if(m_trace->isAnyProcessSingleSteping())
            return;

        if(!m_isAllProcessesStopped) {
            if (m_eventQueue.size() < m_trace->getProcesses().size())
                return;
            else {
                m_isAllProcessesStopped = true;
            }
        }
        while(!m_eventQueue.empty()) {

            Event event =m_eventQueue.front();
            m_eventQueue.pop();
            event.doEvent();
            if(event.getType()==Event::EVENTTYPE::BREAKPOINT)
            //break the loop to wait SIGTRAP
            {
                break;
            }
        }

        if(m_isAllProcessesStopped && !m_trace->isAnyProcessSingleSteping() && m_eventQueue.empty())
        {
            m_isAllProcessesStopped=false;
            m_trace->continueAllProcess();
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
           /*
            * start tracing after initializaitions (load symboltable, enable breakpoints).
            * when a event(clone/fork, syscalls, exiting, breakpoints, etc.) happended to any thread,
            * we stop the thread and submit the event to handler.
            */
            auto event = m_trace->traceEvent();
            //submitting events
            if(event.getType() == Event::EVENTTYPE::COMPLETE) {
                cout << "trace complete." << endl;
                break;
            }
            if(event.getType() != Event::EVENTTYPE::NONE)
                //push event to event queue
                submitEvent(event);
            if(event.getType() == Event::EVENTTYPE::WTHP) {
                cout << "what happened event..." << endl;
            }
            /*
             * Only start to handle events when ALL process are STOPPED,
             * otherwise we just push the event into event queue then wait for all processes to stop.
             * However when a breakpoint event happend, tracer will send SIGSTOP to all processes forcing them to stop.
             */
            TimeEx frontTime, backTime;
            frontTime.getCurrentTime();
            handleEvent();
            backTime.getCurrentTime();
            m_trace->addDelay(backTime - frontTime);
        } while(true);
    }    

}