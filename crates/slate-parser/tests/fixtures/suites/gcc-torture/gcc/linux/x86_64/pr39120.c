struct X {
  int *p;
} x;

struct X __attribute__((noinline)) foo(int *p) {
  struct X x;
  x.p = p;
  return x;
}

void __attribute((noinline)) bar() { *x.p = 1; }

extern void abort(void);
int         main() {
  int i = 0;
  x     = foo(&i);
  bar();
  if (i != 1)
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
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_X]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> @type[[TYPE_X]] [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_X]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%[[VALUE_x_2]]), read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_X]], reason=return>(read<@type[[TYPE_X]]>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(%[[VALUE_x]]))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_x]], copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(ptr<i32>) -> @type[[TYPE_X]], abi=sysv64(scalar) -> native_c>(%[[VALUE_foo]], addr_of<ptr<i32>>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
