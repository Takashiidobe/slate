void abort(void);
void exit(int);

#define VALUE 0x123456789abcdefLL
#define AFTER 0x55

void test1(int a, long long value, int after) {
  if (a != 1 || value != VALUE || after != AFTER)
    abort();
}

void test2(int a, int b, long long value, int after) {
  if (a != 1 || b != 2 || value != VALUE || after != AFTER)
    abort();
}

void test3(int a, int b, int c, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || value != VALUE || after != AFTER)
    abort();
}

void test4(int a, int b, int c, int d, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || value != VALUE || after != AFTER)
    abort();
}

void test5(int a, int b, int c, int d, int e, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || value != VALUE ||
      after != AFTER)
    abort();
}

void test6(int a, int b, int c, int d, int e, int f, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 ||
      value != VALUE || after != AFTER)
    abort();
}

void test7(int a, int b, int c, int d, int e, int f, int g, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      value != VALUE || after != AFTER)
    abort();
}

void test8(int a, int b, int c, int d, int e, int f, int g, int h,
           long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      h != 8 || value != VALUE || after != AFTER)
    abort();
}

int main() {
  test1(1, VALUE, AFTER);
  test2(1, 2, VALUE, AFTER);
  test3(1, 2, 3, VALUE, AFTER);
  test4(1, 2, 3, 4, VALUE, AFTER);
  test5(1, 2, 3, 4, 5, VALUE, AFTER);
  test6(1, 2, 3, 4, 5, 6, VALUE, AFTER);
  test7(1, 2, 3, 4, 5, 6, 7, VALUE, AFTER);
  test8(1, 2, 3, 4, 5, 6, 7, 8, VALUE, AFTER);
  exit(0);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_value:[0-9]+]] value: i64, %[[VALUE_after:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1)), ne<i64>(read<i64>(%[[VALUE_value]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_value_2:[0-9]+]] value: i64, %[[VALUE_after_2:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(2))), ne<i64>(read<i64>(%[[VALUE_value_2]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_2]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_value_3:[0-9]+]] value: i64, %[[VALUE_after_3:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_3]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_2]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(3))), ne<i64>(read<i64>(%[[VALUE_value_3]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_3]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32, %[[VALUE_c_2:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32, %[[VALUE_value_4:[0-9]+]] value: i64, %[[VALUE_after_4:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_4]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_3]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c_2]]), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(4))), ne<i64>(read<i64>(%[[VALUE_value_4]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_4]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_a_5:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32, %[[VALUE_c_3:[0-9]+]] c: i32, %[[VALUE_d_2:[0-9]+]] d: i32, %[[VALUE_e:[0-9]+]] e: i32, %[[VALUE_value_5:[0-9]+]] value: i64, %[[VALUE_after_5:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_5]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_4]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c_3]]), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_d_2]]), const<i32>(4))), ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(5))), ne<i64>(read<i64>(%[[VALUE_value_5]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_5]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_a_6:[0-9]+]] a: i32, %[[VALUE_b_5:[0-9]+]] b: i32, %[[VALUE_c_4:[0-9]+]] c: i32, %[[VALUE_d_3:[0-9]+]] d: i32, %[[VALUE_e_2:[0-9]+]] e: i32, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_value_6:[0-9]+]] value: i64, %[[VALUE_after_6:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_6]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_5]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c_4]]), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_d_3]]), const<i32>(4))), ne<i32>(read<i32>(%[[VALUE_e_2]]), const<i32>(5))), ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(6))), ne<i64>(read<i64>(%[[VALUE_value_6]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_6]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_a_7:[0-9]+]] a: i32, %[[VALUE_b_6:[0-9]+]] b: i32, %[[VALUE_c_5:[0-9]+]] c: i32, %[[VALUE_d_4:[0-9]+]] d: i32, %[[VALUE_e_3:[0-9]+]] e: i32, %[[VALUE_f_2:[0-9]+]] f: i32, %[[VALUE_g:[0-9]+]] g: i32, %[[VALUE_value_7:[0-9]+]] value: i64, %[[VALUE_after_7:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_7]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_6]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c_5]]), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_d_4]]), const<i32>(4))), ne<i32>(read<i32>(%[[VALUE_e_3]]), const<i32>(5))), ne<i32>(read<i32>(%[[VALUE_f_2]]), const<i32>(6))), ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(7))), ne<i64>(read<i64>(%[[VALUE_value_7]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_7]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_a_8:[0-9]+]] a: i32, %[[VALUE_b_7:[0-9]+]] b: i32, %[[VALUE_c_6:[0-9]+]] c: i32, %[[VALUE_d_5:[0-9]+]] d: i32, %[[VALUE_e_4:[0-9]+]] e: i32, %[[VALUE_f_3:[0-9]+]] f: i32, %[[VALUE_g_2:[0-9]+]] g: i32, %[[VALUE_h:[0-9]+]] h: i32, %[[VALUE_value_8:[0-9]+]] value: i64, %[[VALUE_after_8:[0-9]+]] after: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_a_8]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_b_7]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_c_6]]), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_d_5]]), const<i32>(4))), ne<i32>(read<i32>(%[[VALUE_e_4]]), const<i32>(5))), ne<i32>(read<i32>(%[[VALUE_f_3]]), const<i32>(6))), ne<i32>(read<i32>(%[[VALUE_g_2]]), const<i32>(7))), ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(8))), ne<i64>(read<i64>(%[[VALUE_value_8]]), const<i64>(81985529216486895))), ne<i32>(read<i32>(%[[VALUE_after_8]]), const<i32>(85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i64, i32) -> void>(%[[VALUE_test1]], const<i32>(1), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i64, i32) -> void>(%[[VALUE_test2]], const<i32>(1), const<i32>(2), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i64, i32) -> void>(%[[VALUE_test3]], const<i32>(1), const<i32>(2), const<i32>(3), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i64, i32) -> void>(%[[VALUE_test4]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i64, i32) -> void>(%[[VALUE_test5]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%[[VALUE_test6]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%[[VALUE_test7]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i64, i32) -> void>(%[[VALUE_test8]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
