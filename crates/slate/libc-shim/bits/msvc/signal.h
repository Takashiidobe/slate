#ifndef _SLATE_BITS_MSVC_SIGNAL_H
#define _SLATE_BITS_MSVC_SIGNAL_H

typedef int  sig_atomic_t;
typedef void (*_crt_signal_t)(int);

#define NSIG 23

#define SIGINT         2
#define SIGILL         4
#define SIGFPE         8
#define SIGSEGV        11
#define SIGTERM        15
#define SIGBREAK       21
#define SIGABRT        22
#define SIGABRT_COMPAT 6

#define SIG_DFL ((_crt_signal_t)0)
#define SIG_IGN ((_crt_signal_t)1)
#define SIG_GET ((_crt_signal_t)2)
#define SIG_SGE ((_crt_signal_t)3)
#define SIG_ACK ((_crt_signal_t)4)
#define SIG_ERR ((_crt_signal_t) - 1)

#endif
