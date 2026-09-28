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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 x: f64;
// DEFAULT-NEXT:         field1 y: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 point = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 ipoint = @type4;
// DEFAULT-NEXT:     global %6 pts: array<@type2, 4> [storage=static] [align=16] = aggregate<array<@type2, 4>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0)), index1 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(3.0), field1 = const<f64>(4.0)), index2 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(5.0), field1 = const<f64>(6.0)), index3 = aggregate<@type2, zero_fill=false>(field0 = const<f64>(7.0), field1 = const<f64>(8.0))) [linkage=external];
// DEFAULT-NEXT:     global %14 ipts: array<@type4, 4> [storage=static] [align=16] = aggregate<array<@type4, 4>, zero_fill=false>(index0 = aggregate<@type4, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type4, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), index2 = aggregate<@type4, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6)), index3 = aggregate<@type4, zero_fill=false>(field0 = const<i32>(7), field1 = const<i32>(8))) [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%21 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @va1(%8 nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 pi: @type2 [storage=automatic];
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type2>(%11, copy<@type2, reason=assign>(va_arg<@type2>(%9)));
// DEFAULT-NEXT:                     copy<@type2, reason=assign>(va_arg<@type2>(%9));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), read<i32>(%10))))), read<f64>(field0(%11))), ne<f64, exceptions=observable>(read<f64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), read<i32>(%10))))), read<f64>(field1(%11))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @va2(%16 nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %18 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 pi: @type4 [storage=automatic];
// DEFAULT-NEXT:         va_start(%17);
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%18), read<i32>(%16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type4>(%19, copy<@type4, reason=assign>(va_arg<@type4>(%17)));
// DEFAULT-NEXT:                     copy<@type4, reason=assign>(va_arg<@type4>(%17));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), read<i32>(%18))))), read<i32>(field0(%19))), ne<i32>(read<i32>(field1(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), read<i32>(%18))))), read<i32>(field1(%19))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, coerce<f64, f64>, coerce<f64, f64>, coerce<f64, f64>, coerce<f64, f64>) -> scalar>(%7, const<i32>(4), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), const<i32>(0))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), const<i32>(1))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), const<i32>(2))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%6), const<i32>(3))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, coerce<i64>, coerce<i64>, coerce<i64>, coerce<i64>) -> scalar>(%15, const<i32>(4), copy<@type4, reason=vararg>(read<@type4>(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), const<i32>(0))))), copy<@type4, reason=vararg>(read<@type4>(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), const<i32>(1))))), copy<@type4, reason=vararg>(read<@type4>(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), const<i32>(2))))), copy<@type4, reason=vararg>(read<@type4>(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(4)>(%14), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
