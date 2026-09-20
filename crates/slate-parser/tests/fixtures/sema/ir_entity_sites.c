// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct fields {
    _Alignas(16) int field_aligned;
    const int field_const;
    volatile int field_volatile;
    _Atomic int field_atomic;
    int *restrict field_restrict;
};

_Alignas(32) volatile int file_scope;
const int *restrict file_restrict;

void parameters(_Atomic int a, volatile int v, const int c, int *restrict r);

const struct fields *file_literal = &(const struct fields){0};

int block(void) {
    _Alignas(32) volatile int block_scope;
    static _Alignas(64) _Atomic int static_local;
    const int *const_literal = &(const int){7};
    volatile int *volatile_literal = &(volatile int){8};
    return __alignof__(file_scope) + __alignof__(block_scope)
         + __alignof__(static_local) + *const_literal + *volatile_literal;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 fields = struct {
// IR-NEXT:         field0 field_aligned: i32;
// IR-NEXT:         field1 field_const: const i32;
// IR-NEXT:         field2 field_volatile: volatile i32;
// IR-NEXT:         field3 field_atomic: atomic i32;
// IR-NEXT:         field4 field_restrict: ptr<i32>;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 4, 8, 12, 16]];
// IR-NEXT:     global %1 file_scope: volatile i32 [storage=static] [align=32] [linkage=external];
// IR-NEXT:     global %2 file_restrict: ptr<const i32> [storage=static] [restrict] [linkage=external];
// IR-NEXT:     global %4 file_literal: ptr<const @type0> [storage=static] = addr_of<ptr<const @type0>>(compound_literal %14 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(0))) [linkage=external];
// IR-NEXT:     global %7 static_local: atomic i32 [storage=static] [align=64] [linkage=internal];
// IR-NEXT:     fn %3 @parameters(%10 a: atomic i32, %11 v: volatile i32, %12 c: i32 [const], %13 r: ptr<i32> [restrict]) -> void [linkage=external];
// IR-NEXT:     fn %5 @block() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 block_scope: volatile i32 [storage=automatic] [align=32];
// IR-NEXT:         let %8 const_literal: ptr<const i32> [storage=automatic] = addr_of<ptr<const i32>>(compound_literal %15 [storage=automatic] = const<i32>(7));
// IR-NEXT:         let %9 volatile_literal: ptr<volatile i32> [storage=automatic] = addr_of<ptr<volatile i32>>(compound_literal %16 [storage=automatic] = const<i32>(8));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(32), const<u64>(32)), const<u64>(64)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<const i32>>(%8)))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32, volatile>(deref(read<ptr<volatile i32>>(%9))))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
