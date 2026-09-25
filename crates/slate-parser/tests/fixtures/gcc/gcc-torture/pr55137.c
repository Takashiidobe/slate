/* PR c++/55137 */

extern void abort(void);

int foo(unsigned int x) { return ((int)(x + 1U) + 1) < (int)x; }

int bar(unsigned int x) { return (int)(x + 1U) + 1; }

int baz(unsigned int x) { return x + 1U; }

int main() {
  if (foo(__INT_MAX__) != (bar(__INT_MAX__) < __INT_MAX__) ||
      foo(__INT_MAX__) != ((int)baz(__INT_MAX__) + 1 < __INT_MAX__))
    abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%2), const<u32>(1))), const<i32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%4), const<u32>(1))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @baz(%6 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%6), const<u32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), from_bool<i32, reason=promotion>(lt<i32>(call<i32, signature=fn(u32) -> i32>(%3, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), const<i32>(2147483647))))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i32>(call<i32, signature=fn(u32) -> i32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), from_bool<i32, reason=promotion>(lt<i32>(add<i32, overflow=ub>(call<i32, signature=fn(u32) -> i32>(%5, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), const<i32>(1)), const<i32>(2147483647)))));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
