typedef unsigned long uptr;
typedef uptr uw __attribute__((__mode__(__unwind_word__)));
typedef int suw __attribute__((mode(unwind_word)));
typedef int w __attribute__((mode(word)));

int sizes[] = {sizeof(uw), sizeof(suw), sizeof(w)};
int signs[] = {(uw)-1 > 0, (suw)-1 < 0};

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
// DEFAULT-NEXT:     type @type[[TYPE_uptr:[0-9]+]] uptr = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uw:[0-9]+]] uw = u64;
// DEFAULT-NEXT:     type @type[[TYPE_suw:[0-9]+]] suw = i64;
// DEFAULT-NEXT:     type @type[[TYPE_w:[0-9]+]] w = i64;
// DEFAULT-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))), index1 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))), index2 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signs:[0-9]+]] signs: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = from_bool<i32, reason=assign>(gt<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))), index1 = from_bool<i32, reason=assign>(lt<i64>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(0))))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
