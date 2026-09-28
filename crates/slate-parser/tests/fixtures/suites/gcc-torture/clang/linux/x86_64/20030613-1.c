/* PR optimization/10955 */
/* Originator: <heinrich.brand@fujitsu-siemens.com> */

/* This used to fail on SPARC32 at -O3 because the loop unroller
   wrongly thought it could eliminate a pseudo in a loop, while
   the pseudo was used outside the loop.  */

extern void abort(void);

#define COMPLEX struct CS

COMPLEX {
  long x;
  long y;
};

static COMPLEX CCID(COMPLEX x) {
  COMPLEX a;

  a.x = x.x;
  a.y = x.y;

  return a;
}

static COMPLEX CPOW(COMPLEX x, int y) {
  COMPLEX a;
  a = x;

  while (--y > 0)
    a = CCID(a);

  return a;
}

static int c5p(COMPLEX x) {
  COMPLEX a, b;
  a = CPOW(x, 2);
  b = CCID(CPOW(a, 2));

  return (b.x == b.y);
}

int main(void) {
  COMPLEX x;

  x.x = -7;
  x.y = -7;

  if (!c5p(x))
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
// DEFAULT-NEXT:     type @type0 CS = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @CCID(%3 x: @type0) -> @type0 [linkage=internal] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%4), read<i64>(field0(%3)));
// DEFAULT-NEXT:         write<i64>(field1(%4), read<i64>(field1(%3)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @CPOW(%6 x: @type0, %7 y: i32) -> @type0 [linkage=internal] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%8, copy<@type0, reason=assign>(read<@type0>(%6)));
// DEFAULT-NEXT:         while %15 {
// DEFAULT-NEXT:             let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %17: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%17), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type0>(%8, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%2, copy<@type0, reason=arg>(read<@type0>(%8)))));
// DEFAULT-NEXT:             copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%2, copy<@type0, reason=arg>(read<@type0>(%8))));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @c5p(%10 x: @type0) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %12 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%11, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0, i32) -> @type0, abi=sysv64(native_c, scalar) -> native_c>(%5, copy<@type0, reason=arg>(read<@type0>(%10)), const<i32>(2))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0, i32) -> @type0, abi=sysv64(native_c, scalar) -> native_c>(%5, copy<@type0, reason=arg>(read<@type0>(%10)), const<i32>(2)));
// DEFAULT-NEXT:         write<@type0>(%12, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%2, copy<@type0, reason=arg>(call<@type0, signature=fn(@type0, i32) -> @type0, abi=sysv64(native_c, scalar) -> native_c>(%5, copy<@type0, reason=arg>(read<@type0>(%11)), const<i32>(2))))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%2, copy<@type0, reason=arg>(call<@type0, signature=fn(@type0, i32) -> @type0, abi=sysv64(native_c, scalar) -> native_c>(%5, copy<@type0, reason=arg>(read<@type0>(%11)), const<i32>(2)))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i64>(read<i64>(field0(%12)), read<i64>(field1(%12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%14), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         write<i64>(field1(%14), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%9, copy<@type0, reason=arg>(read<@type0>(%14))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
