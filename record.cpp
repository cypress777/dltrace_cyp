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

    void Record::handleEvent() {
        assert(m_eventQueue.size() != 0);
        cout<<m_eventQueue.size()<<" "<<m_trace->getProcesses().size()<<endl;

/*        if(!m_trace->isAnyProcessSingleSteping())
        {
            m_isAllProcessesStopped=false;
        }*/

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
            {
                //m_isAllProcessesStopped=true;
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
            //跟踪事件trace.traceEvent();
            auto event = m_trace->traceEvent();
            //提交事件submitEvent();
            if(event.getType() == Event::EVENTTYPE::COMPLETE) {
                cout << "trace complete." << endl;
                break;
            }
            if(event.getType() != Event::EVENTTYPE::NONE)
                submitEvent(event);
            if(event.getType() == Event::EVENTTYPE::WTHP) {
                cout << "what happened event..." << endl;
            }
            //处理事件handleEvent();
            TimeEx frontTime, backTime;
            frontTime.getCurrentTime();
            handleEvent();
            backTime.getCurrentTime();
            m_trace->addDelay(backTime - frontTime);
        } while(true);
    }    

}