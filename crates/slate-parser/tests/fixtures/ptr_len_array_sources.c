#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int global_values[4] = {2, 4, 6, 8};

static int sum_values(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}

static void bump_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] += 1;
}

static int sum_prefix(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}

static int score_text(const unsigned char *text, int len) {
  (void)strlen((const char *)text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += text[i];
  return score;
}

static int maybe_consume(int *values, int len, int release) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  if (release)
    free(values);
  return sum;
}

int main(void) {
  int           local_values[4] = {1, 3, 5, 7};
  unsigned char text[]          = "abc";
  int           total           = sum_values(global_values, 4);
  bump_values(local_values, 4);
  int score    = score_text(text, 3);
  int borrowed = maybe_consume(local_values, 4, 0);
  int prefix   = sum_prefix(local_values, 3);
  printf("%d %d %d %d %d\n", total, local_values[3], score, borrowed, prefix);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %4 global_values: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(4), index2 = const<i32>(6), index3 = const<i32>(8)) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%37 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @free(%38 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @strlen(%39 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @sum_values(%6 values: ptr<const i32>, %7 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%47));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%6), read<i32>(%9)))));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%49));
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @bump_values(%11 values: ptr<i32>, %12 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %41
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), read<i32>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %52: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%13));
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%52)));
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%52)), read<i32>(%54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @sum_prefix(%15 values: ptr<const i32>, %16 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %18 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%18), read<i32>(%16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %55: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%56));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%57), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%15), read<i32>(%18)))));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%58));
// DEFAULT-NEXT:         return read<i32>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @score_text(%20 text: ptr<const u8>, %21 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const u8>>(%20)));
// DEFAULT-NEXT:         let %22 score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %43
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %23 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%23), read<i32>(%21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%59), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%60));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%61), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%20), read<i32>(%23)))))));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32>(%62));
// DEFAULT-NEXT:         return read<i32>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @maybe_consume(%25 values: ptr<i32>, %26 len: i32, %27 release: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %44
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %29 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%29), read<i32>(%26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %64: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%64));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %65: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:                 let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%25), read<i32>(%29)))));
// DEFAULT-NEXT:                 write<i32>(%28, read<i32>(%66));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%27), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%2, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%25)));
// DEFAULT-NEXT:         return read<i32>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %31 local_values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(3), index2 = const<i32>(5), index3 = const<i32>(7));
// DEFAULT-NEXT:         let %32 text: array<u8, 4> [storage=automatic] = code_units<array<u8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %33 total: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%5, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%4)), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%10, array_decay<ptr<i32>, length=Some(4)>(%31), const<i32>(4));
// DEFAULT-NEXT:         let %34 score: i32 [storage=automatic] = call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%19, pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%32)), const<i32>(3));
// DEFAULT-NEXT:         let %35 borrowed: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32, i32) -> i32>(%24, array_decay<ptr<i32>, length=Some(4)>(%31), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         let %36 prefix: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%14, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%31)), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%45)), read<i32>(%33), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%31), const<i32>(3)))), read<i32>(%34), read<i32>(%35), read<i32>(%36));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
