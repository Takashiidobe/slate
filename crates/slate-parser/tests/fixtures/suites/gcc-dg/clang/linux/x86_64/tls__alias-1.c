/* { dg-do link } */
/* { dg-require-alias "" } */
/* { dg-require-visibility "" } */
/* { dg-require-effective-target tls_runtime } */
/* { dg-add-options tls } */
/* Test that encode_section_info handles the change from externally
   defined to locally defined (via hidden).   Extracted from glibc.  */

struct __res_state {
	char x[123];
};

extern __thread struct __res_state bar
  __attribute__ ((tls_model ("initial-exec")));

int main()
{
  bar.x[0] = 0;
  return 0;
}

__thread struct __res_state foo;
extern __thread struct __res_state bar
  __attribute__ ((alias ("foo")))
  __attribute__ ((visibility ("hidden")));

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
// DEFAULT-NEXT:     type @type[[TYPE___res_state:[0-9]+]] __res_state = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 123>;
// DEFAULT-NEXT:     } [size=123, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: @type[[TYPE___res_state]] [storage=thread] [linkage=external] [visibility=hidden] [alias="foo"] [tls_model=initial-exec];
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: @type[[TYPE___res_state]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(123)>(field0(%[[VALUE_bar]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
