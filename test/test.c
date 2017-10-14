#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int func(int a) {
    if(!a) return 1;
    return func(a-1) + 1;
}

void malloc_test()
{
    char *str;
    str = (char *)malloc(23);
    memset(str, 'a', 22);
    str[22] = 0;
    printf("%s\n", str);
    fflush(stdout);
    free(str);
}

int main() {
    func(3);
    for(int i=0;i<10;i++)
    {
        malloc_test();
    }
    return 0;
}
