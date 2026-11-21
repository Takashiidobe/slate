#define func(vol, op1, op2)                                                    \
  _Bool op1##_##op2##_##vol(int a, int b) {                                    \
    vol int x = op_##op1(a, b);                                                \
    return op_##op2(x, a);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_min(a, b) ((a) < (b) ? (a) : (b))
#define op_max(a, b) ((a) > (b) ? (a) : (b))

#define funcs(a)                                                               \
  a(min, lt) a(max, lt) a(min, gt) a(max, gt) a(min, le) a(max, le) a(min, ge) \
      a(max, ge) a(min, ne) a(max, ne) a(min, eq) a(max, eq)

#define funcs1(a, b) func(, a, b) func(volatile, a, b)

funcs(funcs1)

#define test(op1, op2)                                                         \
  do {                                                                         \
    if (op1##_##op2##_(x, y) != op1##_##op2##_volatile(x, y))                  \
      __builtin_abort();                                                       \
  } while (0);

    int main() {
  for (int x = -10; x < 10; x++)
    for (int y = -10; y < 10; y++) {
      funcs(test)
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_min_lt_:[0-9]+]] @min_lt_(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return lt<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_lt_volatile:[0-9]+]] @min_lt_volatile(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         return lt<i32>(read<i32, volatile>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_lt_:[0-9]+]] @max_lt_(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]])), read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         return lt<i32>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_lt_volatile:[0-9]+]] @max_lt_volatile(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]])), read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         return lt<i32>(read<i32, volatile>(%[[VALUE_x_4]]), read<i32>(%[[VALUE_a_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_gt_:[0-9]+]] @min_gt_(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_5:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]])), read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         return gt<i32>(read<i32>(%[[VALUE_x_5]]), read<i32>(%[[VALUE_a_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_gt_volatile:[0-9]+]] @min_gt_volatile(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_6:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]])), read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         return gt<i32>(read<i32, volatile>(%[[VALUE_x_6]]), read<i32>(%[[VALUE_a_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_gt_:[0-9]+]] @max_gt_(%[[VALUE_a_7:[0-9]+]] a: i32, %[[VALUE_b_7:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_7:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]])), read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:         return gt<i32>(read<i32>(%[[VALUE_x_7]]), read<i32>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_gt_volatile:[0-9]+]] @max_gt_volatile(%[[VALUE_a_8:[0-9]+]] a: i32, %[[VALUE_b_8:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]])), read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:         return gt<i32>(read<i32, volatile>(%[[VALUE_x_8]]), read<i32>(%[[VALUE_a_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_le_:[0-9]+]] @min_le_(%[[VALUE_a_9:[0-9]+]] a: i32, %[[VALUE_b_9:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_9:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]])), read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:         return le<i32>(read<i32>(%[[VALUE_x_9]]), read<i32>(%[[VALUE_a_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_le_volatile:[0-9]+]] @min_le_volatile(%[[VALUE_a_10:[0-9]+]] a: i32, %[[VALUE_b_10:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]])), read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:         return le<i32>(read<i32, volatile>(%[[VALUE_x_10]]), read<i32>(%[[VALUE_a_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_le_:[0-9]+]] @max_le_(%[[VALUE_a_11:[0-9]+]] a: i32, %[[VALUE_b_11:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_11:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]])), read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:         return le<i32>(read<i32>(%[[VALUE_x_11]]), read<i32>(%[[VALUE_a_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_le_volatile:[0-9]+]] @max_le_volatile(%[[VALUE_a_12:[0-9]+]] a: i32, %[[VALUE_b_12:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_12:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]])), read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:         return le<i32>(read<i32, volatile>(%[[VALUE_x_12]]), read<i32>(%[[VALUE_a_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_ge_:[0-9]+]] @min_ge_(%[[VALUE_a_13:[0-9]+]] a: i32, %[[VALUE_b_13:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_13:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]])), read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:         return ge<i32>(read<i32>(%[[VALUE_x_13]]), read<i32>(%[[VALUE_a_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_ge_volatile:[0-9]+]] @min_ge_volatile(%[[VALUE_a_14:[0-9]+]] a: i32, %[[VALUE_b_14:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_14:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]])), read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:         return ge<i32>(read<i32, volatile>(%[[VALUE_x_14]]), read<i32>(%[[VALUE_a_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_ge_:[0-9]+]] @max_ge_(%[[VALUE_a_15:[0-9]+]] a: i32, %[[VALUE_b_15:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_15:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]])), read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:         return ge<i32>(read<i32>(%[[VALUE_x_15]]), read<i32>(%[[VALUE_a_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_ge_volatile:[0-9]+]] @max_ge_volatile(%[[VALUE_a_16:[0-9]+]] a: i32, %[[VALUE_b_16:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_16:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]])), read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:         return ge<i32>(read<i32, volatile>(%[[VALUE_x_16]]), read<i32>(%[[VALUE_a_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_ne_:[0-9]+]] @min_ne_(%[[VALUE_a_17:[0-9]+]] a: i32, %[[VALUE_b_17:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_17:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]])), read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:         return ne<i32>(read<i32>(%[[VALUE_x_17]]), read<i32>(%[[VALUE_a_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_ne_volatile:[0-9]+]] @min_ne_volatile(%[[VALUE_a_18:[0-9]+]] a: i32, %[[VALUE_b_18:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_18:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]])), read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:         return ne<i32>(read<i32, volatile>(%[[VALUE_x_18]]), read<i32>(%[[VALUE_a_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_ne_:[0-9]+]] @max_ne_(%[[VALUE_a_19:[0-9]+]] a: i32, %[[VALUE_b_19:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_19:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]])), read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:         return ne<i32>(read<i32>(%[[VALUE_x_19]]), read<i32>(%[[VALUE_a_19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_ne_volatile:[0-9]+]] @max_ne_volatile(%[[VALUE_a_20:[0-9]+]] a: i32, %[[VALUE_b_20:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_20:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]])), read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:         return ne<i32>(read<i32, volatile>(%[[VALUE_x_20]]), read<i32>(%[[VALUE_a_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_eq_:[0-9]+]] @min_eq_(%[[VALUE_a_21:[0-9]+]] a: i32, %[[VALUE_b_21:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_21:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]])), read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:         return eq<i32>(read<i32>(%[[VALUE_x_21]]), read<i32>(%[[VALUE_a_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_min_eq_volatile:[0-9]+]] @min_eq_volatile(%[[VALUE_a_22:[0-9]+]] a: i32, %[[VALUE_b_22:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_22:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]])), read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:         return eq<i32>(read<i32, volatile>(%[[VALUE_x_22]]), read<i32>(%[[VALUE_a_22]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_eq_:[0-9]+]] @max_eq_(%[[VALUE_a_23:[0-9]+]] a: i32, %[[VALUE_b_23:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_23:[0-9]+]] x: i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]])), read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:         return eq<i32>(read<i32>(%[[VALUE_x_23]]), read<i32>(%[[VALUE_a_23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_max_eq_volatile:[0-9]+]] @max_eq_volatile(%[[VALUE_a_24:[0-9]+]] a: i32, %[[VALUE_b_24:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_24:[0-9]+]] x: volatile i32 [storage=automatic] = conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]])), read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:         return eq<i32>(read<i32, volatile>(%[[VALUE_x_24]]), read<i32>(%[[VALUE_a_24]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_x_25:[0-9]+]] x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_x_25]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_25]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_25]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_y]]), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_lt_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_lt_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_lt_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_lt_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_gt_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_gt_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_gt_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_gt_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_le_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_le_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_le_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_le_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_ge_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_ge_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_ge_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_ge_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_ne_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_ne_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_ne_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_ne_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_eq_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_min_eq_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_eq_]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_max_eq_volatile]], read<i32>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_y]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
