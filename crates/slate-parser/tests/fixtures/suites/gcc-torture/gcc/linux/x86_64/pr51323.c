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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %2 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i32, %5 y: i32, %6 z: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%4), read<i32>(%2)), ne<i32>(read<i32>(%5), const<i32>(0))), ne<i32>(read<i32>(%6), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%8 p: ptr<const @type0>) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field1(deref(read<ptr<const @type0>>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 x: i32, %11 y: @type0) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%3, call<i32, signature=fn(ptr<const @type0>) -> i32>(%7, pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11))), const<i32>(0), read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field0(%13), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(%13), read<i32>(%2));
// DEFAULT-NEXT:         write<i32>(field2(%13), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, @type0) -> void, abi=sysv64(scalar, native_c) -> void>(%9, const<i32>(9), copy<@type0, reason=arg>(read<@type0>(%13)));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(17));
// DEFAULT-NEXT:         write<i32>(field0(%13), sub<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(%13), read<i32>(%2));
// DEFAULT-NEXT:         write<i32>(field2(%13), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, @type0) -> void, abi=sysv64(scalar, native_c) -> void>(%9, const<i32>(9), copy<@type0, reason=arg>(read<@type0>(%13)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
