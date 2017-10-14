#ifndef _DLTRACE_RECORD_H_
#define _DLTRACE_RECORD_H_

#include "./utils/options.hpp"
#include "./utils/util.hpp"
#include "./event/eventhandler.hpp"
#include "./utils/trace.hpp"

namespace dltrace {

    class Record:_implements Options, _implements EventHandler {
        #define TRACELIB (1<<15)
    private:
        std::vector<std::string> m_libFileNames;
        const char *m_testFileName;
        const char * const *m_argv;
        Trace *m_trace;

    CONSTRUCTOR:
        Record(int argc, char **argv);
    DESTRUCTOR:
        ~Record();

    protected:
        /* 
            Parse options for record,
            called in constructor
        */ 
        virtual void parseOptions(int , char **) override;  
        virtual void submitEvent(Event*) override;
        virtual void handleEvent(Event*) override;
        virtual void handleEventInQueue() override;        
    
    public:
        const char* getTestFileName() const;
        const std::vector<std::string>& getLibFileNames() const;
        void startWork();

    };
    
}

#endif /* _DLTRACE_RECORD_H_ */