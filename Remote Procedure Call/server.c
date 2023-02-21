#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>

#define PROG_NUM 0x23451111
#define VERS_NUM 1
#define PROC_NUM 1

int *add_1_svc(int *argp, struct svc_req *rqstp)
{
    static int result;

    result = argp[0] + argp[1];
    return &result;
}

int main(int argc, char *argv[])
{
    SVCXPRT *transp;

    pmap_unset(PROG_NUM, VERS_NUM);
    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        fprintf(stderr, "cannot create udp service.\n");
        exit(1);
    }
    if (!svc_register(transp, PROG_NUM, VERS_NUM, add_1_svc, IPPROTO_UDP)) {
        fprintf(stderr, "unable to register (PROG_NUM, VERS_NUM, udp).\n");
        exit(1);
    }
    svc_run();
    fprintf(stderr, "unable to run RPC server.\n");
    exit(1);
}
