// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c11

unsigned long size(int m, int (*p)[6][m]) {
  return sizeof(*p) + _Alignof(int[6][m]);
}

int step(int n, int m, int a[n][6][m], int (*p)[6][m]) {
  return (p + 1)[0][2][m - 1] - a[1][5][0];
}

int fixed_outer(int m) {
  int a[2][6][m];
  a[1][2][m - 1] = 9;
  return a[1][2][m - 1];
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_size:[0-9]+]] @size(%[[VALUE_m:[0-9]+]] m: i32, %[[VALUE_p:[0-9]+]] p: ptr<array<vla<i32, %[[VALUE0:[0-9]+]]>, 6>>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]]))) [c="__size_t"];
// IR-NEXT:         return reinterpret<u64, reason=return>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(6) [c="__size_t"], mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]) [c="__size_t"], const<u64>(4) [c="__size_t"])) [c="__size_t"], align_of<u64, array<vla<i32, %[[VALUE1:[0-9]+]]>, 6>>(4) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_step:[0-9]+]] @step(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_m_2:[0-9]+]] m: i32, %[[VALUE_a:[0-9]+]] a: ptr<array<vla<i32, %[[VALUE2:[0-9]+]]>, 6>> [array=%[[VALUE3:[0-9]+]]], %[[VALUE_p_2:[0-9]+]] p: ptr<array<vla<i32, %[[VALUE4:[0-9]+]]>, 6>>) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(int, int, int[*][6][*], int ((*)[6])[*])"] {
// IR-NEXT:         let %[[VALUE3]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m_2]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE4]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m_2]]))) [c="__size_t"];
// IR-NEXT:         return sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE4]]>>, subtract=false, element=vla<i32, %[[VALUE4]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE4]]>>, length=Some(6)>(deref(ptr_offset<ptr<array<vla<i32, %[[VALUE4]]>, 6>>, subtract=false, element=array<vla<i32, %[[VALUE4]]>, 6>, overflow=ub>(ptr_offset<ptr<array<vla<i32, %[[VALUE4]]>, 6>>, subtract=false, element=array<vla<i32, %[[VALUE4]]>, 6>, overflow=ub>(read<ptr<array<vla<i32, %[[VALUE4]]>, 6>>>(%[[VALUE_p_2]]), const<i32>(1)), const<i32>(0)))), const<i32>(2)))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_m_2]]), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE2]]>>, subtract=false, element=vla<i32, %[[VALUE2]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE2]]>>, length=Some(6)>(deref(ptr_offset<ptr<array<vla<i32, %[[VALUE2]]>, 6>>, subtract=false, element=array<vla<i32, %[[VALUE2]]>, 6>, overflow=ub>(read<ptr<array<vla<i32, %[[VALUE2]]>, 6>>>(%[[VALUE_a]]), const<i32>(1)))), const<i32>(5)))), const<i32>(0)))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fixed_outer:[0-9]+]] @fixed_outer(%[[VALUE_m_3:[0-9]+]] m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m_3]]))) [c="__size_t"];
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: array<array<vla<i32, %[[VALUE5]]>, 6>, 2> [storage=automatic];
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE5]]>>, subtract=false, element=vla<i32, %[[VALUE5]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE5]]>>, length=Some(6)>(deref(ptr_offset<ptr<array<vla<i32, %[[VALUE5]]>, 6>>, subtract=false, element=array<vla<i32, %[[VALUE5]]>, 6>, overflow=ub>(array_decay<ptr<array<vla<i32, %[[VALUE5]]>, 6>>, length=Some(2)>(%[[VALUE_a_2]]), const<i32>(1)))), const<i32>(2)))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_m_3]]), const<i32>(1)))), const<i32>(9));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE5]]>>, subtract=false, element=vla<i32, %[[VALUE5]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE5]]>>, length=Some(6)>(deref(ptr_offset<ptr<array<vla<i32, %[[VALUE5]]>, 6>>, subtract=false, element=array<vla<i32, %[[VALUE5]]>, 6>, overflow=ub>(array_decay<ptr<array<vla<i32, %[[VALUE5]]>, 6>>, length=Some(2)>(%[[VALUE_a_2]]), const<i32>(1)))), const<i32>(2)))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_m_3]]), const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
