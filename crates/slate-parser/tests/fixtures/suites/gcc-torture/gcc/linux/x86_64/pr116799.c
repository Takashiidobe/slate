/* PR rtl-optimization/116799 */

const char *l;

__attribute__((noipa)) void foo(const char *x, const char *y, int z) {
  if (x != l + 1 || y != x || z)
    __builtin_abort();
}

__attribute__((noipa)) void bar(const char *x, char *v) {
  const char *w = x + __builtin_strlen(x);

  while (x[0] == '*' && x < w - 1)
    x++;

  const char *y = w - 1;
  int         z = 1;
  if (y >= x) {
    while (y - x > 0 && *y == '*')
      y--;
    z = 0;
  }
  int i = 0;
  if (z)
    v[i++] = 'a';
  v[i] = 'b';
  foo(x, y, z);
}

int main() {
  char v[2] = {0};
  l         = "**";
  bar(l, v);
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
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: ptr<const i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([42, 42, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<const i8>, %[[VALUE_y:[0-9]+]] y: ptr<const i8>, %[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_x]]), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(1))), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_y]]), read<ptr<const i8>>(%[[VALUE_x]]))), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: ptr<const i8>, %[[VALUE_v:[0-9]+]] v: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: ptr<const i8> [storage=automatic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_x_2]]), call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], read<ptr<const i8>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_x_2]]), const<i32>(0))))), const<i32>(42)), lt<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_x_2]]), ptr_offset<ptr<const i8>, subtract=true, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_w]]), const<i32>(1))))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<const i8>>(%[[VALUE_x_2]], read<ptr<const i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: ptr<const i8> [storage=automatic] = ptr_offset<ptr<const i8>, subtract=true, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_w]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         if ge<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_y_2]]), read<ptr<const i8>>(%[[VALUE_x_2]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE4:[0-9]+]] logical_and<bool>(gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<const i8>>(%[[VALUE_y_2]]), read<ptr<const i8>>(%[[VALUE_x_2]])), widen<i64, reason=usual_arith>(const<i32>(0))), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_y_2]])))), const<i32>(42)))
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_y_2]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=true, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_y_2]], read<ptr<const i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_z_2]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_z_2]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_v]]), read<i32>(%[[VALUE7]]))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_v]]), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> void>(%[[VALUE_foo]], read<ptr<const i8>>(%[[VALUE_x_2]]), read<ptr<const i8>>(%[[VALUE_y_2]]), read<i32>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: array<i8, 2> [storage=automatic] = aggregate<array<i8, 2>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_l]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<i8>) -> void>(%[[VALUE_bar]], read<ptr<const i8>>(%[[VALUE_l]]), array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
