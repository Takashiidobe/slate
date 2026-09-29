/* PR c/29154 */

extern void abort(void);

void foo(int **p, int *q) { *(*p++)++ = *q++; }

void bar(int **p, int *q) {
  **p = *q++;
  *(*p++)++;
}

void baz(int **p, int *q) {
  **p = *q++;
  (*p++)++;
}

int main(void) {
  int  i = 42, j = 0;
  int *p = &i;
  foo(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 43;
  p = &i;
  bar(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 44;
  p = &i;
  baz(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<ptr<i32>>, %[[VALUE_q:[0-9]+]] q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_q]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q]], read<ptr<i32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_p]], read<ptr<ptr<i32>>>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE2]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE4]])));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE4]])), read<ptr<i32>>(%[[VALUE6]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE5]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE0]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<ptr<i32>>, %[[VALUE_q_2:[0-9]+]] q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_q_2]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_2]], read<ptr<i32>>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_p_2]])))), read<i32>(deref(read<ptr<i32>>(%[[VALUE7]]))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_p_2]], read<ptr<ptr<i32>>>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE9]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE11]])));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE11]])), read<ptr<i32>>(%[[VALUE13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p_3:[0-9]+]] p: ptr<ptr<i32>>, %[[VALUE_q_3:[0-9]+]] q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_q_3]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_3]], read<ptr<i32>>(%[[VALUE15]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_p_3]])))), read<i32>(deref(read<ptr<i32>>(%[[VALUE14]]))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE_p_3]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_p_3]], read<ptr<ptr<i32>>>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%[[VALUE16]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE18]])));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE18]])), read<ptr<i32>>(%[[VALUE20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%[[VALUE_foo]], addr_of<ptr<ptr<i32>>>(%[[VALUE_p_4]]), addr_of<ptr<i32>>(%[[VALUE_j]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(1)), addr_of<ptr<i32>>(%[[VALUE_i]])), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(43));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_4]], addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%[[VALUE_bar]], addr_of<ptr<ptr<i32>>>(%[[VALUE_p_4]]), addr_of<ptr<i32>>(%[[VALUE_j]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(1)), addr_of<ptr<i32>>(%[[VALUE_i]])), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(44));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_4]], addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%[[VALUE_baz]], addr_of<ptr<ptr<i32>>>(%[[VALUE_p_4]]), addr_of<ptr<i32>>(%[[VALUE_j]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(1)), addr_of<ptr<i32>>(%[[VALUE_i]])), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
