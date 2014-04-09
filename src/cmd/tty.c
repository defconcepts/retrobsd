/*
 * Type tty name
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main(argc, argv)
char **argv;
{
    register char *p;

    p = ttyname(0);
    if(argc==2 && !strcmp(argv[1], "-s"))
        ;
    else
        printf("%s\n", (p? p: "not a tty"));
    exit(p? 0: 1);
}
