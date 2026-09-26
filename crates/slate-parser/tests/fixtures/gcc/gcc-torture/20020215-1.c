/* Test failed on an architecture that:

   - had 16-bit registers,
   - passed 64-bit structures in registers,
   - only allowed SImode values in even numbered registers.

   Before reload, s.i2 in foo() was represented as:

        (subreg:SI (reg:DI 0) 2)

   find_dummy_reload would return (reg:SI 1) for the subreg reload,
   despite that not being a valid register.  */

void abort(void);
void exit(int);

struct s {
  short i1;
  long  i2;
  short i3;
};

struct s foo(struct s s) {
  s.i2++;
  return s;
}

int main() {
  struct s s = foo((struct s){1000, 2000L, 3000});
  if (s.i1 != 1000 || s.i2 != 2001L || s.i3 != 3000)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 i1: i16;
// DEFAULT-NEXT:         field1 i2: i64;
// DEFAULT-NEXT:         field2 i3: i16;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 s: @type0) -> @type0 [linkage=external] [abi=sysv64(byval<align=8>) -> sret<align=8>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9: i64 [synthetic] = read<i64>(field1(%4));
// DEFAULT-NEXT:         let %10: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%9), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field1(%4), read<i64>(%10));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 s: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(byval<align=8>) -> sret<align=8>>(%3, copy<@type0, reason=arg>(read<@type0>(compound_literal %8 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(1000)), field1 = const<i64>(2000), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(3000)))))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%6))), const<i32>(1000)), ne<i64>(read<i64>(field1(%6)), const<i64>(2001))), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%6))), const<i32>(3000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
