/*This is a initial part of the code other parts will be added in next revision*/


#include <stdlib.h>
#include <ckpt.h>
extern void ckptSocket(void);
extern void ckptXserver(void);
extern void restartSocket(void);
extern void restartXserver(void);
extern char *cdFile;
extern FILE fpCD;
extern char *cdFile;
extern FILE fpCD;
extern long cdOffset;
long cdOffset;
extern int sock; 
catchRESTART()
{
    while (access("/CDROM/data", R_OK) == -1) {
        perror("Insert CDROM");
        sleep(60);
    }
    if ((fCD= fopen&#40;cdFile, "r"&#41;) == NULL)
        perror("can't open CDFile"), exit(1);
    if (fseek(fCD, cdOffset, SEEK_SET))
        perror("can't seek to CDOffset"), exit(1);
}
catchCKPT()
{
    alarm(60);
    close(sock);
    alarm(0);
    cdOffset = ftell(fCD);
    fclose(fCD);
    umount("/CDROM");
    exit(0);
}

static int

do_checkpoint(ckpt_id_t id, u_long type, char *path_name)
{
    int cr;
    printf("CKP%d, type %s,to dir%s n",
        id, ckpt_type_str(CKPT_REAL_TYPE(type)), path_name);
    if ((cr = ckpt_create(path_name, id, type, 0, 0)) != 0) {
        printf("Failed to checkpointing process %lldn", id);
        return (cr); 
    }
    return (0);
}

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
