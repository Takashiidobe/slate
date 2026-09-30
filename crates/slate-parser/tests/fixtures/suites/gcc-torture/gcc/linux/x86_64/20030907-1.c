// SLATE-FILECHECK-DEFINES DEFAULT

/* PR 11665 
   Orgin: jwhite@cse.unl.edu
   The problem was in initializer_constant_valid_p,
   "for a CONSTRUCTOR, only the last element
   of the CONSTRUCTOR was being checked" 
   (from the email of the patch which fixed this).  
   This used to ICE because GCC thought gdt_table was a 
   constant value when it is not.  */

int x;
struct gdt
{
unsigned a,b,c,d,e,f;
};
void f()
{
struct gdt gdt_table[2]=
{
    {
		0,
		( (((unsigned)(&x))<<(24))&(-1<<(8)) ),
    },
};
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
// DEFAULT-NEXT:     type @type[[TYPE_gdt:[0-9]+]] gdt = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:         field5 f: u32;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 8, 12, 16, 20]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_gdt_table:[0-9]+]] gdt_table: array<@type[[TYPE_gdt]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_gdt]], 2>, zero_fill=true>(index0 = aggregate<@type[[TYPE_gdt]], zero_fill=true>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field1 = and<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(ptr_to_int<u32, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_x]])), const<i32>(24)), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(8))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
