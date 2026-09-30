#include <regex.h>

#ifndef REGS_FIXED
#error "regex.h:REGS_FIXED macro is missing from libc-shim"
#endif

#ifndef REGS_REALLOCATE
#error "regex.h:REGS_REALLOCATE macro is missing from libc-shim"
#endif

#ifndef REGS_UNALLOCATED
#error "regex.h:REGS_UNALLOCATED macro is missing from libc-shim"
#endif

#ifndef REG_BADBR
#error "regex.h:REG_BADBR macro is missing from libc-shim"
#endif

#ifndef REG_BADPAT
#error "regex.h:REG_BADPAT macro is missing from libc-shim"
#endif

#ifndef REG_BADRPT
#error "regex.h:REG_BADRPT macro is missing from libc-shim"
#endif

#ifndef REG_EBRACE
#error "regex.h:REG_EBRACE macro is missing from libc-shim"
#endif

#ifndef REG_EBRACK
#error "regex.h:REG_EBRACK macro is missing from libc-shim"
#endif

#ifndef REG_ECOLLATE
#error "regex.h:REG_ECOLLATE macro is missing from libc-shim"
#endif

#ifndef REG_ECTYPE
#error "regex.h:REG_ECTYPE macro is missing from libc-shim"
#endif

#ifndef REG_EEND
#error "regex.h:REG_EEND macro is missing from libc-shim"
#endif

#ifndef REG_EESCAPE
#error "regex.h:REG_EESCAPE macro is missing from libc-shim"
#endif

#ifndef REG_ENOSYS
#error "regex.h:REG_ENOSYS macro is missing from libc-shim"
#endif

#ifndef REG_EPAREN
#error "regex.h:REG_EPAREN macro is missing from libc-shim"
#endif

#ifndef REG_ERANGE
#error "regex.h:REG_ERANGE macro is missing from libc-shim"
#endif

#ifndef REG_ERPAREN
#error "regex.h:REG_ERPAREN macro is missing from libc-shim"
#endif

#ifndef REG_ESIZE
#error "regex.h:REG_ESIZE macro is missing from libc-shim"
#endif

#ifndef REG_ESPACE
#error "regex.h:REG_ESPACE macro is missing from libc-shim"
#endif

#ifndef REG_ESUBREG
#error "regex.h:REG_ESUBREG macro is missing from libc-shim"
#endif

#ifndef REG_EXTENDED
#error "regex.h:REG_EXTENDED macro is missing from libc-shim"
#endif

#ifndef REG_ICASE
#error "regex.h:REG_ICASE macro is missing from libc-shim"
#endif

#ifndef REG_NEWLINE
#error "regex.h:REG_NEWLINE macro is missing from libc-shim"
#endif

#ifndef REG_NOERROR
#error "regex.h:REG_NOERROR macro is missing from libc-shim"
#endif

#ifndef REG_NOMATCH
#error "regex.h:REG_NOMATCH macro is missing from libc-shim"
#endif

#ifndef REG_NOSUB
#error "regex.h:REG_NOSUB macro is missing from libc-shim"
#endif

#ifndef REG_NOTBOL
#error "regex.h:REG_NOTBOL macro is missing from libc-shim"
#endif

#ifndef REG_NOTEOL
#error "regex.h:REG_NOTEOL macro is missing from libc-shim"
#endif

#ifndef REG_STARTEND
#error "regex.h:REG_STARTEND macro is missing from libc-shim"
#endif

#ifndef RE_BACKSLASH_ESCAPE_IN_LISTS
#error "regex.h:RE_BACKSLASH_ESCAPE_IN_LISTS macro is missing from libc-shim"
#endif

#ifndef RE_BK_PLUS_QM
#error "regex.h:RE_BK_PLUS_QM macro is missing from libc-shim"
#endif

#ifndef RE_CARET_ANCHORS_HERE
#error "regex.h:RE_CARET_ANCHORS_HERE macro is missing from libc-shim"
#endif

#ifndef RE_CHAR_CLASSES
#error "regex.h:RE_CHAR_CLASSES macro is missing from libc-shim"
#endif

#ifndef RE_CONTEXT_INDEP_ANCHORS
#error "regex.h:RE_CONTEXT_INDEP_ANCHORS macro is missing from libc-shim"
#endif

#ifndef RE_CONTEXT_INDEP_OPS
#error "regex.h:RE_CONTEXT_INDEP_OPS macro is missing from libc-shim"
#endif

