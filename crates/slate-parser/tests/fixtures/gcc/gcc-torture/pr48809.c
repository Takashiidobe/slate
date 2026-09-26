/* PR tree-optimization/48809 */

extern void abort(void);

int foo(signed char x) {
  int y = 0;
  switch (x) {
  case 0:
    y = 1;
    break;
  case 1:
    y = 7;
    break;
  case 2:
    y = 2;
    break;
  case 3:
    y = 19;
    break;
  case 4:
    y = 5;
    break;
  case 5:
    y = 17;
    break;
  case 6:
    y = 31;
    break;
  case 7:
    y = 8;
    break;
  case 8:
    y = 28;
    break;
  case 9:
    y = 16;
    break;
  case 10:
    y = 31;
    break;
  case 11:
    y = 12;
    break;
  case 12:
    y = 15;
    break;
  case 13:
    y = 111;
    break;
  case 14:
    y = 17;
    break;
  case 15:
    y = 10;
    break;
  case 16:
    y = 31;
    break;
  case 17:
    y = 7;
    break;
  case 18:
    y = 2;
    break;
  case 19:
    y = 19;
    break;
  case 20:
    y = 5;
    break;
  case 21:
    y = 107;
    break;
  case 22:
    y = 31;
    break;
  case 23:
    y = 8;
    break;
  case 24:
    y = 28;
    break;
  case 25:
    y = 106;
    break;
  case 26:
    y = 31;
    break;
  case 27:
    y = 102;
    break;
  case 28:
    y = 105;
    break;
  case 29:
    y = 111;
    break;
  case 30:
    y = 17;
    break;
  case 31:
    y = 10;
    break;
  case 32:
    y = 31;
    break;
  case 98:
    y = 18;
    break;
  case -62:
    y = 19;
    break;
  }
  return y;
}

int main() {
  if (foo(98) != 18 || foo(97) != 0 || foo(99) != 0)
    abort();
  if (foo(-62) != 19 || foo(-63) != 0 || foo(-61) != 0)
    abort();
  if (foo(28) != 105 || foo(27) != 102 || foo(29) != 111)
    abort();
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
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %5 widen<i32, reason=promotion>(read<i8>(%2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %5 const<i32>(0):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(1):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(7));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(2):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(2));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(3):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(19));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(4):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(17));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(6):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(7):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(8));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(8):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(28));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(9):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(16));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(10):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(11):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(12));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(12):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(15));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(13):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(111));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(14):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(17));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(15):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(10));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(16):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(17):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(7));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(18):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(2));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(19):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(19));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(20):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(21):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(107));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(22):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(23):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(8));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(24):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(28));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(25):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(106));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(26):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(27):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(102));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(28):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(105));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(29):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(111));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(30):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(17));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(31):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(10));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(32):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(31));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(98):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(18));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:                 case %5 const<i32>(-62):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(19));
// DEFAULT-NEXT:                 break %5;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(98))), const<i32>(18))
// DEFAULT-NEXT:             write<bool>(%6, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%6, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(97))), const<i32>(0)));
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(99))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(62)))), const<i32>(19))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(63)))), const<i32>(0)));
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(61)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(28))), const<i32>(105))
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(27))), const<i32>(102)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i8) -> i32>(%1, truncate<i8, reason=arg, fits=always>(const<i32>(29))), const<i32>(111)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
