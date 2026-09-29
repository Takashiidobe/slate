/* PR middle-end/51323 */

extern void abort(void);
struct S {
  int a, b, c;
};
int v;

__attribute__((noinline, noclone)) void foo(int x, int y, int z) {
  if (x != v || y != 0 || z != 9)
    abort();
}

static inline int baz(const struct S *p) { return p->b; }

__attribute__((noinline, noclone)) void bar(int x, struct S y) {
  foo(baz(&y), 0, x);
}

int main() {
  struct S s;
  v   = 3;
  s.a = v - 1;
  s.b = v;
  s.c = v + 1;
  bar(9, s);
  v   = 17;
  s.a = v - 1;
  s.b = v;
  s.c = v + 1;
  bar(9, s);
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
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_v]])), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p:[0-9]+]] p: ptr<const @type[[TYPE_S]]>) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field1(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_S]]) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_foo]], call<i32, signature=fn(ptr<const @type[[TYPE_S]]>) -> i32>(%[[VALUE_baz]], pointer_cast<ptr<const @type[[TYPE_S]]>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_y_2]]))), const<i32>(0), read<i32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_s]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_s]]), read<i32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_s]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, @type[[TYPE_S]]) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_bar]], const<i32>(9), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_s]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v]], const<i32>(17));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_s]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_s]]), read<i32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_s]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, @type[[TYPE_S]]) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_bar]], const<i32>(9), copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_s]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
