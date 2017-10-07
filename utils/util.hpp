#ifndef _DLTRACE_UTIL_H_
#define _DLTRACE_UTIL_H_

#define _implements public
#define _interface class
#define CONSTRUCTOR public
#define DESTRUCTOR public

#define NEWFAILEDMSG(_ptr)  \
{   \
    if(_ptr == nullptr) {   \
        cout << "new failed" << endl;   \
    }   \
}


#define ERROR_START "\033[31m"
#define ERROR_END "\033[0m"
#endif /* _DLTRACE_UTIL_H_ */