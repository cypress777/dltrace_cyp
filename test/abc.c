#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void* threadFunc(void * args) {
    char *str;
    str = (char *)malloc(23);
    memset(str, 'a', 22);
    str[22] = 0;
    printf("%s\n", str);
	fflush(stdout);
    free(str);
}

int main() {
    int n = 5;
    while(n--) {
        pthread_t tid;
        pthread_create(&tid, NULL, threadFunc, NULL);
    }

    while(wait(NULL) > 0);

sleep(1);

    return 0;
}
