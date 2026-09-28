#include <stddef.h>
#include <stdio.h>

#include "gnu_pragmas_once.h"

#pragma message("GNU pragma message probe")
#pragma GCC warning "GNU pragma warning probe"

#define GNU_PRAGMA_MACRO 7
#pragma push_macro("GNU_PRAGMA_MACRO")
#undef GNU_PRAGMA_MACRO
#define GNU_PRAGMA_MACRO 11
static int gnu_pragma_inner_macro = GNU_PRAGMA_MACRO;
#pragma pop_macro("GNU_PRAGMA_MACRO")
static int gnu_pragma_outer_macro = GNU_PRAGMA_MACRO;

#pragma pack(push, 1)
struct GNUPragmaPacked {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

#pragma GCC visibility push(hidden)
int                    gnu_pragma_hidden(int value) { return value + 13; }
#pragma GCC visibility pop

int gnu_pragma_weak_target(void) { return 17; }

#pragma weak gnu_pragma_weak_alias = gnu_pragma_weak_target
extern int   gnu_pragma_weak_alias(void);

#pragma redefine_extname gnu_pragma_renamed gnu_pragma_actual
int gnu_pragma_renamed(void) { return 19; }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static int gnu_pragma_diagnostic(void) {
  int ignored;
  return 23;
}
#pragma GCC diagnostic pop

#pragma GCC poison gnu_pragma_poisoned_identifier

int main(void) {
  struct GNUPragmaPacked packed = {29, 31};
  printf("%d %d %d %d %d %d %d %d\n", GNU_PRAGMA_ONCE_VALUE,
         gnu_pragma_inner_macro, gnu_pragma_outer_macro, (int)sizeof(packed),
         (int)offsetof(struct GNUPragmaPacked, value), gnu_pragma_hidden(37),
         gnu_pragma_weak_alias(),
         gnu_pragma_renamed() + gnu_pragma_diagnostic() + packed.tag +
             (int)packed.value);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs/gnu-pragmas-headers

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
// DEFAULT-NEXT:     type @type0 GNUPragmaPacked = struct {
// DEFAULT-NEXT:         field0 tag: u8;
// DEFAULT-NEXT:         field1 value: u32;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %1 gnu_pragma_inner_macro: i32 [storage=static] = const<i32>(11) [linkage=internal];
// DEFAULT-NEXT:     global %2 gnu_pragma_outer_macro: i32 [storage=static] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @gnu_pragma_hidden(%5 value: i32) -> i32 [linkage=external] [visibility=hidden] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%5), const<i32>(13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @gnu_pragma_weak_target() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @gnu_pragma_weak_alias() -> i32 [linkage=external] [weak] [alias="gnu_pragma_weak_target"];
// DEFAULT-NEXT:     fn %8 @gnu_pragma_renamed() -> i32 [linkage=external] [asm_name="gnu_pragma_actual"] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @gnu_pragma_diagnostic() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 ignored: i32 [storage=automatic];
// DEFAULT-NEXT:         return const<i32>(23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 packed: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(29))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%14)), const<i32>(5), read<i32>(%1), read<i32>(%2), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(5))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(37)), call<i32, signature=fn() -> i32>(%7), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%8), call<i32, signature=fn() -> i32>(%9)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(%12))))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field1(%12)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
