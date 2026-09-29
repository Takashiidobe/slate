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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([95, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(48));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %[[VALUE1:[0-9]+]] widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_x]]))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE1]] const<i32>(95):
// DEFAULT-NEXT:                             case %[[VALUE1]] const<i32>(43):
// DEFAULT-NEXT:                                 write<i8>(%[[VALUE_a]], read<i8>(deref(read<ptr<const i8>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<const i8>>(%[[VALUE_x]], read<ptr<const i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                         continue %[[VALUE0]];
// DEFAULT-NEXT:                         default %[[VALUE1]]:
// DEFAULT-NEXT:                             break %[[VALUE1]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a]])), const<i32>(48)), eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a]])), const<i32>(43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
