/* Originally added to test SH constant pool layout.  t1() failed for
   non-PIC and t2() failed for PIC.  */

void abort(void);
void exit(int);

int t1(float *f, int i, void (*f1)(double), void (*f2)(float, float)) {
  f1(3.0);
  f[i] = f[i + 1];
  f2(2.5f, 3.5f);
}

int t2(float *f, int i, void (*f1)(double), void (*f2)(float, float),
       void (*f3)(float)) {
  f3(6.0f);
  f1(3.0);
  f[i] = f[i + 1];
  f2(2.5f, 3.5f);
}

void f1(double d) {
  if (d != 3.0)
    abort();
}

void f2(float f1, float f2) {
  if (f1 != 2.5f || f2 != 3.5f)
    abort();
}

void f3(float f) {
  if (f != 6.0f)
    abort();
}

int main() {
  float f[3] = {2.0f, 3.0f, 4.0f};
  t1(f, 0, f1, f2);
  t2(f, 1, f1, f2, f3);
  if (f[0] != 3.0f && f[1] != 4.0f)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_t1:[0-9]+]] @t1(%[[VALUE_f:[0-9]+]] f: ptr<f32>, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_f1:[0-9]+]] f1: ptr<fn(f64) -> void>, %[[VALUE_f2:[0-9]+]] f2: ptr<fn(f32, f32) -> void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(read<ptr<fn(f64) -> void>>(%[[VALUE_f1]]), const<f64>(3.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_f]]), read<i32>(%[[VALUE_i]]))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_f]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(read<ptr<fn(f32, f32) -> void>>(%[[VALUE_f2]]), const<f32>(2.5), const<f32>(3.5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t2:[0-9]+]] @t2(%[[VALUE_f_2:[0-9]+]] f: ptr<f32>, %[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_f1_2:[0-9]+]] f1: ptr<fn(f64) -> void>, %[[VALUE_f2_2:[0-9]+]] f2: ptr<fn(f32, f32) -> void>, %[[VALUE_f3:[0-9]+]] f3: ptr<fn(f32) -> void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(read<ptr<fn(f32) -> void>>(%[[VALUE_f3]]), const<f32>(6.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(read<ptr<fn(f64) -> void>>(%[[VALUE_f1_2]]), const<f64>(3.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_f_2]]), read<i32>(%[[VALUE_i_2]]))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_f_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(read<ptr<fn(f32, f32) -> void>>(%[[VALUE_f2_2]]), const<f32>(2.5), const<f32>(3.5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1_3:[0-9]+]] @f1(%[[VALUE_d:[0-9]+]] d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2_3:[0-9]+]] @f2(%[[VALUE_f1_4:[0-9]+]] f1: f32, %[[VALUE_f2_4:[0-9]+]] f2: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_f1_4]]), const<f32>(2.5)), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_f2_4]]), const<f32>(3.5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3_2:[0-9]+]] @f3(%[[VALUE_f_3:[0-9]+]] f: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_f_3]]), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f_4:[0-9]+]] f: array<f32, 3> [storage=automatic] = aggregate<array<f32, 3>, zero_fill=false>(index0 = const<f32>(2.0), index1 = const<f32>(3.0), index2 = const<f32>(4.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<f32>, i32, ptr<fn(f64) -> void>, ptr<fn(f32, f32) -> void>) -> i32>(%[[VALUE_t1]], array_decay<ptr<f32>, length=Some(3)>(%[[VALUE_f_4]]), const<i32>(0), function_decay<ptr<fn(f64) -> void>>(%[[VALUE_f1_3]]), function_decay<ptr<fn(f32, f32) -> void>>(%[[VALUE_f2_3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<f32>, i32, ptr<fn(f64) -> void>, ptr<fn(f32, f32) -> void>, ptr<fn(f32) -> void>) -> i32>(%[[VALUE_t2]], array_decay<ptr<f32>, length=Some(3)>(%[[VALUE_f_4]]), const<i32>(1), function_decay<ptr<fn(f64) -> void>>(%[[VALUE_f1_3]]), function_decay<ptr<fn(f32, f32) -> void>>(%[[VALUE_f2_3]]), function_decay<ptr<fn(f32) -> void>>(%[[VALUE_f3_2]]));
// DEFAULT-NEXT:         if logical_and<bool>(ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(%[[VALUE_f_4]]), const<i32>(0)))), const<f32>(3.0)), ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(%[[VALUE_f_4]]), const<i32>(1)))), const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
