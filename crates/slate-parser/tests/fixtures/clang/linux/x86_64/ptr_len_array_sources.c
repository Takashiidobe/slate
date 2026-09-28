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
// DEFAULT-NEXT:     global %7 global_values: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(4), index2 = const<i32>(6), index3 = const<i32>(8)) [linkage=internal];
// DEFAULT-NEXT:     global %48 .str48: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%40 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @free(%41 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @strlen(%42 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %8 @sum_values(%9 values: ptr<const i32>, %10 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %43
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), read<i32>(%10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%50));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %52: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%51), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%9), read<i32>(%12)))));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%52));
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bump_values(%14 values: ptr<i32>, %15 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %44
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %16 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), read<i32>(%15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%54));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %55: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), read<i32>(%16));
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%55)));
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%55)), read<i32>(%57));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @sum_prefix(%18 values: ptr<const i32>, %19 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %45
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %21 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), read<i32>(%19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%18), read<i32>(%21)))));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%61));
// DEFAULT-NEXT:         return read<i32>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @score_text(%23 text: ptr<const u8>, %24 len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%6, pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const u8>>(%23)));
// DEFAULT-NEXT:         let %25 score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %46
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %26 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%26), read<i32>(%24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = read<i32>(%26);
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%26, read<i32>(%63));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %64: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                 let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%23), read<i32>(%26)))))));
// DEFAULT-NEXT:                 write<i32>(%25, read<i32>(%65));
// DEFAULT-NEXT:         return read<i32>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @maybe_consume(%28 values: ptr<i32>, %29 len: i32, %30 release: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %47
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %32 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%32), read<i32>(%29))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %66: i32 [synthetic] = read<i32>(%32);
// DEFAULT-NEXT:                 let %67: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%32, read<i32>(%67));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %68: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%28), read<i32>(%32)))));
// DEFAULT-NEXT:                 write<i32>(%31, read<i32>(%69));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%4, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%28)));
// DEFAULT-NEXT:         return read<i32>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %34 local_values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(3), index2 = const<i32>(5), index3 = const<i32>(7));
// DEFAULT-NEXT:         let %35 text: array<u8, 4> [storage=automatic] = code_units<array<u8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %36 total: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%8, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%7)), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%13, array_decay<ptr<i32>, length=Some(4)>(%34), const<i32>(4));
// DEFAULT-NEXT:         let %37 score: i32 [storage=automatic] = call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%22, pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%35)), const<i32>(3));
// DEFAULT-NEXT:         let %38 borrowed: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32, i32) -> i32>(%27, array_decay<ptr<i32>, length=Some(4)>(%34), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         let %39 prefix: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%17, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%34)), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%48)), read<i32>(%36), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%34), const<i32>(3)))), read<i32>(%37), read<i32>(%38), read<i32>(%39));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
