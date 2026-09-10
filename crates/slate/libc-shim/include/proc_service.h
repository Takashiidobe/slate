#ifndef _SLATE_PROC_SERVICE_H
#define _SLATE_PROC_SERVICE_H

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/procfs.h>

typedef enum {
  PS_OK,
  PS_ERR,
  PS_BADPID,
  PS_BADLID,
  PS_BADADDR,
  PS_NOSYM,
  PS_NOFREGS
} ps_err_e;

struct ps_prochandle;

ps_err_e ps_pdread(struct ps_prochandle *, psaddr_t, void *, size_t);
ps_err_e ps_pdwrite(struct ps_prochandle *, psaddr_t, const void *, size_t);
ps_err_e ps_ptread(struct ps_prochandle *, psaddr_t, void *, size_t);
ps_err_e ps_ptwrite(struct ps_prochandle *, psaddr_t, const void *, size_t);
ps_err_e ps_lgetregs(struct ps_prochandle *, lwpid_t, prgregset_t);
ps_err_e ps_lsetregs(struct ps_prochandle *, lwpid_t, const prgregset_t);
ps_err_e ps_lgetfpregs(struct ps_prochandle *, lwpid_t, prfpregset_t *);
ps_err_e ps_lsetfpregs(struct ps_prochandle *, lwpid_t, const prfpregset_t *);
pid_t ps_getpid(struct ps_prochandle *);
ps_err_e ps_get_thread_area(struct ps_prochandle *, lwpid_t, int, psaddr_t *);
ps_err_e ps_pglobal_lookup(struct ps_prochandle *, const char *, const char *, psaddr_t *);
ps_err_e ps_pstop(struct ps_prochandle *);
ps_err_e ps_pcontinue(struct ps_prochandle *);
ps_err_e ps_lstop(struct ps_prochandle *, lwpid_t);
ps_err_e ps_lcontinue(struct ps_prochandle *, lwpid_t);
#endif

#endif
