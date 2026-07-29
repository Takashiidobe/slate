#include <errno.h>
#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>

static _Atomic int worker_ready;
static _Atomic int worker_release;
static _Atomic int worker_done;
static int worker_errno_before;
static int worker_errno_after;

static void print_error(const char *name, int value) {
  printf("%s %d\n", name, value);
}

static int errno_worker(void *argument) {
  int result = *(int *)argument;
  errno = EILSEQ;
  worker_errno_before = errno == EILSEQ;
  atomic_store(&worker_ready, 1);
  while (!atomic_load(&worker_release))
    ;
  worker_errno_after = errno == EILSEQ;
  atomic_store(&worker_done, 1);
  return result;
}

int main(void) {
  print_error("EPERM", EPERM);
  print_error("ENOENT", ENOENT);
  print_error("ESRCH", ESRCH);
  print_error("EINTR", EINTR);
  print_error("EIO", EIO);
  print_error("ENXIO", ENXIO);
  print_error("E2BIG", E2BIG);
  print_error("ENOEXEC", ENOEXEC);
  print_error("EBADF", EBADF);
  print_error("ECHILD", ECHILD);
  print_error("EAGAIN", EAGAIN);
  print_error("ENOMEM", ENOMEM);
  print_error("EACCES", EACCES);
  print_error("EFAULT", EFAULT);
  print_error("ENOTBLK", ENOTBLK);
  print_error("EBUSY", EBUSY);
  print_error("EEXIST", EEXIST);
  print_error("EXDEV", EXDEV);
  print_error("ENODEV", ENODEV);
  print_error("ENOTDIR", ENOTDIR);
  print_error("EISDIR", EISDIR);
  print_error("EINVAL", EINVAL);
  print_error("ENFILE", ENFILE);
  print_error("EMFILE", EMFILE);
  print_error("ENOTTY", ENOTTY);
  print_error("ETXTBSY", ETXTBSY);
  print_error("EFBIG", EFBIG);
  print_error("ENOSPC", ENOSPC);
  print_error("ESPIPE", ESPIPE);
  print_error("EROFS", EROFS);
  print_error("EMLINK", EMLINK);
  print_error("EPIPE", EPIPE);
  print_error("EDOM", EDOM);
  print_error("ERANGE", ERANGE);
  print_error("EDEADLK", EDEADLK);
  print_error("ENAMETOOLONG", ENAMETOOLONG);
  print_error("ENOLCK", ENOLCK);
  print_error("ENOSYS", ENOSYS);
  print_error("ENOTEMPTY", ENOTEMPTY);
  print_error("ELOOP", ELOOP);
  print_error("ENOMSG", ENOMSG);
  print_error("EIDRM", EIDRM);
  print_error("ECHRNG", ECHRNG);
  print_error("EL2NSYNC", EL2NSYNC);
  print_error("EL3HLT", EL3HLT);
  print_error("EL3RST", EL3RST);
  print_error("ELNRNG", ELNRNG);
  print_error("EUNATCH", EUNATCH);
  print_error("ENOCSI", ENOCSI);
  print_error("EL2HLT", EL2HLT);
  print_error("EBADE", EBADE);
  print_error("EBADR", EBADR);
  print_error("EXFULL", EXFULL);
  print_error("ENOANO", ENOANO);
  print_error("EBADRQC", EBADRQC);
  print_error("EBADSLT", EBADSLT);
  print_error("EBFONT", EBFONT);
  print_error("ENOSTR", ENOSTR);
  print_error("ENODATA", ENODATA);
  print_error("ETIME", ETIME);
  print_error("ENOSR", ENOSR);
  print_error("ENONET", ENONET);
  print_error("ENOPKG", ENOPKG);
  print_error("EREMOTE", EREMOTE);
  print_error("ENOLINK", ENOLINK);
  print_error("EADV", EADV);
  print_error("ESRMNT", ESRMNT);
  print_error("ECOMM", ECOMM);
  print_error("EPROTO", EPROTO);
  print_error("EMULTIHOP", EMULTIHOP);
  print_error("EDOTDOT", EDOTDOT);
  print_error("EBADMSG", EBADMSG);
  print_error("EOVERFLOW", EOVERFLOW);
  print_error("ENOTUNIQ", ENOTUNIQ);
  print_error("EBADFD", EBADFD);
  print_error("EREMCHG", EREMCHG);
  print_error("ELIBACC", ELIBACC);
  print_error("ELIBBAD", ELIBBAD);
  print_error("ELIBSCN", ELIBSCN);
  print_error("ELIBMAX", ELIBMAX);
  print_error("ELIBEXEC", ELIBEXEC);
  print_error("EILSEQ", EILSEQ);
  print_error("ERESTART", ERESTART);
  print_error("ESTRPIPE", ESTRPIPE);
  print_error("EUSERS", EUSERS);
  print_error("ENOTSOCK", ENOTSOCK);
  print_error("EDESTADDRREQ", EDESTADDRREQ);
  print_error("EMSGSIZE", EMSGSIZE);
  print_error("EPROTOTYPE", EPROTOTYPE);
  print_error("ENOPROTOOPT", ENOPROTOOPT);
  print_error("EPROTONOSUPPORT", EPROTONOSUPPORT);
  print_error("ESOCKTNOSUPPORT", ESOCKTNOSUPPORT);
  print_error("EOPNOTSUPP", EOPNOTSUPP);
  print_error("EPFNOSUPPORT", EPFNOSUPPORT);
  print_error("EAFNOSUPPORT", EAFNOSUPPORT);
  print_error("EADDRINUSE", EADDRINUSE);
  print_error("EADDRNOTAVAIL", EADDRNOTAVAIL);
  print_error("ENETDOWN", ENETDOWN);
  print_error("ENETUNREACH", ENETUNREACH);
  print_error("ENETRESET", ENETRESET);
  print_error("ECONNABORTED", ECONNABORTED);
  print_error("ECONNRESET", ECONNRESET);
  print_error("ENOBUFS", ENOBUFS);
  print_error("EISCONN", EISCONN);
  print_error("ENOTCONN", ENOTCONN);
  print_error("ESHUTDOWN", ESHUTDOWN);
  print_error("ETOOMANYREFS", ETOOMANYREFS);
  print_error("ETIMEDOUT", ETIMEDOUT);
  print_error("ECONNREFUSED", ECONNREFUSED);
  print_error("EHOSTDOWN", EHOSTDOWN);
  print_error("EHOSTUNREACH", EHOSTUNREACH);
  print_error("EALREADY", EALREADY);
  print_error("EINPROGRESS", EINPROGRESS);
  print_error("ESTALE", ESTALE);
  print_error("EUCLEAN", EUCLEAN);
  print_error("ENOTNAM", ENOTNAM);
  print_error("ENAVAIL", ENAVAIL);
  print_error("EISNAM", EISNAM);
  print_error("EREMOTEIO", EREMOTEIO);
  print_error("EDQUOT", EDQUOT);
  print_error("ENOMEDIUM", ENOMEDIUM);
  print_error("EMEDIUMTYPE", EMEDIUMTYPE);
  print_error("ECANCELED", ECANCELED);
  print_error("ENOKEY", ENOKEY);
  print_error("EKEYEXPIRED", EKEYEXPIRED);
  print_error("EKEYREVOKED", EKEYREVOKED);
  print_error("EKEYREJECTED", EKEYREJECTED);
  print_error("EOWNERDEAD", EOWNERDEAD);
  print_error("ENOTRECOVERABLE", ENOTRECOVERABLE);
  print_error("ERFKILL", ERFKILL);
  print_error("EHWPOISON", EHWPOISON);
  print_error("ENOTSUP", ENOTSUP);

  errno = 0;
  int *slot = &errno;
  printf("zero %d\n", errno == 0);
  *slot = EDOM;
  printf("modifiable %d\n", errno == EDOM);

  thrd_t worker;
  int worker_result = 23;
  int joined_result = 0;
  int create_status = thrd_create(&worker, errno_worker, &worker_result);
  while (!atomic_load(&worker_ready))
    ;
  errno = ERANGE;
  atomic_store(&worker_release, 1);
  while (!atomic_load(&worker_done))
    ;
  int main_errno_after = errno == ERANGE;
  int join_status = thrd_join(worker, &joined_result);
  printf("thread %d %d %d %d %d %d\n", create_status == thrd_success,
         worker_errno_before, worker_errno_after, main_errno_after,
         join_status == thrd_success, joined_result == worker_result);
  return 0;
}
