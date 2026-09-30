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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_R:[0-9]+]] R = struct {
// DEFAULT-NEXT:         field0 a: @type[[TYPE_A]];
// DEFAULT-NEXT:         field1 b: @type[[TYPE_A]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_R:[0-9]+]] R: @type[[TYPE_R]] [storage=static] = aggregate<@type[[TYPE_R]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(100))), field1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(200)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_r:[0-9]+]] r: @type[[TYPE_R]]) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field0(field0(%[[VALUE_r]]))), read<i64>(field0(field0(%[[VALUE_R]])))), ne<i64>(read<i64>(field0(field1(%[[VALUE_r]]))), read<i64>(field0(field1(%[[VALUE_R]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> @type[[TYPE_R]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_R]], reason=return>(read<@type[[TYPE_R]]>(%[[VALUE_R]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_R]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_R]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_f]], copy<@type[[TYPE_R]], reason=arg>(read<@type[[TYPE_R]]>(%[[VALUE_R]])));
// DEFAULT-NEXT:         write<@type[[TYPE_R]]>(%[[VALUE_r_2]], copy<@type[[TYPE_R]], reason=assign>(call<@type[[TYPE_R]], signature=fn() -> @type[[TYPE_R]], abi=sysv64() -> native_c>(%[[VALUE_g]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field0(field0(%[[VALUE_r_2]]))), read<i64>(field0(field0(%[[VALUE_R]])))), ne<i64>(read<i64>(field0(field1(%[[VALUE_r_2]]))), read<i64>(field0(field1(%[[VALUE_R]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
