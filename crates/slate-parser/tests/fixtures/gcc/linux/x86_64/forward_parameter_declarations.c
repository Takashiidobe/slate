int bounded(int n; int (*rows)[n], int n) { return sizeof(*rows) + n; }

int chained(int n; int (*x)[n]; int (*y)[x[0][1]], int (*x)[n], int n) {
  return sizeof(*y) / sizeof(int);
}

int repeated(int a; int a; int a) { return a; }

int caller(void) {
  int grid[2][3] = {{0, 4, 0}, {0}};
  int wide[2][4];
  return bounded(grid, 3) + chained(wide, grid, 3) + repeated(1);
}

// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     fn %[[VALUE_bounded:[0-9]+]] @bounded(%[[VALUE_rows:[0-9]+]] rows: ptr<vla<i32, %[[VALUE0:[0-9]+]]>>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]]))) [c="__size_t"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]) [c="__size_t"], const<u64>(4) [c="__size_t"]) [c="__size_t"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n]]))) [c="__size_t"]) [c="__size_t"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_chained:[0-9]+]] @chained(%[[VALUE_y:[0-9]+]] y: ptr<vla<i32, %[[VALUE1:[0-9]+]]>>, %[[VALUE_x:[0-9]+]] x: ptr<vla<i32, %[[VALUE2:[0-9]+]]>>, %[[VALUE_n_2:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, *>>, subtract=false, element=vla<i32, *>, overflow=ub>(read<ptr<vla<i32, *>>>(%[[VALUE_x]]), const<i32>(0)))), const<i32>(1)))))) [c="__size_t"];
// DEFAULT-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]]))) [c="__size_t"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]) [c="__size_t"], const<u64>(4) [c="__size_t"]) [c="__size_t"], size_of<u64, i32>(4) [c="__size_t"]) [c="__size_t"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_repeated:[0-9]+]] @repeated(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_grid:[0-9]+]] grid: array<array<i32, 3>, 2> [storage=automatic] [align=16] = aggregate<array<array<i32, 3>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4), index2 = const<i32>(0)), index1 = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: array<array<i32, 4>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<vla<i32, *>>, i32) -> i32>(%[[VALUE_bounded]], pointer_cast<ptr<vla<i32, *>>, reason=arg>(array_decay<ptr<array<i32, 3>>, length=Some(2)>(%[[VALUE_grid]])), const<i32>(3)), call<i32, signature=fn(ptr<vla<i32, *>>, ptr<vla<i32, *>>, i32) -> i32>(%[[VALUE_chained]], pointer_cast<ptr<vla<i32, *>>, reason=arg>(array_decay<ptr<array<i32, 4>>, length=Some(2)>(%[[VALUE_wide]])), pointer_cast<ptr<vla<i32, *>>, reason=arg>(array_decay<ptr<array<i32, 3>>, length=Some(2)>(%[[VALUE_grid]])), const<i32>(3))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_repeated]], const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
