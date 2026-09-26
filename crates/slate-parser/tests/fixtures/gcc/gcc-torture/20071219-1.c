/* PR c++/34459 */

extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);

struct S {
  char s[25];
};

struct S *p;

void __attribute__((noinline, noclone)) foo(struct S *x, int set) {
  int i;
  for (i = 0; i < sizeof(x->s); ++i)
    if (x->s[i] != 0)
      abort();
    else if (set)
      x->s[i] = set;
  p = x;
}

void __attribute__((noinline, noclone)) test1(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = b;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test2(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = *p;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test3(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  *p = a;
  *p = b;
  foo(&b, 0);
}

int main(void) {
  test1();
  test2();
  test3();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 s: array<i8, 25>;
// DEFAULT-NEXT:     } [size=25, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %3 p: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @memset(%18 s: ptr<void>, %19 c: i32, %20 n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x: ptr<@type0>, %6 set: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), const<u64>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(25)>(field0(deref(read<ptr<@type0>>(%5)))), read<i32>(%7))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(25)>(field0(deref(read<ptr<@type0>>(%5)))), read<i32>(%7))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%6)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%3, read<ptr<@type0>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%9))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%9), const<i32>(0));
// DEFAULT-NEXT:         let %10 b: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%9));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<@type0>(%10, copy<@type0, reason=assign>(read<@type0>(%9)));
// DEFAULT-NEXT:         write<@type0>(%10, copy<@type0, reason=assign>(read<@type0>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%10), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%12))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%12), const<i32>(0));
// DEFAULT-NEXT:         let %13 b: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%12));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%13), const<i32>(1));
// DEFAULT-NEXT:         write<@type0>(%13, copy<@type0, reason=assign>(read<@type0>(%12)));
// DEFAULT-NEXT:         write<@type0>(%13, copy<@type0, reason=assign>(read<@type0>(deref(read<ptr<@type0>>(%3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%13), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test3() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%15))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%15), const<i32>(0));
// DEFAULT-NEXT:         let %16 b: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%15));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%3)), copy<@type0, reason=assign>(read<@type0>(%15)));
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%3)), copy<@type0, reason=assign>(read<@type0>(%16)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%4, addr_of<ptr<@type0>>(%16), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
