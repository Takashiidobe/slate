#include <stddef.h>

void abort(void);
void exit(int);

const wchar_t ws[] = L"foo";

int main(void) {
  if (ws[0] != L'f' || ws[1] != L'o' || ws[2] != L'o' || ws[3] != L'\0')
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
// DEFAULT-NEXT:     type @type0 wchar_t = i32;
// DEFAULT-NEXT:     global %3 ws: array<i32, 4> [storage=static] [const] = code_units<array<i32, 4>>([102, 111, 111, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(4)>(%3), const<i32>(0)))), const<i32>(102)), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(4)>(%3), const<i32>(1)))), const<i32>(111))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(4)>(%3), const<i32>(2)))), const<i32>(111))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(4)>(%3), const<i32>(3)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
