// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu89

/* { dg-additional-options "-std=gnu89" } */

typedef struct string STR;
typedef struct atbl ARRAY;
struct string {
    unsigned char str_pok;
};
struct atbl {
    int ary_fill;
};
blah(size,strp)
register int size;
register STR **strp;
{
    register ARRAY *ar;
    ar->ary_fill = size - 1;
    while (size--)
     (*strp)->str_pok &= ~128;
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
// DEFAULT-NEXT:     type @type[[TYPE_string:[0-9]+]] string = struct {
// DEFAULT-NEXT:         field0 str_pok: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_STR:[0-9]+]] STR = @type[[TYPE_string]];
// DEFAULT-NEXT:     type @type[[TYPE_atbl:[0-9]+]] atbl = struct {
// DEFAULT-NEXT:         field0 ary_fill: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ARRAY:[0-9]+]] ARRAY = @type[[TYPE_atbl]];
// DEFAULT-NEXT:     fn %[[VALUE_blah:[0-9]+]] @blah(%[[VALUE_size:[0-9]+]] size: i32, %[[VALUE_strp:[0-9]+]] strp: ptr<ptr<@type[[TYPE_string]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ar:[0-9]+]] ar: ptr<@type[[TYPE_atbl]]> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_atbl]]>>(%[[VALUE_ar]]))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_size]]), const<i32>(1)));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_size]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_size]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_string]]> [synthetic] = read<ptr<@type[[TYPE_string]]>>(deref(read<ptr<ptr<@type[[TYPE_string]]>>>(%[[VALUE_strp]])));
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: u8 [synthetic] = read<u8>(field0(deref(read<ptr<@type[[TYPE_string]]>>(%[[VALUE3]]))));
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE4]]))), not<i32>(const<i32>(128)))));
// DEFAULT-NEXT:             write<u8>(field0(deref(read<ptr<@type[[TYPE_string]]>>(%[[VALUE3]]))), read<u8>(%[[VALUE5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
