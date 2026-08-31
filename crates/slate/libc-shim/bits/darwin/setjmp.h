#ifndef _SLATE_BITS_DARWIN_SETJMP_H
#define _SLATE_BITS_DARWIN_SETJMP_H

#define _JBLEN ((14 + 8 + 2) * 2)

typedef int jmp_buf[_JBLEN];
typedef int sigjmp_buf[_JBLEN + 1];

int            setjmp(jmp_buf);
_Noreturn void longjmp(jmp_buf, int);
int            _setjmp(jmp_buf);
_Noreturn void _longjmp(jmp_buf, int);
int            sigsetjmp(sigjmp_buf, int);
_Noreturn void siglongjmp(sigjmp_buf, int);
void           longjmperror(void);

#define setjmp setjmp

#endif
