/* PR target/7559
   This testcase was miscompiled on x86-64, because classify_argument
   wrongly computed the offset of nested structure fields.  */

extern void abort(void);

struct A {
  long x;
};

struct R {
  struct A a, b;
};

struct R R = {100, 200};

void f(struct R r) {
  if (r.a.x != R.a.x || r.b.x != R.b.x)
    abort();
}

struct R g(void) { return R; }

int main(void) {
  struct R r;
  f(R);
  r = g();
  if (r.a.x != R.a.x || r.b.x != R.b.x)
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 R = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:         field1 b: @type0;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %3 R: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(100))), field1 = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(200)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 r: @type1) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field0(field0(%5))), read<i64>(field0(field0(%3)))), ne<i64>(read<i64>(field0(field1(%5))), read<i64>(field0(field1(%3)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @g() -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 r: @type1 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type1) -> void, abi=sysv64(native_c) -> void>(%4, copy<@type1, reason=arg>(read<@type1>(%3)));
// DEFAULT-NEXT:         write<@type1>(%8, copy<@type1, reason=assign>(call<@type1, signature=fn() -> @type1, abi=sysv64() -> native_c>(%6)));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn() -> @type1, abi=sysv64() -> native_c>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field0(field0(%8))), read<i64>(field0(field0(%3)))), ne<i64>(read<i64>(field0(field1(%8))), read<i64>(field0(field1(%3)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
