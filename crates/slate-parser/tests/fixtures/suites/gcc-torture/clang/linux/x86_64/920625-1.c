#include <stdarg.h>

void abort(void);
void exit(int);

typedef struct {
  double x, y;
} point;
point      pts[] = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}};
static int va1(int nargs, ...) {
  va_list args;
  int     i;
  point   pi;
  va_start(args, nargs);
  for (i = 0; i < nargs; i++) {
    pi = va_arg(args, point);
    if (pts[i].x != pi.x || pts[i].y != pi.y)
      abort();
  }
  va_end(args);
}

typedef struct {
  int x, y;
} ipoint;
ipoint     ipts[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
static int va2(int nargs, ...) {
  va_list args;
  int     i;
  ipoint  pi;
  va_start(args, nargs);
  for (i = 0; i < nargs; i++) {
    pi = va_arg(args, ipoint);
    if (ipts[i].x != pi.x || ipts[i].y != pi.y)
      abort();
  }
  va_end(args);
}

int main(void) {
  va1(4, pts[0], pts[1], pts[2], pts[3]);
  va2(4, ipts[0], ipts[1], ipts[2], ipts[3]);
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 x: f64;
// DEFAULT-NEXT:         field1 y: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 point = @type1;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 ipoint = @type3;
// DEFAULT-NEXT:     global %5 pts: array<@type1, 4> [storage=static] [align=16] = aggregate<array<@type1, 4>, zero_fill=false>(index0 = aggregate<@type1, zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0)), index1 = aggregate<@type1, zero_fill=false>(field0 = const<f64>(3.0), field1 = const<f64>(4.0)), index2 = aggregate<@type1, zero_fill=false>(field0 = const<f64>(5.0), field1 = const<f64>(6.0)), index3 = aggregate<@type1, zero_fill=false>(field0 = const<f64>(7.0), field1 = const<f64>(8.0))) [linkage=external];
// DEFAULT-NEXT:     global %13 ipts: array<@type3, 4> [storage=static] [align=16] = aggregate<array<@type3, 4>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), index2 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6)), index3 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(7), field1 = const<i32>(8))) [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%20 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @va1(%7 nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 pi: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type1>(%10, copy<@type1, reason=assign>(va_arg<@type1>(%8)));
// DEFAULT-NEXT:                     copy<@type1, reason=assign>(va_arg<@type1>(%8));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), read<i32>(%9))))), read<f64>(field0(%10))), ne<f64, exceptions=ignore>(read<f64>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), read<i32>(%9))))), read<f64>(field1(%10))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @va2(%15 nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %18 pi: @type3 [storage=automatic];
// DEFAULT-NEXT:         va_start(%16);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type3>(%18, copy<@type3, reason=assign>(va_arg<@type3>(%16)));
// DEFAULT-NEXT:                     copy<@type3, reason=assign>(va_arg<@type3>(%16));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), read<i32>(%17))))), read<i32>(field0(%18))), ne<i32>(read<i32>(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), read<i32>(%17))))), read<i32>(field1(%18))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, coerce<f64, f64>, coerce<f64, f64>, coerce<f64, f64>, coerce<f64, f64>) -> scalar>(%6, const<i32>(4), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), const<i32>(0))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), const<i32>(1))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), const<i32>(2))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(4)>(%5), const<i32>(3))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, coerce<i64>, coerce<i64>, coerce<i64>, coerce<i64>) -> scalar>(%14, const<i32>(4), copy<@type3, reason=vararg>(read<@type3>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), const<i32>(0))))), copy<@type3, reason=vararg>(read<@type3>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), const<i32>(1))))), copy<@type3, reason=vararg>(read<@type3>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), const<i32>(2))))), copy<@type3, reason=vararg>(read<@type3>(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(array_decay<ptr<@type3>, length=Some(4)>(%13), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
