
#include <stdarg.h>

int simple_int(va_list ap) {
  return va_arg(ap, int);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = ptr<i8>;
// DEFAULT-NEXT:     fn %[[VALUE_simple_int:[0-9]+]] @simple_int(%[[VALUE_ap:[0-9]+]] ap: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if gt<u64>(const<u64>(4), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8)))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ap]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE1]]), const<u64>(8));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_ap]], read<ptr<i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], read<i32>(deref(read<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<u64>(8))))))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ap]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE3]]), add<u64, overflow=wrap>(and<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(not<i32>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))))), and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(null<ptr<i8>>, read<ptr<i8>>(%[[VALUE_ap]]))), sub<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_ap]], read<ptr<i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), and<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(not<i32>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))))))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE0]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
