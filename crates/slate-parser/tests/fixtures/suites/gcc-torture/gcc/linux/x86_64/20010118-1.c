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
// DEFAULT-NEXT:     global %3 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @bar(%1 h: ptr<void>, %2 n: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%12));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @baz(%5 x: ptr<u32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13: ptr<u32> [synthetic] = read<ptr<u32>>(%5);
// DEFAULT-NEXT:         let %14: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%13)));
// DEFAULT-NEXT:         let %15: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%13)), read<u32>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 h: ptr<void>, %8 l: u32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 n: u32 [storage=automatic];
// DEFAULT-NEXT:         let %10 m: i64 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%9, call<u32, signature=fn(ptr<void>, u32) -> u32>(%0, read<ptr<void>>(%7), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<void>, u32) -> u32>(%0, read<ptr<void>>(%7), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(%9, call<u32, signature=fn(ptr<void>, u32) -> u32>(%0, read<ptr<void>>(%7), read<u32>(%9)));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<void>, u32) -> u32>(%0, read<ptr<void>>(%7), read<u32>(%9));
// DEFAULT-NEXT:         let %16: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<void, signature=fn(ptr<u32>) -> void>(%4, addr_of<ptr<u32>>(%9));
// DEFAULT-NEXT:             write<i32>(%16, const<i32>(21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i64>(%10, widen<i64, reason=assign>(read<i32>(%16)));
// DEFAULT-NEXT:         return read<i64>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
