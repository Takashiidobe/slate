/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
struct S {
  int      a : 3;
  unsigned b : 1, c : 28;
};

struct S x = {1, 1, 1};

int main(void) {
  x = (struct S){
    b : 0,
    a : 0,
    c : ({
      struct S o = x;
      o.a == 1 ? 10 : 20;
    })
  };
  if (x.c != 10)
    abort();
  exit(0);
}
#endif


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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32 : 3;
// DEFAULT-NEXT:         field1 b: u32 : 1;
// DEFAULT-NEXT:         field2 c: u32 : 28;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(4)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %3 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 o: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%3));
// DEFAULT-NEXT:             write<i32>(%8, conditional<i32>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%5)), const<i32>(1)), const<i32>(10), const<i32>(20)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(compound_literal %7 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%8))))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=4..32>(%3))), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
