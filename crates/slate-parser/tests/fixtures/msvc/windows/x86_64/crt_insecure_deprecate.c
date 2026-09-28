// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
#include <string.h>

__declspec(deprecated("a" "b")) int old_api(void);

char *copy(char *destination, const char *source) {
    return strcpy(destination, source) + old_api();
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %2 @strcpy(%7 _Destination: ptr<i8>, %8 _Source: ptr<const i8>) -> ptr<i8> [linkage=external];
// IR-NEXT:     fn %3 @old_api() -> i32 [linkage=external];
// IR-NEXT:     fn %4 @copy(%5 destination: ptr<i8>, %6 source: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%2, read<ptr<i8>>(%5), read<ptr<const i8>>(%6)), call<i32, signature=fn() -> i32>(%3));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
