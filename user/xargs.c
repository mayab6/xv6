# include  "kernel/types.h"
# include  "user/user.h"
# include  "kernel/param.h"

#define MAX_BUFF 256

int main(int argc, char *argv[]) {
    /* initialize variables */
    char *cmdargs[MAXARG] = { 0 };
    char buff[MAX_BUFF];
    int i, xargc, bufflen, check, reached_end, pid;
    char *arg_beginning, *curr_char;

    i = xargc = 0;
    check = 1;

    /* check correct usage */
    if (argc < 2) {
        printf("xargs: wrong usage\n");
        exit(1);
    }

    /* initializes commands from argv */
    for (i = 1; i < argc; i++) {
        cmdargs[i - 1] = argv[i];
    }


    while (check) {
        xargc = argc - 1;

        /* recieve command line and check length */
        gets(buff, sizeof(buff));
        bufflen = strlen(buff);

        /* reched EOF */
        if (bufflen == 0) {
            check = 0;
            break;
        }

        /* changes newline to \0 for execution */
        if (buff[bufflen - 1] == '\n' || buff[bufflen - 1] == '\r') {
            buff[bufflen - 1] = '\0';
            bufflen--;
        }


        arg_beginning = curr_char = buff;
        reached_end = 0;

        /* iterates over lines, separates commands by ' ' or '\t' */
        while (!reached_end)
        {
            if (*curr_char == ' ' || *curr_char == '\t' || *curr_char == '\0') {
                cmdargs[xargc] = arg_beginning;
                xargc++;

                if (*curr_char == '\0') {
                    reached_end = 1;
                    break;
                }

                arg_beginning = curr_char + 1;
                *curr_char = '\0';
            }

            curr_char++;
        }
        /* mark end of commands */

        cmdargs[xargc] = 0;

        /* fork and execute command*/
        pid = fork();

        if (pid != 0) {
            wait(0);
        }
        else {

            exec(cmdargs[0], cmdargs);
            /* if reached here, exec failed */
            printf("exec ");
            for (i = 0; i < xargc; i++) {
                printf("%s, ", cmdargs[i]);
            }
            printf("\n");
            exit(-1);
        }
    }

    exit(0);
}