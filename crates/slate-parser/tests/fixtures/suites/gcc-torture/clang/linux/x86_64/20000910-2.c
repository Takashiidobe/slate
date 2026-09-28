/* Copyright (C) 2000  Free Software Foundation  */
/* by Alexandre Oliva <aoliva@redhat.com> */

#include <stdlib.h>
#include <string.h>

char *list[] = {"*", "e"};

static int bar(const char *fmt) { return (strchr(fmt, '*') != 0); }

static void foo() {
  int i;
  for (i = 0; i < sizeof(list) / sizeof(*list); i++) {
    const char *fmt = list[i];
    if (bar(fmt))
      continue;
    if (i == 0)
      abort();
    else
      exit(0);
  }
}

int main() { foo(); }



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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([42, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 list: array<ptr<i8>, 2> [storage=static] [align=16] = aggregate<array<ptr<i8>, 2>, zero_fill=false>(index0 = array_decay<ptr<i8>, length=Some(2)>(%13), index1 = array_decay<ptr<i8>, length=Some(2)>(%14)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%10 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @strchr(%11 __s: ptr<const i8>, %12 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %4 @bar(%5 fmt: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<const i8>>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%2, read<ptr<const i8>>(%5), const<i32>(42))), null<ptr<const i8>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), div<u64, by_zero=ub>(const<u64>(16), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %8 fmt: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(2)>(%3), read<i32>(%7)))));
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%4, read<ptr<const i8>>(%8)), const<i32>(0))
// DEFAULT-NEXT:                         continue %15;
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
