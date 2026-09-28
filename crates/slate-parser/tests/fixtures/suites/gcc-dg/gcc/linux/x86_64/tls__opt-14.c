/* This testcase generated invalid assembly on ARM Thumb-2.  Two
   PIC additions of pc were combined, but the deleted label was still
   used.  */
/* { dg-do assemble } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls } */

struct __res_state
{
  int options;
};
extern __thread struct __res_state *__resp
  __attribute__ ((tls_model ("initial-exec")));

void foo (void);

int main(void)
{
  int count, total = 0;

  for (count = 0; count < 10; count++)
    {
      if (((*__resp).options & 0x00000001) == 0)
	foo ();
      (*__resp).options &= ~((0x00000002 | 0x00000200 | 0x00000080));
    }
  return 0;
}

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
// DEFAULT-NEXT:     type @type0 __res_state = struct {
// DEFAULT-NEXT:         field0 options: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     extern %1 __resp: ptr<@type0> [storage=thread] [linkage=external] [tls_model=initial-exec];
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<i32>(and<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%1)))), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     let %9: ptr<@type0> [synthetic] = read<ptr<@type0>>(%1);
// DEFAULT-NEXT:                     let %10: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type0>>(%9))));
// DEFAULT-NEXT:                     let %11: i32 [synthetic] = and<i32>(read<i32>(%10), not<i32>(or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(128))));
// DEFAULT-NEXT:                     write<i32>(field0(deref(read<ptr<@type0>>(%9))), read<i32>(%11));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
