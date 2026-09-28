/* { dg-require-effective-target int32plus } */

#include <stdio.h>

void abort(void);
void exit(int);

typedef struct {
  int x;
  int y;
} S;
extern int foo(S *);
int        hi = 0, lo = 0;

int main() {
  S   a;
  int r;
  a.x = (int)0x00010000;
  a.y = 1;
  r   = foo(&a);
  if (r == 2 && lo == 0 && hi == 1) {
    exit(0);
  }
  abort();
}

typedef unsigned short u16;

__attribute__((noinline)) int foo(S *ptr) {
  int a = ptr->x;
  int c = 0;
  u16 b = (u16)a;
  if (b != 0) {
    lo  = 1;
    c  += ptr->y;
  }
  b = a >> 16;
  if (b != 0) {
    hi  = 1;
    c  += ptr->y;
  }
  c += ptr->y;
  return c;
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
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 S = @type0;
// DEFAULT-NEXT:     type @type2 u16 = u16;
// DEFAULT-NEXT:     global %5 hi: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %6 lo: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @foo(%11 ptr: ptr<@type0>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 a: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type0>>(%11))));
// DEFAULT-NEXT:         let %13 c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %14 b: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(read<i32>(%12)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%14))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), read<i32>(field1(deref(read<ptr<@type0>>(%11)))));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u16>(%14, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%12), const<i32>(16)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%14))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), read<i32>(field1(deref(read<ptr<@type0>>(%11)))));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %21: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), read<i32>(field1(deref(read<ptr<@type0>>(%11)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%22));
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9 r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%8), const<i32>(65536));
// DEFAULT-NEXT:         write<i32>(field1(%8), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, call<i32, signature=fn(ptr<@type0>) -> i32>(%4, addr_of<ptr<@type0>>(%8)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type0>) -> i32>(%4, addr_of<ptr<@type0>>(%8));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i32>(read<i32>(%9), const<i32>(2)), eq<i32>(read<i32>(%6), const<i32>(0))), eq<i32>(read<i32>(%5), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
