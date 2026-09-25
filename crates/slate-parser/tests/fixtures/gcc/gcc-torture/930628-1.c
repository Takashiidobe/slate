void abort(void);
void exit(int);

void f(double x[2], double y[2]) {
  if (x == y)
    abort();
}

int main(void) {
  struct {
    int    f[3];
    double x[1][2];
  } tp[4][2];
  int   i, j, ki, kj, mi, mj;
  float bdm[4][2][4][2];

  for (i = 0; i < 4; i++)
    for (j = i; j < 4; j++)
      for (ki = 0; ki < 2; ki++)
        for (kj = 0; kj < 2; kj++)
          if ((j == i) && (ki == kj))
            bdm[i][ki][j][kj] = 1000.0;
          else {
            for (mi = 0; mi < 1; mi++)
              for (mj = 0; mj < 1; mj++)
                f(tp[i][ki].x[mi], tp[j][kj].x[mj]);
            bdm[i][ki][j][kj] = 1000.0;
          }
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 f: array<i32, 3>;
// DEFAULT-NEXT:         field1 x: array<array<f64, 2>, 1>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 x: ptr<f64> [array=2], %4 y: ptr<f64> [array=2]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<f64>>(read<ptr<f64>>(%3), read<ptr<f64>>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 tp: array<array<@type0, 2>, 4> [storage=automatic];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 ki: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 kj: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 mi: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 mj: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 bdm: array<array<array<array<f32, 2>, 4>, 2>, 4> [storage=automatic];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %17
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%9, read<i32>(%8));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%9), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%9, read<i32>(%25));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %18
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%10), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %26: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%10, read<i32>(%27));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 for %19
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%11), const<i32>(2))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %28: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                                         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%11, read<i32>(%29));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         if logical_and<bool>(eq<i32>(read<i32>(%9), read<i32>(%8)), eq<i32>(read<i32>(%10), read<i32>(%11)))
// DEFAULT-NEXT:                                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(4)>(deref(ptr_offset<ptr<array<array<f32, 2>, 4>>, subtract=false, element=array<array<f32, 2>, 4>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 4>, 2>>, subtract=false, element=array<array<array<f32, 2>, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 4>, 2>>, length=Some(4)>(%14), read<i32>(%8)))), read<i32>(%10)))), read<i32>(%9)))), read<i32>(%11))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1000.0)));
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 for %20
// DEFAULT-NEXT:                                                     init:
// DEFAULT-NEXT:                                                         write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                                                     condition: lt<i32>(read<i32>(%12), const<i32>(1))
// DEFAULT-NEXT:                                                     increment: {
// DEFAULT-NEXT:                                                         let %30: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                                         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                                                         write<i32>(%12, read<i32>(%31));
// DEFAULT-NEXT:                                                         yield void;
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                     body:
// DEFAULT-NEXT:                                                         for %21
// DEFAULT-NEXT:                                                             init:
// DEFAULT-NEXT:                                                                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                                                             condition: lt<i32>(read<i32>(%13), const<i32>(1))
// DEFAULT-NEXT:                                                             increment: {
// DEFAULT-NEXT:                                                                 let %32: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                                                                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                                                                 write<i32>(%13, read<i32>(%33));
// DEFAULT-NEXT:                                                                 yield void;
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                             body:
// DEFAULT-NEXT:                                                                 call<void, signature=fn(ptr<f64>, ptr<f64>) -> void>(%2, array_decay<ptr<f64>, length=Some(2)>(deref(ptr_offset<ptr<array<f64, 2>>, subtract=false, element=array<f64, 2>, overflow=ub>(array_decay<ptr<array<f64, 2>>, length=Some(1)>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(deref(ptr_offset<ptr<array<@type0, 2>>, subtract=false, element=array<@type0, 2>, overflow=ub>(array_decay<ptr<array<@type0, 2>>, length=Some(4)>(%7), read<i32>(%8)))), read<i32>(%10))))), read<i32>(%12)))), array_decay<ptr<f64>, length=Some(2)>(deref(ptr_offset<ptr<array<f64, 2>>, subtract=false, element=array<f64, 2>, overflow=ub>(array_decay<ptr<array<f64, 2>>, length=Some(1)>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(deref(ptr_offset<ptr<array<@type0, 2>>, subtract=false, element=array<@type0, 2>, overflow=ub>(array_decay<ptr<array<@type0, 2>>, length=Some(4)>(%7), read<i32>(%9)))), read<i32>(%11))))), read<i32>(%13)))));
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(4)>(deref(ptr_offset<ptr<array<array<f32, 2>, 4>>, subtract=false, element=array<array<f32, 2>, 4>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 4>, 2>>, subtract=false, element=array<array<array<f32, 2>, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 4>, 2>>, length=Some(4)>(%14), read<i32>(%8)))), read<i32>(%10)))), read<i32>(%9)))), read<i32>(%11))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1000.0)));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
