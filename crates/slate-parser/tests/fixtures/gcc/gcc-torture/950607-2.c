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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 p_x: i64;
// DEFAULT-NEXT:         field1 p_y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 Point = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 basePt: @type0, %6 pt1: @type0, %7 pt2: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64, i64>, coerce<i64, i64>, coerce<i64, i64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 vector: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%8, sub<i64, overflow=ub>(mul<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(field0(%6)), read<i64>(field0(%5))), sub<i64, overflow=ub>(read<i64>(field1(%7)), read<i64>(field1(%5)))), mul<i64, overflow=ub>(sub<i64, overflow=ub>(read<i64>(field1(%6)), read<i64>(field1(%5))), sub<i64, overflow=ub>(read<i64>(field0(%7)), read<i64>(field0(%5))))));
// DEFAULT-NEXT:         if gt<i64>(read<i64>(%8), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if lt<i64>(read<i64>(%8), widen<i64, reason=explicit>(const<i32>(0)))
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %11 p1: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %12 p2: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %13 answer: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%10), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field1(%10), widen<i64, reason=assign>(const<i32>(23250)));
// DEFAULT-NEXT:         write<i64>(field0(%11), widen<i64, reason=assign>(const<i32>(23250)));
// DEFAULT-NEXT:         write<i64>(field1(%11), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field0(%12), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i64>(field1(%12), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(23250))));
// DEFAULT-NEXT:         write<i32>(%13, call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i64>, coerce<i64, i64>, coerce<i64, i64>) -> scalar>(%4, copy<@type0, reason=arg>(read<@type0>(%10)), copy<@type0, reason=arg>(read<@type0>(%11)), copy<@type0, reason=arg>(read<@type0>(%12))));
// DEFAULT-NEXT:         call<i32, signature=fn(@type0, @type0, @type0) -> i32, abi=sysv64(coerce<i64, i64>, coerce<i64, i64>, coerce<i64, i64>) -> scalar>(%4, copy<@type0, reason=arg>(read<@type0>(%10)), copy<@type0, reason=arg>(read<@type0>(%11)), copy<@type0, reason=arg>(read<@type0>(%12)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%13), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
