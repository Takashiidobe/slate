int main() {
#ifdef _WIN32
  return 2;
#else
  return 3;
#endif
}

typedef int HANDLE;

#ifdef _WIN32
typedef HANDLE Socket;
#else
typedef int Socket;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIN32 _WIN32

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
// DEFAULT-NEXT:     type @type0 HANDLE = i32;
// DEFAULT-NEXT:     type @type1 Socket = i32;
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIN32
// WIN32: module {
// WIN32-NEXT:     target "x86_64-unknown-linux-gnu" {
// WIN32-NEXT:         endian = little;
// WIN32-NEXT:         pointer [size=8, align=8];
// WIN32-NEXT:         stack_alignment = 16;
// WIN32-NEXT:         long_double = f80;
// WIN32-NEXT:         storage bool [size=1, align=1];
// WIN32-NEXT:         storage i8, u8 [size=1, align=1];
// WIN32-NEXT:         storage i16, u16 [size=2, align=2];
// WIN32-NEXT:         storage i32, u32 [size=4, align=4];
// WIN32-NEXT:         storage i64, u64 [size=8, align=8];
// WIN32-NEXT:         storage i128, u128 [size=16, align=16];
// WIN32-NEXT:         storage bf16 [size=2, align=2];
// WIN32-NEXT:         storage f16 [size=2, align=2];
// WIN32-NEXT:         storage f32 [size=4, align=4];
// WIN32-NEXT:         storage f64 [size=8, align=8];
// WIN32-NEXT:         storage f80 [size=16, align=16];
// WIN32-NEXT:         storage f128 [size=16, align=16];
// WIN32-NEXT:         storage d32 [size=4, align=4];
// WIN32-NEXT:         storage d64 [size=8, align=8];
// WIN32-NEXT:         storage d128 [size=16, align=16];
// WIN32-NEXT:     }
// WIN32-NEXT:     type @type0 HANDLE = i32;
// WIN32-NEXT:     type @type1 Socket = i32;
// WIN32-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// WIN32-NEXT:         return const<i32>(2);
// WIN32-NEXT:     }
// WIN32-NEXT: }
// SLATE-FILECHECK-END WIN32
