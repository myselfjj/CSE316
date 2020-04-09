/*This is a final part of the code revision*/


#include <stdlib.h>
#include <ckpt.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>
#include <stdio.h>
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
    printf("CKP%d, type %s,to dir%s \n",
        id, ckpt_type_str(CKPT_REAL_TYPE(type)), path_name);
    if ((cr = ckpt_create(path_name, id, type, 0, 0)) != 0) {
        printf("Failure to the checkpoint process %ld \n", id);
        return (cr);
    }
    return (0);
}

do_restart(char *path_n)
{
    printf("Restarting remaining processes %s \n", path_n);
    if (ckpt_restart(path_n, 0, 0) < 0) {
        printf("Restarting %s failedn", path_n);
        return (-1);
    }
}

ckpt_info(char *path_n)
{
    ckpt_stat_t *kp, *kp_next;
    int cr;
    if ((cr = ckpt_stat(path_n, &kp)) != 0) 
	{
        printf("Can't get the info. %s \n", path_n);
        return (cr);
    }
    printf("nInformation About Statefile %s (%s):\n",
        path, rev_to_str(kp->cs_revision));
    while (kp) {
        printf(" Process:\t \t %s \n", kp->cs_psargs);
        printf(" PID,PPID:\t \t %d, %d \n", kp->cs_pid, kp->cs_ppid);
        printf(" PGRP,SID:\t \t %d, %d \n", kp->cs_pgrp, kp->cs_sid);
        printf(" Working at dir:\t %s \n", kp->cs_cdir);
        printf(" Num of Openfiles:\t %d \n", kp->cs_nfiles);
        printf(" Checkpointed at %s \n", ctime(&kp->cs_stat.st_mtime));
        kp_next = kp->cs_next;
        free(kp);
        kp = kp_next;
    }
    return (0);
}

do_remove(char *path_name)
{
    int cr = 0;
    if ((cr = ckpt_remove(path)) != 0) {
        printf("Remove checkpoint statefile %s failedn", path_name);
        return (cr);
    }
}

int ckpt_setup(struct ckpt_args *args[], size_t nargs)
{
    return(0);
}

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
            perror("Cannot setup checkpoint and restart handling");
    exit(0);
}
}
