#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x, y);                                                     \
  }

#define op_lt(a, b)  ((a) < (b))
#define op_le(a, b)  ((a) <= (b))
#define op_eq(a, b)  ((a) == (b))
#define op_ne(a, b)  ((a) != (b))
#define op_gt(a, b)  ((a) > (b))
#define op_ge(a, b)  ((a) >= (b))
#define op_xor(a, b) ((a) ^ (b))

#define funcs(a)                                                               \
  a(lt, lt, ne) a(lt, lt, eq) a(lt, lt, xor) a(lt, le, ne) a(lt, le, eq) a(    \
      lt, le, xor) a(lt, gt, ne) a(lt, gt, eq) a(lt, gt, xor) a(lt, ge, ne)    \
      a(lt, ge, eq) a(lt, ge, xor) a(lt, eq, ne) a(lt, eq, eq) a(lt, eq, xor)  \
          a(lt, ne, ne) a(lt, ne, eq) a(lt, ne, xor)                           \
                                                                               \
              a(le, lt, ne) a(le, lt, eq) a(le, lt, xor) a(le, le, ne)         \
                  a(le, le, eq) a(le, le, xor) a(le, gt, ne) a(le, gt, eq) a(  \
                      le, gt, xor) a(le, ge, ne) a(le, ge, eq) a(le, ge, xor)  \
                      a(le, eq, ne) a(le, eq, eq) a(le, eq, xor) a(le, ne, ne) \
                          a(le, ne, eq) a(le, ne, xor)                         \
                                                                               \
                              a(gt, lt, ne) a(gt, lt, eq) a(gt, lt, xor)       \
                                  a(gt, le, ne) a(gt, le, eq) a(               \
                                      gt, le, xor) a(gt, gt, ne) a(gt, gt, eq) \
                                      a(gt, gt, xor) a(gt, ge, ne) a(          \
                                          gt, ge, eq) a(gt, ge, xor)           \
                                          a(gt, eq, ne) a(gt, eq, eq) a(       \
                                              gt, eq,                          \
                                              xor) a(gt, ne,                   \
                                                     ne) a(gt, ne,             \
                                                           eq) a(gt, ne, xor)  \
                                                                               \
                                              a(ge, lt, ne) a(ge, lt, eq) a(   \
                                                  ge, lt,                      \
                                                  xor) a(ge, le,               \
                                                         ne) a(ge, le, eq)     \
                                                  a(ge, le, xor) a(ge, gt, ne) \
                                                      a(ge, gt, eq) a(         \
                                                          ge, gt,              \
                                                          xor) a(ge, ge, ne)   \
                                                          a(ge, ge, eq) a(     \
                                                              ge, ge,          \
                                                              xor) a(ge, eq,   \
                                                                     ne) a(ge, \
                                                                           eq, \
                                                                           eq) \
                                                              a(ge, eq,        \
                                                                xor) a(ge, ne, \
                                                                       ne)     \
                                                                  a(ge, ne,    \
                                                                    eq) a(ge,  \
                                                                          ne,  \
                                                                          xor)

#define funcs1(a, b, c) func(, a, b, c) func(volatile, a, b, c)

funcs(funcs1)

