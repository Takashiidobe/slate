/* { dg-do run } */
/* { dg-options "-O2" } */

typedef int A;
typedef int __attribute__ (( hardbool(0, 1) )) B;

_Static_assert(_Generic((A*){ 0 }, B*: 1), "");

void* foo(void* a, void *b, A *c, B *d)
{
        *(A**)a = c;
        *(B**)b = d;
        return *(A**)a;
}

int main()
{
        A *a, b, c;
        if (&c != (A*)foo(&a, &a, &b, &c))
                __builtin_abort();
}


// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = i32;
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = i32;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: ptr<void>, %[[VALUE_c:[0-9]+]] c: ptr<i32>, %[[VALUE_d:[0-9]+]] d: ptr<i32>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%[[VALUE_a]]))), read<ptr<i32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%[[VALUE_b]]))), read<ptr<i32>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%[[VALUE_a]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_c_2]]), pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<i32>, ptr<i32>) -> ptr<void>>(%[[VALUE_foo]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%[[VALUE_a_2]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%[[VALUE_a_2]])), addr_of<ptr<i32>>(%[[VALUE_b_2]]), addr_of<ptr<i32>>(%[[VALUE_c_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
