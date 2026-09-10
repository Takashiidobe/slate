#ifndef _REGEX_H
#define _REGEX_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/types.h>
#endif

#define __NEED_regoff_t
#define __NEED_size_t
#if defined(__SLATE_LIBC_MUSL)
#define REG_EXTENDED 1
#define REG_ICASE 2
#define REG_NEWLINE 4
#define REG_NOSUB 8
#define REG_NOTBOL 1
#define REG_NOTEOL 2
#define REG_OK 0
#define REG_NOMATCH 1
#define REG_BADPAT 2
#define REG_ECOLLATE 3
#define REG_ECTYPE 4
#define REG_EESCAPE 5
#define REG_ESUBREG 6
#define REG_EBRACK 7
#define REG_EPAREN 8
#define REG_EBRACE 9
#define REG_BADBR 10
#define REG_ERANGE 11
#define REG_ESPACE 12
#define REG_BADRPT 13
#define REG_ENOSYS -1
#endif
#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/regex.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/regex.h>

#else

#if defined(__SLATE_LIBC_GLIBC)

typedef struct re_pattern_buffer {
  void          *__buffer;
  unsigned long  __allocated;
  unsigned long  __used;
  unsigned long  __syntax;
  char          *__fastmap;
  unsigned char *__translate;
  size_t         re_nsub;
  unsigned       __regex_flags;
} regex_t;

#else

typedef struct re_pattern_buffer {
  size_t re_nsub;
  void  *__opaque, *__padding[4];
  size_t __nsub2;
  char   __padding2;
} regex_t;

#endif

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

#if !defined(__SLATE_LIBC_MUSL)
enum {
  REG_EXTENDED = 1,
  REG_ICASE    = 2,
  REG_NEWLINE  = 4,
  REG_NOSUB    = 8,
};
#endif

#if !defined(__SLATE_LIBC_MUSL)
enum {
  REG_NOTBOL = 1,
  REG_NOTEOL = 2,
};
#endif

#if !defined(__SLATE_LIBC_MUSL)
enum {
  REG_OK       = 0,
  REG_NOMATCH  = 1,
  REG_BADPAT   = 2,
  REG_ECOLLATE = 3,
  REG_ECTYPE   = 4,
  REG_EESCAPE  = 5,
  REG_ESUBREG  = 6,
  REG_EBRACK   = 7,
  REG_EPAREN   = 8,
  REG_EBRACE   = 9,
  REG_BADBR    = 10,
  REG_ERANGE   = 11,
  REG_ESPACE   = 12,
  REG_BADRPT   = 13,
  REG_ENOSYS   = -1,
};
#endif

#if defined(__SLATE_LIBC_GLIBC)
#define REGS_FIXED REGS_FIXED
#define REGS_REALLOCATE REGS_REALLOCATE
#define REGS_UNALLOCATED REGS_UNALLOCATED
#define REG_BADBR REG_BADBR
#define REG_BADPAT REG_BADPAT
#define REG_BADRPT REG_BADRPT
#define REG_EBRACE REG_EBRACE
#define REG_EBRACK REG_EBRACK
#define REG_ECOLLATE REG_ECOLLATE
#define REG_ECTYPE REG_ECTYPE
#define REG_EEND REG_EEND
#define REG_EESCAPE REG_EESCAPE
#define REG_ENOSYS REG_ENOSYS
#define REG_EPAREN REG_EPAREN
#define REG_ERANGE REG_ERANGE
#define REG_ERPAREN REG_ERPAREN
#define REG_ESIZE REG_ESIZE
#define REG_ESPACE REG_ESPACE
#define REG_ESUBREG REG_ESUBREG
#define REG_EXTENDED REG_EXTENDED
#define REG_ICASE REG_ICASE
#define REG_NEWLINE REG_NEWLINE
#define REG_NOERROR REG_NOERROR
#define REG_NOMATCH REG_NOMATCH
#define REG_NOSUB REG_NOSUB
#define REG_NOTBOL REG_NOTBOL
#define REG_NOTEOL REG_NOTEOL
#define REG_STARTEND REG_STARTEND
#define RE_DUP_MAX RE_DUP_MAX
#define RE_NREGS RE_NREGS
#define RE_SYNTAX_AWK RE_SYNTAX_AWK
#define RE_SYNTAX_ED RE_SYNTAX_ED
#define RE_SYNTAX_EGREP RE_SYNTAX_EGREP
#define RE_SYNTAX_GNU_AWK RE_SYNTAX_GNU_AWK
#define RE_SYNTAX_GREP RE_SYNTAX_GREP
#define RE_SYNTAX_POSIX_AWK RE_SYNTAX_POSIX_AWK
#define RE_SYNTAX_POSIX_EGREP RE_SYNTAX_POSIX_EGREP
#define RE_SYNTAX_POSIX_MINIMAL_BASIC RE_SYNTAX_POSIX_MINIMAL_BASIC
#define RE_SYNTAX_POSIX_MINIMAL_EXTENDED RE_SYNTAX_POSIX_MINIMAL_EXTENDED
#define RE_SYNTAX_SED RE_SYNTAX_SED
#define RE_TRANSLATE_TYPE RE_TRANSLATE_TYPE
#define RE_BACKSLASH_ESCAPE_IN_LISTS RE_BACKSLASH_ESCAPE_IN_LISTS
#define RE_BK_PLUS_QM RE_BK_PLUS_QM
#define RE_CARET_ANCHORS_HERE RE_CARET_ANCHORS_HERE
#define RE_CHAR_CLASSES RE_CHAR_CLASSES
#define RE_CONTEXT_INDEP_ANCHORS RE_CONTEXT_INDEP_ANCHORS
#define RE_CONTEXT_INDEP_OPS RE_CONTEXT_INDEP_OPS
#define RE_CONTEXT_INVALID_DUP RE_CONTEXT_INVALID_DUP
#define RE_CONTEXT_INVALID_OPS RE_CONTEXT_INVALID_OPS
#define RE_DEBUG RE_DEBUG
#define RE_DOT_NEWLINE RE_DOT_NEWLINE
#define RE_DOT_NOT_NULL RE_DOT_NOT_NULL
#define RE_HAT_LISTS_NOT_NEWLINE RE_HAT_LISTS_NOT_NEWLINE
#define RE_INTERVALS RE_INTERVALS
#define RE_INVALID_INTERVAL_ORD RE_INVALID_INTERVAL_ORD
#define RE_LIMITED_OPS RE_LIMITED_OPS
#define RE_NEWLINE_ALT RE_NEWLINE_ALT
#define RE_NO_BK_BRACES RE_NO_BK_BRACES
#define RE_NO_BK_PARENS RE_NO_BK_PARENS
#define RE_NO_BK_REFS RE_NO_BK_REFS
#define RE_NO_BK_VBAR RE_NO_BK_VBAR
#define RE_NO_EMPTY_RANGES RE_NO_EMPTY_RANGES
#define RE_NO_GNU_OPS RE_NO_GNU_OPS
#define RE_NO_POSIX_BACKTRACKING RE_NO_POSIX_BACKTRACKING
#define RE_NO_SUB RE_NO_SUB
#endif

