// SLATE-FILECHECK-DEFINES DEFAULT


typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;

struct X { int y; };

void func(va_list va)
{
  char* a = __builtin_va_arg(va, char**)[0];
  int b = __builtin_va_arg(va, struct X*)->y;
}

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
// DEFAULT-NEXT:     type @type2 X = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %3 @func(%4 va: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: ptr<i8> [storage=automatic] = read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(va_arg<ptr<ptr<i8>>>(%4), const<i32>(0))));
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic] = read<i32>(field0(deref(va_arg<ptr<@type2>>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
