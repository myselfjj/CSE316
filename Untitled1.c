#include <stdlib.h>
#include <ckpt.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>
#include <stdio.h>
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

do_restart(char *path_n)
{
    printf("Restarting remaining processes %s n", path_n);
    if (ckpt_restart(path_n, 0, 0) < 0) {
        printf("Restarting %s failedn", path_n);
        return (-1);
    }
}

ckpt_info(char *path)
{
    ckpt_stat_t *sp, *sp_next;
    int cr;
    if ((cr = ckpt_stat(path, &sp)) != 0) 
	{
        printf("Not able to get info about CPR file %s \n", path);
        return (cr);
    }
    printf("nInformation About Statefile %s (%s):\n",
        path, rev_to_str(sp->cs_revision));
    while (sp) {
        printf(" Process:tt%sn", sp->cs_psargs);
        printf(" PID,PPID:tt%d,%dn", sp->cs_pid, sp->cs_ppid);
        printf(" PGRP,SID:tt%d,%dn", sp->cs_pgrp, sp->cs_sid);
        printf(" Working at dir:t%sn", sp->cs_cdir);
        printf(" Num of Openfiles:t%dn", sp->cs_nfiles);
        printf(" Checkpointed @t%sn", ctime(&sp->cs_stat.st_mtime));
        sp_next = sp->cs_next;
        free(sp);
        sp = sp_next;
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
