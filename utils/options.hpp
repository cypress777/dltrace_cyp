#ifndef _DLTRACE_OPTIONS_H_
#define _DLTRACE_OPTIONS_H_


#include <stdint.h>
#include <vector>
#include <string>
#include "util.hpp"

namespace dltrace {

    _interface Options {
    protected:
        uint16_t m_flags; //参数位掩码

    CONSTRUCTOR:
    DESTRUCTOR:
        virtual ~Options() {}

    public:
        virtual void parseOptions(int arc, char **argv) = 0;   
    };

}

#endif /* _DLTRACE_OPTIONS_H_ */