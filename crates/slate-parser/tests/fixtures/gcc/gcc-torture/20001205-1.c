// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do assemble } */

static inline unsigned long rdfpcr(void)
{
        unsigned long tmp, ret;
        __asm__ ("" : "=r"(tmp), "=r"(ret));
        return ret;
}

static inline unsigned long
swcr_update_status(unsigned long swcr, unsigned long fpcr)
{
	swcr &= ~0x7e0000ul;
        swcr |= (fpcr >> 3) & 0x7e0000ul;
        return swcr;
}

unsigned long osf_getsysinfo(unsigned long flags)
{
        unsigned long w;
	w = swcr_update_status(flags, rdfpcr());
	return w;
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
// DEFAULT-NEXT:     fn %0 @rdfpcr() -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %1 tmp: u64 [storage=automatic];
// DEFAULT-NEXT:         let %2 ret: u64 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "=r" place<u64>(%1);
// DEFAULT-NEXT:             out 1 "=r" place<u64>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u64>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @swcr_update_status(%4 swcr: u64, %5 fpcr: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %10: u64 [synthetic] = and<u64>(read<u64>(%9), not<u64>(const<u64>(8257536)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%10));
// DEFAULT-NEXT:         let %11: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %12: u64 [synthetic] = or<u64>(read<u64>(%11), and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%5), const<i32>(3)), const<u64>(8257536)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%12));
// DEFAULT-NEXT:         return read<u64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @osf_getsysinfo(%7 flags: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 w: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%8, call<u64, signature=fn(u64, u64) -> u64>(%3, read<u64>(%7), call<u64, signature=fn() -> u64>(%0)));
// DEFAULT-NEXT:         call<u64, signature=fn(u64, u64) -> u64>(%3, read<u64>(%7), call<u64, signature=fn() -> u64>(%0));
// DEFAULT-NEXT:         return read<u64>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
