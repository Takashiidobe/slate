int f(int **, int *, int *, int **, int **) __attribute__((__noinline__));
int f(int **ipp, int *i1p, int *i2p, int **i3, int **i4) {
  **ipp = *i1p;
  *ipp  = i2p;
  *i3   = *i4;
  **ipp = 99;
  return 3;
}

extern void exit(int);
extern void abort(void);

int main(void) {
  int  i = 42, i1 = 66, i2 = 1, i3 = -1, i4 = 55;
  int *ip  = &i;
  int *i3p = &i3;
  int *i4p = &i4;

  f(&ip, &i1, &i2, &i3p, &i4p);
  if (i != 66 || ip != &i2 || i2 != 99 || i3 != -1 || i3p != i4p || i4 != 55)
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_ipp:[0-9]+]] ipp: ptr<ptr<i32>>, %[[VALUE_i1p:[0-9]+]] i1p: ptr<i32>, %[[VALUE_i2p:[0-9]+]] i2p: ptr<i32>, %[[VALUE_i3:[0-9]+]] i3: ptr<ptr<i32>>, %[[VALUE_i4:[0-9]+]] i4: ptr<ptr<i32>>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_ipp]])))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_i1p]]))));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_ipp]])), read<ptr<i32>>(%[[VALUE_i2p]]));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_i3]])), read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_i4]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_ipp]])))), const<i32>(99));
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic] = const<i32>(66);
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_i3_2:[0-9]+]] i3: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i4_2:[0-9]+]] i4: i32 [storage=automatic] = const<i32>(55);
// DEFAULT-NEXT:         let %[[VALUE_ip:[0-9]+]] ip: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE_i3p:[0-9]+]] i3p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_i3_2]]);
// DEFAULT-NEXT:         let %[[VALUE_i4p:[0-9]+]] i4p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_i4_2]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<ptr<i32>>, ptr<i32>, ptr<i32>, ptr<ptr<i32>>, ptr<ptr<i32>>) -> i32>(%[[VALUE_f]], addr_of<ptr<ptr<i32>>>(%[[VALUE_ip]]), addr_of<ptr<i32>>(%[[VALUE_i1]]), addr_of<ptr<i32>>(%[[VALUE_i2]]), addr_of<ptr<ptr<i32>>>(%[[VALUE_i3p]]), addr_of<ptr<ptr<i32>>>(%[[VALUE_i4p]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(66)), ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ip]]), addr_of<ptr<i32>>(%[[VALUE_i2]]))), ne<i32>(read<i32>(%[[VALUE_i2]]), const<i32>(99))), ne<i32>(read<i32>(%[[VALUE_i3_2]]), neg<i32, overflow=ub>(const<i32>(1)))), ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i3p]]), read<ptr<i32>>(%[[VALUE_i4p]]))), ne<i32>(read<i32>(%[[VALUE_i4_2]]), const<i32>(55)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
