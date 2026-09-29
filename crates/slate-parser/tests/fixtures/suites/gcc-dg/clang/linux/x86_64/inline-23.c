/* { dg-do compile } */
/* { dg-options "-std=gnu89" } */
/* Make sure we can inline a varargs function whose variable arguments
   are not used.  See PR32493.  */
#include <stddef.h>

typedef __INTPTR_TYPE__ my_intptr_t;

static inline __attribute__((always_inline)) void __check_printsym_format(const
char *fmt, ...)
{
}
static inline __attribute__((always_inline)) void print_symbol(const char *fmt,
my_intptr_t addr)
{
 __check_printsym_format(fmt, "");
}
void do_initcalls(void **call)
{
   print_symbol(": %s()", (my_intptr_t) *call);
}

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type[[TYPE_my_intptr_t:[0-9]+]] my_intptr_t = i64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([58, 32, 37, 115, 40, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___check_printsym_format:[0-9]+]] @__check_printsym_format(%[[VALUE_fmt:[0-9]+]] fmt: ptr<const i8>, ...) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_print_symbol:[0-9]+]] @print_symbol(%[[VALUE_fmt_2:[0-9]+]] fmt: ptr<const i8>, %[[VALUE_addr:[0-9]+]] addr: i64) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ...) -> void>(%[[VALUE___check_printsym_format]], read<ptr<const i8>>(%[[VALUE_fmt_2]]), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_do_initcalls:[0-9]+]] @do_initcalls(%[[VALUE_call:[0-9]+]] call: ptr<ptr<void>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i64) -> void>(%[[VALUE_print_symbol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]])), ptr_to_int<i64, reason=explicit>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_call]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
