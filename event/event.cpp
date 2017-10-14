#include "event.hpp"

using namespace std;

namespace dltrace {

    Event::Event(TimeEx time, Process *process, Trace *trace, TYPE type):m_createTime(time), m_type(type), m_process(process), m_trace(trace) 
    {

    }

    Event::~Event() 
    {

    }

    TimeEx Event::getCreateTime() const {
        return m_createTime;
    }

    Event::TYPE Event::getType() const {
        return m_type;
    }

    Process* Event::getProcess() const {
        return m_process;
    }

    Trace* Event::getTrace() const {
        return m_trace;
    }    

}