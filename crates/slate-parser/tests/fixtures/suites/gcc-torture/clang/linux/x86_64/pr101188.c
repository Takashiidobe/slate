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
// DEFAULT-NEXT:     type @type0 uint8_t = u8;
// DEFAULT-NEXT:     type @type1 uint16_t = u16;
// DEFAULT-NEXT:     type @type2 fn1 = ptr<fn(ptr<void>) -> u8>;
// DEFAULT-NEXT:     type @type3 fn2 = ptr<fn(ptr<void>, ptr<i32>) -> void>;
// DEFAULT-NEXT:     type @type4 S = struct {
// DEFAULT-NEXT:         field0 buffer: array<u8, 64>;
// DEFAULT-NEXT:         field1 n: u16;
// DEFAULT-NEXT:         field2 f2: ptr<fn(ptr<void>, ptr<i32>) -> void>;
// DEFAULT-NEXT:         field3 a: ptr<void>;
// DEFAULT-NEXT:         field4 f1: ptr<fn(ptr<void>) -> u8>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 64, 72, 80, 88]];
// DEFAULT-NEXT:     global %8 x: volatile u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 myfn2_called: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     fn %9 @foo(%10 n: u16) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u16, volatile>(%8, read<u16>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @testfn(%12 self: ptr<@type4>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 arg: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%9, read<u16>(field1(deref(read<ptr<@type4>>(%12)))));
// DEFAULT-NEXT:         let %23: ptr<@type4> [synthetic] = read<ptr<@type4>>(%12);
// DEFAULT-NEXT:         let %24: u16 [synthetic] = read<u16>(field1(deref(read<ptr<@type4>>(%23))));
// DEFAULT-NEXT:         let %25: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%24))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(field1(deref(read<ptr<@type4>>(%23))), read<u16>(%25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<i32>) -> void>(read<ptr<fn(ptr<void>, ptr<i32>) -> void>>(field2(deref(read<ptr<@type4>>(%12)))), read<ptr<void>>(field3(deref(read<ptr<@type4>>(%12)))), addr_of<ptr<i32>>(%13));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(64)>(field0(deref(read<ptr<@type4>>(%12)))), const<i32>(0))), call<u8, signature=fn(ptr<void>) -> u8>(read<ptr<fn(ptr<void>) -> u8>>(field4(deref(read<ptr<@type4>>(%12)))), read<ptr<void>>(field3(deref(read<ptr<@type4>>(%12))))));
// DEFAULT-NEXT:         call<u8, signature=fn(ptr<void>) -> u8>(read<ptr<fn(ptr<void>) -> u8>>(field4(deref(read<ptr<@type4>>(%12)))), read<ptr<void>>(field3(deref(read<ptr<@type4>>(%12)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @myfn2(%16 a: ptr<void>, %17 arg: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u8>(%14, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @myfn1(%19 a: ptr<void>) -> u8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 s: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field1(%21), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<i32>) -> void>>(field2(%21), function_decay<ptr<fn(ptr<void>, ptr<i32>) -> void>>(%15));
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>) -> u8>>(field4(%21), function_decay<ptr<fn(ptr<void>) -> u8>>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>) -> void>(%11, addr_of<ptr<@type4>>(%21));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
