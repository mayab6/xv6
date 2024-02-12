#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char parent_buf[1];
  char child_buff[1];
  int first[2];
  int second[2];
  int pid;

  pipe(first);
  pipe(second);
  parent_buf[1] = 'a';
  child_buff[1] = 'a';

  pid = fork();

  if (pid < 0) {
    printf("error in fork");
  }

  else if (pid > 0) {

    close(first[0]);
    close(second[1]);

    write(first[1], parent_buf, 1);
    close(first[1]);
    
    read(second[0], parent_buf, 1);
    close(second[0]);

    printf("%d: recieved pong\n", getpid());
  }

  else {

    close(first[1]);
    close(second[0]);

    read(first[0], child_buff, 1);
    close(first[0]);

    printf("%d: recieved ping\n",getpid());

    write(second[1], child_buff, 1);
    close(second[1]);
  }

  exit(0);
}
