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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] widen<i32, reason=promotion>(read<i8>(%[[VALUE_x]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(0):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(1));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(7));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(2));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(19));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(5));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(17));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(7):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(8));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(28));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(9):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(16));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(10):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(11):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(12));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(12):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(15));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(13):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(111));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(14):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(17));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(15):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(10));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(16):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(17):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(7));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(18):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(2));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(19):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(19));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(20):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(5));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(21):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(107));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(22):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(23):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(8));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(24):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(28));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(25):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(106));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(26):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(27):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(102));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(28):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(105));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(29):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(111));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(30):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(17));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(31):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(10));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(32):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(31));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(98):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(18));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(-62):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], const<i32>(19));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(98))), const<i32>(18))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(97))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(99))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(62)))), const<i32>(19))
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(63)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(61)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(28))), const<i32>(105))
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(27))), const<i32>(102)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(29))), const<i32>(111)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
