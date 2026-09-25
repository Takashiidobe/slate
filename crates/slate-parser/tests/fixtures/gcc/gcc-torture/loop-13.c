/* PR opt/7130 */
void abort(void);
#define TYPE long

void scale(TYPE *alpha, TYPE *x, int n) {
  int i, ix;

  if (*alpha != 1)
    for (i = 0, ix = 0; i < n; i++, ix += 2) {
      TYPE tmpr, tmpi;
      tmpr      = *alpha * x[ix];
      tmpi      = *alpha * x[ix + 1];
      x[ix]     = tmpr;
      x[ix + 1] = tmpi;
    }
}

int main(void) {
  int  i;
  TYPE x[10];
  TYPE alpha = 2;

  for (i = 0; i < 10; i++)
    x[i] = i;

  scale(&alpha, x, 5);

  if (x[9] != 18)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @scale(%2 alpha: ptr<i64>, %3 x: ptr<i64>, %4 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 ix: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(read<i64>(deref(read<ptr<i64>>(%2))), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             for %13
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%5), read<i32>(%4))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%5, read<i32>(%16));
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %7 tmpr: i64 [storage=automatic];
// DEFAULT-NEXT:                         let %8 tmpi: i64 [storage=automatic];
// DEFAULT-NEXT:                         write<i64>(%7, mul<i64, overflow=ub>(read<i64>(deref(read<ptr<i64>>(%2))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%3), read<i32>(%6))))));
// DEFAULT-NEXT:                         write<i64>(%8, mul<i64, overflow=ub>(read<i64>(deref(read<ptr<i64>>(%2))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%3), add<i32, overflow=ub>(read<i32>(%6), const<i32>(1)))))));
// DEFAULT-NEXT:                         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%3), read<i32>(%6))), read<i64>(%7));
// DEFAULT-NEXT:                         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%3), add<i32, overflow=ub>(read<i32>(%6), const<i32>(1)))), read<i64>(%8));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 x: array<i64, 10> [storage=automatic];
// DEFAULT-NEXT:         let %12 alpha: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(2));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%11), read<i32>(%10))), widen<i64, reason=assign>(read<i32>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, ptr<i64>, i32) -> void>(%1, addr_of<ptr<i64>>(%12), array_decay<ptr<i64>, length=Some(10)>(%11), const<i32>(5));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%11), const<i32>(9)))), widen<i64, reason=usual_arith>(const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
