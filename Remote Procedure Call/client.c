#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>

#define PROG_NUM 0x23451111
#define VERS_NUM 1
#define PROC_NUM 1

int main(int argc, char *argv[])
{
    CLIENT *cl;
    int *result;
    int arg1, arg2;

    if (argc != 3) {
        fprintf(stderr, "usage: %s arg1 arg2\n", argv[0]);
        exit(1);
    }
    arg1 = atoi(argv[1]);
    arg2 = atoi(argv[2]);

    cl = clnt_create(argv[1], PROG_NUM, VERS_NUM, "udp");
    if (cl == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }
    result = add_1(&arg1, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        exit(1);
    }
    printf("result: %d\n", *result);
    clnt_destroy(cl);
    return 0;
}
