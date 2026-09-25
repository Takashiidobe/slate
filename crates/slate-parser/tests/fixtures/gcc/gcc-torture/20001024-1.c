// SLATE-FILECHECK-DEFINES DEFAULT

/* Copyright (C) 2000  Free Software Foundation  */
/* Contributed by Nathan Sidwell <nathan@codesourcery.com> */

typedef __SIZE_TYPE__ size_t;

extern void *memset(void *, int, size_t);

struct Baz;

void quux(struct Baz *context)
{
  memset(context, 0, 4);
}

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 Baz = struct incomplete;
// DEFAULT-NEXT:     fn %1 @memset(%5 <unnamed>: ptr<void>, %6 <unnamed>: i32, %7 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @quux(%4 context: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%4)), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
