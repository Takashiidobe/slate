/* { dg-do run } */
/* { dg-options "-fsanitize=shift -std=c2y" } */

int
main ()
{
  int a = -42;
  unsigned b = 42;
  unsigned c = __builtin_stdc_rotate_left (b, a);
  unsigned d = __builtin_stdc_rotate_right (b, a - 1);
  volatile int e = c + d;
}
/* { dg-output "shift exponent -42 is negative\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*shift exponent -43 is negative" } */

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_rotate_left:[0-9]+]] @__builtin_stdc_rotate_left(%[[VALUE0:[0-9]+]] <unnamed>: u32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_rotate_right:[0-9]+]] @__builtin_stdc_rotate_right(%[[VALUE2:[0-9]+]] <unnamed>: u32, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(42));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(42));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u32 [storage=automatic] = call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_left]], read<u32>(%[[VALUE_b]]), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u32 [storage=automatic] = call<u32, signature=fn(u32, i32) -> u32>(%[[VALUE___builtin_stdc_rotate_right]], read<u32>(%[[VALUE_b]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: volatile i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_c]]), read<u32>(%[[VALUE_d]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
