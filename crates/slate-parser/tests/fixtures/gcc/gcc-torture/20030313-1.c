struct A {
  unsigned long p, q, r, s;
} x = {13, 14, 15, 16};

extern void abort(void);
extern void exit(int);

static inline struct A *bar(void) {
  struct A *r;

  switch (8) {
  case 2:
    abort();
    break;
  case 8:
    r = &x;
    break;
  default:
    abort();
    break;
  }
  return r;
}

void foo(unsigned long *x, int y) {
  if (y != 12)
    abort();
  if (x[0] != 1 || x[1] != 11)
    abort();
  if (x[2] != 2 || x[3] != 12)
    abort();
  if (x[4] != 3 || x[5] != 13)
    abort();
  if (x[6] != 4 || x[7] != 14)
    abort();
  if (x[8] != 5 || x[9] != 15)
    abort();
  if (x[10] != 6 || x[11] != 16)
    abort();
}

int main(void) {
  unsigned long a[40];
  int           b = 0;

  a[b++] = 1;
  a[b++] = 11;
  a[b++] = 2;
  a[b++] = 12;
  a[b++] = 3;
  a[b++] = bar()->p;
  a[b++] = 4;
  a[b++] = bar()->q;
  a[b++] = 5;
  a[b++] = bar()->r;
  a[b++] = 6;
  a[b++] = bar()->s;
  foo(a, b);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 p: u64;
// DEFAULT-NEXT:         field1 q: u64;
// DEFAULT-NEXT:         field2 r: u64;
// DEFAULT-NEXT:         field3 s: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     global %1 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(13))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(14))), field2 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(15))), field3 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(16)))) [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar() -> ptr<@type0> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 r: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         switch %13 const<i32>(8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %13 const<i32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 case %13 const<i32>(8):
// DEFAULT-NEXT:                     write<ptr<@type0>>(%5, addr_of<ptr<@type0>>(%1));
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:                 default %13:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<@type0>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 x: ptr<u64>, %8 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(4)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(5)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(13)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(6)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(14)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(9)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(10)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))), ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(11)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 a: array<u64, 40> [storage=automatic];
// DEFAULT-NEXT:         let %11 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%15));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%14))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%17));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%16))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(11))));
// DEFAULT-NEXT:         let %18: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%19));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%18))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%21));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%20))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(12))));
// DEFAULT-NEXT:         let %22: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%23));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%22))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%25));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%24))), read<u64>(field0(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         read<u64>(field0(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4))));
// DEFAULT-NEXT:         let %26: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%27));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%26))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:         let %28: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%29));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%28))), read<u64>(field1(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         read<u64>(field1(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4))));
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%31));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%30))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%33));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%32))), read<u64>(field2(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         read<u64>(field2(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4))));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%35));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%34))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6))));
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%37));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%36))), read<u64>(field3(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         read<u64>(field3(deref(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%4))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>, i32) -> void>(%6, array_decay<ptr<u64>, length=Some(40)>(%10), read<i32>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
