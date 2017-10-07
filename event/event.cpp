#include "event.hpp"
#include <sys/reg.h>
#include <iostream>
#include <cassert>
using namespace std;

namespace dltrace {

    Event::Event(TimeEx &time, Process &process, EVENTTYPE type):m_createTime(time), m_process(process), m_type(type) {}

    void Event::doBreakPointEvent() {
        cout << "breakpoint event" << endl;
        assert(!m_funcMsgs.empty());
        FuncMsg funcMsg = m_funcMsgs.top();
        BreakPointSptr bp = funcMsg.getBreakPoint();
        m_process.setRegister(RIP, bp->getAddr());
        bp->disable(m_process.getPid());
        string inOrOut = funcMsg.getType() == FuncMsg::FUNCMSGTYPE::FUNCIN ? "in" : "out";
        while(!m_funcMsgs.empty()) {
            funcMsg = m_funcMsgs.top();
            cout << funcMsg.getFuncName() << " " << inOrOut << endl;
            m_funcMsgs.pop();            
        };
        m_process.singleStep(bp);
    }

    void Event::doExitedEvent() {
        cout << "exit event" << endl;
    }

    void Event::doExitSignaledEvent() {
        cout << "exit signaled event" << endl;
    }    

    void Event::doSignaledEvent() {
        cout << "signaled event" << endl;
    }

    void Event::doCloneEvent() {
        cout << "clone event" << endl;        
    }

    void Event::doProcessStopEvent() {
        cout << "process stop event" << endl;        
    }    

    void Event::doSingleStepEvent() {
        cout << "single step event" << endl;
    }

    Event::EVENTTYPE Event::getType() const {
        return m_type;
    }    

    void Event::setCreateTime(struct timeval time) {
        m_createTime = time;
    }
    
    TimeEx Event::getCreateTime() const {
        return m_createTime;
    }

    void Event::setFuncMsgs(const stack<FuncMsg> &funcMsgs) {
        m_funcMsgs = funcMsgs;
    }

    void Event::setFuncMsgs(stack<FuncMsg> &&funcMsgs) {
        m_funcMsgs = funcMsgs;
    }

    const stack<FuncMsg>& Event::getFuncMsgs() const {
        return m_funcMsgs;
    }

    void Event::setNewPid(pid_t pid) {
        m_newPid = pid;
    }

    pid_t Event::getNewPid() const {
        return m_newPid;
    }

    void Event::doEvent() {
        cout << "pid:" << dec << m_process.getPid() << endl;
        switch(m_type) {
            case EVENTTYPE::BREAKPOINT:
                doBreakPointEvent();
                break;
            case EVENTTYPE::EXITED:
                doExitedEvent();
                break;
            case EVENTTYPE::SIGNALED:
                doSignaledEvent();
                break;
            case EVENTTYPE::CLONE:
                doCloneEvent();
                break;
            case EVENTTYPE::PROCESSSTOP:
                doProcessStopEvent();
                break;
            case EVENTTYPE::SINGLESTEP:
                doSingleStepEvent();
                break;
            default:
                cout <<ERROR_START<< "unknow event type. what happened." << ERROR_END<<endl;
                break;
        }
    }

}