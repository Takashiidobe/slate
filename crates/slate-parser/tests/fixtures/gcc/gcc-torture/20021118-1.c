void abort(void);
void exit(int);

struct s {
  int f[4];
};

int foo(struct s s, int x1, int x2, int x3, int x4, int x5, int x6, int x7) {
  return s.f[3] + x7;
}

int main() {
  struct s s = {1, 2, 3, 4};

  if (foo(s, 100, 200, 300, 400, 500, 600, 700) != 704)
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 f: array<i32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 s: @type0, %5 x1: i32, %6 x2: i32, %7 x3: i32, %8 x4: i32, %9 x5: i32, %10 x6: i32, %11 x7: i32) -> i32 [linkage=external] [abi=sysv64(native_c, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field0(%4)), const<i32>(3)))), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(@type0, i32, i32, i32, i32, i32, i32, i32) -> i32, abi=sysv64(native_c, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> scalar>(%3, copy<@type0, reason=arg>(read<@type0>(%13)), const<i32>(100), const<i32>(200), const<i32>(300), const<i32>(400), const<i32>(500), const<i32>(600), const<i32>(700)), const<i32>(704))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