#define test(op1, op2, op3)                                                    \
  do {                                                                         \
    if (op1##_##op2##_##op3##_(x, y) != op1##_##op2##_##op3##_volatile(x, y))  \
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
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_ne_:[0-9]+]] @lt_lt_ne_(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_ne_volatile:[0-9]+]] @lt_lt_ne_volatile(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_2]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_eq_:[0-9]+]] @lt_lt_eq_(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_3]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_eq_volatile:[0-9]+]] @lt_lt_eq_volatile(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_4]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_xor_:[0-9]+]] @lt_lt_xor_(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_5:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_5]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_5]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_lt_xor_volatile:[0-9]+]] @lt_lt_xor_volatile(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_6:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_6]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_6]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_ne_:[0-9]+]] @lt_le_ne_(%[[VALUE_a_7:[0-9]+]] a: i32, %[[VALUE_b_7:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_7:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:         let %[[VALUE_y_7:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_7]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_ne_volatile:[0-9]+]] @lt_le_ne_volatile(%[[VALUE_a_8:[0-9]+]] a: i32, %[[VALUE_b_8:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:         let %[[VALUE_y_8:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_8]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_eq_:[0-9]+]] @lt_le_eq_(%[[VALUE_a_9:[0-9]+]] a: i32, %[[VALUE_b_9:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_9:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:         let %[[VALUE_y_9:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_9]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_eq_volatile:[0-9]+]] @lt_le_eq_volatile(%[[VALUE_a_10:[0-9]+]] a: i32, %[[VALUE_b_10:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:         let %[[VALUE_y_10:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_10]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_xor_:[0-9]+]] @lt_le_xor_(%[[VALUE_a_11:[0-9]+]] a: i32, %[[VALUE_b_11:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_11:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:         let %[[VALUE_y_11:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_11]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_11]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_le_xor_volatile:[0-9]+]] @lt_le_xor_volatile(%[[VALUE_a_12:[0-9]+]] a: i32, %[[VALUE_b_12:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_12:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:         let %[[VALUE_y_12:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_12]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_12]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_ne_:[0-9]+]] @lt_gt_ne_(%[[VALUE_a_13:[0-9]+]] a: i32, %[[VALUE_b_13:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_13:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:         let %[[VALUE_y_13:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_13]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_ne_volatile:[0-9]+]] @lt_gt_ne_volatile(%[[VALUE_a_14:[0-9]+]] a: i32, %[[VALUE_b_14:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_14:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:         let %[[VALUE_y_14:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_14]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_eq_:[0-9]+]] @lt_gt_eq_(%[[VALUE_a_15:[0-9]+]] a: i32, %[[VALUE_b_15:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_15:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:         let %[[VALUE_y_15:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_15]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_eq_volatile:[0-9]+]] @lt_gt_eq_volatile(%[[VALUE_a_16:[0-9]+]] a: i32, %[[VALUE_b_16:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_16:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:         let %[[VALUE_y_16:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_16]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_xor_:[0-9]+]] @lt_gt_xor_(%[[VALUE_a_17:[0-9]+]] a: i32, %[[VALUE_b_17:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_17:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:         let %[[VALUE_y_17:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_17]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_17]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_xor_volatile:[0-9]+]] @lt_gt_xor_volatile(%[[VALUE_a_18:[0-9]+]] a: i32, %[[VALUE_b_18:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_18:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:         let %[[VALUE_y_18:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_18]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_18]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_ne_:[0-9]+]] @lt_ge_ne_(%[[VALUE_a_19:[0-9]+]] a: i32, %[[VALUE_b_19:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_19:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:         let %[[VALUE_y_19:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_19]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_19]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_ne_volatile:[0-9]+]] @lt_ge_ne_volatile(%[[VALUE_a_20:[0-9]+]] a: i32, %[[VALUE_b_20:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_20:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:         let %[[VALUE_y_20:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_20]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_20]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_eq_:[0-9]+]] @lt_ge_eq_(%[[VALUE_a_21:[0-9]+]] a: i32, %[[VALUE_b_21:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_21:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:         let %[[VALUE_y_21:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_21]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_21]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_eq_volatile:[0-9]+]] @lt_ge_eq_volatile(%[[VALUE_a_22:[0-9]+]] a: i32, %[[VALUE_b_22:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_22:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:         let %[[VALUE_y_22:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_22]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_22]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_xor_:[0-9]+]] @lt_ge_xor_(%[[VALUE_a_23:[0-9]+]] a: i32, %[[VALUE_b_23:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_23:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:         let %[[VALUE_y_23:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_23]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_23]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ge_xor_volatile:[0-9]+]] @lt_ge_xor_volatile(%[[VALUE_a_24:[0-9]+]] a: i32, %[[VALUE_b_24:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_24:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:         let %[[VALUE_y_24:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_24]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_24]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_ne_:[0-9]+]] @lt_eq_ne_(%[[VALUE_a_25:[0-9]+]] a: i32, %[[VALUE_b_25:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_25:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_25]]), read<i32>(%[[VALUE_b_25]]));
// DEFAULT-NEXT:         let %[[VALUE_y_25:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_25]]), read<i32>(%[[VALUE_b_25]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_25]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_25]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_ne_volatile:[0-9]+]] @lt_eq_ne_volatile(%[[VALUE_a_26:[0-9]+]] a: i32, %[[VALUE_b_26:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_26:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_26]]), read<i32>(%[[VALUE_b_26]]));
// DEFAULT-NEXT:         let %[[VALUE_y_26:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_26]]), read<i32>(%[[VALUE_b_26]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_26]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_26]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_eq_:[0-9]+]] @lt_eq_eq_(%[[VALUE_a_27:[0-9]+]] a: i32, %[[VALUE_b_27:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_27:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_27]]), read<i32>(%[[VALUE_b_27]]));
// DEFAULT-NEXT:         let %[[VALUE_y_27:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_27]]), read<i32>(%[[VALUE_b_27]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_27]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_27]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_eq_volatile:[0-9]+]] @lt_eq_eq_volatile(%[[VALUE_a_28:[0-9]+]] a: i32, %[[VALUE_b_28:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_28:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_28]]), read<i32>(%[[VALUE_b_28]]));
// DEFAULT-NEXT:         let %[[VALUE_y_28:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_28]]), read<i32>(%[[VALUE_b_28]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_28]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_28]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_xor_:[0-9]+]] @lt_eq_xor_(%[[VALUE_a_29:[0-9]+]] a: i32, %[[VALUE_b_29:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_29:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_29]]), read<i32>(%[[VALUE_b_29]]));
// DEFAULT-NEXT:         let %[[VALUE_y_29:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_29]]), read<i32>(%[[VALUE_b_29]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_29]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_29]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_eq_xor_volatile:[0-9]+]] @lt_eq_xor_volatile(%[[VALUE_a_30:[0-9]+]] a: i32, %[[VALUE_b_30:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_30:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_30]]), read<i32>(%[[VALUE_b_30]]));
// DEFAULT-NEXT:         let %[[VALUE_y_30:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_30]]), read<i32>(%[[VALUE_b_30]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_30]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_30]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_ne_:[0-9]+]] @lt_ne_ne_(%[[VALUE_a_31:[0-9]+]] a: i32, %[[VALUE_b_31:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_31:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_31]]), read<i32>(%[[VALUE_b_31]]));
// DEFAULT-NEXT:         let %[[VALUE_y_31:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_31]]), read<i32>(%[[VALUE_b_31]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_31]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_31]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_ne_volatile:[0-9]+]] @lt_ne_ne_volatile(%[[VALUE_a_32:[0-9]+]] a: i32, %[[VALUE_b_32:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_32:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_32]]), read<i32>(%[[VALUE_b_32]]));
// DEFAULT-NEXT:         let %[[VALUE_y_32:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_32]]), read<i32>(%[[VALUE_b_32]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_32]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_32]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_eq_:[0-9]+]] @lt_ne_eq_(%[[VALUE_a_33:[0-9]+]] a: i32, %[[VALUE_b_33:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_33:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_33]]), read<i32>(%[[VALUE_b_33]]));
// DEFAULT-NEXT:         let %[[VALUE_y_33:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_33]]), read<i32>(%[[VALUE_b_33]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_33]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_33]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_eq_volatile:[0-9]+]] @lt_ne_eq_volatile(%[[VALUE_a_34:[0-9]+]] a: i32, %[[VALUE_b_34:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_34:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_34]]), read<i32>(%[[VALUE_b_34]]));
// DEFAULT-NEXT:         let %[[VALUE_y_34:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_34]]), read<i32>(%[[VALUE_b_34]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_34]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_34]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_xor_:[0-9]+]] @lt_ne_xor_(%[[VALUE_a_35:[0-9]+]] a: i32, %[[VALUE_b_35:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_35:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_35]]), read<i32>(%[[VALUE_b_35]]));
// DEFAULT-NEXT:         let %[[VALUE_y_35:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_35]]), read<i32>(%[[VALUE_b_35]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_35]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_35]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_ne_xor_volatile:[0-9]+]] @lt_ne_xor_volatile(%[[VALUE_a_36:[0-9]+]] a: i32, %[[VALUE_b_36:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_36:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_36]]), read<i32>(%[[VALUE_b_36]]));
// DEFAULT-NEXT:         let %[[VALUE_y_36:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_36]]), read<i32>(%[[VALUE_b_36]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_36]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_36]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_ne_:[0-9]+]] @le_lt_ne_(%[[VALUE_a_37:[0-9]+]] a: i32, %[[VALUE_b_37:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_37:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_37]]), read<i32>(%[[VALUE_b_37]]));
// DEFAULT-NEXT:         let %[[VALUE_y_37:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_37]]), read<i32>(%[[VALUE_b_37]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_37]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_37]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_ne_volatile:[0-9]+]] @le_lt_ne_volatile(%[[VALUE_a_38:[0-9]+]] a: i32, %[[VALUE_b_38:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_38:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_38]]), read<i32>(%[[VALUE_b_38]]));
// DEFAULT-NEXT:         let %[[VALUE_y_38:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_38]]), read<i32>(%[[VALUE_b_38]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_38]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_38]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_eq_:[0-9]+]] @le_lt_eq_(%[[VALUE_a_39:[0-9]+]] a: i32, %[[VALUE_b_39:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_39:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_39]]), read<i32>(%[[VALUE_b_39]]));
// DEFAULT-NEXT:         let %[[VALUE_y_39:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_39]]), read<i32>(%[[VALUE_b_39]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_39]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_39]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_eq_volatile:[0-9]+]] @le_lt_eq_volatile(%[[VALUE_a_40:[0-9]+]] a: i32, %[[VALUE_b_40:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_40:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_40]]), read<i32>(%[[VALUE_b_40]]));
// DEFAULT-NEXT:         let %[[VALUE_y_40:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_40]]), read<i32>(%[[VALUE_b_40]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_40]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_40]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_xor_:[0-9]+]] @le_lt_xor_(%[[VALUE_a_41:[0-9]+]] a: i32, %[[VALUE_b_41:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_41:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_41]]), read<i32>(%[[VALUE_b_41]]));
// DEFAULT-NEXT:         let %[[VALUE_y_41:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_41]]), read<i32>(%[[VALUE_b_41]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_41]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_41]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_lt_xor_volatile:[0-9]+]] @le_lt_xor_volatile(%[[VALUE_a_42:[0-9]+]] a: i32, %[[VALUE_b_42:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_42:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_42]]), read<i32>(%[[VALUE_b_42]]));
// DEFAULT-NEXT:         let %[[VALUE_y_42:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_42]]), read<i32>(%[[VALUE_b_42]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_42]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_42]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_ne_:[0-9]+]] @le_le_ne_(%[[VALUE_a_43:[0-9]+]] a: i32, %[[VALUE_b_43:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_43:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_43]]), read<i32>(%[[VALUE_b_43]]));
// DEFAULT-NEXT:         let %[[VALUE_y_43:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_43]]), read<i32>(%[[VALUE_b_43]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_43]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_43]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_ne_volatile:[0-9]+]] @le_le_ne_volatile(%[[VALUE_a_44:[0-9]+]] a: i32, %[[VALUE_b_44:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_44:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_44]]), read<i32>(%[[VALUE_b_44]]));
// DEFAULT-NEXT:         let %[[VALUE_y_44:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_44]]), read<i32>(%[[VALUE_b_44]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_44]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_44]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_eq_:[0-9]+]] @le_le_eq_(%[[VALUE_a_45:[0-9]+]] a: i32, %[[VALUE_b_45:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_45:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_45]]), read<i32>(%[[VALUE_b_45]]));
// DEFAULT-NEXT:         let %[[VALUE_y_45:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_45]]), read<i32>(%[[VALUE_b_45]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_45]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_45]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_eq_volatile:[0-9]+]] @le_le_eq_volatile(%[[VALUE_a_46:[0-9]+]] a: i32, %[[VALUE_b_46:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_46:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_46]]), read<i32>(%[[VALUE_b_46]]));
// DEFAULT-NEXT:         let %[[VALUE_y_46:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_46]]), read<i32>(%[[VALUE_b_46]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_46]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_46]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_xor_:[0-9]+]] @le_le_xor_(%[[VALUE_a_47:[0-9]+]] a: i32, %[[VALUE_b_47:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_47:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_47]]), read<i32>(%[[VALUE_b_47]]));
// DEFAULT-NEXT:         let %[[VALUE_y_47:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_47]]), read<i32>(%[[VALUE_b_47]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_47]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_47]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_le_xor_volatile:[0-9]+]] @le_le_xor_volatile(%[[VALUE_a_48:[0-9]+]] a: i32, %[[VALUE_b_48:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_48:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_48]]), read<i32>(%[[VALUE_b_48]]));
// DEFAULT-NEXT:         let %[[VALUE_y_48:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_48]]), read<i32>(%[[VALUE_b_48]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_48]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_48]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_ne_:[0-9]+]] @le_gt_ne_(%[[VALUE_a_49:[0-9]+]] a: i32, %[[VALUE_b_49:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_49:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_49]]), read<i32>(%[[VALUE_b_49]]));
// DEFAULT-NEXT:         let %[[VALUE_y_49:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_49]]), read<i32>(%[[VALUE_b_49]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_49]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_49]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_ne_volatile:[0-9]+]] @le_gt_ne_volatile(%[[VALUE_a_50:[0-9]+]] a: i32, %[[VALUE_b_50:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_50:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_50]]), read<i32>(%[[VALUE_b_50]]));
// DEFAULT-NEXT:         let %[[VALUE_y_50:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_50]]), read<i32>(%[[VALUE_b_50]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_50]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_50]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_eq_:[0-9]+]] @le_gt_eq_(%[[VALUE_a_51:[0-9]+]] a: i32, %[[VALUE_b_51:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_51:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_51]]), read<i32>(%[[VALUE_b_51]]));
// DEFAULT-NEXT:         let %[[VALUE_y_51:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_51]]), read<i32>(%[[VALUE_b_51]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_51]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_51]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_eq_volatile:[0-9]+]] @le_gt_eq_volatile(%[[VALUE_a_52:[0-9]+]] a: i32, %[[VALUE_b_52:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_52:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_52]]), read<i32>(%[[VALUE_b_52]]));
// DEFAULT-NEXT:         let %[[VALUE_y_52:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_52]]), read<i32>(%[[VALUE_b_52]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_52]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_52]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_xor_:[0-9]+]] @le_gt_xor_(%[[VALUE_a_53:[0-9]+]] a: i32, %[[VALUE_b_53:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_53:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_53]]), read<i32>(%[[VALUE_b_53]]));
// DEFAULT-NEXT:         let %[[VALUE_y_53:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_53]]), read<i32>(%[[VALUE_b_53]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_53]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_53]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_gt_xor_volatile:[0-9]+]] @le_gt_xor_volatile(%[[VALUE_a_54:[0-9]+]] a: i32, %[[VALUE_b_54:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_54:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_54]]), read<i32>(%[[VALUE_b_54]]));
// DEFAULT-NEXT:         let %[[VALUE_y_54:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_54]]), read<i32>(%[[VALUE_b_54]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_54]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_54]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_ne_:[0-9]+]] @le_ge_ne_(%[[VALUE_a_55:[0-9]+]] a: i32, %[[VALUE_b_55:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_55:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_55]]), read<i32>(%[[VALUE_b_55]]));
// DEFAULT-NEXT:         let %[[VALUE_y_55:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_55]]), read<i32>(%[[VALUE_b_55]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_55]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_55]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_ne_volatile:[0-9]+]] @le_ge_ne_volatile(%[[VALUE_a_56:[0-9]+]] a: i32, %[[VALUE_b_56:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_56:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_56]]), read<i32>(%[[VALUE_b_56]]));
// DEFAULT-NEXT:         let %[[VALUE_y_56:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_56]]), read<i32>(%[[VALUE_b_56]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_56]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_56]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_eq_:[0-9]+]] @le_ge_eq_(%[[VALUE_a_57:[0-9]+]] a: i32, %[[VALUE_b_57:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_57:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_57]]), read<i32>(%[[VALUE_b_57]]));
// DEFAULT-NEXT:         let %[[VALUE_y_57:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_57]]), read<i32>(%[[VALUE_b_57]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_57]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_57]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_eq_volatile:[0-9]+]] @le_ge_eq_volatile(%[[VALUE_a_58:[0-9]+]] a: i32, %[[VALUE_b_58:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_58:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_58]]), read<i32>(%[[VALUE_b_58]]));
// DEFAULT-NEXT:         let %[[VALUE_y_58:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_58]]), read<i32>(%[[VALUE_b_58]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_58]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_58]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_xor_:[0-9]+]] @le_ge_xor_(%[[VALUE_a_59:[0-9]+]] a: i32, %[[VALUE_b_59:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_59:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_59]]), read<i32>(%[[VALUE_b_59]]));
// DEFAULT-NEXT:         let %[[VALUE_y_59:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_59]]), read<i32>(%[[VALUE_b_59]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_59]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_59]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_xor_volatile:[0-9]+]] @le_ge_xor_volatile(%[[VALUE_a_60:[0-9]+]] a: i32, %[[VALUE_b_60:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_60:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_60]]), read<i32>(%[[VALUE_b_60]]));
// DEFAULT-NEXT:         let %[[VALUE_y_60:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_60]]), read<i32>(%[[VALUE_b_60]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_60]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_60]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_ne_:[0-9]+]] @le_eq_ne_(%[[VALUE_a_61:[0-9]+]] a: i32, %[[VALUE_b_61:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_61:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_61]]), read<i32>(%[[VALUE_b_61]]));
// DEFAULT-NEXT:         let %[[VALUE_y_61:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_61]]), read<i32>(%[[VALUE_b_61]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_61]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_61]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_ne_volatile:[0-9]+]] @le_eq_ne_volatile(%[[VALUE_a_62:[0-9]+]] a: i32, %[[VALUE_b_62:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_62:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_62]]), read<i32>(%[[VALUE_b_62]]));
// DEFAULT-NEXT:         let %[[VALUE_y_62:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_62]]), read<i32>(%[[VALUE_b_62]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_62]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_62]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_eq_:[0-9]+]] @le_eq_eq_(%[[VALUE_a_63:[0-9]+]] a: i32, %[[VALUE_b_63:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_63:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_63]]), read<i32>(%[[VALUE_b_63]]));
// DEFAULT-NEXT:         let %[[VALUE_y_63:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_63]]), read<i32>(%[[VALUE_b_63]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_63]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_63]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_eq_volatile:[0-9]+]] @le_eq_eq_volatile(%[[VALUE_a_64:[0-9]+]] a: i32, %[[VALUE_b_64:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_64:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_64]]), read<i32>(%[[VALUE_b_64]]));
// DEFAULT-NEXT:         let %[[VALUE_y_64:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_64]]), read<i32>(%[[VALUE_b_64]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_64]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_64]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_xor_:[0-9]+]] @le_eq_xor_(%[[VALUE_a_65:[0-9]+]] a: i32, %[[VALUE_b_65:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_65:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_65]]), read<i32>(%[[VALUE_b_65]]));
// DEFAULT-NEXT:         let %[[VALUE_y_65:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_65]]), read<i32>(%[[VALUE_b_65]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_65]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_65]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_eq_xor_volatile:[0-9]+]] @le_eq_xor_volatile(%[[VALUE_a_66:[0-9]+]] a: i32, %[[VALUE_b_66:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_66:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_66]]), read<i32>(%[[VALUE_b_66]]));
// DEFAULT-NEXT:         let %[[VALUE_y_66:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_66]]), read<i32>(%[[VALUE_b_66]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_66]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_66]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_ne_:[0-9]+]] @le_ne_ne_(%[[VALUE_a_67:[0-9]+]] a: i32, %[[VALUE_b_67:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_67:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_67]]), read<i32>(%[[VALUE_b_67]]));
// DEFAULT-NEXT:         let %[[VALUE_y_67:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_67]]), read<i32>(%[[VALUE_b_67]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_67]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_67]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_ne_volatile:[0-9]+]] @le_ne_ne_volatile(%[[VALUE_a_68:[0-9]+]] a: i32, %[[VALUE_b_68:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_68:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_68]]), read<i32>(%[[VALUE_b_68]]));
// DEFAULT-NEXT:         let %[[VALUE_y_68:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_68]]), read<i32>(%[[VALUE_b_68]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_68]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_68]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_eq_:[0-9]+]] @le_ne_eq_(%[[VALUE_a_69:[0-9]+]] a: i32, %[[VALUE_b_69:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_69:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_69]]), read<i32>(%[[VALUE_b_69]]));
// DEFAULT-NEXT:         let %[[VALUE_y_69:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_69]]), read<i32>(%[[VALUE_b_69]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_69]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_69]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_eq_volatile:[0-9]+]] @le_ne_eq_volatile(%[[VALUE_a_70:[0-9]+]] a: i32, %[[VALUE_b_70:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_70:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_70]]), read<i32>(%[[VALUE_b_70]]));
// DEFAULT-NEXT:         let %[[VALUE_y_70:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_70]]), read<i32>(%[[VALUE_b_70]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_70]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_70]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_xor_:[0-9]+]] @le_ne_xor_(%[[VALUE_a_71:[0-9]+]] a: i32, %[[VALUE_b_71:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_71:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_71]]), read<i32>(%[[VALUE_b_71]]));
// DEFAULT-NEXT:         let %[[VALUE_y_71:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_71]]), read<i32>(%[[VALUE_b_71]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_71]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_71]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ne_xor_volatile:[0-9]+]] @le_ne_xor_volatile(%[[VALUE_a_72:[0-9]+]] a: i32, %[[VALUE_b_72:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_72:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_72]]), read<i32>(%[[VALUE_b_72]]));
// DEFAULT-NEXT:         let %[[VALUE_y_72:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_72]]), read<i32>(%[[VALUE_b_72]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_72]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_72]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_ne_:[0-9]+]] @gt_lt_ne_(%[[VALUE_a_73:[0-9]+]] a: i32, %[[VALUE_b_73:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_73:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_73]]), read<i32>(%[[VALUE_b_73]]));
// DEFAULT-NEXT:         let %[[VALUE_y_73:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_73]]), read<i32>(%[[VALUE_b_73]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_73]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_73]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_ne_volatile:[0-9]+]] @gt_lt_ne_volatile(%[[VALUE_a_74:[0-9]+]] a: i32, %[[VALUE_b_74:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_74:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_74]]), read<i32>(%[[VALUE_b_74]]));
// DEFAULT-NEXT:         let %[[VALUE_y_74:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_74]]), read<i32>(%[[VALUE_b_74]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_74]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_74]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_eq_:[0-9]+]] @gt_lt_eq_(%[[VALUE_a_75:[0-9]+]] a: i32, %[[VALUE_b_75:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_75:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_75]]), read<i32>(%[[VALUE_b_75]]));
// DEFAULT-NEXT:         let %[[VALUE_y_75:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_75]]), read<i32>(%[[VALUE_b_75]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_75]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_75]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_eq_volatile:[0-9]+]] @gt_lt_eq_volatile(%[[VALUE_a_76:[0-9]+]] a: i32, %[[VALUE_b_76:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_76:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_76]]), read<i32>(%[[VALUE_b_76]]));
// DEFAULT-NEXT:         let %[[VALUE_y_76:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_76]]), read<i32>(%[[VALUE_b_76]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_76]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_76]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_xor_:[0-9]+]] @gt_lt_xor_(%[[VALUE_a_77:[0-9]+]] a: i32, %[[VALUE_b_77:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_77:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_77]]), read<i32>(%[[VALUE_b_77]]));
// DEFAULT-NEXT:         let %[[VALUE_y_77:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_77]]), read<i32>(%[[VALUE_b_77]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_77]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_77]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_xor_volatile:[0-9]+]] @gt_lt_xor_volatile(%[[VALUE_a_78:[0-9]+]] a: i32, %[[VALUE_b_78:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_78:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_78]]), read<i32>(%[[VALUE_b_78]]));
// DEFAULT-NEXT:         let %[[VALUE_y_78:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_78]]), read<i32>(%[[VALUE_b_78]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_78]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_78]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_ne_:[0-9]+]] @gt_le_ne_(%[[VALUE_a_79:[0-9]+]] a: i32, %[[VALUE_b_79:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_79:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_79]]), read<i32>(%[[VALUE_b_79]]));
// DEFAULT-NEXT:         let %[[VALUE_y_79:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_79]]), read<i32>(%[[VALUE_b_79]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_79]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_79]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_ne_volatile:[0-9]+]] @gt_le_ne_volatile(%[[VALUE_a_80:[0-9]+]] a: i32, %[[VALUE_b_80:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_80:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_80]]), read<i32>(%[[VALUE_b_80]]));
// DEFAULT-NEXT:         let %[[VALUE_y_80:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_80]]), read<i32>(%[[VALUE_b_80]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_80]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_80]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_eq_:[0-9]+]] @gt_le_eq_(%[[VALUE_a_81:[0-9]+]] a: i32, %[[VALUE_b_81:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_81:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_81]]), read<i32>(%[[VALUE_b_81]]));
// DEFAULT-NEXT:         let %[[VALUE_y_81:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_81]]), read<i32>(%[[VALUE_b_81]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_81]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_81]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_eq_volatile:[0-9]+]] @gt_le_eq_volatile(%[[VALUE_a_82:[0-9]+]] a: i32, %[[VALUE_b_82:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_82:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_82]]), read<i32>(%[[VALUE_b_82]]));
// DEFAULT-NEXT:         let %[[VALUE_y_82:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_82]]), read<i32>(%[[VALUE_b_82]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_82]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_82]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_xor_:[0-9]+]] @gt_le_xor_(%[[VALUE_a_83:[0-9]+]] a: i32, %[[VALUE_b_83:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_83:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_83]]), read<i32>(%[[VALUE_b_83]]));
// DEFAULT-NEXT:         let %[[VALUE_y_83:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_83]]), read<i32>(%[[VALUE_b_83]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_83]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_83]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_le_xor_volatile:[0-9]+]] @gt_le_xor_volatile(%[[VALUE_a_84:[0-9]+]] a: i32, %[[VALUE_b_84:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_84:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_84]]), read<i32>(%[[VALUE_b_84]]));
// DEFAULT-NEXT:         let %[[VALUE_y_84:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_84]]), read<i32>(%[[VALUE_b_84]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_84]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_84]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_ne_:[0-9]+]] @gt_gt_ne_(%[[VALUE_a_85:[0-9]+]] a: i32, %[[VALUE_b_85:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_85:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_85]]), read<i32>(%[[VALUE_b_85]]));
// DEFAULT-NEXT:         let %[[VALUE_y_85:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_85]]), read<i32>(%[[VALUE_b_85]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_85]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_85]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_ne_volatile:[0-9]+]] @gt_gt_ne_volatile(%[[VALUE_a_86:[0-9]+]] a: i32, %[[VALUE_b_86:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_86:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_86]]), read<i32>(%[[VALUE_b_86]]));
// DEFAULT-NEXT:         let %[[VALUE_y_86:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_86]]), read<i32>(%[[VALUE_b_86]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_86]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_86]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_eq_:[0-9]+]] @gt_gt_eq_(%[[VALUE_a_87:[0-9]+]] a: i32, %[[VALUE_b_87:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_87:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_87]]), read<i32>(%[[VALUE_b_87]]));
// DEFAULT-NEXT:         let %[[VALUE_y_87:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_87]]), read<i32>(%[[VALUE_b_87]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_87]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_87]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_eq_volatile:[0-9]+]] @gt_gt_eq_volatile(%[[VALUE_a_88:[0-9]+]] a: i32, %[[VALUE_b_88:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_88:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_88]]), read<i32>(%[[VALUE_b_88]]));
// DEFAULT-NEXT:         let %[[VALUE_y_88:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_88]]), read<i32>(%[[VALUE_b_88]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_88]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_88]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_xor_:[0-9]+]] @gt_gt_xor_(%[[VALUE_a_89:[0-9]+]] a: i32, %[[VALUE_b_89:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_89:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_89]]), read<i32>(%[[VALUE_b_89]]));
// DEFAULT-NEXT:         let %[[VALUE_y_89:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_89]]), read<i32>(%[[VALUE_b_89]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_89]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_89]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_gt_xor_volatile:[0-9]+]] @gt_gt_xor_volatile(%[[VALUE_a_90:[0-9]+]] a: i32, %[[VALUE_b_90:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_90:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_90]]), read<i32>(%[[VALUE_b_90]]));
// DEFAULT-NEXT:         let %[[VALUE_y_90:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_90]]), read<i32>(%[[VALUE_b_90]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_90]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_90]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_ne_:[0-9]+]] @gt_ge_ne_(%[[VALUE_a_91:[0-9]+]] a: i32, %[[VALUE_b_91:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_91:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_91]]), read<i32>(%[[VALUE_b_91]]));
// DEFAULT-NEXT:         let %[[VALUE_y_91:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_91]]), read<i32>(%[[VALUE_b_91]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_91]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_91]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_ne_volatile:[0-9]+]] @gt_ge_ne_volatile(%[[VALUE_a_92:[0-9]+]] a: i32, %[[VALUE_b_92:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_92:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_92]]), read<i32>(%[[VALUE_b_92]]));
// DEFAULT-NEXT:         let %[[VALUE_y_92:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_92]]), read<i32>(%[[VALUE_b_92]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_92]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_92]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_eq_:[0-9]+]] @gt_ge_eq_(%[[VALUE_a_93:[0-9]+]] a: i32, %[[VALUE_b_93:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_93:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_93]]), read<i32>(%[[VALUE_b_93]]));
// DEFAULT-NEXT:         let %[[VALUE_y_93:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_93]]), read<i32>(%[[VALUE_b_93]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_93]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_93]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_eq_volatile:[0-9]+]] @gt_ge_eq_volatile(%[[VALUE_a_94:[0-9]+]] a: i32, %[[VALUE_b_94:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_94:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_94]]), read<i32>(%[[VALUE_b_94]]));
// DEFAULT-NEXT:         let %[[VALUE_y_94:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_94]]), read<i32>(%[[VALUE_b_94]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_94]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_94]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_xor_:[0-9]+]] @gt_ge_xor_(%[[VALUE_a_95:[0-9]+]] a: i32, %[[VALUE_b_95:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_95:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_95]]), read<i32>(%[[VALUE_b_95]]));
// DEFAULT-NEXT:         let %[[VALUE_y_95:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_95]]), read<i32>(%[[VALUE_b_95]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_95]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_95]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ge_xor_volatile:[0-9]+]] @gt_ge_xor_volatile(%[[VALUE_a_96:[0-9]+]] a: i32, %[[VALUE_b_96:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_96:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_96]]), read<i32>(%[[VALUE_b_96]]));
// DEFAULT-NEXT:         let %[[VALUE_y_96:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_96]]), read<i32>(%[[VALUE_b_96]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_96]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_96]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_ne_:[0-9]+]] @gt_eq_ne_(%[[VALUE_a_97:[0-9]+]] a: i32, %[[VALUE_b_97:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_97:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_97]]), read<i32>(%[[VALUE_b_97]]));
// DEFAULT-NEXT:         let %[[VALUE_y_97:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_97]]), read<i32>(%[[VALUE_b_97]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_97]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_97]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_ne_volatile:[0-9]+]] @gt_eq_ne_volatile(%[[VALUE_a_98:[0-9]+]] a: i32, %[[VALUE_b_98:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_98:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_98]]), read<i32>(%[[VALUE_b_98]]));
// DEFAULT-NEXT:         let %[[VALUE_y_98:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_98]]), read<i32>(%[[VALUE_b_98]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_98]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_98]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_eq_:[0-9]+]] @gt_eq_eq_(%[[VALUE_a_99:[0-9]+]] a: i32, %[[VALUE_b_99:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_99:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_99]]), read<i32>(%[[VALUE_b_99]]));
// DEFAULT-NEXT:         let %[[VALUE_y_99:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_99]]), read<i32>(%[[VALUE_b_99]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_99]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_99]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_eq_volatile:[0-9]+]] @gt_eq_eq_volatile(%[[VALUE_a_100:[0-9]+]] a: i32, %[[VALUE_b_100:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_100:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_100]]), read<i32>(%[[VALUE_b_100]]));
// DEFAULT-NEXT:         let %[[VALUE_y_100:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_100]]), read<i32>(%[[VALUE_b_100]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_100]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_100]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_xor_:[0-9]+]] @gt_eq_xor_(%[[VALUE_a_101:[0-9]+]] a: i32, %[[VALUE_b_101:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_101:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_101]]), read<i32>(%[[VALUE_b_101]]));
// DEFAULT-NEXT:         let %[[VALUE_y_101:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_101]]), read<i32>(%[[VALUE_b_101]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_101]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_101]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_eq_xor_volatile:[0-9]+]] @gt_eq_xor_volatile(%[[VALUE_a_102:[0-9]+]] a: i32, %[[VALUE_b_102:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_102:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_102]]), read<i32>(%[[VALUE_b_102]]));
// DEFAULT-NEXT:         let %[[VALUE_y_102:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_102]]), read<i32>(%[[VALUE_b_102]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_102]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_102]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_ne_:[0-9]+]] @gt_ne_ne_(%[[VALUE_a_103:[0-9]+]] a: i32, %[[VALUE_b_103:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_103:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_103]]), read<i32>(%[[VALUE_b_103]]));
// DEFAULT-NEXT:         let %[[VALUE_y_103:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_103]]), read<i32>(%[[VALUE_b_103]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_103]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_103]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_ne_volatile:[0-9]+]] @gt_ne_ne_volatile(%[[VALUE_a_104:[0-9]+]] a: i32, %[[VALUE_b_104:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_104:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_104]]), read<i32>(%[[VALUE_b_104]]));
// DEFAULT-NEXT:         let %[[VALUE_y_104:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_104]]), read<i32>(%[[VALUE_b_104]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_104]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_104]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_eq_:[0-9]+]] @gt_ne_eq_(%[[VALUE_a_105:[0-9]+]] a: i32, %[[VALUE_b_105:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_105:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_105]]), read<i32>(%[[VALUE_b_105]]));
// DEFAULT-NEXT:         let %[[VALUE_y_105:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_105]]), read<i32>(%[[VALUE_b_105]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_105]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_105]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_eq_volatile:[0-9]+]] @gt_ne_eq_volatile(%[[VALUE_a_106:[0-9]+]] a: i32, %[[VALUE_b_106:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_106:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_106]]), read<i32>(%[[VALUE_b_106]]));
// DEFAULT-NEXT:         let %[[VALUE_y_106:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_106]]), read<i32>(%[[VALUE_b_106]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_106]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_106]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_xor_:[0-9]+]] @gt_ne_xor_(%[[VALUE_a_107:[0-9]+]] a: i32, %[[VALUE_b_107:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_107:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_107]]), read<i32>(%[[VALUE_b_107]]));
// DEFAULT-NEXT:         let %[[VALUE_y_107:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_107]]), read<i32>(%[[VALUE_b_107]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_107]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_107]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_ne_xor_volatile:[0-9]+]] @gt_ne_xor_volatile(%[[VALUE_a_108:[0-9]+]] a: i32, %[[VALUE_b_108:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_108:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_108]]), read<i32>(%[[VALUE_b_108]]));
// DEFAULT-NEXT:         let %[[VALUE_y_108:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_108]]), read<i32>(%[[VALUE_b_108]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_108]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_108]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_ne_:[0-9]+]] @ge_lt_ne_(%[[VALUE_a_109:[0-9]+]] a: i32, %[[VALUE_b_109:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_109:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_109]]), read<i32>(%[[VALUE_b_109]]));
// DEFAULT-NEXT:         let %[[VALUE_y_109:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_109]]), read<i32>(%[[VALUE_b_109]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_109]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_109]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_ne_volatile:[0-9]+]] @ge_lt_ne_volatile(%[[VALUE_a_110:[0-9]+]] a: i32, %[[VALUE_b_110:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_110:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_110]]), read<i32>(%[[VALUE_b_110]]));
// DEFAULT-NEXT:         let %[[VALUE_y_110:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_110]]), read<i32>(%[[VALUE_b_110]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_110]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_110]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_eq_:[0-9]+]] @ge_lt_eq_(%[[VALUE_a_111:[0-9]+]] a: i32, %[[VALUE_b_111:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_111:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_111]]), read<i32>(%[[VALUE_b_111]]));
// DEFAULT-NEXT:         let %[[VALUE_y_111:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_111]]), read<i32>(%[[VALUE_b_111]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_111]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_111]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_eq_volatile:[0-9]+]] @ge_lt_eq_volatile(%[[VALUE_a_112:[0-9]+]] a: i32, %[[VALUE_b_112:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_112:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_112]]), read<i32>(%[[VALUE_b_112]]));
// DEFAULT-NEXT:         let %[[VALUE_y_112:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_112]]), read<i32>(%[[VALUE_b_112]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_112]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_112]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_xor_:[0-9]+]] @ge_lt_xor_(%[[VALUE_a_113:[0-9]+]] a: i32, %[[VALUE_b_113:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_113:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_113]]), read<i32>(%[[VALUE_b_113]]));
// DEFAULT-NEXT:         let %[[VALUE_y_113:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_113]]), read<i32>(%[[VALUE_b_113]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_113]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_113]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_lt_xor_volatile:[0-9]+]] @ge_lt_xor_volatile(%[[VALUE_a_114:[0-9]+]] a: i32, %[[VALUE_b_114:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_114:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_114]]), read<i32>(%[[VALUE_b_114]]));
// DEFAULT-NEXT:         let %[[VALUE_y_114:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_114]]), read<i32>(%[[VALUE_b_114]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_114]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_114]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_ne_:[0-9]+]] @ge_le_ne_(%[[VALUE_a_115:[0-9]+]] a: i32, %[[VALUE_b_115:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_115:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_115]]), read<i32>(%[[VALUE_b_115]]));
// DEFAULT-NEXT:         let %[[VALUE_y_115:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_115]]), read<i32>(%[[VALUE_b_115]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_115]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_115]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_ne_volatile:[0-9]+]] @ge_le_ne_volatile(%[[VALUE_a_116:[0-9]+]] a: i32, %[[VALUE_b_116:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_116:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_116]]), read<i32>(%[[VALUE_b_116]]));
// DEFAULT-NEXT:         let %[[VALUE_y_116:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_116]]), read<i32>(%[[VALUE_b_116]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_116]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_116]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_eq_:[0-9]+]] @ge_le_eq_(%[[VALUE_a_117:[0-9]+]] a: i32, %[[VALUE_b_117:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_117:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_117]]), read<i32>(%[[VALUE_b_117]]));
// DEFAULT-NEXT:         let %[[VALUE_y_117:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_117]]), read<i32>(%[[VALUE_b_117]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_117]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_117]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_eq_volatile:[0-9]+]] @ge_le_eq_volatile(%[[VALUE_a_118:[0-9]+]] a: i32, %[[VALUE_b_118:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_118:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_118]]), read<i32>(%[[VALUE_b_118]]));
// DEFAULT-NEXT:         let %[[VALUE_y_118:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_118]]), read<i32>(%[[VALUE_b_118]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_118]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_118]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_xor_:[0-9]+]] @ge_le_xor_(%[[VALUE_a_119:[0-9]+]] a: i32, %[[VALUE_b_119:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_119:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_119]]), read<i32>(%[[VALUE_b_119]]));
// DEFAULT-NEXT:         let %[[VALUE_y_119:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_119]]), read<i32>(%[[VALUE_b_119]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_119]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_119]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_xor_volatile:[0-9]+]] @ge_le_xor_volatile(%[[VALUE_a_120:[0-9]+]] a: i32, %[[VALUE_b_120:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_120:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_120]]), read<i32>(%[[VALUE_b_120]]));
// DEFAULT-NEXT:         let %[[VALUE_y_120:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_120]]), read<i32>(%[[VALUE_b_120]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_120]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_120]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_ne_:[0-9]+]] @ge_gt_ne_(%[[VALUE_a_121:[0-9]+]] a: i32, %[[VALUE_b_121:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_121:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_121]]), read<i32>(%[[VALUE_b_121]]));
// DEFAULT-NEXT:         let %[[VALUE_y_121:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_121]]), read<i32>(%[[VALUE_b_121]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_121]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_121]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_ne_volatile:[0-9]+]] @ge_gt_ne_volatile(%[[VALUE_a_122:[0-9]+]] a: i32, %[[VALUE_b_122:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_122:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_122]]), read<i32>(%[[VALUE_b_122]]));
// DEFAULT-NEXT:         let %[[VALUE_y_122:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_122]]), read<i32>(%[[VALUE_b_122]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_122]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_122]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_eq_:[0-9]+]] @ge_gt_eq_(%[[VALUE_a_123:[0-9]+]] a: i32, %[[VALUE_b_123:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_123:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_123]]), read<i32>(%[[VALUE_b_123]]));
// DEFAULT-NEXT:         let %[[VALUE_y_123:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_123]]), read<i32>(%[[VALUE_b_123]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_123]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_123]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_eq_volatile:[0-9]+]] @ge_gt_eq_volatile(%[[VALUE_a_124:[0-9]+]] a: i32, %[[VALUE_b_124:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_124:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_124]]), read<i32>(%[[VALUE_b_124]]));
// DEFAULT-NEXT:         let %[[VALUE_y_124:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_124]]), read<i32>(%[[VALUE_b_124]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_124]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_124]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_xor_:[0-9]+]] @ge_gt_xor_(%[[VALUE_a_125:[0-9]+]] a: i32, %[[VALUE_b_125:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_125:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_125]]), read<i32>(%[[VALUE_b_125]]));
// DEFAULT-NEXT:         let %[[VALUE_y_125:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_125]]), read<i32>(%[[VALUE_b_125]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_125]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_125]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_gt_xor_volatile:[0-9]+]] @ge_gt_xor_volatile(%[[VALUE_a_126:[0-9]+]] a: i32, %[[VALUE_b_126:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_126:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_126]]), read<i32>(%[[VALUE_b_126]]));
// DEFAULT-NEXT:         let %[[VALUE_y_126:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_126]]), read<i32>(%[[VALUE_b_126]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_126]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_126]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_ne_:[0-9]+]] @ge_ge_ne_(%[[VALUE_a_127:[0-9]+]] a: i32, %[[VALUE_b_127:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_127:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_127]]), read<i32>(%[[VALUE_b_127]]));
// DEFAULT-NEXT:         let %[[VALUE_y_127:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_127]]), read<i32>(%[[VALUE_b_127]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_127]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_127]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_ne_volatile:[0-9]+]] @ge_ge_ne_volatile(%[[VALUE_a_128:[0-9]+]] a: i32, %[[VALUE_b_128:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_128:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_128]]), read<i32>(%[[VALUE_b_128]]));
// DEFAULT-NEXT:         let %[[VALUE_y_128:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_128]]), read<i32>(%[[VALUE_b_128]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_128]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_128]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_eq_:[0-9]+]] @ge_ge_eq_(%[[VALUE_a_129:[0-9]+]] a: i32, %[[VALUE_b_129:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_129:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_129]]), read<i32>(%[[VALUE_b_129]]));
// DEFAULT-NEXT:         let %[[VALUE_y_129:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_129]]), read<i32>(%[[VALUE_b_129]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_129]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_129]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_eq_volatile:[0-9]+]] @ge_ge_eq_volatile(%[[VALUE_a_130:[0-9]+]] a: i32, %[[VALUE_b_130:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_130:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_130]]), read<i32>(%[[VALUE_b_130]]));
// DEFAULT-NEXT:         let %[[VALUE_y_130:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_130]]), read<i32>(%[[VALUE_b_130]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_130]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_130]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_xor_:[0-9]+]] @ge_ge_xor_(%[[VALUE_a_131:[0-9]+]] a: i32, %[[VALUE_b_131:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_131:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_131]]), read<i32>(%[[VALUE_b_131]]));
// DEFAULT-NEXT:         let %[[VALUE_y_131:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_131]]), read<i32>(%[[VALUE_b_131]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_131]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_131]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ge_xor_volatile:[0-9]+]] @ge_ge_xor_volatile(%[[VALUE_a_132:[0-9]+]] a: i32, %[[VALUE_b_132:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_132:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_132]]), read<i32>(%[[VALUE_b_132]]));
// DEFAULT-NEXT:         let %[[VALUE_y_132:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_132]]), read<i32>(%[[VALUE_b_132]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_132]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_132]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_ne_:[0-9]+]] @ge_eq_ne_(%[[VALUE_a_133:[0-9]+]] a: i32, %[[VALUE_b_133:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_133:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_133]]), read<i32>(%[[VALUE_b_133]]));
// DEFAULT-NEXT:         let %[[VALUE_y_133:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_133]]), read<i32>(%[[VALUE_b_133]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_133]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_133]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_ne_volatile:[0-9]+]] @ge_eq_ne_volatile(%[[VALUE_a_134:[0-9]+]] a: i32, %[[VALUE_b_134:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_134:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_134]]), read<i32>(%[[VALUE_b_134]]));
// DEFAULT-NEXT:         let %[[VALUE_y_134:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_134]]), read<i32>(%[[VALUE_b_134]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_134]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_134]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_eq_:[0-9]+]] @ge_eq_eq_(%[[VALUE_a_135:[0-9]+]] a: i32, %[[VALUE_b_135:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_135:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_135]]), read<i32>(%[[VALUE_b_135]]));
// DEFAULT-NEXT:         let %[[VALUE_y_135:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_135]]), read<i32>(%[[VALUE_b_135]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_135]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_135]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_eq_volatile:[0-9]+]] @ge_eq_eq_volatile(%[[VALUE_a_136:[0-9]+]] a: i32, %[[VALUE_b_136:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_136:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_136]]), read<i32>(%[[VALUE_b_136]]));
// DEFAULT-NEXT:         let %[[VALUE_y_136:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_136]]), read<i32>(%[[VALUE_b_136]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_136]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_136]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_xor_:[0-9]+]] @ge_eq_xor_(%[[VALUE_a_137:[0-9]+]] a: i32, %[[VALUE_b_137:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_137:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_137]]), read<i32>(%[[VALUE_b_137]]));
// DEFAULT-NEXT:         let %[[VALUE_y_137:[0-9]+]] y: bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_137]]), read<i32>(%[[VALUE_b_137]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_137]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_137]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_eq_xor_volatile:[0-9]+]] @ge_eq_xor_volatile(%[[VALUE_a_138:[0-9]+]] a: i32, %[[VALUE_b_138:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_138:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_138]]), read<i32>(%[[VALUE_b_138]]));
// DEFAULT-NEXT:         let %[[VALUE_y_138:[0-9]+]] y: volatile bool [storage=automatic] = eq<i32>(read<i32>(%[[VALUE_a_138]]), read<i32>(%[[VALUE_b_138]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_138]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_138]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_ne_:[0-9]+]] @ge_ne_ne_(%[[VALUE_a_139:[0-9]+]] a: i32, %[[VALUE_b_139:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_139:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_139]]), read<i32>(%[[VALUE_b_139]]));
// DEFAULT-NEXT:         let %[[VALUE_y_139:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_139]]), read<i32>(%[[VALUE_b_139]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_139]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_139]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_ne_volatile:[0-9]+]] @ge_ne_ne_volatile(%[[VALUE_a_140:[0-9]+]] a: i32, %[[VALUE_b_140:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_140:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_140]]), read<i32>(%[[VALUE_b_140]]));
// DEFAULT-NEXT:         let %[[VALUE_y_140:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_140]]), read<i32>(%[[VALUE_b_140]]));
// DEFAULT-NEXT:         return ne<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_140]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_140]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_eq_:[0-9]+]] @ge_ne_eq_(%[[VALUE_a_141:[0-9]+]] a: i32, %[[VALUE_b_141:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_141:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_141]]), read<i32>(%[[VALUE_b_141]]));
// DEFAULT-NEXT:         let %[[VALUE_y_141:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_141]]), read<i32>(%[[VALUE_b_141]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_141]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_141]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_eq_volatile:[0-9]+]] @ge_ne_eq_volatile(%[[VALUE_a_142:[0-9]+]] a: i32, %[[VALUE_b_142:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_142:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_142]]), read<i32>(%[[VALUE_b_142]]));
// DEFAULT-NEXT:         let %[[VALUE_y_142:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_142]]), read<i32>(%[[VALUE_b_142]]));
// DEFAULT-NEXT:         return eq<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_142]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_142]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_xor_:[0-9]+]] @ge_ne_xor_(%[[VALUE_a_143:[0-9]+]] a: i32, %[[VALUE_b_143:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_143:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_143]]), read<i32>(%[[VALUE_b_143]]));
// DEFAULT-NEXT:         let %[[VALUE_y_143:[0-9]+]] y: bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_143]]), read<i32>(%[[VALUE_b_143]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_143]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_143]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_ne_xor_volatile:[0-9]+]] @ge_ne_xor_volatile(%[[VALUE_a_144:[0-9]+]] a: i32, %[[VALUE_b_144:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_144:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_144]]), read<i32>(%[[VALUE_b_144]]));
// DEFAULT-NEXT:         let %[[VALUE_y_144:[0-9]+]] y: volatile bool [storage=automatic] = ne<i32>(read<i32>(%[[VALUE_a_144]]), read<i32>(%[[VALUE_b_144]]));
// DEFAULT-NEXT:         return ne<i32, reason=return>(xor<i32>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_144]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_144]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_x_145:[0-9]+]] x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_x_145]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_145]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_145]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_y_145:[0-9]+]] y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_y_145]]), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_145]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y_145]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_lt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_le_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ge_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_eq_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_ne_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_lt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_le_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_gt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_eq_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ne_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_le_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_gt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ge_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_eq_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE59:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_ne_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_lt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE63:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE68:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_gt_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE71:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ge_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_eq_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE75:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_ne_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_ne_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_eq_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_eq_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_xor_]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_ne_xor_volatile]], read<i32>(%[[VALUE_x_145]]), read<i32>(%[[VALUE_y_145]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
