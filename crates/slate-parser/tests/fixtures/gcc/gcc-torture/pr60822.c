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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 fill0: array<i8, 800000>;
// DEFAULT-NEXT:         field1 a: i32;
// DEFAULT-NEXT:         field2 fill1: array<i8, 900000>;
// DEFAULT-NEXT:         field3 b: i32;
// DEFAULT-NEXT:     } [size=1700008, align=4, offsets=[0, 800000, 800004, 1700004]];
// DEFAULT-NEXT:     global %4 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @Avg(%2 p: ptr<@type0>, %3 s: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%3)), widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(field1(deref(read<ptr<@type0>>(%2)))), read<i32>(field3(deref(read<ptr<@type0>>(%2))))))), const<i32>(17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field1(%4), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)));
// DEFAULT-NEXT:         write<i32>(field3(%4), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), const<i32>(17)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, i32) -> i32>(%1, addr_of<ptr<@type0>>(%4), const<i32>(1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
