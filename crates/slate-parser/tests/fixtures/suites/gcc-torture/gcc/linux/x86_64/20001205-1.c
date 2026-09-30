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
// DEFAULT-NEXT:     fn %[[VALUE_rdfpcr:[0-9]+]] @rdfpcr() -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: u64 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 64 place<u64>(%[[VALUE_tmp]]);
// DEFAULT-NEXT:             lateout 1 "r" [reg] width 64 place<u64>(%[[VALUE_ret]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_swcr_update_status:[0-9]+]] @swcr_update_status(%[[VALUE_swcr:[0-9]+]] swcr: u64, %[[VALUE_fpcr:[0-9]+]] fpcr: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_swcr]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = and<u64>(read<u64>(%[[VALUE0]]), not<u64>(const<u64>(8257536)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_swcr]], read<u64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_swcr]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE2]]), and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_fpcr]]), const<i32>(3)), const<u64>(8257536)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_swcr]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_swcr]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_osf_getsysinfo:[0-9]+]] @osf_getsysinfo(%[[VALUE_flags:[0-9]+]] flags: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_w]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_swcr_update_status]], read<u64>(%[[VALUE_flags]]), call<u64, signature=fn() -> u64>(%[[VALUE_rdfpcr]])));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_w]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
