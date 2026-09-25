/* PR middle-end/77718 */

char a[64] __attribute__((aligned(8)));

__attribute__((noinline, noclone)) int foo(void) {
  return __builtin_memcmp("bbbbbb", a, 6);
}

__attribute__((noinline, noclone)) int bar(void) {
  return __builtin_memcmp(a, "bbbbbb", 6);
}

int main() {
  __builtin_memset(a, 'a', sizeof(a));
  if (((foo() < 0) ^ ('a' > 'b')) || ((bar() < 0) ^ ('a' < 'b')))
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: array<i8, 64> [storage=static] [align=8] [linkage=external];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([98, 98, 98, 98, 98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([98, 98, 98, 98, 98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%4)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%0)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%5)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%0)), const<i32>(97), const<u64>(64));
// DEFAULT-NEXT:         let %6: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0))), from_bool<i32, reason=promotion>(gt<i32>(const<i32>(97), const<i32>(98)))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%6, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%6, ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0))), from_bool<i32, reason=promotion>(lt<i32>(const<i32>(97), const<i32>(98)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
