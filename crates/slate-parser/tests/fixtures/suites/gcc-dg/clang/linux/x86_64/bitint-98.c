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
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i256b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i255b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memmove:[0-9]+]] @__builtin_memmove(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%[[VALUE_d]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%[[VALUE_s]])), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i512b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: vector<i64, 8> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%[[VALUE_s_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i512b>>(%[[VALUE_x]])), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%[[VALUE_d]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%[[VALUE_s_2]])), const<u64>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_s_3:[0-9]+]] s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: i256b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i256b>>(%[[VALUE_d_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%[[VALUE_s_3]])), const<u64>(32));
// DEFAULT-NEXT:         write<i255b>(%[[VALUE_e]], truncate<i255b, reason=assign, fits=unknown>(read<i256b>(%[[VALUE_d_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_s_4:[0-9]+]] s: vector<i64, 8>) -> void [linkage=external] [abi=sysv64(byval<align=64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: i192b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memmove]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i192b>>(%[[VALUE_d_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 8>>>(%[[VALUE_s_4]])), const<u64>(24));
// DEFAULT-NEXT:         write<i255b>(%[[VALUE_e]], widen<i255b, reason=assign>(read<i192b>(%[[VALUE_d_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_corge:[0-9]+]] @corge(%[[VALUE_s_5:[0-9]+]] s: vector<i64, 128>) -> i512b [linkage=external] [abi=sysv64(byval<align=1024>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d_4:[0-9]+]] d: i512b [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i512b>>(%[[VALUE_d_4]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i64, 128>>>(%[[VALUE_s_5]])), const<u64>(64));
// DEFAULT-NEXT:         return read<i512b>(%[[VALUE_d_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
