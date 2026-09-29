void abort(void);
void exit(int);

int g(unsigned long long int *v, int n, unsigned int a[], int b) {
  int cnt;
  *v = 0;
  for (cnt = 0; cnt < n; ++cnt)
    *v = *v * b + a[cnt];
  return n;
}

int main(void) {
  int                    res;
  unsigned int           ar[] = {10, 11, 12, 13, 14};
  unsigned long long int v;

  res = g(&v, sizeof(ar) / sizeof(ar[0]), ar, 16);
  if (v != 0xabcdeUL)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_v:[0-9]+]] v: ptr<u64>, %[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_a:[0-9]+]] a: ptr<u32>, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_v]])), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_cnt]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_cnt]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u64>(deref(read<ptr<u64>>(%[[VALUE_v]])), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(deref(read<ptr<u64>>(%[[VALUE_v]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_b]])))), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_cnt]])))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ar:[0-9]+]] ar: array<u32, 5> [storage=automatic] [align=16] = aggregate<array<u32, 5>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(10)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(11)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(12)), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(13)), index4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(14)));
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: u64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_res]], call<i32, signature=fn(ptr<u64>, i32, ptr<u32>, i32) -> i32>(%[[VALUE_g]], addr_of<ptr<u64>>(%[[VALUE_v_2]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(const<u64>(20), const<u64>(4)))), array_decay<ptr<u32>, length=Some(5)>(%[[VALUE_ar]]), const<i32>(16)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, i32, ptr<u32>, i32) -> i32>(%[[VALUE_g]], addr_of<ptr<u64>>(%[[VALUE_v_2]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(const<u64>(20), const<u64>(4)))), array_decay<ptr<u32>, length=Some(5)>(%[[VALUE_ar]]), const<i32>(16));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_v_2]]), const<u64>(703710))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
