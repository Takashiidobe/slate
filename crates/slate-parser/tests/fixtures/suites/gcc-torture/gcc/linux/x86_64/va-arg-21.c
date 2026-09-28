/* Copyright (C) 2000  Free Software Foundation.

   If the argument to va_end() has side effects, test whether side
   effects from that argument are honored.

   Written by Kaveh R. Ghazi, 10/31/2000.  */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#define __attribute__(x)
#endif

static void __attribute__((__format__(__printf__, 1, 2))) doit(const char *s,
                                                               ...) {
  va_list *ap_array[3], **ap_ptr = ap_array;

  ap_array[0] = malloc(sizeof(va_list));
  ap_array[1] = NULL;
  ap_array[2] = malloc(sizeof(va_list));

  va_start(*ap_array[0], s);
  vprintf(s, **ap_ptr);
  /* Increment the va_list pointer once.  */
  va_end(**ap_ptr++);

  /* Increment the va_list pointer a second time.  */
  ap_ptr++;

  va_start(*ap_array[2], s);
  /* If we failed to increment ap_ptr twice, then the parameter passed
     in here will dereference NULL and should cause a crash.  */
  vprintf(s, **ap_ptr);
  va_end(**ap_ptr);

  /* Just in case, If *ap_ptr is NULL abort anyway.  */
  if (*ap_ptr == 0)
    abort();
}

int main() {
  doit("%s", "hello world\n");
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
// DEFAULT-NEXT:     type @type2 size_t = u64;
// DEFAULT-NEXT:     global %16 .str16: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @vprintf(%12 __format: ptr<const i8> [restrict], %13 __arg: va_list) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%14 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%15 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @doit(%8 s: ptr<const i8>, ...) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 ap_array: array<ptr<va_list>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %10 ap_ptr: ptr<ptr<va_list>> [storage=automatic] = array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9);
// DEFAULT-NEXT:         write<ptr<va_list>>(deref(ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9), const<i32>(0))), pointer_cast<ptr<va_list>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(24))));
// DEFAULT-NEXT:         pointer_cast<ptr<va_list>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(24)));
// DEFAULT-NEXT:         write<ptr<va_list>>(deref(ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9), const<i32>(1))), null<ptr<va_list>>);
// DEFAULT-NEXT:         write<ptr<va_list>>(deref(ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9), const<i32>(2))), pointer_cast<ptr<va_list>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(24))));
// DEFAULT-NEXT:         pointer_cast<ptr<va_list>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(24)));
// DEFAULT-NEXT:         va_start(deref(read<ptr<va_list>>(deref(ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9), const<i32>(0))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%3, read<ptr<const i8>>(%8), read<va_list>(deref(read<ptr<va_list>>(deref(read<ptr<ptr<va_list>>>(%10))))));
// DEFAULT-NEXT:         let %18: ptr<ptr<va_list>> [synthetic] = read<ptr<ptr<va_list>>>(%10);
// DEFAULT-NEXT:         let %19: ptr<ptr<va_list>> [synthetic] = ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(read<ptr<ptr<va_list>>>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<va_list>>>(%10, read<ptr<ptr<va_list>>>(%19));
// DEFAULT-NEXT:         va_end(deref(read<ptr<va_list>>(deref(read<ptr<ptr<va_list>>>(%18)))));
// DEFAULT-NEXT:         let %20: ptr<ptr<va_list>> [synthetic] = read<ptr<ptr<va_list>>>(%10);
// DEFAULT-NEXT:         let %21: ptr<ptr<va_list>> [synthetic] = ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(read<ptr<ptr<va_list>>>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<va_list>>>(%10, read<ptr<ptr<va_list>>>(%21));
// DEFAULT-NEXT:         va_start(deref(read<ptr<va_list>>(deref(ptr_offset<ptr<ptr<va_list>>, subtract=false, element=ptr<va_list>, overflow=ub>(array_decay<ptr<ptr<va_list>>, length=Some(3)>(%9), const<i32>(2))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, va_list) -> i32>(%3, read<ptr<const i8>>(%8), read<va_list>(deref(read<ptr<va_list>>(deref(read<ptr<ptr<va_list>>>(%10))))));
// DEFAULT-NEXT:         va_end(deref(read<ptr<va_list>>(deref(read<ptr<ptr<va_list>>>(%10)))));
// DEFAULT-NEXT:         if eq<ptr<va_list>>(read<ptr<va_list>>(deref(read<ptr<ptr<va_list>>>(%10))), null<ptr<va_list>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%16)), array_decay<ptr<i8>, length=Some(13)>(%17));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
