int i;

__attribute__((noinline, noclone)) void bar(char *p) {
  if (i < 1 || i > 6)
    __builtin_abort();
  if (__builtin_memcmp(p, "abcdefg", i + 1) != 0)
    __builtin_abort();
  __builtin_memset(p, ' ', 7);
}

__attribute__((noinline, noclone)) void foo(char *p, unsigned long l) {
  if (l < 1 || l > 6)
    return;
  char buf[7];
  __builtin_memcpy(buf, p, l + 1);
  bar(buf);
}

int main() {
  for (i = 0; i < 16; i++)
    foo("abcdefghijklmnop", i);
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
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([97, 98, 99, 100, 101, 102, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @bar(%2 p: ptr<i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%0), const<i32>(1)), gt<i32>(read<i32>(%0), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%2)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%8)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%0), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%2)), const<i32>(32), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 p: ptr<i8>, %5 l: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), gt<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         let %6 buf: array<i8, 7> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%6)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%4)), add<u64, overflow=wrap>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%1, array_decay<ptr<i8>, length=Some(7)>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%0), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>, u64) -> void>(%3, array_decay<ptr<i8>, length=Some(17)>(%10), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%0))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
