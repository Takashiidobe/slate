/* PR target/30185 */

extern void abort(void);

typedef struct S {
  char      a;
  long long b;
} S;

S foo(S x, S y) {
  S z;
  z.b = x.b / y.b;
  return z;
}

int main(void) {
  S a, b;
  a.b = 32LL;
  b.b = 4LL;
  if (foo(a, b).b != 8LL)
    abort();
  a.b = -8LL;
  b.b = -2LL;
  if (foo(a, b).b != 4LL)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_S_2:[0-9]+]] S = @type[[TYPE_S]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: @type[[TYPE_S]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE_S]]) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_z]]), div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(field1(%[[VALUE_x]])), read<i64>(field1(%[[VALUE_y]]))));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_a]]), const<i64>(32));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_b]]), const<i64>(4));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(temporary %[[VALUE0:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(@type[[TYPE_S]], @type[[TYPE_S]]) -> @type[[TYPE_S]], abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_foo]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_a]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_b]]))))), const<i64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_a]]), neg<i64, overflow=ub>(const<i64>(8)));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_b]]), neg<i64, overflow=ub>(const<i64>(2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field1(temporary %[[VALUE1:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(@type[[TYPE_S]], @type[[TYPE_S]]) -> @type[[TYPE_S]], abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_foo]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_a]])), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_b]]))))), const<i64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
