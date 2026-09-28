/* PR middle-end/114157 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23 -Wno-psabi -w" } */

#if __BITINT_MAXWIDTH__ >= 256
_BitInt(256) d;
_BitInt(255) e;

void
foo (long __attribute__((vector_size (64))) s)
{
  __builtin_memmove (&d, &s, sizeof (d));
}

void
bar (_BitInt(512) x)
{
  long __attribute__((vector_size (64))) s;
  __builtin_memcpy (&s, &x, sizeof (s));
  __builtin_memcpy (&d, &s, sizeof (d));
}

void
baz (long __attribute__((vector_size (64))) s)
{
  _BitInt(256) d;
  __builtin_memmove (&d, &s, sizeof (d));
  e = d;
}

void
qux (long __attribute__((vector_size (64))) s)
{
  _BitInt(192) d;
  __builtin_memmove (&d, &s, sizeof (d));
  e = d;
}
#else
int i;
#endif

#if __BITINT_MAXWIDTH__ >= 1024
_BitInt(512)
corge (long __attribute__((vector_size (1024))) s)
{
  _BitInt(512) d;
  __builtin_memcpy (&d, &s, sizeof (d));
  return d;
}
#endif

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %0 d: i256b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 e: i255b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_memmove(%16 <unnamed>: ptr<void>, %17 <unnamed>: ptr<const void>, %18 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%19, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%0)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%3)), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_memcpy(%20 <unnamed>: ptr<void>, %21 <unnamed>: ptr<const void>, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%5 x: i512b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 s: vector<i64, 8> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%6)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i512b>>(%5)), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%0)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%6)), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%8 s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 d: i256b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%19, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%8)), const<u64>(32));
// DEFAULT-NEXT:         write<i255b>(%1, truncate<i255b, reason=assign, fits=unknown>(read<i256b>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @qux(%11 s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 d: i192b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%19, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i192b>>(%12)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%11)), const<u64>(24));
// DEFAULT-NEXT:         write<i255b>(%1, widen<i255b, reason=assign>(read<i192b>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @corge(%14 s: vector<i64, 128>) -> i512b [linkage=external] [abi=sysv64(byval<align=1024>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 d: i512b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i512b>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 128>>>(%14)), const<u64>(64));
// DEFAULT-NEXT:         return read<i512b>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
