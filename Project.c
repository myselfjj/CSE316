#include <stdlib.h>
#include <ckpt.h>
extern void ckptSocket(void);
extern void ckptXserver(void);
extern void restartSocket(void);
extern void restartXserver(void);
main(int argc, char *argv[])
{
    int err = 0;
    if ((atcheckpoint(ckptSocket) == -1) ||
        (atcheckpoint(ckptXserver) == -1) ||
        (atrestart(restartSocket) == -1) ||
        (atrestart(restartXserver) == -1))
            perror("Can't setup checkpoint and restart handling");
    
    exit(0);
}
