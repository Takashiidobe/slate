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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %1 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<i32>) -> @type0 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%4), read<ptr<i32>>(%3));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(%1))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<@type0>(%1, copy<@type0, reason=assign>(call<@type0, signature=fn(ptr<i32>) -> @type0, abi=sysv64(scalar) -> coerce<i64>>(%2, addr_of<ptr<i32>>(%8))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(ptr<i32>) -> @type0, abi=sysv64(scalar) -> coerce<i64>>(%2, addr_of<ptr<i32>>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
