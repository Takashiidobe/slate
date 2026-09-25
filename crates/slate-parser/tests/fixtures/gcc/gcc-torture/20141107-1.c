#define bool _Bool

bool f(int a, bool c) __attribute__((noinline));
bool f(int a, bool c) {
  if (!a)
    c = !c;
  return c;
}

void checkf(int a, bool b) {
  bool c = f(a, b);
  char d;
  __builtin_memcpy(&d, &c, 1);
  if (d != (a == 0) ^ b)
    __builtin_abort();
}

int main(void) {
  checkf(0, 0);
  checkf(0, 1);
  checkf(1, 1);
  checkf(1, 0);
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
// DEFAULT-NEXT:     fn %0 @f(%1 a: i32, %2 c: bool) -> bool [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%1), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%2, not<bool>(read<bool>(%2)));
// DEFAULT-NEXT:         return read<bool>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @checkf(%4 a: i32, %5 b: bool) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 c: bool [storage=automatic] = call<bool, signature=fn(i32, bool) -> bool>(%0, read<i32>(%4), read<bool>(%5));
// DEFAULT-NEXT:         let %7 d: i8 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i8>>(%7)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<bool>>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(xor<i32>(from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%7)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%4), const<i32>(0))))), from_bool<i32, reason=promotion>(read<bool>(%5))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, bool) -> void>(%3, const<i32>(0), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, bool) -> void>(%3, const<i32>(0), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, bool) -> void>(%3, const<i32>(1), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, bool) -> void>(%3, const<i32>(1), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
