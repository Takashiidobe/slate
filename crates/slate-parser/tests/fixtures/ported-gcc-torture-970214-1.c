void exit(int);

#define L 1
int main(void) { exit(L'1' != L'1'); }

// SLATE-FILECHECK-DEFINES GCC

// SLATE-FILECHECK-BEGIN GCC
// GCC: module {
// GCC-NEXT:     target "x86_64-unknown-linux-gnu" {
// GCC-NEXT:         endian = little;
// GCC-NEXT:         pointer [size=8, align=8];
// GCC-NEXT:         stack_alignment = 16;
// GCC-NEXT:         long_double = f80;
// GCC-NEXT:         storage bool [size=1, align=1];
// GCC-NEXT:         storage i8, u8 [size=1, align=1];
// GCC-NEXT:         storage i16, u16 [size=2, align=2];
// GCC-NEXT:         storage i32, u32 [size=4, align=4];
// GCC-NEXT:         storage i64, u64 [size=8, align=8];
// GCC-NEXT:         storage i128, u128 [size=16, align=16];
// GCC-NEXT:         storage bf16 [size=2, align=2];
// GCC-NEXT:         storage f16 [size=2, align=2];
// GCC-NEXT:         storage f32 [size=4, align=4];
// GCC-NEXT:         storage f64 [size=8, align=8];
// GCC-NEXT:         storage f80 [size=16, align=16];
// GCC-NEXT:         storage f128 [size=16, align=16];
// GCC-NEXT:         storage d32 [size=4, align=4];
// GCC-NEXT:         storage d64 [size=8, align=8];
// GCC-NEXT:         storage d128 [size=16, align=16];
// GCC-NEXT:     }
// GCC-NEXT:     fn %0 @exit(%2 <unnamed>: i32) -> void [linkage=external] [noreturn];
// GCC-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// GCC-NEXT:         call<void, signature=fn(i32) -> void>(%0, from_bool<i32, reason=arg>(ne<i32>(const<i32>(49), const<i32>(49))));
// GCC-NEXT:     }
// GCC-NEXT: }
// SLATE-FILECHECK-END GCC
