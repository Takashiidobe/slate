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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_global_values:[0-9]+]] global_values: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(4), index2 = const<i32>(6), index3 = const<i32>(8)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_sum_values:[0-9]+]] @sum_values(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_len:[0-9]+]] len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_sum]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_sum]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bump_values:[0-9]+]] @bump_values(%[[VALUE_values_2:[0-9]+]] values: ptr<i32>, %[[VALUE_len_2:[0-9]+]] len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_len_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values_2]]), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE8]])));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE8]])), read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sum_prefix:[0-9]+]] @sum_prefix(%[[VALUE_values_3:[0-9]+]] values: ptr<const i32>, %[[VALUE_len_3:[0-9]+]] len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sum_2:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), read<i32>(%[[VALUE_len_3]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum_2]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_values_3]]), read<i32>(%[[VALUE_i_3]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_sum_2]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_sum_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_score_text:[0-9]+]] @score_text(%[[VALUE_text:[0-9]+]] text: ptr<const u8>, %[[VALUE_len_4:[0-9]+]] len: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const u8>>(%[[VALUE_text]])));
// DEFAULT-NEXT:         let %[[VALUE_score:[0-9]+]] score: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_4]]), read<i32>(%[[VALUE_len_4]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_score]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%[[VALUE_text]]), read<i32>(%[[VALUE_i_4]])))))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_score]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_score]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_maybe_consume:[0-9]+]] @maybe_consume(%[[VALUE_values_4:[0-9]+]] values: ptr<i32>, %[[VALUE_len_5:[0-9]+]] len: i32, %[[VALUE_release:[0-9]+]] release: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sum_3:[0-9]+]] sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_5]]), read<i32>(%[[VALUE_len_5]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sum_3]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values_4]]), read<i32>(%[[VALUE_i_5]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_sum_3]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_release]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_values_4]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_sum_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_local_values:[0-9]+]] local_values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(3), index2 = const<i32>(5), index3 = const<i32>(7));
// DEFAULT-NEXT:         let %[[VALUE_text_2:[0-9]+]] text: array<u8, 4> [storage=automatic] = code_units<array<u8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%[[VALUE_sum_values]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_global_values]])), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE_bump_values]], array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_local_values]]), const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_score_2:[0-9]+]] score: i32 [storage=automatic] = call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%[[VALUE_score_text]], pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_text_2]])), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_borrowed:[0-9]+]] borrowed: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32, i32) -> i32>(%[[VALUE_maybe_consume]], array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_local_values]]), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_prefix:[0-9]+]] prefix: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i32>, i32) -> i32>(%[[VALUE_sum_prefix]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_local_values]])), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), read<i32>(%[[VALUE_total]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_local_values]]), const<i32>(3)))), read<i32>(%[[VALUE_score_2]]), read<i32>(%[[VALUE_borrowed]]), read<i32>(%[[VALUE_prefix]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
