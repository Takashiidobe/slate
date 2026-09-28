#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int present_check(void) {
  setenv("SLATE_GETENV_FIXTURE_PRESENT", "1", 1);
  char *value = getenv("SLATE_GETENV_FIXTURE_PRESENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}

static int absent_check(void) {
  unsetenv("SLATE_GETENV_FIXTURE_ABSENT");
  char *value = getenv("SLATE_GETENV_FIXTURE_ABSENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}

static int rejected_check(void) {
  setenv("SLATE_GETENV_FIXTURE_REJECTED", "ready", 1);
  char *value = getenv("SLATE_GETENV_FIXTURE_REJECTED");
  int   found;
  if (value != NULL) {
    found = strcmp(value, "ready") == 0;
  } else {
    found = 0;
  }
  return found;
}

int main(void) {
  printf("%d\n", present_check());
  printf("%d\n", absent_check());
  printf("%d\n", rejected_check());
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
// DEFAULT-NEXT:     global %29 .str29: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%21 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @getenv(%22 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %7 @setenv(%23 __name: ptr<const i8>, %24 __value: ptr<const i8>, %25 __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @unsetenv(%26 __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @strcmp(%27 __s1: ptr<const i8>, %28 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %13 @present_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%29)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%30)), const<i32>(1));
// DEFAULT-NEXT:         let %14 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%31)));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%14), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @absent_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%32)));
// DEFAULT-NEXT:         let %16 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%33)));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%16), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @rejected_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%34)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%35)), const<i32>(1));
// DEFAULT-NEXT:         let %18 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%36)));
// DEFAULT-NEXT:         let %19 found: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%18), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%19, from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%12, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%18)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%37))), const<i32>(0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%38)), call<i32, signature=fn() -> i32>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%39)), call<i32, signature=fn() -> i32>(%15));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%40)), call<i32, signature=fn() -> i32>(%17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
