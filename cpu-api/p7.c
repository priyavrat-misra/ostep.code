#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int child_func(void *arg) {
    printf("hello, I am child (pid:%d)\n", (int) getpid());
    return EXIT_SUCCESS;
}

int main() {
    const int STACK_SIZE = 1024 * 1024;
    char *stack = malloc(STACK_SIZE); // 1MB

    if (!stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int child_pid = clone(child_func, stack + STACK_SIZE, SIGCHLD, NULL);
    if (child_pid == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    int wc = wait(NULL);
    printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", child_pid, wc, (int) getpid());

    free(stack);
    return EXIT_SUCCESS;
}
