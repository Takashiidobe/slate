/* PR target/44575 */

#include <stdarg.h>

void abort(void);

int fails = 0;
struct S {
  float a[3];
};
struct S a[5];

void check(int z, ...) {
  struct S arg, *p;
  va_list  ap;
  int      j = 0, k = 0;
  int      i;
  va_start(ap, z);
  for (i = 2; i < 4; ++i) {
    p = 0;
    j++;
    k += 2;
    switch ((z << 4) | i) {
    case 0x12:
    case 0x13:
      p   = &a[2];
      arg = va_arg(ap, struct S);
      break;
    default:
      ++fails;
      break;
    }
    if (p && p->a[2] != arg.a[2])
      ++fails;
    if (fails)
      break;
  }
  va_end(ap);
}

int main() {
  a[2].a[2] = -49026;
  check(1, a[2], a[2]);
  if (fails)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: array<f32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %2 fails: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %4 a: array<@type1, 5> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @check(%6 z: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 arg: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %8 p: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %9 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %10 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %11 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(2));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type1>>(%8, null<ptr<@type1>>);
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                     let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%10, read<i32>(%19));
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%11, read<i32>(%21));
// DEFAULT-NEXT:                     switch %15 or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%6), const<i32>(4)), read<i32>(%12))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %15 const<i32>(18):
// DEFAULT-NEXT:                                 case %15 const<i32>(19):
// DEFAULT-NEXT:                                     write<ptr<@type1>>(%8, addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(5)>(%4), const<i32>(2)))));
// DEFAULT-NEXT:                             write<@type1>(%7, copy<@type1, reason=assign>(va_arg<@type1>(%9)));
// DEFAULT-NEXT:                             copy<@type1, reason=assign>(va_arg<@type1>(%9));
// DEFAULT-NEXT:                             break %15;
// DEFAULT-NEXT:                             default %15:
// DEFAULT-NEXT:                                 let %22: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%2, read<i32>(%23));
// DEFAULT-NEXT:                             break %15;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%8), null<ptr<@type1>>), ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(read<ptr<@type1>>(%8)))), const<i32>(2)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(%7)), const<i32>(2))))))
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%2, read<i32>(%25));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                         break %14;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(5)>(%4), const<i32>(2))))), const<i32>(2))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(49026))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c) -> void>(%5, const<i32>(1), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(5)>(%4), const<i32>(2))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(5)>(%4), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
