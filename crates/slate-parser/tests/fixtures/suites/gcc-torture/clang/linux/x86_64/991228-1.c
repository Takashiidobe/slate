void abort(void);
void exit(int);

__extension__ union {
  double d;
  int    i[2];
} u = {d : -0.25};

/* This assumes the endianness of words in a long long is the same as
   that for doubles, which doesn't hold for a few platforms, but we
   can probably special case them here, as appropriate.  */
long long endianness_test = 1;
#define MSW (*(int *)&endianness_test)

int signbit(double x) {
  __extension__ union {
    double d;
    int    i[2];
  } u = {d : x};
  return u.i[MSW] < 0;
}

int main(void) {
  if (2 * sizeof(int) != sizeof(double) || u.i[MSW] >= 0)
    exit(0);

  if (!signbit(-0.25))
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = neg<f64>(const<f64>(0.25))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_endianness_test:[0-9]+]] endianness_test: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_signbit:[0-9]+]] @signbit(%[[VALUE_x:[0-9]+]] x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE1]] [storage=automatic] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u_2]])), read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<i64>>(%[[VALUE_endianness_test]]))))))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(4)), const<u64>(8)), ge<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(%[[VALUE_u]])), read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<i64>>(%[[VALUE_endianness_test]]))))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%[[VALUE_signbit]], neg<f64>(const<f64>(0.25))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
