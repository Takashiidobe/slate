/* Copyright (C) 2002  Free Software Foundation

   Check that constant folding of shift operations is working.

   Roger Sayle, 10th October 2002.  */

extern void abort(void);
extern void link_error(void);

void utest(unsigned int x) {
  if (x >> 0 != x)
    link_error();

  if (x << 0 != x)
    link_error();

  if (0 << x != 0)
    link_error();

  if (0 >> x != 0)
    link_error();

  if (-1 >> x != -1)
    link_error();

  if (~0 >> x != ~0)
    link_error();
}

void stest(int x) {
  if (x >> 0 != x)
    link_error();

  if (x << 0 != x)
    link_error();

  if (0 << x != 0)
    link_error();

  if (0 >> x != 0)
    link_error();
}

int main() {
  utest(9);
  utest(0);

  stest(9);
  stest(0);

  return 0;
}

#ifndef __OPTIMIZE__
void link_error() { abort(); }
#endif


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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @link_error() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @utest(%3 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%3), const<i32>(0)), read<u32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%3), const<i32>(0)), read<u32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), read<u32>(%3)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), read<u32>(%3)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(neg<i32, overflow=ub>(const<i32>(1)), read<u32>(%3)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(not<i32>(const<i32>(0)), read<u32>(%3)), not<i32>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @stest(%5 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%5), const<i32>(0)), read<i32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%5), const<i32>(0)), read<i32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), read<i32>(%5)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), read<i32>(%5)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%2, reinterpret<u32, reason=arg, fits=always>(const<i32>(9)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%2, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(9));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
