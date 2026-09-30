extern void abort(void);

void __attribute__((noinline)) bar(int **p) {
  float *q = (float *)p;
  *q       = 0.0;
}

float __attribute__((noinline)) foo(int b) {
  int  *i = 0;
  float f = 1.0;
  int **p;
  if (b)
    p = &i;
  else
    p = (int **)&f;
  bar(p);
  if (b)
    return **p;
  return f;
}

int main() {
  if (foo(0) != 0.0)
    abort();
  return 0;
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p:[0-9]+]] p: ptr<ptr<i32>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<f32> [storage=automatic] = pointer_cast<ptr<f32>, reason=explicit>(read<ptr<ptr<i32>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%[[VALUE_q]])), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: i32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: f32 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<ptr<i32>>>(%[[VALUE_p_2]], addr_of<ptr<ptr<i32>>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<ptr<i32>>>(%[[VALUE_p_2]], pointer_cast<ptr<ptr<i32>>, reason=explicit>(addr_of<ptr<f32>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>) -> void>(%[[VALUE_bar]], read<ptr<ptr<i32>>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             return int_to_float<f32, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_p_2]]))))));
// DEFAULT-NEXT:         return read<f32>(%[[VALUE_f]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(i32) -> f32>(%[[VALUE_foo]], const<i32>(0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
