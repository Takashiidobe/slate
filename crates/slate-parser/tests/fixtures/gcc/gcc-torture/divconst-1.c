void abort(void);
void exit(int);

typedef struct {
  unsigned a, b, c, d;
} t1;

void f(t1 *ps) {
  ps->a = 10000;
  ps->b = ps->a / 3;
  ps->c = 10000;
  ps->d = ps->c / 3;
}

int main(void) {
  t1 s;
  f(&s);
  if (s.a != 10000 || s.b != 3333 || s.c != 10000 || s.d != 3333)
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type1 t1 = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 ps: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(field0(deref(read<ptr<@type0>>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(10000)));
// DEFAULT-NEXT:         write<u32>(field1(deref(read<ptr<@type0>>(%5))), div<u32, by_zero=ub>(read<u32>(field0(deref(read<ptr<@type0>>(%5)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type0>>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(10000)));
// DEFAULT-NEXT:         write<u32>(field3(deref(read<ptr<@type0>>(%5))), div<u32, by_zero=ub>(read<u32>(field2(deref(read<ptr<@type0>>(%5)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%4, addr_of<ptr<@type0>>(%7));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(field0(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10000))), ne<u32>(read<u32>(field1(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3333)))), ne<u32>(read<u32>(field2(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10000)))), ne<u32>(read<u32>(field3(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3333))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
