#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_sieve(int read_pipe) {
    int p;
    int n, pid, ret;

    ret = read(read_pipe, &p, sizeof(p));

    if (ret == 0) {
        return;
    }

    int write_pipe[2];
    pipe(write_pipe);
    printf("prime %d\n", p);

    ret = read(read_pipe, &n, sizeof(n));

    while (ret != 0) {
        if (n % p != 0) {
            write(write_pipe[1], &n, sizeof(n));
        }
        ret = read(read_pipe, &n, sizeof(n));
    }

    close(read_pipe);
    close(write_pipe[1]);

    pid = fork();
    if (pid == 0) {
        prime_sieve(write_pipe[0]);
    }

    else {
        wait(0);
        close(write_pipe[0]);
    }

    return;
}

int main(int argc, char *argv[]) {
    int i, n;

    if (argc != 1) {
        printf("wrong usage");
        exit(1);
    }

    int first_pipe[2];
    pipe(first_pipe);

    for (i = 2; i <= 35; i++) {
        n = i;
        write(first_pipe[1], &n, sizeof(i));
    }
    
    close(first_pipe[1]);
    prime_sieve(first_pipe[0]);
    close(first_pipe[0]);

    exit(0);
}
