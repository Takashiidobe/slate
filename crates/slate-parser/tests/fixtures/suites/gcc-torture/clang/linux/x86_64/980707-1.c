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
// DEFAULT-NEXT:     global %11 arglist: array<ptr<i8>, 256> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([32, 97, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%17 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @strcpy(%18 __dest: ptr<i8> [restrict], %19 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %8 @strcmp(%20 __s1: ptr<const i8>, %21 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @buildargv(%10 input: ptr<i8>) -> ptr<ptr<i8>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 numargs: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %22 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %23 eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%10)))), const<i32>(32))
// DEFAULT-NEXT:                     let %28: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:                     let %29: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%28), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%10, read<ptr<i8>>(%29));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%10)))), const<i32>(0))
// DEFAULT-NEXT:                     break %22;
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%31));
// DEFAULT-NEXT:                 write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%11), read<i32>(%30))), read<ptr<i8>>(%10));
// DEFAULT-NEXT:                 while %24 logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%10)))), const<i32>(32)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%10)))), const<i32>(0)))
// DEFAULT-NEXT:                     let %32: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:                     let %33: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%32), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%10, read<ptr<i8>>(%33));
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%10)))), const<i32>(0))
// DEFAULT-NEXT:                     break %22;
// DEFAULT-NEXT:                 let %34: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:                 let %35: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%10, read<ptr<i8>>(%35));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%34)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(256)>(%11), read<i32>(%12))), null<ptr<i8>>);
// DEFAULT-NEXT:         return array_decay<ptr<ptr<i8>>, length=Some(256)>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 args: ptr<ptr<i8>> [storage=automatic];
// DEFAULT-NEXT:         let %15 input: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%5, array_decay<ptr<i8>, length=Some(256)>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%25)));
// DEFAULT-NEXT:         write<ptr<ptr<i8>>>(%14, call<ptr<ptr<i8>>, signature=fn(ptr<i8>) -> ptr<ptr<i8>>>(%9, array_decay<ptr<i8>, length=Some(256)>(%15)));
// DEFAULT-NEXT:         call<ptr<ptr<i8>>, signature=fn(ptr<i8>) -> ptr<ptr<i8>>>(%9, array_decay<ptr<i8>, length=Some(256)>(%15));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%14), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%26))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%14), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%27))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%14), const<i32>(2)))), null<ptr<i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
