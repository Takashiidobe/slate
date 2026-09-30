
// Make it possible to pass NULL through variadic functions on platforms where
// NULL has an integer type that is more narrow than a pointer. On such
// platforms we widen null pointer constants passed to variadic functions to a
// pointer-sized integer. We don't apply this special case to K&R-style
// unprototyped functions, because MSVC doesn't either.

#define NULL 0

void v(const char *f, ...);
void kr();
void f(const char *f) {
  v(f, 1, 2, 3, NULL);
  kr(f, 1, 2, 3, 0);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_v:[0-9]+]] @v(%[[VALUE_f:[0-9]+]] f: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_kr:[0-9]+]] @kr(unprototyped) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f(%[[VALUE_f_3:[0-9]+]] f: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE_v]], read<ptr<const i8>>(%[[VALUE_f_3]]), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_kr]], read<ptr<const i8>>(%[[VALUE_f_3]]), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
