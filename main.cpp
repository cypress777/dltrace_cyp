#include "record.hpp"
#include <iostream>
using namespace std;
using namespace dltrace;

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "should print help." << endl;
        return 0;
    }
    cout << "main" << endl;
    // program name and libname are passed to rec
    Record rec(argc - 1, argv + 1);

    rec.startWork();
    
    return 0;
}