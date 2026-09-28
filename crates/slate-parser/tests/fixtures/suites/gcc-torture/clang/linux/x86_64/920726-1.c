#include <stdarg.h>
#include <stdio.h>

void abort(void);
void exit(int);

struct spurious {
  int anumber;
};

int first(char *buf, char *fmt, ...) {
  int     pos, number;
  va_list args;
  int     dummy;
  char   *bp = buf;

  va_start(args, fmt);
  for (pos = 0; fmt[pos]; pos++)
    if (fmt[pos] == 'i') {
      number = va_arg(args, int);
      sprintf(bp, "%d", number);
      bp += __builtin_strlen(bp);
    } else
      *bp++ = fmt[pos];

  va_end(args);
  *bp = 0;
  return dummy;
}

struct spurious second(char *buf, char *fmt, ...) {
  int             pos, number;
  va_list         args;
  struct spurious dummy;
  char           *bp = buf;

  va_start(args, fmt);
  for (pos = 0; fmt[pos]; pos++)
    if (fmt[pos] == 'i') {
      number = va_arg(args, int);
      sprintf(bp, "%d", number);
      bp += __builtin_strlen(bp);
    } else
      *bp++ = fmt[pos];

  va_end(args);
  *bp = 0;
  return dummy;
}

int main(void) {
  char buf1[100], buf2[100];
  first(buf1, "i i ", 5, 20);
  second(buf2, "i i ", 5, 20);
  if (__builtin_strcmp("5 20 ", buf1) || __builtin_strcmp("5 20 ", buf2))
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 va_list = va_list;
// DEFAULT-NEXT:     type @type3 spurious = struct {
// DEFAULT-NEXT:         field0 anumber: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 32, 105, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([105, 32, 105, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([53, 32, 50, 48, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([53, 32, 50, 48, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @sprintf(%27 __s: ptr<i8> [restrict], %28 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%29 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %33 @__builtin_strlen(%32 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @first(%9 buf: ptr<i8>, %10 fmt: ptr<i8>, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 pos: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 number: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %14 dummy: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 bp: ptr<i8> [storage=automatic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:         va_start(%13);
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), read<i32>(%11)))), const<i8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), read<i32>(%11))))), const<i32>(105))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%12, va_arg<i32>(%13));
// DEFAULT-NEXT:                         va_arg<i32>(%13);
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%4, read<ptr<i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%31)), read<i32>(%12));
// DEFAULT-NEXT:                         let %45: ptr<i8> [synthetic] = read<ptr<i8>>(%15);
// DEFAULT-NEXT:                         let %46: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%45), call<u64, signature=fn(ptr<const i8>) -> u64>(%33, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%15))));
// DEFAULT-NEXT:                         write<ptr<i8>>(%15, read<ptr<i8>>(%46));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     let %47: ptr<i8> [synthetic] = read<ptr<i8>>(%15);
// DEFAULT-NEXT:                     let %48: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%47), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%15, read<ptr<i8>>(%48));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%47)), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), read<i32>(%11)))));
// DEFAULT-NEXT:         va_end(%13);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%15)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @second(%17 buf: ptr<i8>, %18 fmt: ptr<i8>, ...) -> @type3 [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 pos: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 number: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %22 dummy: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %23 bp: ptr<i8> [storage=automatic] = read<ptr<i8>>(%17);
// DEFAULT-NEXT:         va_start(%21);
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), read<i32>(%19)))), const<i8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%50));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), read<i32>(%19))))), const<i32>(105))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%20, va_arg<i32>(%21));
// DEFAULT-NEXT:                         va_arg<i32>(%21);
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%4, read<ptr<i8>>(%23), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%35)), read<i32>(%20));
// DEFAULT-NEXT:                         let %51: ptr<i8> [synthetic] = read<ptr<i8>>(%23);
// DEFAULT-NEXT:                         let %52: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%51), call<u64, signature=fn(ptr<const i8>) -> u64>(%33, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%23))));
// DEFAULT-NEXT:                         write<ptr<i8>>(%23, read<ptr<i8>>(%52));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     let %53: ptr<i8> [synthetic] = read<ptr<i8>>(%23);
// DEFAULT-NEXT:                     let %54: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%53), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%23, read<ptr<i8>>(%54));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%53)), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), read<i32>(%19)))));
// DEFAULT-NEXT:         va_end(%21);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%23)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return copy<@type3, reason=return>(read<@type3>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @__builtin_strcmp(%38 <unnamed>: ptr<const i8>, %39 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %25 buf1: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %26 buf2: array<i8, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<i8>, ...) -> i32>(%8, array_decay<ptr<i8>, length=Some(100)>(%25), array_decay<ptr<i8>, length=Some(5)>(%36), const<i32>(5), const<i32>(20));
// DEFAULT-NEXT:         call<@type3, signature=fn(ptr<i8>, ptr<i8>, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar) -> native_c>(%16, array_decay<ptr<i8>, length=Some(100)>(%26), array_decay<ptr<i8>, length=Some(5)>(%37), const<i32>(5), const<i32>(20));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%41)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%25))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%55, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%42)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%26))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
