#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char write_buf[1];
  char read_buf[1];
  int first[2];
  int second[2];
  int pid;

  pipe(first);
  pipe(second);
  write_buf[1] = 'a';

  if (fork() == 0) {
    close(first[0]);
    close(second[1]);

    write(first[1], write_buf, 1);
    close(first[1]);
    
    read(second[0], read_buf, 1);
    close(second[0]);

    pid = getpid();
    printf("%d: recieved pong");

    exit(0);
  }


  else {
    close(second[0]);
    close(first[1]);

    read(first[0], read_buf, 1);
    close(first[0]);

    pid = getpid();
    printf("%d: recieved ping");

    write(second[0], write_buf, 1);
    close(second[0]);

    exit(0);
  }
}
