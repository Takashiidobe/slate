/* { dg-require-effective-target int32plus } */
struct X {
  char fill0[800000];
  int  a;
  char fill1[900000];
  int  b;
};

int __attribute__((noinline, noclone)) Avg(struct X *p, int s) {
  return (s * (long long)(p->a + p->b)) >> 17;
}

struct X x;

int main() {
  x.a = 1 << 17;
  x.b = 2 << 17;
  if (Avg(&x, 1) != 3)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = struct {
// DEFAULT-NEXT:         field0 fill0: array<i8, 800000>;
// DEFAULT-NEXT:         field1 a: i32;
// DEFAULT-NEXT:         field2 fill1: array<i8, 900000>;
// DEFAULT-NEXT:         field3 b: i32;
// DEFAULT-NEXT:     } [size=1700008, align=4, offsets=[0, 800000, 800004, 1700004]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_X]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Avg:[0-9]+]] @Avg(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_X]]>, %[[VALUE_s:[0-9]+]] s: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_s]])), widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(field1(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p]])))), read<i32>(field3(deref(read<ptr<@type[[TYPE_X]]>>(%[[VALUE_p]]))))))), const<i32>(17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_x]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_x]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), const<i32>(17)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_X]]>, i32) -> i32>(%[[VALUE_Avg]], addr_of<ptr<@type[[TYPE_X]]>>(%[[VALUE_x]]), const<i32>(1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
