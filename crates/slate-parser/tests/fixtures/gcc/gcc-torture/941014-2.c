#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned short a;
  unsigned short b;
} foo_t;

void a1(unsigned long offset) {}

volatile foo_t *f() {
  volatile foo_t *foo_p = (volatile foo_t *)malloc(sizeof(foo_t));

  a1((unsigned long)foo_p - 30);
  if (foo_p->a & 0xf000)
    printf("%d\n", foo_p->a);
  foo_p->b = 0x0100;
  a1((unsigned long)foo_p + 2);
  a1((unsigned long)foo_p - 30);
  return foo_p;
}

int main(void) {
  volatile foo_t *foo_p;

  foo_p = f();
  if (foo_p->b != 0x0100)
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 a: u16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type2 foo_t = @type1;
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%14 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%15 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @a1(%8 offset: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f() -> ptr<volatile @type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 foo_p: ptr<volatile @type1> [storage=automatic] = pointer_cast<ptr<volatile @type1>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(malloc, const<u64>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%7, sub<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type1>>(%10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30)))));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(field0(deref(read<ptr<volatile @type1>>(%10)))))), const<i32>(61440)), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16, volatile>(field0(deref(read<ptr<volatile @type1>>(%10)))))));
// DEFAULT-NEXT:         write<u16, volatile>(field1(deref(read<ptr<volatile @type1>>(%10))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(256))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%7, add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type1>>(%10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%7, sub<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type1>>(%10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30)))));
// DEFAULT-NEXT:         return read<ptr<volatile @type1>>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 foo_p: ptr<volatile @type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<volatile @type1>>(%12, call<ptr<volatile @type1>, signature=fn() -> ptr<volatile @type1>>(%9));
// DEFAULT-NEXT:         call<ptr<volatile @type1>, signature=fn() -> ptr<volatile @type1>>(%9);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(field1(deref(read<ptr<volatile @type1>>(%12)))))), const<i32>(256))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
