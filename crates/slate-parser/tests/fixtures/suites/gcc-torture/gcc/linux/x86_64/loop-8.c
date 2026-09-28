void abort(void);
void exit(int);

double a[3] = {0.0, 1.0, 2.0};

void bar(int x, double *y) {
  if (x || *y != 1.0)
    abort();
}

int main() {
  double c;
  int    d;
  for (d = 0; d < 3; d++) {
    c = a[d];
    if (c > 0.0)
      goto e;
  }
  bar(1, &c);
  exit(1);
e:
  bar(0, &c);
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
// DEFAULT-NEXT:     global %2 a: array<f64, 3> [storage=static] [align=16] = aggregate<array<f64, 3>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0), index2 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bar(%4 x: i32, %5 y: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(0)), ne<f64, exceptions=observable>(read<f64>(deref(read<ptr<f64>>(%5))), const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 c: f64 [storage=automatic];
// DEFAULT-NEXT:         let %9 d: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(%8, read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(3)>(%2), read<i32>(%9)))));
// DEFAULT-NEXT:                     if gt<f64, exceptions=observable>(read<f64>(%8), const<f64>(0.0))
// DEFAULT-NEXT:                         goto %7;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<f64>) -> void>(%3, const<i32>(1), addr_of<ptr<f64>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(1));
// DEFAULT-NEXT:         label %7 e:
// DEFAULT-NEXT:             call<void, signature=fn(i32, ptr<f64>) -> void>(%3, const<i32>(0), addr_of<ptr<f64>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
