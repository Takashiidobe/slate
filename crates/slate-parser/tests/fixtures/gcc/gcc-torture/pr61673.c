/* PR rtl-optimization/61673 */

char e;

__attribute__((noinline, noclone)) void bar(char x) {
  if (x != 0x54 && x != (char)0x87)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void foo(const char *x) {
  char d = x[0];
  int  c = d;
  if ((c >= 0 && c <= 0x7f) == 0)
    e = d;
  bar(d);
}

__attribute__((noinline, noclone)) void baz(const char *x) {
  char d = x[0];
  int  c = d;
  if ((c >= 0 && c <= 0x7f) == 0)
    e = d;
}

int main() {
  const char c[] = {0x54, 0x87};
  e              = 0x21;
  foo(c);
  if (e != 0x21)
    __builtin_abort();
  foo(c + 1);
  if (e != (char)0x87)
    __builtin_abort();
  e = 0x21;
  baz(c);
  if (e != 0x21)
    __builtin_abort();
  baz(c + 1);
  if (e != (char)0x87)
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
// DEFAULT-NEXT:     global %0 e: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%2 x: i8) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(84)), ne<i32>(widen<i32, reason=promotion>(read<i8>(%2)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(135)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 x: ptr<const i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 d: i8 [storage=automatic] = read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%4), const<i32>(0))));
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic] = widen<i32, reason=assign>(read<i8>(%5));
// DEFAULT-NEXT:         if eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<i32>(read<i32>(%6), const<i32>(0)), le<i32>(read<i32>(%6), const<i32>(127)))), const<i32>(0))
// DEFAULT-NEXT:             write<i8>(%0, read<i8>(%5));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%1, read<i8>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%8 x: ptr<const i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 d: i8 [storage=automatic] = read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%8), const<i32>(0))));
// DEFAULT-NEXT:         let %10 c: i32 [storage=automatic] = widen<i32, reason=assign>(read<i8>(%9));
// DEFAULT-NEXT:         if eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<i32>(read<i32>(%10), const<i32>(0)), le<i32>(read<i32>(%10), const<i32>(127)))), const<i32>(0))
// DEFAULT-NEXT:             write<i8>(%0, read<i8>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 c: array<i8, 2> [storage=automatic] [const] = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(84)), index1 = truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<i8>(%0, truncate<i8, reason=assign, fits=always>(const<i32>(33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%3, array_decay<ptr<const i8>, length=Some(2)>(%12));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(33))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%3, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(2)>(%12), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(135))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i8>(%0, truncate<i8, reason=assign, fits=always>(const<i32>(33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%7, array_decay<ptr<const i8>, length=Some(2)>(%12));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), const<i32>(33))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%7, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(2)>(%12), const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%0)), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(135))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
