#include <stdlib.h>
#include <string.h>

char **buildargv(char *input) {
  static char *arglist[256];
  int          numargs = 0;

  while (1) {
    while (*input == ' ')
      input++;
    if (*input == 0)
      break;
    arglist[numargs++] = input;
    while (*input != ' ' && *input != 0)
      input++;
    if (*input == 0)
      break;
    *(input++) = 0;
  }
  arglist[numargs] = NULL;
  return arglist;
}

int main() {
  char **args;
  char   input[256];
  int    i;

  strcpy(input, " a b");
  args = buildargv(input);

  if (strcmp(args[0], "a"))
    abort();
  if (strcmp(args[1], "b"))
    abort();
  if (args[2] != NULL)
    abort();

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
// DEFAULT-NEXT:     global %6 arglist: array<ptr<i8>, 256> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([32, 97, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%12 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @strcpy(%13 __dest: ptr<i8> [restrict], %14 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %3 @strcmp(%15 __s1: ptr<const i8>, %16 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @buildargv(%5 input: ptr<i8>) -> ptr<ptr<i8>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 numargs: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %17 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %18 eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(32))
// DEFAULT-NEXT:                     let %23: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:                     let %24: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%5, read<ptr<i8>>(%24));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(0))
// DEFAULT-NEXT:                     break %17;
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%26));
// DEFAULT-NEXT:                 write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%6), read<i32>(%25))), read<ptr<i8>>(%5));
// DEFAULT-NEXT:                 while %19 logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(32)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(0)))
// DEFAULT-NEXT:                     let %27: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:                     let %28: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%27), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%5, read<ptr<i8>>(%28));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(0))
// DEFAULT-NEXT:                     break %17;
// DEFAULT-NEXT:                 let %29: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:                 let %30: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%5, read<ptr<i8>>(%30));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%29)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%6), read<i32>(%7))), null<ptr<i8>>);
// DEFAULT-NEXT:         return array_decay<ptr<ptr<i8>>, length=Some(256)>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 args: ptr<ptr<i8>> [storage=automatic];
// DEFAULT-NEXT:         let %10 input: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%2, array_decay<ptr<i8>, length=Some(256)>(%10), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%20)));
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%9, call<ptr<ptr<i8>>, signature=fn(ptr<i8>) -> ptr<ptr<i8>>>(%4, array_decay<ptr<i8>, length=Some(256)>(%10)));
// DEFAULT-NEXT:         call<ptr<ptr<i8>>, signature=fn(ptr<i8>) -> ptr<ptr<i8>>>(%4, array_decay<ptr<i8>, length=Some(256)>(%10));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%9), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%21))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%9), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%22))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%9), const<i32>(2)))), null<ptr<i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
