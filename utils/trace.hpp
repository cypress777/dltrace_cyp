#ifndef _DLTRACE_TRACE_H_
#define _DLTRACE_TRACE_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <list>
#include "funcmsg.hpp"
#include "process.hpp"
#include "../event/event.hpp"
#include "timeex.hpp"

namespace dltrace {

    class Trace {
    private:
        bool m_initialized;
        TimeEx m_delay;
        std::string m_testFileName;
        char * const *m_argv;
        std::vector<std::string> m_libFileNames;
        FuncMsgMap m_funcMsgs;
        std::unordered_map<unsigned long, BreakPointSptr> m_breakPoints;
        std::unordered_map<unsigned long, std::string> m_entryFuncNames;
        std::list<Process> m_processes;

    CONSTRUCTOR:
        Trace(std::string, char * const *, std::vector<std::string>&);
    DESTRUCTOR:
        
    private:
        void initTrace();
        void setTraceOptions(const Process&);
        void getPtraceEventMessage(const Process&, long*);
        void addEntryFuncMsgs();
        void addLibEntryFuncMsgs();
        void enableBreakPoints() const;
        void addEntryFuncNames();
    public:
        const std::list<Process>& getProcesses() const;
        void addDelay(TimeEx);
        void continueAllProcess();
        void stopAllProcess();
        bool isAnyProcessSingleSteping();
        Event traceEvent();
    };

}


#endif /* _DLTRACE_TRACE_H_ */