void abort(void);
void exit(int);

int main(void) {
  int   i, j, k, l;
  float x[8][2][8][2];

  for (i = 0; i < 8; i++)
    for (j = i; j < 8; j++)
      for (k = 0; k < 2; k++)
        for (l = 0; l < 2; l++) {
          if ((i == j) && (k == l))
            x[i][k][j][l] = 0.8;
          else
            x[i][k][j][l] = 0.8;
          if (x[i][k][j][l] < 0.0)
            abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 x: array<array<array<array<f32, 2>, 8>, 2>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %10
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%3));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%4), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%16));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %11
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%5), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %17: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%5, read<i32>(%18));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 for %12
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%6), const<i32>(2))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %19: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                                         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%6, read<i32>(%20));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             if logical_and<bool>(eq<i32>(read<i32>(%3), read<i32>(%4)), eq<i32>(read<i32>(%5), read<i32>(%6)))
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%7), read<i32>(%3)))), read<i32>(%5)))), read<i32>(%4)))), read<i32>(%6))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.8)));
// DEFAULT-NEXT:                                             else
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%7), read<i32>(%3)))), read<i32>(%5)))), read<i32>(%4)))), read<i32>(%6))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.8)));
// DEFAULT-NEXT:                                             if lt<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%7), read<i32>(%3)))), read<i32>(%5)))), read<i32>(%4)))), read<i32>(%6))))), const<f64>(0.0))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
