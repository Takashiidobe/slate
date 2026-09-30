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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_u16:[0-9]+]] u16 = u16;
// DEFAULT-NEXT:     global %[[VALUE_hi:[0-9]+]] hi: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lo:[0-9]+]] lo: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_ptr:[0-9]+]] ptr: ptr<@type[[TYPE0]]>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_ptr]]))));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u16 [storage=automatic] = reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]]))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_lo]], const<i32>(1));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), read<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_ptr]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_a]]), const<i32>(16)))));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]]))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_hi]], const<i32>(1));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_ptr]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), read<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_ptr]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_a_2]]), const<i32>(65536));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_a_2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_r]]), const<i32>(2)), eq<i32>(read<i32>(%[[VALUE_lo]]), const<i32>(0))), eq<i32>(read<i32>(%[[VALUE_hi]]), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
