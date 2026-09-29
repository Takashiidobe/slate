/* { dg-do compile } */
/* { dg-options "-O2 -fPIC" } */
/* { dg-require-effective-target tls } */
/* { dg-require-effective-target fpic } */

static __thread void *baz [4] __attribute__((tls_model ("initial-exec")));
void foo (void)
{
  void **u = (void **) baz;
  
  u[0] = 0;
  u[1] = 0;
}

/* { dg-final { scan-assembler-not "\[48\]\\+baz" { target i?86-*-* x86_64-*-* } } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_baz:[0-9]+]] baz: array<ptr<void>, 4> [storage=thread] [align=16] [linkage=internal] [tls_model=initial-exec];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<ptr<void>> [storage=automatic] = array_decay<ptr<ptr<void>>, length=Some(4)>(%[[VALUE_baz]]);
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(read<ptr<ptr<void>>>(%[[VALUE_u]]), const<i32>(0))), null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(read<ptr<ptr<void>>>(%[[VALUE_u]]), const<i32>(1))), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
