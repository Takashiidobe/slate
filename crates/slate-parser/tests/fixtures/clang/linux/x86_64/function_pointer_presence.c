static int bump(int value) { return value + 1; }

static int maybe_apply(int (*op)(int), int value) {
  if (op) {
    return op(value);
  }
  return value;
}

int main(void) {
  int (*op)(int) = 0;
  int total      = maybe_apply(op, 4);
  op             = bump;
  if (op != 0) {
    total = total + maybe_apply(op, 5);
  }
  if (op == 0) {
    return 2;
  }
  return total == 10 ? 0 : 1;
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
// DEFAULT-NEXT:     fn %0 @bump(%1 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%1), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @maybe_apply(%3 op: ptr<fn(i32) -> i32>, %4 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<ptr<fn(i32) -> i32>>(read<ptr<fn(i32) -> i32>>(%3), null<ptr<fn(i32) -> i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%3), read<i32>(%4));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 op: ptr<fn(i32) -> i32> [storage=automatic] = null<ptr<fn(i32) -> i32>>;
// DEFAULT-NEXT:         let %7 total: i32 [storage=automatic] = call<i32, signature=fn(ptr<fn(i32) -> i32>, i32) -> i32>(%2, read<ptr<fn(i32) -> i32>>(%6), const<i32>(4));
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%6, function_decay<ptr<fn(i32) -> i32>>(%0));
// DEFAULT-NEXT:         if ne<ptr<fn(i32) -> i32>>(read<ptr<fn(i32) -> i32>>(%6), null<ptr<fn(i32) -> i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%7, add<i32, overflow=ub>(read<i32>(%7), call<i32, signature=fn(ptr<fn(i32) -> i32>, i32) -> i32>(%2, read<ptr<fn(i32) -> i32>>(%6), const<i32>(5))));
// DEFAULT-NEXT:                 add<i32, overflow=ub>(read<i32>(%7), call<i32, signature=fn(ptr<fn(i32) -> i32>, i32) -> i32>(%2, read<ptr<fn(i32) -> i32>>(%6), const<i32>(5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<ptr<fn(i32) -> i32>>(read<ptr<fn(i32) -> i32>>(%6), null<ptr<fn(i32) -> i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%7), const<i32>(10)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
