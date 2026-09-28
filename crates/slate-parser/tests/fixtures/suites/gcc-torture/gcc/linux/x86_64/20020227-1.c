/* This testcase failed on mmix-knuth-mmixware.  Problem was with storing
   to an unaligned mem:SC, gcc tried doing it by parts from a (concat:SC
   (reg:SF 293) (reg:SF 294)).  */

void abort(void);
void exit(int);

typedef __complex__ float cf;
struct x {
  char c;
  cf   f;
} __attribute__((__packed__));
extern void f2(struct x *);
extern void f1(void);
int         main(void) {
  f1();
  exit(0);
}

void f1(void) {
  struct x s;
  s.f = 1;
  s.c = 42;
  f2(&s);
}

void f2(struct x *y) {
  if (y->f != 1 || y->c != 42)
    abort();
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
// DEFAULT-NEXT:     type @type0 cf = complex<f32>;
// DEFAULT-NEXT:     type @type1 x = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: complex<f32>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @f2(%8 y: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=observable>(read<complex<f32>>(field1(deref(read<ptr<@type1>>(%8)))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type1>>(%8))))), const<i32>(42)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 s: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<complex<f32>>(field1(%7), real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))));
// DEFAULT-NEXT:         write<i8>(field0(%7), truncate<i8, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%4, addr_of<ptr<@type1>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
