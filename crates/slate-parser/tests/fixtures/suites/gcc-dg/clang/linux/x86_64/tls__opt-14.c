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
// DEFAULT-NEXT:     type @type[[TYPE___res_state:[0-9]+]] __res_state = struct {
// DEFAULT-NEXT:         field0 options: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     extern %[[VALUE___resp:[0-9]+]] __resp: ptr<@type[[TYPE___res_state]]> [storage=thread] [linkage=external] [tls_model=initial-exec];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_count]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if eq<i32>(and<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE___res_state]]>>(%[[VALUE___resp]])))), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE___res_state]]> [synthetic] = read<ptr<@type[[TYPE___res_state]]>>(%[[VALUE___resp]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE___res_state]]>>(%[[VALUE3]]))));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = and<i32>(read<i32>(%[[VALUE4]]), not<i32>(or<i32>(or<i32>(const<i32>(2), const<i32>(512)), const<i32>(128))));
// DEFAULT-NEXT:                     write<i32>(field0(deref(read<ptr<@type[[TYPE___res_state]]>>(%[[VALUE3]]))), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
