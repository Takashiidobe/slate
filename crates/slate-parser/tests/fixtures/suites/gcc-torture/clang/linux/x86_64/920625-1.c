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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: f64;
// DEFAULT-NEXT:         field1 y: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_point:[0-9]+]] point = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_ipoint:[0-9]+]] ipoint = @type[[TYPE1]];
// DEFAULT-NEXT:     global %[[VALUE_pts:[0-9]+]] pts: array<@type[[TYPE0]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE0]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(1.0), field1 = const<f64>(2.0)), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(3.0), field1 = const<f64>(4.0)), index2 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(5.0), field1 = const<f64>(6.0)), index3 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<f64>(7.0), field1 = const<f64>(8.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ipts:[0-9]+]] ipts: array<@type[[TYPE1]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE1]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), index2 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6)), index3 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(7), field1 = const<i32>(8))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_va1:[0-9]+]] @va1(%[[VALUE_nargs:[0-9]+]] nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_args:[0-9]+]] args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pi:[0-9]+]] pi: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_args]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_nargs]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_pi]], copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_args]])));
// DEFAULT-NEXT:                     copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_args]]));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(4)>(%[[VALUE_pts]]), read<i32>(%[[VALUE_i]]))))), read<f64>(field0(%[[VALUE_pi]]))), ne<f64, exceptions=ignore>(read<f64>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(4)>(%[[VALUE_pts]]), read<i32>(%[[VALUE_i]]))))), read<f64>(field1(%[[VALUE_pi]]))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_args]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_va2:[0-9]+]] @va2(%[[VALUE_nargs_2:[0-9]+]] nargs: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_args_2:[0-9]+]] args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pi_2:[0-9]+]] pi: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_args_2]]);
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_nargs_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type[[TYPE1]]>(%[[VALUE_pi_2]], copy<@type[[TYPE1]], reason=assign>(va_arg<@type[[TYPE1]]>(%[[VALUE_args_2]])));
// DEFAULT-NEXT:                     copy<@type[[TYPE1]], reason=assign>(va_arg<@type[[TYPE1]]>(%[[VALUE_args_2]]));
// DEFAULT-NEXT:                     if logical_or<bool>(ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(array_decay<ptr<@type[[TYPE1]]>, length=Some(4)>(%[[VALUE_ipts]]), read<i32>(%[[VALUE_i_2]]))))), read<i32>(field0(%[[VALUE_pi_2]]))), ne<i32>(read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(array_decay<ptr<@type[[TYPE1]]>, length=Some(4)>(%[[VALUE_ipts]]), read<i32>(%[[VALUE_i_2]]))))), read<i32>(field1(%[[VALUE_pi_2]]))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_args_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, native_c, native_c, native_c, native_c) -> scalar>(%[[VALUE_va1]], const<i32>(4),
// DEFAULT-SAME: copy<@type[[TYPE0]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE0]]>(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE0]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE0]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_pts]]), const<i32>(0))))),
// DEFAULT-SAME: copy<@type[[TYPE0]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE0]]>(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE0]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE0]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_pts]]), const<i32>(1))))),
// DEFAULT-SAME: copy<@type[[TYPE0]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE0]]>(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE0]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE0]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_pts]]), const<i32>(2))))),
// DEFAULT-SAME: copy<@type[[TYPE0]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE0]]>(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE0]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE0]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_pts]]), const<i32>(3))))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, native_c, native_c, native_c, native_c) -> scalar>(%[[VALUE_va2]], const<i32>(4),
// DEFAULT-SAME: copy<@type[[TYPE1]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE1]]>(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE1]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE1]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_ipts]]), const<i32>(0))))),
// DEFAULT-SAME: copy<@type[[TYPE1]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE1]]>(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE1]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE1]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_ipts]]), const<i32>(1))))),
// DEFAULT-SAME: copy<@type[[TYPE1]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE1]]>(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE1]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE1]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_ipts]]), const<i32>(2))))),
// DEFAULT-SAME: copy<@type[[TYPE1]],
// DEFAULT-SAME: reason=vararg>(read<@type[[TYPE1]]>(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE1]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE1]]>,
// DEFAULT-SAME: length=Some(4)>(%[[VALUE_ipts]]), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
