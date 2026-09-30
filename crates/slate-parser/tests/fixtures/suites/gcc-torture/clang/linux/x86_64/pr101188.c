/* { dg-require-effective-target indirect_calls } */
typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;

typedef uint8_t (*fn1)(void *a);
typedef void    (*fn2)(void *a, int *arg);

struct S {
  uint8_t  buffer[64];
  uint16_t n;
  fn2      f2;
  void    *a;
  fn1      f1;
};

volatile uint16_t x;

void __attribute__((__noinline__, __noclone__)) foo(uint16_t n) { x = n; }

void __attribute__((__noinline__, __noclone__)) testfn(struct S *self) {
  int arg;

  foo(self->n);
  self->n++;
  self->f2(self->a, &arg);
  self->buffer[0] = self->f1(self->a);
}

static unsigned char myfn2_called = 0;

static void myfn2(void *a, int *arg) { myfn2_called = 1; }

static uint8_t myfn1(void *a) { return 0; }

int main(void) {
  struct S s;
  s.n  = 0;
  s.f2 = myfn2;
  s.f1 = myfn1;
  testfn(&s);
  if (myfn2_called != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_fn1:[0-9]+]] fn1 = ptr<fn(ptr<void>) -> u8>;
// DEFAULT-NEXT:     type @type[[TYPE_fn2:[0-9]+]] fn2 = ptr<fn(ptr<void>, ptr<i32>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 buffer: array<u8, 64>;
// DEFAULT-NEXT:         field1 n: u16;
// DEFAULT-NEXT:         field2 f2: ptr<fn(ptr<void>, ptr<i32>) -> void>;
// DEFAULT-NEXT:         field3 a: ptr<void>;
// DEFAULT-NEXT:         field4 f1: ptr<fn(ptr<void>) -> u8>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 64, 72, 80, 88]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: volatile u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_myfn2_called:[0-9]+]] myfn2_called: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: u16) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u16, volatile>(%[[VALUE_x]], read<u16>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfn:[0-9]+]] @testfn(%[[VALUE_self:[0-9]+]] self: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_arg:[0-9]+]] arg: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%[[VALUE_foo]], read<u16>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]])))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u16 [synthetic] = read<u16>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE1]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))), read<u16>(%[[VALUE2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<i32>) -> void>(read<ptr<fn(ptr<void>, ptr<i32>) -> void>>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]])))), read<ptr<void>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]])))), addr_of<ptr<i32>>(%[[VALUE_arg]]));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(64)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]])))), const<i32>(0))), call<u8, signature=fn(ptr<void>) -> u8>(read<ptr<fn(ptr<void>) -> u8>>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]])))), read<ptr<void>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_self]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_myfn2:[0-9]+]] @myfn2(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_arg_2:[0-9]+]] arg: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u8>(%[[VALUE_myfn2_called]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_myfn1:[0-9]+]] @myfn1(%[[VALUE_a_2:[0-9]+]] a: ptr<void>) -> u8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field1(%[[VALUE_s]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<i32>) -> void>>(field2(%[[VALUE_s]]), function_decay<ptr<fn(ptr<void>, ptr<i32>) -> void>>(%[[VALUE_myfn2]]));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> u8>>(field4(%[[VALUE_s]]), function_decay<ptr<fn(ptr<void>) -> u8>>(%[[VALUE_myfn1]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_testfn]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_myfn2_called]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
