// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

typedef long T;

int f(int n) {
  typedef int T[n];
  n += 1;
  T a;
  T b = {};
  {
    typedef char T;
    T c = 1;
    n += c;
  }
  return sizeof a + sizeof b + sizeof(T) + n;
}

T g(int n) {
  int a[n] = {};
  return a[0];
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
// IR-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i64;
// IR-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i8;
// IR-NEXT:     type @type[[TYPE_T_3:[0-9]+]] T = vla<i32, %[[VALUE0:[0-9]+]]>;
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: vla<i32, %[[VALUE0]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: vla<i32, %[[VALUE0]]> [storage=automatic] = aggregate<vla<i32, %[[VALUE0]]>, zero_fill=true>();
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// IR-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])));
// IR-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE4]]));
// IR-NEXT:         }
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4)), mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_n]]))))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_n_2:[0-9]+]] n: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: vla<i32, %[[VALUE5]]> [storage=automatic] = aggregate<vla<i32, %[[VALUE5]]>, zero_fill=true>();
// IR-NEXT:         return widen<i64, reason=return>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_a_2]]), const<i32>(0)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
