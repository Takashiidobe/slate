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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 80, 82, 69, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 65, 66, 83, 69, 78, 84, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([83, 76, 65, 84, 69, 95, 71, 69, 84, 69, 78, 86, 95, 70, 73, 88, 84, 85, 82, 69, 95, 82, 69, 74, 69, 67, 84, 69, 68, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_getenv:[0-9]+]] @getenv(%[[VALUE___name:[0-9]+]] __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_setenv:[0-9]+]] @setenv(%[[VALUE___name_2:[0-9]+]] __name: ptr<const i8>, %[[VALUE___value:[0-9]+]] __value: ptr<const i8>, %[[VALUE___replace:[0-9]+]] __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unsetenv:[0-9]+]] @unsetenv(%[[VALUE___name_3:[0-9]+]] __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_present_check:[0-9]+]] @present_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%[[VALUE_setenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_getenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(29)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_value]]), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_absent_check:[0-9]+]] @absent_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_unsetenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         let %[[VALUE_value_2:[0-9]+]] value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_getenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_value_2]]), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_rejected_check:[0-9]+]] @rejected_check() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%[[VALUE_setenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str_6]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_7]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_value_3:[0-9]+]] value: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_getenv]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:         let %[[VALUE_found:[0-9]+]] found: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_value_3]]), null<ptr<i8>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_found]], from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_value_3]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_9]]))), const<i32>(0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_found]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_found]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_10]])), call<i32, signature=fn() -> i32>(%[[VALUE_present_check]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_11]])), call<i32, signature=fn() -> i32>(%[[VALUE_absent_check]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_12]])), call<i32, signature=fn() -> i32>(%[[VALUE_rejected_check]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
