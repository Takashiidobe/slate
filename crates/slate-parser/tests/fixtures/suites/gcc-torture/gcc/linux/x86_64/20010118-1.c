// SLATE-FILECHECK-DEFINES DEFAULT

static unsigned int bar(void *h, unsigned int n)
{
  static int i;
  return i++;
}

static void baz(unsigned int *x)
{
  (*x)++;
}

long
foo(void *h, unsigned int l)
{
  unsigned int n;
  long m;
  n = bar(h, 0);
  n = bar(h, n);
  m = ({ baz(&n); 21; });
  return m;
}

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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_h:[0-9]+]] h: ptr<void>, %[[VALUE_n:[0-9]+]] n: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x:[0-9]+]] x: ptr<u32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE2]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE2]])), read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_h_2:[0-9]+]] h: ptr<void>, %[[VALUE_l:[0-9]+]] l: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i64 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_n_2]], call<u32, signature=fn(ptr<void>, u32) -> u32>(%[[VALUE_bar]], read<ptr<void>>(%[[VALUE_h_2]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_n_2]], call<u32, signature=fn(ptr<void>, u32) -> u32>(%[[VALUE_bar]], read<ptr<void>>(%[[VALUE_h_2]]), read<u32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<void, signature=fn(ptr<u32>) -> void>(%[[VALUE_baz]], addr_of<ptr<u32>>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE5]], const<i32>(21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i64>(%[[VALUE_m]], widen<i64, reason=assign>(read<i32>(%[[VALUE5]])));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_m]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
