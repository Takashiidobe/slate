#define func(vol, op1, op2, op3)                                               \
  _Bool op1##_##op2##_##op3##_##vol(int a, int b) {                            \
    vol _Bool x = op_##op1(a, b);                                              \
    vol _Bool y = op_##op2(a, b);                                              \
    return op_##op3(x - y, 0);                                                 \
  }

#define op_lt(a, b) ((a) < (b))
#define op_le(a, b) ((a) <= (b))
#define op_gt(a, b) ((a) > (b))
#define op_ge(a, b) ((a) >= (b))

#define funcs(a)                                                               \
  a(gt, lt, lt) a(gt, lt, le) a(gt, lt, gt) a(gt, lt, ge)                      \
                                                                               \
      a(ge, le, lt) a(ge, le, le) a(ge, le, gt) a(ge, le, ge)                  \
                                                                               \
          a(lt, gt, lt) a(lt, gt, le) a(lt, gt, gt) a(lt, gt, ge)              \
                                                                               \
              a(le, ge, lt) a(le, ge, le) a(le, ge, gt) a(le, ge, ge)

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
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_lt_:[0-9]+]] @gt_lt_lt_(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_lt_volatile:[0-9]+]] @gt_lt_lt_volatile(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_2]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_2]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_le_:[0-9]+]] @gt_lt_le_(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_3]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_3]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_le_volatile:[0-9]+]] @gt_lt_le_volatile(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_4]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_4]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_gt_:[0-9]+]] @gt_lt_gt_(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_5:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_5]]), read<i32>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_5]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_5]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_gt_volatile:[0-9]+]] @gt_lt_gt_volatile(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_6:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_6]]), read<i32>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_6]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_6]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_ge_:[0-9]+]] @gt_lt_ge_(%[[VALUE_a_7:[0-9]+]] a: i32, %[[VALUE_b_7:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_7:[0-9]+]] x: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:         let %[[VALUE_y_7:[0-9]+]] y: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_7]]), read<i32>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_7]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_7]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gt_lt_ge_volatile:[0-9]+]] @gt_lt_ge_volatile(%[[VALUE_a_8:[0-9]+]] a: i32, %[[VALUE_b_8:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:         let %[[VALUE_y_8:[0-9]+]] y: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_8]]), read<i32>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_8]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_8]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_lt_:[0-9]+]] @ge_le_lt_(%[[VALUE_a_9:[0-9]+]] a: i32, %[[VALUE_b_9:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_9:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:         let %[[VALUE_y_9:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_9]]), read<i32>(%[[VALUE_b_9]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_9]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_9]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_lt_volatile:[0-9]+]] @ge_le_lt_volatile(%[[VALUE_a_10:[0-9]+]] a: i32, %[[VALUE_b_10:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:         let %[[VALUE_y_10:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_10]]), read<i32>(%[[VALUE_b_10]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_10]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_10]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_le_:[0-9]+]] @ge_le_le_(%[[VALUE_a_11:[0-9]+]] a: i32, %[[VALUE_b_11:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_11:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:         let %[[VALUE_y_11:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_11]]), read<i32>(%[[VALUE_b_11]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_11]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_11]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_le_volatile:[0-9]+]] @ge_le_le_volatile(%[[VALUE_a_12:[0-9]+]] a: i32, %[[VALUE_b_12:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_12:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:         let %[[VALUE_y_12:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_12]]), read<i32>(%[[VALUE_b_12]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_12]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_12]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_gt_:[0-9]+]] @ge_le_gt_(%[[VALUE_a_13:[0-9]+]] a: i32, %[[VALUE_b_13:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_13:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:         let %[[VALUE_y_13:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_13]]), read<i32>(%[[VALUE_b_13]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_13]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_13]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_gt_volatile:[0-9]+]] @ge_le_gt_volatile(%[[VALUE_a_14:[0-9]+]] a: i32, %[[VALUE_b_14:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_14:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:         let %[[VALUE_y_14:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_14]]), read<i32>(%[[VALUE_b_14]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_14]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_14]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_ge_:[0-9]+]] @ge_le_ge_(%[[VALUE_a_15:[0-9]+]] a: i32, %[[VALUE_b_15:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_15:[0-9]+]] x: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:         let %[[VALUE_y_15:[0-9]+]] y: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_15]]), read<i32>(%[[VALUE_b_15]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_15]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_15]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge_le_ge_volatile:[0-9]+]] @ge_le_ge_volatile(%[[VALUE_a_16:[0-9]+]] a: i32, %[[VALUE_b_16:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_16:[0-9]+]] x: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:         let %[[VALUE_y_16:[0-9]+]] y: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_16]]), read<i32>(%[[VALUE_b_16]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_16]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_16]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_lt_:[0-9]+]] @lt_gt_lt_(%[[VALUE_a_17:[0-9]+]] a: i32, %[[VALUE_b_17:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_17:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:         let %[[VALUE_y_17:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_b_17]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_17]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_17]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_lt_volatile:[0-9]+]] @lt_gt_lt_volatile(%[[VALUE_a_18:[0-9]+]] a: i32, %[[VALUE_b_18:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_18:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:         let %[[VALUE_y_18:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_18]]), read<i32>(%[[VALUE_b_18]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_18]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_18]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_le_:[0-9]+]] @lt_gt_le_(%[[VALUE_a_19:[0-9]+]] a: i32, %[[VALUE_b_19:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_19:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:         let %[[VALUE_y_19:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_19]]), read<i32>(%[[VALUE_b_19]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_19]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_19]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_le_volatile:[0-9]+]] @lt_gt_le_volatile(%[[VALUE_a_20:[0-9]+]] a: i32, %[[VALUE_b_20:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_20:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:         let %[[VALUE_y_20:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_20]]), read<i32>(%[[VALUE_b_20]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_20]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_20]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_gt_:[0-9]+]] @lt_gt_gt_(%[[VALUE_a_21:[0-9]+]] a: i32, %[[VALUE_b_21:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_21:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:         let %[[VALUE_y_21:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_21]]), read<i32>(%[[VALUE_b_21]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_21]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_21]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_gt_volatile:[0-9]+]] @lt_gt_gt_volatile(%[[VALUE_a_22:[0-9]+]] a: i32, %[[VALUE_b_22:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_22:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:         let %[[VALUE_y_22:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_22]]), read<i32>(%[[VALUE_b_22]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_22]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_22]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_ge_:[0-9]+]] @lt_gt_ge_(%[[VALUE_a_23:[0-9]+]] a: i32, %[[VALUE_b_23:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_23:[0-9]+]] x: bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:         let %[[VALUE_y_23:[0-9]+]] y: bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_23]]), read<i32>(%[[VALUE_b_23]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_23]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_23]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lt_gt_ge_volatile:[0-9]+]] @lt_gt_ge_volatile(%[[VALUE_a_24:[0-9]+]] a: i32, %[[VALUE_b_24:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_24:[0-9]+]] x: volatile bool [storage=automatic] = lt<i32>(read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:         let %[[VALUE_y_24:[0-9]+]] y: volatile bool [storage=automatic] = gt<i32>(read<i32>(%[[VALUE_a_24]]), read<i32>(%[[VALUE_b_24]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_24]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_24]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_lt_:[0-9]+]] @le_ge_lt_(%[[VALUE_a_25:[0-9]+]] a: i32, %[[VALUE_b_25:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_25:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_25]]), read<i32>(%[[VALUE_b_25]]));
// DEFAULT-NEXT:         let %[[VALUE_y_25:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_25]]), read<i32>(%[[VALUE_b_25]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_25]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_25]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_lt_volatile:[0-9]+]] @le_ge_lt_volatile(%[[VALUE_a_26:[0-9]+]] a: i32, %[[VALUE_b_26:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_26:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_26]]), read<i32>(%[[VALUE_b_26]]));
// DEFAULT-NEXT:         let %[[VALUE_y_26:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_26]]), read<i32>(%[[VALUE_b_26]]));
// DEFAULT-NEXT:         return lt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_26]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_26]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_le_:[0-9]+]] @le_ge_le_(%[[VALUE_a_27:[0-9]+]] a: i32, %[[VALUE_b_27:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_27:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_27]]), read<i32>(%[[VALUE_b_27]]));
// DEFAULT-NEXT:         let %[[VALUE_y_27:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_27]]), read<i32>(%[[VALUE_b_27]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_27]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_27]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_le_volatile:[0-9]+]] @le_ge_le_volatile(%[[VALUE_a_28:[0-9]+]] a: i32, %[[VALUE_b_28:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_28:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_28]]), read<i32>(%[[VALUE_b_28]]));
// DEFAULT-NEXT:         let %[[VALUE_y_28:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_28]]), read<i32>(%[[VALUE_b_28]]));
// DEFAULT-NEXT:         return le<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_28]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_28]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_gt_:[0-9]+]] @le_ge_gt_(%[[VALUE_a_29:[0-9]+]] a: i32, %[[VALUE_b_29:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_29:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_29]]), read<i32>(%[[VALUE_b_29]]));
// DEFAULT-NEXT:         let %[[VALUE_y_29:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_29]]), read<i32>(%[[VALUE_b_29]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_29]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_29]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_gt_volatile:[0-9]+]] @le_ge_gt_volatile(%[[VALUE_a_30:[0-9]+]] a: i32, %[[VALUE_b_30:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_30:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_30]]), read<i32>(%[[VALUE_b_30]]));
// DEFAULT-NEXT:         let %[[VALUE_y_30:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_30]]), read<i32>(%[[VALUE_b_30]]));
// DEFAULT-NEXT:         return gt<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_30]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_30]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_ge_:[0-9]+]] @le_ge_ge_(%[[VALUE_a_31:[0-9]+]] a: i32, %[[VALUE_b_31:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_31:[0-9]+]] x: bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_31]]), read<i32>(%[[VALUE_b_31]]));
// DEFAULT-NEXT:         let %[[VALUE_y_31:[0-9]+]] y: bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_31]]), read<i32>(%[[VALUE_b_31]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_x_31]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_y_31]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_le_ge_ge_volatile:[0-9]+]] @le_ge_ge_volatile(%[[VALUE_a_32:[0-9]+]] a: i32, %[[VALUE_b_32:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x_32:[0-9]+]] x: volatile bool [storage=automatic] = le<i32>(read<i32>(%[[VALUE_a_32]]), read<i32>(%[[VALUE_b_32]]));
// DEFAULT-NEXT:         let %[[VALUE_y_32:[0-9]+]] y: volatile bool [storage=automatic] = ge<i32>(read<i32>(%[[VALUE_a_32]]), read<i32>(%[[VALUE_b_32]]));
// DEFAULT-NEXT:         return ge<i32>(sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_x_32]])), from_bool<i32, reason=promotion>(read<bool, volatile>(%[[VALUE_y_32]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_x_33:[0-9]+]] x: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_x_33]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_33]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x_33]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_y_33:[0-9]+]] y: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(10));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_y_33]]), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_33]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y_33]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_lt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_lt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_le_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_le_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_gt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_gt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_ge_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_gt_lt_ge_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_lt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_lt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_le_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_le_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_gt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_gt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_ge_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_ge_le_ge_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_lt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_lt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_le_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_le_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_gt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_gt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_ge_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_lt_gt_ge_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_lt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_lt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_le_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_le_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_gt_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_gt_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_ge_]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))), from_bool<i32, reason=promotion>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_le_ge_ge_volatile]], read<i32>(%[[VALUE_x_33]]), read<i32>(%[[VALUE_y_33]]))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
