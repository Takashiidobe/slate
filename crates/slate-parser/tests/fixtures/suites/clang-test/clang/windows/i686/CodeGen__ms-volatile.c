struct foo {
  volatile int x;
};
struct bar {
  int x;
};
typedef _Complex float __declspec(align(8)) baz;

#pragma pack(push)
#pragma pack(1)
struct qux {
   volatile int f;
};
#pragma pack(pop)

void test1(struct foo *p, struct foo *q) {
  *p = *q;
}
void test2(volatile int *p, volatile int *q) {
  *p = *q;
}
void test3(struct foo *p, struct foo *q) {
  p->x = q->x;
}
void test4(volatile struct foo *p, volatile struct foo *q) {
  p->x = q->x;
}
void test5(volatile struct foo *p, volatile struct foo *q) {
  *p = *q;
}
void test6(struct bar *p, struct bar *q) {
  *p = *q;
}
void test7(volatile struct bar *p, volatile struct bar *q) {
  *p = *q;
}
void test8(volatile double *p, volatile double *q) {
  *p = *q;
}
void test9(volatile baz *p, baz *q) {
  *p = *q;
}
void test10(volatile long long *p, volatile long long *q) {
  *p = *q;
}
void test11(volatile float *p, volatile float *q) {
  *p = *q;
}
int test12(struct qux *p) {
  return p->f;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: volatile i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_baz:[0-9]+]] baz = complex<f32>;
// DEFAULT-NEXT:     type @type[[TYPE_qux:[0-9]+]] qux = struct {
// DEFAULT-NEXT:         field0 f: volatile i32;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_foo]]>, %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_foo]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_foo]]>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_p]])), copy<@type[[TYPE_foo]], reason=assign>(read<@type[[TYPE_foo]]>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_p_2:[0-9]+]] p: ptr<volatile i32>, %[[VALUE_q_2:[0-9]+]] q: ptr<volatile i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(deref(read<ptr<volatile i32>>(%[[VALUE_p_2]])), read<i32, volatile>(deref(read<ptr<volatile i32>>(%[[VALUE_q_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_foo]]>, %[[VALUE_q_3:[0-9]+]] q: ptr<@type[[TYPE_foo]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_p_3]]))), read<i32, volatile>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_p_4:[0-9]+]] p: ptr<volatile @type[[TYPE_foo]]>, %[[VALUE_q_4:[0-9]+]] q: ptr<volatile @type[[TYPE_foo]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(field0(deref(read<ptr<volatile @type[[TYPE_foo]]>>(%[[VALUE_p_4]]))), read<i32, volatile>(field0(deref(read<ptr<volatile @type[[TYPE_foo]]>>(%[[VALUE_q_4]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_p_5:[0-9]+]] p: ptr<volatile @type[[TYPE_foo]]>, %[[VALUE_q_5:[0-9]+]] q: ptr<volatile @type[[TYPE_foo]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_foo]], volatile>(deref(read<ptr<volatile @type[[TYPE_foo]]>>(%[[VALUE_p_5]])), copy<@type[[TYPE_foo]], reason=assign>(read<@type[[TYPE_foo]], volatile>(deref(read<ptr<volatile @type[[TYPE_foo]]>>(%[[VALUE_q_5]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_p_6:[0-9]+]] p: ptr<@type[[TYPE_bar]]>, %[[VALUE_q_6:[0-9]+]] q: ptr<@type[[TYPE_bar]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_p_6]])), copy<@type[[TYPE_bar]], reason=assign>(read<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_q_6]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_p_7:[0-9]+]] p: ptr<volatile @type[[TYPE_bar]]>, %[[VALUE_q_7:[0-9]+]] q: ptr<volatile @type[[TYPE_bar]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]], volatile>(deref(read<ptr<volatile @type[[TYPE_bar]]>>(%[[VALUE_p_7]])), copy<@type[[TYPE_bar]], reason=assign>(read<@type[[TYPE_bar]], volatile>(deref(read<ptr<volatile @type[[TYPE_bar]]>>(%[[VALUE_q_7]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_p_8:[0-9]+]] p: ptr<volatile f64>, %[[VALUE_q_8:[0-9]+]] q: ptr<volatile f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64, volatile>(deref(read<ptr<volatile f64>>(%[[VALUE_p_8]])), read<f64, volatile>(deref(read<ptr<volatile f64>>(%[[VALUE_q_8]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(%[[VALUE_p_9:[0-9]+]] p: ptr<volatile complex<f32>>, %[[VALUE_q_9:[0-9]+]] q: ptr<complex<f32>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f32>, volatile>(deref(read<ptr<volatile complex<f32>>>(%[[VALUE_p_9]])), read<complex<f32>>(deref(read<ptr<complex<f32>>>(%[[VALUE_q_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10:[0-9]+]] @test10(%[[VALUE_p_10:[0-9]+]] p: ptr<volatile i64>, %[[VALUE_q_10:[0-9]+]] q: ptr<volatile i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64, volatile>(deref(read<ptr<volatile i64>>(%[[VALUE_p_10]])), read<i64, volatile>(deref(read<ptr<volatile i64>>(%[[VALUE_q_10]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11:[0-9]+]] @test11(%[[VALUE_p_11:[0-9]+]] p: ptr<volatile f32>, %[[VALUE_q_11:[0-9]+]] q: ptr<volatile f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, volatile>(deref(read<ptr<volatile f32>>(%[[VALUE_p_11]])), read<f32, volatile>(deref(read<ptr<volatile f32>>(%[[VALUE_q_11]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12:[0-9]+]] @test12(%[[VALUE_p_12:[0-9]+]] p: ptr<@type[[TYPE_qux]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, volatile>(field0(deref(read<ptr<@type[[TYPE_qux]]>>(%[[VALUE_p_12]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
