#include "timeex.hpp"

namespace dltrace {


    TimeEx::TimeEx():m_time{0,0} {}

    TimeEx::TimeEx(struct timeval& time):m_time(time) {}

    void TimeEx::getCurrentTime() {
        gettimeofday(&m_time, 0);
    }    

    void TimeEx::setTime(struct timeval& time) {
        m_time = time;
    }
    
    struct timeval TimeEx::getTime() const {
        return m_time;
    }

    TimeEx TimeEx::operator-(const TimeEx& x) {
        TimeEx ret;
        int sign = 1;
        if (m_time.tv_sec < x.m_time.tv_sec || (m_time.tv_sec == x.m_time.tv_sec && m_time.tv_usec < x.m_time.tv_usec))
            sign = -1;

        ret.m_time.tv_sec = (m_time.tv_sec - x.m_time.tv_sec) * sign;
        ret.m_time.tv_usec = (m_time.tv_usec - x.m_time.tv_usec) * sign;
        while (ret.m_time.tv_usec < 0) {
            ret.m_time.tv_sec -= 1;
            ret.m_time.tv_usec += 1000000;
        }
        ret.m_time.tv_sec *= sign;
        ret.m_time.tv_usec *= sign;
        return ret;
    }

    TimeEx TimeEx::operator+(const TimeEx& x) {
        TimeEx ret;
        ret.m_time.tv_sec = m_time.tv_sec - x.m_time.tv_sec;
        ret.m_time.tv_usec = m_time.tv_usec - x.m_time.tv_usec;
        while (ret.m_time.tv_usec > 1000000) {
            ret.m_time.tv_usec -= 1000000;
            ret.m_time.tv_sec += 1;
        }
        return ret;
    }

    TimeEx& TimeEx::operator+=(const TimeEx& x) {
        m_time.tv_sec += x.m_time.tv_sec;
        m_time.tv_usec -= x.m_time.tv_usec;
        while (m_time.tv_usec > 1000000) {
            m_time.tv_usec -= 1000000;
            m_time.tv_sec += 1;
        }
        return *this;
    }

    TimeEx& TimeEx::operator-=(const TimeEx& x) {
        int sign = 1;
        if (m_time.tv_sec < x.m_time.tv_sec || (m_time.tv_sec == x.m_time.tv_sec && m_time.tv_usec < x.m_time.tv_usec))
            sign = -1;

        m_time.tv_sec -= x.m_time.tv_sec;
        m_time.tv_usec -= x.m_time.tv_usec;
        while (m_time.tv_usec < 0) {
            m_time.tv_sec -= 1;
            m_time.tv_usec += 1000000;
        }
        
        m_time.tv_sec *= sign;
        m_time.tv_usec *= sign;
        return *this;
    }

}