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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 f: array<i32, 3>;
// DEFAULT-NEXT:         field1 x: array<array<f64, 2>, 1>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: ptr<f64> [array=2], %[[VALUE_y:[0-9]+]] y: ptr<f64> [array=2]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<f64>>(read<ptr<f64>>(%[[VALUE_x]]), read<ptr<f64>>(%[[VALUE_y]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_tp:[0-9]+]] tp: array<array<@type[[TYPE0]], 2>, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ki:[0-9]+]] ki: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_kj:[0-9]+]] kj: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mi:[0-9]+]] mi: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mj:[0-9]+]] mj: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bdm:[0-9]+]] bdm: array<array<array<array<f32, 2>, 4>, 2>, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
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
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_ki]], const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_ki]]), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ki]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_ki]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_kj]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_kj]]), const<i32>(2))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_kj]]);
// DEFAULT-NEXT:                                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_kj]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_i]])), eq<i32>(read<i32>(%[[VALUE_ki]]), read<i32>(%[[VALUE_kj]])))
// DEFAULT-NEXT:                                             write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(4)>(deref(ptr_offset<ptr<array<array<f32, 2>, 4>>, subtract=false, element=array<array<f32, 2>, 4>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 4>, 2>>, subtract=false, element=array<array<array<f32, 2>, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 4>, 2>>, length=Some(4)>(%[[VALUE_bdm]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_ki]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_kj]]))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1000.0)));
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                                     init:
// DEFAULT-NEXT:                                                         write<i32>(%[[VALUE_mi]], const<i32>(0));
// DEFAULT-NEXT:                                                     condition: lt<i32>(read<i32>(%[[VALUE_mi]]), const<i32>(1))
// DEFAULT-NEXT:                                                     increment: {
// DEFAULT-NEXT:                                                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_mi]]);
// DEFAULT-NEXT:                                                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                                                         write<i32>(%[[VALUE_mi]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                                                         yield void;
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                     body:
// DEFAULT-NEXT:                                                         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                                                             init:
// DEFAULT-NEXT:                                                                 write<i32>(%[[VALUE_mj]], const<i32>(0));
// DEFAULT-NEXT:                                                             condition: lt<i32>(read<i32>(%[[VALUE_mj]]), const<i32>(1))
// DEFAULT-NEXT:                                                             increment: {
// DEFAULT-NEXT:                                                                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_mj]]);
// DEFAULT-NEXT:                                                                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                                                                 write<i32>(%[[VALUE_mj]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                                                                 yield void;
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                             body:
// DEFAULT-NEXT:                                                                 call<void, signature=fn(ptr<f64>, ptr<f64>) -> void>(%[[VALUE_f]], array_decay<ptr<f64>, length=Some(2)>(deref(ptr_offset<ptr<array<f64, 2>>, subtract=false, element=array<f64, 2>, overflow=ub>(array_decay<ptr<array<f64, 2>>, length=Some(1)>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(2)>(deref(ptr_offset<ptr<array<@type[[TYPE0]], 2>>, subtract=false, element=array<@type[[TYPE0]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE0]], 2>>, length=Some(4)>(%[[VALUE_tp]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_ki]]))))), read<i32>(%[[VALUE_mi]])))), array_decay<ptr<f64>, length=Some(2)>(deref(ptr_offset<ptr<array<f64, 2>>, subtract=false, element=array<f64, 2>, overflow=ub>(array_decay<ptr<array<f64, 2>>, length=Some(1)>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(2)>(deref(ptr_offset<ptr<array<@type[[TYPE0]], 2>>, subtract=false, element=array<@type[[TYPE0]], 2>, overflow=ub>(array_decay<ptr<array<@type[[TYPE0]], 2>>, length=Some(4)>(%[[VALUE_tp]]), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_kj]]))))), read<i32>(%[[VALUE_mj]])))));
// DEFAULT-NEXT:                                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(2)>(deref(ptr_offset<ptr<array<f32, 2>>, subtract=false, element=array<f32, 2>, overflow=ub>(array_decay<ptr<array<f32, 2>>, length=Some(4)>(deref(ptr_offset<ptr<array<array<f32, 2>, 4>>, subtract=false, element=array<array<f32, 2>, 4>, overflow=ub>(array_decay<ptr<array<array<f32, 2>, 4>>, length=Some(2)>(deref(ptr_offset<ptr<array<array<array<f32, 2>, 4>, 2>>, subtract=false, element=array<array<array<f32, 2>, 4>, 2>, overflow=ub>(array_decay<ptr<array<array<array<f32, 2>, 4>, 2>>, length=Some(4)>(%[[VALUE_bdm]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_ki]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_kj]]))), float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1000.0)));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
