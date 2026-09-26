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
// DEFAULT-NEXT:     global %21 .str21: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @getenv(%14 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %2 @setenv(%15 __name: ptr<const i8>, %16 __value: ptr<const i8>, %17 __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @unsetenv(%18 __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @strcmp(%19 __s1: ptr<const i8>, %20 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @present_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%21)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%22)), const<i32>(1));
// DEFAULT-NEXT:         let %6 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%23)));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%6), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @absent_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%24)));
// DEFAULT-NEXT:         let %8 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%25)));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%8), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @rejected_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%26)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%27)), const<i32>(1));
// DEFAULT-NEXT:         let %10 value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%28)));
// DEFAULT-NEXT:         let %11 found: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%10), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%11, from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%29))), const<i32>(0))));
// DEFAULT-NEXT:                 from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%29))), const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%30)), call<i32, signature=fn() -> i32>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%31)), call<i32, signature=fn() -> i32>(%7));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%32)), call<i32, signature=fn() -> i32>(%9));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
