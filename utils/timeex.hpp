#ifndef _DLTRACE_TIMEEX_H_
#define _DLTRACE_TIMEEX_H_


#include <sys/time.h>
#include "util.hpp"

namespace dltrace {

    class TimeEx {
    private:
        struct timeval m_time;

    CONSTRUCTOR:
        TimeEx();
        TimeEx(struct timeval&);
    DESTRUCTOR:

    public:
        void getCurrentTime();
        void setTime(struct timeval&);
        struct timeval getTime() const;
        TimeEx operator-(const TimeEx& x);
        TimeEx operator+(const TimeEx& x);
        TimeEx& operator+=(const TimeEx& x);
        TimeEx& operator-=(const TimeEx& x);
    };
}


#endif /* _DLTRACE_TIMEEX_H_ */