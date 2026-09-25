/* bf-ms-attrib.c */
/* Adapted from Donn Terry <donnte@microsoft.com> testcase
   posted to GCC-patches
   http://gcc.gnu.org/ml/gcc-patches/2000-08/msg00577.html */

/* { dg-do run { target *-*-mingw* *-*-cygwin* } } */

/* We don't want the default "pedantic-errors" in this case, since we're
   testing nonstandard stuff to begin with. */
/* { dg-options "-ansi" } */

extern void abort(void);

struct one_gcc {
  int            d;
  unsigned char  a;
  unsigned short b : 7;
  char           c;
} __attribute__((__gcc_struct__));

struct one_ms {
  int            d;
  unsigned char  a;
  unsigned short b : 7;
  char           c;
} __attribute__((__ms_struct__));

int main() {
  /* As long as the sizes are as expected, we know attributes are working.
       bf-ms-layout.c makes sure the right thing happens when the attribute
       is on. */
  if (sizeof(struct one_ms) != 12)
    abort();
  if (sizeof(struct one_gcc) != 8)
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
// DEFAULT-NEXT:     type @type0 one_gcc = struct {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u16 : 7;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 5, 6], bit_offsets=[None, None, Some(40), None], bit_units=[(5, 1)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     type @type1 one_ms = struct {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u16 : 7;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 5, 6], bit_offsets=[None, None, Some(40), None], bit_units=[(5, 1)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
