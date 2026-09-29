#include <stdio.h>

typedef unsigned long long BigCount;

struct Accounting {
  BigCount direct;
  BigCount indirect;
};

struct Accounting acc = {0, 0};

static void add(int isDirect, BigCount amount) {
  BigCount *const target  = isDirect ? &acc.direct : &acc.indirect;
  *target                += amount;
}

int main(void) {
  add(1, 3);
  add(0, 5);
  add(1, 7);
  printf("%llu %llu\n", acc.direct, acc.indirect);
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
// DEFAULT-NEXT:     type @type[[TYPE_BigCount:[0-9]+]] BigCount = u64;
// DEFAULT-NEXT:     type @type[[TYPE_Accounting:[0-9]+]] Accounting = struct {
// DEFAULT-NEXT:         field0 direct: u64;
// DEFAULT-NEXT:         field1 indirect: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_acc:[0-9]+]] acc: @type[[TYPE_Accounting]] [storage=static] = aggregate<@type[[TYPE_Accounting]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([37, 108, 108, 117, 32, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_isDirect:[0-9]+]] isDirect: i32, %[[VALUE_amount:[0-9]+]] amount: u64) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_target:[0-9]+]] target: ptr<u64> [storage=automatic] [const] = conditional<ptr<u64>>(ne<i32>(read<i32>(%[[VALUE_isDirect]]), const<i32>(0)), addr_of<ptr<u64>>(field0(%[[VALUE_acc]])), addr_of<ptr<u64>>(field1(%[[VALUE_acc]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<u64> [synthetic] = read<ptr<u64>>(%[[VALUE_target]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), read<u64>(%[[VALUE_amount]]));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE0]])), read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, u64) -> void>(%[[VALUE_add]], const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u64) -> void>(%[[VALUE_add]], const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u64) -> void>(%[[VALUE_add]], const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])), read<u64>(field0(%[[VALUE_acc]])), read<u64>(field1(%[[VALUE_acc]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
