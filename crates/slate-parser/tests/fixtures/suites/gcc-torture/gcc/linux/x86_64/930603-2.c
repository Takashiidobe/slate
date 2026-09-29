void abort(void);
void exit(int);

int w[2][2];

void f(void) {
  int i, j;

  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      if (i == j)
        w[i][j] = 1;
}

int main(void) {
  f();
  if (w[0][0] != 1 || w[1][1] != 1 || w[1][0] != 0 || w[0][1] != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: array<array<i32, 2>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]))
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>, length=Some(2)>(%[[VALUE_w]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]]))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_w]]), const<i32>(0)))), const<i32>(0)))), const<i32>(1)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_w]]), const<i32>(1)))), const<i32>(1)))), const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_w]]), const<i32>(1)))), const<i32>(0)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(deref(ptr_offset<ptr<array<i32, 2>>, subtract=false, element=array<i32, 2>, overflow=ub>(array_decay<ptr<array<i32, 2>>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_w]]), const<i32>(0)))), const<i32>(1)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
