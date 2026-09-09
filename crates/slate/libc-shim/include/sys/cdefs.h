#ifndef _SLATE_SYS_CDEFS_H
#define _SLATE_SYS_CDEFS_H

#if defined(__SLATE_LIBC_GLIBC)
#define __BEGIN_DECLS
#define __END_DECLS
#define __THROW
#define __THROWNL
#define __NTH(fct) fct
#define __NTHNL(fct) fct
#define __nonnull(params)
#define __wur
#define __attribute_malloc__
#define __attribute_alloc_size__(params)
#define __attribute_const__
#define __attribute_pure__
#define __attribute_deprecated__
#define __attribute_format_arg__(arg)
#define __attribute_artificial__
#define __attribute_warn_unused_result__
#define __always_inline inline
#define __extern_always_inline extern inline
#define __fortify_function __extern_always_inline
#define __glibc_likely(cond) (cond)
#define __glibc_unlikely(cond) (cond)
#define __GNUC_PREREQ(maj, min) 0
#define __GLIBC_PREREQ(maj, min) 0
#define __ASMNAME(cname) cname
#define __REDIRECT(name, proto, alias) name proto
#define __REDIRECT_NTH(name, proto, alias) name proto
#define __REDIR(name, alias)
#endif

#endif