#ifndef RE_CONTEXT_INVALID_DUP
#error "regex.h:RE_CONTEXT_INVALID_DUP macro is missing from libc-shim"
#endif

#ifndef RE_CONTEXT_INVALID_OPS
#error "regex.h:RE_CONTEXT_INVALID_OPS macro is missing from libc-shim"
#endif

#ifndef RE_DEBUG
#error "regex.h:RE_DEBUG macro is missing from libc-shim"
#endif

#ifndef RE_DOT_NEWLINE
#error "regex.h:RE_DOT_NEWLINE macro is missing from libc-shim"
#endif

#ifndef RE_DOT_NOT_NULL
#error "regex.h:RE_DOT_NOT_NULL macro is missing from libc-shim"
#endif

#ifndef RE_DUP_MAX
#error "regex.h:RE_DUP_MAX macro is missing from libc-shim"
#endif

#ifndef RE_HAT_LISTS_NOT_NEWLINE
#error "regex.h:RE_HAT_LISTS_NOT_NEWLINE macro is missing from libc-shim"
#endif

#ifndef RE_ICASE
#error "regex.h:RE_ICASE macro is missing from libc-shim"
#endif

#ifndef RE_INTERVALS
#error "regex.h:RE_INTERVALS macro is missing from libc-shim"
#endif

#ifndef RE_INVALID_INTERVAL_ORD
#error "regex.h:RE_INVALID_INTERVAL_ORD macro is missing from libc-shim"
#endif

#ifndef RE_LIMITED_OPS
#error "regex.h:RE_LIMITED_OPS macro is missing from libc-shim"
#endif

#ifndef RE_NEWLINE_ALT
#error "regex.h:RE_NEWLINE_ALT macro is missing from libc-shim"
#endif

#ifndef RE_NO_BK_BRACES
#error "regex.h:RE_NO_BK_BRACES macro is missing from libc-shim"
#endif

#ifndef RE_NO_BK_PARENS
#error "regex.h:RE_NO_BK_PARENS macro is missing from libc-shim"
#endif

#ifndef RE_NO_BK_REFS
#error "regex.h:RE_NO_BK_REFS macro is missing from libc-shim"
#endif

#ifndef RE_NO_BK_VBAR
#error "regex.h:RE_NO_BK_VBAR macro is missing from libc-shim"
#endif

#ifndef RE_NO_EMPTY_RANGES
#error "regex.h:RE_NO_EMPTY_RANGES macro is missing from libc-shim"
#endif

#ifndef RE_NO_GNU_OPS
#error "regex.h:RE_NO_GNU_OPS macro is missing from libc-shim"
#endif

#ifndef RE_NO_POSIX_BACKTRACKING
#error "regex.h:RE_NO_POSIX_BACKTRACKING macro is missing from libc-shim"
#endif

#ifndef RE_NO_SUB
#error "regex.h:RE_NO_SUB macro is missing from libc-shim"
#endif

#ifndef RE_NREGS
#error "regex.h:RE_NREGS macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_AWK
#error "regex.h:RE_SYNTAX_AWK macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_ED
#error "regex.h:RE_SYNTAX_ED macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_EGREP
#error "regex.h:RE_SYNTAX_EGREP macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_EMACS
#error "regex.h:RE_SYNTAX_EMACS macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_GNU_AWK
#error "regex.h:RE_SYNTAX_GNU_AWK macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_GREP
#error "regex.h:RE_SYNTAX_GREP macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_AWK
#error "regex.h:RE_SYNTAX_POSIX_AWK macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_BASIC
#error "regex.h:RE_SYNTAX_POSIX_BASIC macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_EGREP
#error "regex.h:RE_SYNTAX_POSIX_EGREP macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_EXTENDED
#error "regex.h:RE_SYNTAX_POSIX_EXTENDED macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_MINIMAL_BASIC
#error "regex.h:RE_SYNTAX_POSIX_MINIMAL_BASIC macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_POSIX_MINIMAL_EXTENDED
#error "regex.h:RE_SYNTAX_POSIX_MINIMAL_EXTENDED macro is missing from libc-shim"
#endif

#ifndef RE_SYNTAX_SED
#error "regex.h:RE_SYNTAX_SED macro is missing from libc-shim"
#endif

#ifndef RE_TRANSLATE_TYPE
#error "regex.h:RE_TRANSLATE_TYPE macro is missing from libc-shim"
#endif

#ifndef RE_UNMATCHED_RIGHT_PAREN_ORD
#error "regex.h:RE_UNMATCHED_RIGHT_PAREN_ORD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
