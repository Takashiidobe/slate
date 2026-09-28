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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 S = struct {
// DEFAULT-NEXT:         field0 a: array<f32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %3 fails: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %5 a: array<@type2, 5> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @check(%7 z: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 arg: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %9 p: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         let %10 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %11 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %12 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%10);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(2));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type2>>(%9, null<ptr<@type2>>);
// DEFAULT-NEXT:                     let %19: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%11, read<i32>(%20));
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%12, read<i32>(%22));
// DEFAULT-NEXT:                     switch %16 or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%7), const<i32>(4)), read<i32>(%13))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %16 const<i32>(18):
// DEFAULT-NEXT:                                 case %16 const<i32>(19):
// DEFAULT-NEXT:                                     write<ptr<@type2>>(%9, addr_of<ptr<@type2>>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(5)>(%5), const<i32>(2)))));
// DEFAULT-NEXT:                             write<@type2>(%8, copy<@type2, reason=assign>(va_arg<@type2>(%10)));
// DEFAULT-NEXT:                             copy<@type2, reason=assign>(va_arg<@type2>(%10));
// DEFAULT-NEXT:                             break %16;
// DEFAULT-NEXT:                             default %16:
// DEFAULT-NEXT:                                 let %23: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%3, read<i32>(%24));
// DEFAULT-NEXT:                             break %16;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if logical_and<bool>(ne<ptr<@type2>>(read<ptr<@type2>>(%9), null<ptr<@type2>>), ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(read<ptr<@type2>>(%9)))), const<i32>(2)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(%8)), const<i32>(2))))))
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                         let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%3, read<i32>(%26));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                         break %15;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(5)>(%5), const<i32>(2))))), const<i32>(2))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(49026))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c) -> void>(%6, const<i32>(1), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(5)>(%5), const<i32>(2))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(5)>(%5), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
