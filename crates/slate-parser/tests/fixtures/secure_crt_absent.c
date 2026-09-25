#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

extern int _ctime64_s;
extern int _dupenv_s;
extern int _get_errno;
extern int _itoa_s;
extern int _set_invalid_parameter_handler;
extern int fopen_s;
extern int strcpy_s;
extern int wcscpy_s;

int main(void) { return 0; }




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     extern %0 _ctime64_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %1 _dupenv_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %2 _get_errno: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %3 _itoa_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %4 _set_invalid_parameter_handler: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 fopen_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %6 strcpy_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %7 wcscpy_s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
