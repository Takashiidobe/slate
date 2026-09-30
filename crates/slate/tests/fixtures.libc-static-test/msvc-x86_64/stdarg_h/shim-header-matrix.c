#include <stdarg.h>

#ifndef _INC_STDARG
#error "stdarg.h:_INC_STDARG macro is missing from libc-shim"
#endif

#ifndef va_arg
#error "stdarg.h:va_arg macro is missing from libc-shim"
#endif

#ifndef va_copy
#error "stdarg.h:va_copy macro is missing from libc-shim"
#endif

#ifndef va_end
#error "stdarg.h:va_end macro is missing from libc-shim"
#endif

#ifndef va_start
#error "stdarg.h:va_start macro is missing from libc-shim"
#endif

int main(void) { return 0; }
