/* { dg-do compile } */

#define ATTR_PRINTF __attribute__ ((format (printf, 1, 2)))
#define ATTR_USED __attribute__ ((used))

void bar (int, ...);

/* gcc would segfault on the nested attribute.  */
void foo (void)
{
  bar (0, (void (*ATTR_PRINTF) (const char *, ...)) 0);
}

/* For consistency, unnamed decls should give the same warnings as
   named ones.  */
void proto1 (int (*ATTR_USED) (void)); /* { dg-warning "attribute ignored" } */
void proto2 (int (*ATTR_USED bar) (void)); /* { dg-warning "attribute ignored" } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %0 @bar(%4 <unnamed>: i32, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%0, const<i32>(0), null<ptr<fn(ptr<const i8>, ...) -> void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @proto1(%5 <unnamed>: ptr<fn() -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @proto2(%6 bar: ptr<fn() -> i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