int  regcomp(regex_t *__restrict, const char *__restrict, int);
int  regexec(const regex_t *__restrict, const char *__restrict, size_t,
             regmatch_t *__restrict, int);
void regfree(regex_t *);

size_t regerror(int, const regex_t *__restrict, char *__restrict, size_t);

#if defined(_GNU_SOURCE)
typedef unsigned long int reg_syntax_t;

#define RE_BACKSLASH_ESCAPE_IN_LISTS 1ul
#define RE_BK_PLUS_QM (RE_BACKSLASH_ESCAPE_IN_LISTS << 1)
#define RE_CHAR_CLASSES (RE_BK_PLUS_QM << 1)
#define RE_CONTEXT_INDEP_ANCHORS (RE_CHAR_CLASSES << 1)
#define RE_CONTEXT_INDEP_OPS (RE_CONTEXT_INDEP_ANCHORS << 1)
#define RE_CONTEXT_INVALID_OPS (RE_CONTEXT_INDEP_OPS << 1)
#define RE_DOT_NEWLINE (RE_CONTEXT_INVALID_OPS << 1)
#define RE_DOT_NOT_NULL (RE_DOT_NEWLINE << 1)
#define RE_HAT_LISTS_NOT_NEWLINE (RE_DOT_NOT_NULL << 1)
#define RE_INTERVALS (RE_HAT_LISTS_NOT_NEWLINE << 1)
#define RE_LIMITED_OPS (RE_INTERVALS << 1)
#define RE_NEWLINE_ALT (RE_LIMITED_OPS << 1)
#define RE_NO_BK_BRACES (RE_NEWLINE_ALT << 1)
#define RE_NO_BK_PARENS (RE_NO_BK_BRACES << 1)
#define RE_NO_BK_REFS (RE_NO_BK_PARENS << 1)
#define RE_NO_BK_VBAR (RE_NO_BK_REFS << 1)
#define RE_NO_EMPTY_RANGES (RE_NO_BK_VBAR << 1)
#define RE_UNMATCHED_RIGHT_PAREN_ORD (RE_NO_EMPTY_RANGES << 1)
#define RE_NO_POSIX_BACKTRACKING (RE_UNMATCHED_RIGHT_PAREN_ORD << 1)
#define RE_NO_GNU_OPS (RE_NO_POSIX_BACKTRACKING << 1)
#define RE_DEBUG (RE_NO_GNU_OPS << 1)
#define RE_INVALID_INTERVAL_ORD (RE_DEBUG << 1)
#define RE_ICASE (RE_INVALID_INTERVAL_ORD << 1)
#define RE_CARET_ANCHORS_HERE (RE_ICASE << 1)
#define RE_CONTEXT_INVALID_DUP (RE_CARET_ANCHORS_HERE << 1)
#define RE_NO_SUB (RE_CONTEXT_INVALID_DUP << 1)

#define RE_SYNTAX_EMACS 0

#define _RE_SYNTAX_POSIX_COMMON                                              \
  (RE_CHAR_CLASSES | RE_DOT_NEWLINE | RE_DOT_NOT_NULL | RE_INTERVALS |       \
   RE_NO_EMPTY_RANGES)

#define RE_SYNTAX_POSIX_BASIC                                                \
  (_RE_SYNTAX_POSIX_COMMON | RE_BK_PLUS_QM | RE_CONTEXT_INVALID_DUP)

#define RE_SYNTAX_POSIX_EXTENDED                                             \
  (_RE_SYNTAX_POSIX_COMMON | RE_CONTEXT_INDEP_ANCHORS |                      \
   RE_CONTEXT_INDEP_OPS | RE_NO_BK_BRACES | RE_NO_BK_PARENS |                \
   RE_NO_BK_VBAR | RE_CONTEXT_INVALID_OPS | RE_UNMATCHED_RIGHT_PAREN_ORD)

extern reg_syntax_t re_syntax_options;

reg_syntax_t re_set_syntax(reg_syntax_t);
const char  *re_compile_pattern(const char *__restrict, size_t,
                                regex_t *__restrict);
regoff_t     re_match(regex_t *__restrict, const char *__restrict, size_t,
                      regoff_t, void *__restrict);
regoff_t     re_search(regex_t *__restrict, const char *__restrict, size_t,
                       regoff_t, regoff_t, void *__restrict);
#endif

#endif 

#endif
