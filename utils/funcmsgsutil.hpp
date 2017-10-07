#ifndef _DLTRACE_BREAKPOINTSUTIL_H_
#define _DLTRACE_BREAKPOINTSUTIL_H_

#include "util.hpp"
#include "funcmsg.hpp"
#include "process.hpp"
#include <string>
#include <cstdint>
#include <unordered_map>

namespace dltrace {

    class FuncMsgsUtil {

    CONSTRUCTOR:
        FuncMsgsUtil() = delete;

    public:
        static bool loadEntryFuncMsgs(const std::string&, FuncMsgMap&,std::unordered_map<unsigned long, BreakPointSptr>&, size_t offset = 0);
    };

}

#endif /* _DLTRACE_BREAKPOINTSUTIL_H_ */