extern void abort(void);

static int inv_J(int a[][2]) {
  int i, j;
  int det = 0.0;
  for (j = 0; j < 2; ++j)
    det += a[j][0] + a[j][1];
  return det;
}

int foo() {
  int mat[2][2];
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[1][0] = 4;
  mat[1][1] = 8;
  return inv_J(mat);
}

int main() {
  if (foo() != 15)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_inv_J:[0-9]+]] @inv_J(%[[VALUE_a:[0-9]+]] a: ptr<array<i32, 2>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_det:[0-9]+]] det: i32 [storage=automatic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(0.0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_det]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(read<ptr<array<i32, 2>>>(%[[VALUE_a]]), read<i32>(%[[VALUE_j]])))), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(read<ptr<array<i32, 2>>>(%[[VALUE_a]]), read<i32>(%[[VALUE_j]])))), const<i32>(1))))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_det]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_det]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_mat:[0-9]+]] mat: array<array<i32, 2>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_mat]]), const<i32>(0)))), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_mat]]), const<i32>(0)))), const<i32>(1))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_mat]]), const<i32>(1)))), const<i32>(0))), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_mat]]), const<i32>(1)))), const<i32>(1))), const<i32>(8));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<array<i32, 2>>) -> i32>(%[[VALUE_inv_J]], array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_mat]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
