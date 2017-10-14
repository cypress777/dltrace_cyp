#include "record.hpp"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace dltrace;

int main(int argc, char **argv) {
    //close(1);
    if(argc < 2) {
        cout << "should print help." << endl;
        return 0;
    }
    cout << "main" << endl;
    Record rec(argc - 1, argv + 1);

    rec.startWork();
    
    return 0;
}