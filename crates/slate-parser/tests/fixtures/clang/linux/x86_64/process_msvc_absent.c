#include <stdlib.h>
#include <wchar.h>

extern int _beginthread;
extern int _cwait;
extern int _execv;
extern int _get_initial_narrow_environment;
extern int _getpid;
extern int _spawnv;
extern int _wexecv;
extern int _wspawnv;

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
// DEFAULT-NEXT:     extern %0 _beginthread: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %1 _cwait: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %2 _execv: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %3 _get_initial_narrow_environment: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %4 _getpid: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 _spawnv: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %6 _wexecv: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %7 _wspawnv: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
