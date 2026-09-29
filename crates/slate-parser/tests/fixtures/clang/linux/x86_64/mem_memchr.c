#include <stdio.h>

int main(void) {
  unsigned char       buf[8]    = {10, 20, 30, 40, 50, 60, 70, 80};
  const unsigned char cbuf[4]   = {9, 8, 7, 6};
  char                word[]    = "abc";
  int                 needle    = (int)buf[3];
  size_t volatile partial_count = 4;
  unsigned char *hit        = (unsigned char *)__builtin_memchr(buf, needle, 8);
  unsigned char *miss       = (unsigned char *)__builtin_memchr(buf, 99, 8);
  unsigned char *zero       = (unsigned char *)__builtin_memchr(buf, 10, 0);
  char          *nul_after  = (char *)__builtin_memchr(word, 0, sizeof word);
  char          *nul_equal  = (char *)__builtin_memchr(word, 0, 3);
  char          *nul_before = (char *)__builtin_memchr(word, 0, 2);
  unsigned char *partial =
      (unsigned char *)__builtin_memchr(buf, needle, partial_count);
  unsigned char *offset_base = buf + 2;
  unsigned char *offset =
      (unsigned char *)__builtin_memchr(offset_base, needle, 2);
  const unsigned char *const_hit =
      (const unsigned char *)__builtin_memchr(cbuf, 7, sizeof cbuf);
  printf("%ld %d %d %ld %d %d %ld %ld %ld\n", (long)(hit - buf), miss == 0,
         zero == 0, (long)(nul_after - word), nul_equal == 0, nul_before == 0,
         (long)(partial - buf), (long)(offset - offset_base),
         (long)(const_hit - cbuf));
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([37, 108, 100, 32, 37, 100, 32, 37, 100, 32, 37, 108, 100, 32, 37, 100, 32, 37, 100, 32, 37, 108, 100, 32, 37, 108, 100, 32, 37, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memchr:[0-9]+]] @__builtin_memchr(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(10))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(20))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(30))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(40))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(60))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(70))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(80))));
// DEFAULT-NEXT:         let %[[VALUE_cbuf:[0-9]+]] cbuf: array<u8, 4> [storage=automatic] [const] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(9))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))));
// DEFAULT-NEXT:         let %[[VALUE_word:[0-9]+]] word: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %[[VALUE_needle:[0-9]+]] needle: i32 [storage=automatic] = reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]]), const<i32>(3))))));
// DEFAULT-NEXT:         let %[[VALUE_partial_count:[0-9]+]] partial_count: volatile u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_hit:[0-9]+]] hit: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), read<i32>(%[[VALUE_needle]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_miss:[0-9]+]] miss: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_zero:[0-9]+]] zero: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), const<i32>(10), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE_nul_after:[0-9]+]] nul_after: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_word]])), const<i32>(0), const<u64>(4)));
// DEFAULT-NEXT:         let %[[VALUE_nul_equal:[0-9]+]] nul_equal: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_word]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         let %[[VALUE_nul_before:[0-9]+]] nul_before: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_word]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE_partial:[0-9]+]] partial: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), read<i32>(%[[VALUE_needle]]), read<u64, volatile>(%[[VALUE_partial_count]])));
// DEFAULT-NEXT:         let %[[VALUE_offset_base:[0-9]+]] offset_base: ptr<u8> [storage=automatic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_offset:[0-9]+]] offset: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<u8>>(%[[VALUE_offset_base]])), read<i32>(%[[VALUE_needle]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE_const_hit:[0-9]+]] const_hit: ptr<const u8> [storage=automatic] = pointer_cast<ptr<const u8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const u8>, length=Some(4)>(%[[VALUE_cbuf]])), const<i32>(7), const<u64>(4)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str]])), ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<u8>>(%[[VALUE_hit]]), array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), from_bool<i32, reason=vararg>(eq<ptr<u8>>(read<ptr<u8>>(%[[VALUE_miss]]), null<ptr<u8>>)), from_bool<i32, reason=vararg>(eq<ptr<u8>>(read<ptr<u8>>(%[[VALUE_zero]]), null<ptr<u8>>)), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%[[VALUE_nul_after]]), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_word]])), from_bool<i32, reason=vararg>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_nul_equal]]), null<ptr<i8>>)), from_bool<i32, reason=vararg>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_nul_before]]), null<ptr<i8>>)), ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<u8>>(%[[VALUE_partial]]), array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_buf]])), ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<u8>>(%[[VALUE_offset]]), read<ptr<u8>>(%[[VALUE_offset_base]])), ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<const u8>>(%[[VALUE_const_hit]]), array_decay<ptr<const u8>, length=Some(4)>(%[[VALUE_cbuf]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
