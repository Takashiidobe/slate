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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<f32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_fails:[0-9]+]] fails: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<@type[[TYPE_S]], 5> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_z:[0-9]+]] z: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_arg:[0-9]+]] arg: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(2));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], null<ptr<@type[[TYPE_S]]>>);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                     switch %[[VALUE7:[0-9]+]] or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_z]]), const<i32>(4)), read<i32>(%[[VALUE_i]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE7]] const<i32>(18):
// DEFAULT-NEXT:                                 case %[[VALUE7]] const<i32>(19):
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2)))));
// DEFAULT-NEXT:                             write<@type[[TYPE_S]]>(%[[VALUE_arg]], copy<@type[[TYPE_S]], reason=assign>(va_arg<@type[[TYPE_S]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                             break %[[VALUE7]];
// DEFAULT-NEXT:                             default %[[VALUE7]]:
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fails]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fails]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                             break %[[VALUE7]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if logical_and<bool>(ne<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_S]]>>), ne<f32, exceptions=observable>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])))), const<i32>(2)))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(%[[VALUE_arg]])), const<i32>(2))))))
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fails]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_fails]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_fails]]), const<i32>(0))
// DEFAULT-NEXT:                         break %[[VALUE0]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(3)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2))))), const<i32>(2))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(49026))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c) -> void>(%[[VALUE_check]], const<i32>(1), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2))))), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(5)>(%[[VALUE_a]]), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fails]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
