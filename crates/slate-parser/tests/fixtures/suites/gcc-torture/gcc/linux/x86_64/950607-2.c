void abort(void);
void exit(int);

typedef struct {
  long int p_x, p_y;
} Point;

int f(Point basePt, Point pt1, Point pt2) {
  long long vector;

  vector =
      (long long)(pt1.p_x - basePt.p_x) * (long long)(pt2.p_y - basePt.p_y) -
      (long long)(pt1.p_y - basePt.p_y) * (long long)(pt2.p_x - basePt.p_x);

  if (vector > (long long)0)
    return 0;
  else if (vector < (long long)0)
    return 1;
  else
    return 2;
}

int main(void) {
  Point b, p1, p2;
  int   answer;

  b.p_x = -23250;
  b.p_y = 23250;

  p1.p_x = 23250;
  p1.p_y = -23250;

  p2.p_x = -23250;
  p2.p_y = -23250;

  answer = f(b, p1, p2);

  if (answer != 1)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 p_x: i64;
// DEFAULT-NEXT:         field1 p_y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Point:[0-9]+]] Point = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_basePt:[0-9]+]] basePt: @type[[TYPE0]], %[[VALUE_pt1:[0-9]+]] pt1: @type[[TYPE0]], %[[VALUE_pt2:[0-9]+]] pt2: @type[[TYPE0]]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_vector:[0-9]+]] vector: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_vector]], sub<i64, overflow=ub>(mul<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(field0(%[[VALUE_pt1]])), read<i64>(field0(%[[VALUE_basePt]]))), sub<i64, overflow=ub>(read<i64>(field1(%[[VALUE_pt2]])), read<i64>(field1(%[[VALUE_basePt]])))), mul<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(field1(%[[VALUE_pt1]])), read<i64>(field1(%[[VALUE_basePt]]))), sub<i64, overflow=ub>(read<i64>(field0(%[[VALUE_pt2]])), read<i64>(field0(%[[VALUE_basePt]]))))));
// DEFAULT-NEXT:         if gt<i64>(read<i64>(%[[VALUE_vector]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i64>(read<i64>(%[[VALUE_vector]]), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_answer:[0-9]+]] answer: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_b]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_b]]), widen<i64, reason=assign>(const<i32>(23250)));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_p1]]), widen<i64, reason=assign>(const<i32>(23250)));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_p1]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_p2]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_p2]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_answer]], call<i32, signature=fn(@type[[TYPE0]], @type[[TYPE0]], @type[[TYPE0]]) -> i32, abi=sysv64(native_c, native_c, native_c) -> scalar>(%[[VALUE_f]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_b]])), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_p1]])), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_p2]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_answer]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
