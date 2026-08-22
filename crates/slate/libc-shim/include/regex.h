#ifndef _REGEX_H
#define _REGEX_H

#include <features.h>

#define __NEED_regoff_t
#define __NEED_size_t
#include <bits/types.h>

typedef struct re_pattern_buffer {
  size_t re_nsub;
  void  *__opaque, *__padding[4];
  size_t __nsub2;
  char   __padding2;
} regex_t;

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

enum {
  REG_EXTENDED = 1,
  REG_ICASE    = 2,
  REG_NEWLINE  = 4,
  REG_NOSUB    = 8,
};

enum {
  REG_NOTBOL = 1,
  REG_NOTEOL = 2,
};

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
