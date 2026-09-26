// SLATE-FILECHECK-DEFINES DEFAULT

/* Test for proper preparation of the comparison operands for 
   generation of a conditional trap.  Produced unrecognizable
   rtl on Sparc.  */

struct blah { char *b_data; };

void set_bh_page(struct blah *bh, unsigned long offset)
{
        if ((1UL << 12 ) <= offset)
                __builtin_trap() ;
        bh->b_data = (char *)offset;
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
// DEFAULT-NEXT:     type @type0 blah = struct {
// DEFAULT-NEXT:         field0 b_data: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %4 @__builtin_trap() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @set_bh_page(%2 bh: ptr<@type0>, %3 offset: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if le<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(12)), read<u64>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<ptr<i8>>(field0(deref(read<ptr<@type0>>(%2))), int_to_ptr<ptr<i8>, reason=explicit>(read<u64>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
