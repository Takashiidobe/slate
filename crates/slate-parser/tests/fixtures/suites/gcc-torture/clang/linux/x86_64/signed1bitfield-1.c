/* PR tree-optimization/115154 */
/* This was being miscompiled to `(signed:1)(t*5)`
   being transformed into `-((signed:1)t)` which is undefined.
   Note there is a pattern which removes the negative in some cases
   which works around the issue.  */

struct {
  signed b : 1;
} f;
int                            i = 55;
__attribute__((noinline)) void check(int a) {
  if (!a)
    __builtin_abort();
}
int main() {
  int t  = i != 5;
  t      = t * 5;
  f.b    = t;
  int tt = f.b;
  check(f.b);
}


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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 b: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(55) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = from_bool<i32, reason=assign>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_t]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_t]]), const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_f]]), read<i32>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE_tt:[0-9]+]] tt: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_check]], read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_f]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
