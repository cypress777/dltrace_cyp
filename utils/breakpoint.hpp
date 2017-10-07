#ifndef _DLTRACE_BREAKPOINT_H_
#define _DLTRACE_BREAKPOINT_H_

#include <memory>
#include <stdint.h>
#include <sys/types.h>
#include "util.hpp"

namespace dltrace {

    class BreakPoint {
    private:
        unsigned long m_addr;
        uint8_t m_data;

    CONSTRUCTOR:
        BreakPoint();
        explicit BreakPoint(unsigned long addr);
    DESTRUCTOR:

    public:
        unsigned long getAddr() const;
        void enable(pid_t);
        void disable(pid_t);
    };

    typedef std::shared_ptr<BreakPoint> BreakPointSptr;

}

#endif /* _DLTRACE_BREAKPOINT_H_ */