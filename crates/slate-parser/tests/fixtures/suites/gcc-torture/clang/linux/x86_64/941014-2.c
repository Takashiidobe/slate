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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: u16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_t:[0-9]+]] foo_t = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_a1:[0-9]+]] @a1(%[[VALUE_offset:[0-9]+]] offset: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> ptr<volatile @type[[TYPE0]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_foo_p:[0-9]+]] foo_p: ptr<volatile @type[[TYPE0]]> [storage=automatic] = pointer_cast<ptr<volatile @type[[TYPE0]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_a1]], sub<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30)))));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(field0(deref(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]])))))), const<i32>(61440)), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16, volatile>(field0(deref(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]])))))));
// DEFAULT-NEXT:         write<u16, volatile>(field1(deref(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]]))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(256))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_a1]], add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_a1]], sub<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30)))));
// DEFAULT-NEXT:         return read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_foo_p_2:[0-9]+]] foo_p: ptr<volatile @type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p_2]], call<ptr<volatile @type[[TYPE0]]>, signature=fn() -> ptr<volatile @type[[TYPE0]]>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<ptr<volatile @type[[TYPE0]]>, signature=fn() -> ptr<volatile @type[[TYPE0]]>>(%[[VALUE_f]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16, volatile>(field1(deref(read<ptr<volatile @type[[TYPE0]]>>(%[[VALUE_foo_p_2]])))))), const<i32>(256))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
