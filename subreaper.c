#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
        int status;
        int ret;
        pid_t pid;

        prctl(PR_SET_CHILD_SUBREAPER, 1);

        if (argc <= 1) {
                printf("usage: %s PROG [ARGS]\n", argv[0]);
                exit(1);
        }

        pid = fork();

        if (pid == 0) {
                execvp(argv[1], &argv[1]);
        }

        while (waitpid(pid, &status, 0) < 0 && errno == EINTR) {
                continue;
        }

        if (WIFEXITED(status)) {
                ret = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
                ret = WTERMSIG(status) + 128;
        }

        return ret;
}
