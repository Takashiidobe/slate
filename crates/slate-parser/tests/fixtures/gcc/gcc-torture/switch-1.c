/* Copyright (C) 2003  Free Software Foundation.

   Test that switch statements suitable using case bit tests are
   implemented correctly.

   Written by Roger Sayle, 01/25/2001.  */

extern void abort(void);

int foo(int x) {
  switch (x) {
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  }
  return 31;
}

int main() {
  int i, r;

  for (i = -1; i < 66; i++) {
    r = foo(i);
    if (i == 4) {
      if (r != 30)
        abort();
    } else if (i == 6) {
      if (r != 30)
        abort();
    } else if (i == 9) {
      if (r != 30)
        abort();
    } else if (i == 11) {
      if (r != 30)
        abort();
    } else if (r != 31)
      abort();
  }
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %6 read<i32>(%2)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %6 const<i32>(4):
// DEFAULT-NEXT:                     case %6 const<i32>(6):
// DEFAULT-NEXT:                         case %6 const<i32>(9):
// DEFAULT-NEXT:                             case %6 const<i32>(11):
// DEFAULT-NEXT:                                 return const<i32>(30);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 r: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(66))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%5, call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%4)));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%4), const<i32>(4))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%5), const<i32>(30))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%4), const<i32>(6))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%5), const<i32>(30))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%4), const<i32>(9))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%5), const<i32>(30))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%4), const<i32>(11))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%5), const<i32>(30))
// DEFAULT-NEXT:                                             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%5), const<i32>(31))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
