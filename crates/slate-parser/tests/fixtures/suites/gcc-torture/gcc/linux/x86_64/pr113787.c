void foo(int x, int y, int z, int d, int *buf) {
  for (int i = z; i < y - z; ++i)
    for (int j = 0; j < d; ++j)
      /* buf[x(i+1) + j] = buf[x(i+1)-j-1] */
      buf[i * x + (x - z + j)] = buf[i * x + (x - z - 1 - j)];
}

void bar(int x, int y, int z, int d, int *buf) {
  for (int i = 0; i < d; ++i)
    for (int j = z; j < x - z; ++j)
      /* buf[j+(y+i)*x] = buf[j+(y-1-i)*x] */
      buf[j + (y - z + i) * x] = buf[j + (y - z - 1 - i) * x];
}

__attribute__((noipa)) void baz(int x, int y, int d, int *buf) {
  foo(x, y, 0, d, buf);
  bar(x, y, 0, d, buf);
}

int main(void) {
  int a[] = {1, 2, 3};
  baz(1, 2, 1, a);
  /* foo does:
     buf[1] = buf[0];
     buf[2] = buf[1];

     bar does:
     buf[2] = buf[1]; (no-op)
     so we should have { 1, 1, 1 }.  */
  for (int i = 0; i < 3; i++)
    if (a[i] != 1)
      __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32, %[[VALUE_d:[0-9]+]] d: i32, %[[VALUE_buf:[0-9]+]] buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = read<i32>(%[[VALUE_z]]);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_z]])))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_d]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_buf]]), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_x]])), add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_z]])), read<i32>(%[[VALUE_j]]))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_buf]]), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_x]])), sub<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_z]])), const<i32>(1)), read<i32>(%[[VALUE_j]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_z_2:[0-9]+]] z: i32, %[[VALUE_d_2:[0-9]+]] d: i32, %[[VALUE_buf_2:[0-9]+]] buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_d_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = read<i32>(%[[VALUE_z_2]]);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_z_2]])))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_buf_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_j_2]]), mul<i32, overflow=ub>(add<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_y_2]]), read<i32>(%[[VALUE_z_2]])), read<i32>(%[[VALUE_i_2]])), read<i32>(%[[VALUE_x_2]]))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_buf_2]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_j_2]]), mul<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_y_2]]), read<i32>(%[[VALUE_z_2]])), const<i32>(1)), read<i32>(%[[VALUE_i_2]])), read<i32>(%[[VALUE_x_2]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32, %[[VALUE_d_3:[0-9]+]] d: i32, %[[VALUE_buf_3:[0-9]+]] buf: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i32>) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]]), const<i32>(0), read<i32>(%[[VALUE_d_3]]), read<ptr<i32>>(%[[VALUE_buf_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i32>) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]]), const<i32>(0), read<i32>(%[[VALUE_d_3]]), read<ptr<i32>>(%[[VALUE_buf_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i32>) -> void>(%[[VALUE_baz]], const<i32>(1), const<i32>(2), const<i32>(1), array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_a]]));
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_3]])))), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
