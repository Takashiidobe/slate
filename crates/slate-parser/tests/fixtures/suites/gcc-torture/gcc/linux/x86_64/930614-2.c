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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: array<array<array<array<f32, 2>, 8>, 2>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_l]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_l]]), const<i32>(2))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_l]]);
// DEFAULT-NEXT:                                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])), eq<i32>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_l]])))
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_k]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_l]]))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.8)));
// DEFAULT-NEXT:                                             else
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_k]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_l]]))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.8)));
// DEFAULT-NEXT:                                             if lt<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(8)>(deref(ptr_offset<ptr<array<array<f32, 2>, 8>>, subtract=false, element=array<array<f32, 2>, 8>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 8>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 8>, 2>>, subtract=false, element=array<array<array<f32, 2>, 8>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 8>, 2>>, length=Some(8)>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_k]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_l]]))))), const<f64>(0.0))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
