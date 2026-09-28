struct Point {
  int x;
  int y;
};

typedef struct Point Point;

int sum_point(struct Point p) {
  return p.x + p.y;
}

int compute(void) {
  struct Point a = (struct Point){1, 2};
  int total = sum_point((struct Point){.x = 3, .y = 4});
  Point b = (Point){.y = 5};
  total += a.x + b.y;
  return total;
}

int main(void) {
  return compute();
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
// DEFAULT-NEXT:     type @type0 Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 Point = @type0;
// DEFAULT-NEXT:     fn %2 @sum_point(%3 p: @type0) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(%3)), read<i32>(field1(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @compute() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(compound_literal %9 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))));
// DEFAULT-NEXT:         let %6 total: i32 [storage=automatic] = call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(compound_literal %10 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)))));
// DEFAULT-NEXT:         let %7 b: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(compound_literal %11 [storage=automatic] = aggregate<@type0, zero_fill=true>(field1 = const<i32>(5))));
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), add<i32, overflow=ub>(read<i32>(field0(%5)), read<i32>(field1(%7))));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%13));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
