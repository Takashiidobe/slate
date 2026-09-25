/* PR tree-optimization/114965 */

static void foo(const char *x) {

  char a = '0';
  while (1) {
    switch (*x) {
    case '_':
    case '+':
      a = *x;
      x++;
      continue;
    default:
      break;
    }
    break;
  }
  if (a == '0' || a == '+')
    __builtin_abort();
}

int main() { foo("_"); }


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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([95, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 a: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(48));
// DEFAULT-NEXT:         while %4 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %5 widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%1))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %5 const<i32>(95):
// DEFAULT-NEXT:                             case %5 const<i32>(43):
// DEFAULT-NEXT:                                 write<i8>(%2, read<i8>(deref(read<ptr<const i8>>(%1))));
// DEFAULT-NEXT:                         let %7: ptr<const i8> [synthetic] = read<ptr<const i8>>(%1);
// DEFAULT-NEXT:                         let %8: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%7), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<const i8>>(%1, read<ptr<const i8>>(%8));
// DEFAULT-NEXT:                         continue %4;
// DEFAULT-NEXT:                         default %5:
// DEFAULT-NEXT:                             break %5;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 break %4;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(48)), eq<i32>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
