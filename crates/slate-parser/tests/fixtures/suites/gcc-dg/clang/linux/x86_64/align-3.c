/* { dg-do compile } */
/* { dg-options "-O2 -fdump-rtl-expand" } */

typedef struct { char a[2]; } __attribute__((__packed__)) TU2;
unsigned short get16_unaligned(const void *p) {
    unsigned short v;
    *(TU2 *)(void *)(&v) = *(const TU2 *)p;
    return v;
}

/* { dg-final { scan-rtl-dump "MEM\[^\n\r\]*A8\\\]" "expand" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_TU2:[0-9]+]] TU2 = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_get16_unaligned:[0-9]+]] @get16_unaligned(%[[VALUE_p:[0-9]+]] p: ptr<const void>) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: u16 [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(deref(pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<u16>>(%[[VALUE_v]])))), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(deref(pointer_cast<ptr<const @type[[TYPE0]]>, reason=explicit>(read<ptr<const void>>(%[[VALUE_p]]))))));
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
