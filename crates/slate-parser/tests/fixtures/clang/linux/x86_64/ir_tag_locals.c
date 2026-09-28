// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct S { int a; };
union U { int i; float f; };
enum E { A, B };
typedef struct S Alias;
typedef enum E EnumAlias;

enum E global_enum = B;

int record_local(void) { struct S s; s.a = 1; return s.a; }
int union_local(void) { union U u; u.i = 2; return u.i; }
int alias_local(void) { Alias a; a.a = 3; return a.a; }
int pointer_local(struct S *p) { struct S *q = p; return q->a; }
int enum_local(void) { enum E e = B; return e == B; }
int enum_alias_local(void) { EnumAlias e = A; return e != B; }
int enum_parameter(enum E e) { return e == B; }
int enum_assigned(void) { enum E e; e = B; return e == A; }
int enum_arithmetic(enum E e) { return -e + (int)B + !e; }
int enum_condition(enum E e) { if (e) return 1; return 0; }
int enum_switch(enum E e) { switch (e) { case A: return 1; default: return 0; } }
int nested_block(struct S *p) { { struct S s = *p; return s.a; } }

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
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type1 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type2 E = enum : u32 {
// IR-NEXT:         %0 A = const<i32>(0);
// IR-NEXT:         %1 B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type3 Alias = @type0;
// IR-NEXT:     type @type4 EnumAlias = @type2;
// IR-NEXT:     global %7 global_enum: @type2 [storage=static] = int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// IR-NEXT:     fn %8 @record_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 s: @type0 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%9), const<i32>(1));
// IR-NEXT:         return read<i32>(field0(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @union_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %11 u: @type1 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%11), const<i32>(2));
// IR-NEXT:         return read<i32>(field0(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @alias_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %13 a: @type0 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%13), const<i32>(3));
// IR-NEXT:         return read<i32>(field0(%13));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @pointer_local(%15 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %16 q: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(%15);
// IR-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%16))));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @enum_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18 e: @type2 [storage=automatic] = int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%18)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @enum_alias_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %20 e: @type2 [storage=automatic] = int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// IR-NEXT:         return from_bool<i32, reason=return>(ne<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%20)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @enum_parameter(%22 e: @type2) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%22)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %23 @enum_assigned() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %24 e: @type2 [storage=automatic];
// IR-NEXT:         write<@type2>(%24, int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%24)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @enum_arithmetic(%26 e: @type2) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(neg<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type2>(%26))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(not<bool>(ne<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%26)), const<u32>(0)))))));
// IR-NEXT:     }
// IR-NEXT:     fn %27 @enum_condition(%28 e: @type2) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(%28)), const<u32>(0))
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %29 @enum_switch(%30 e: @type2) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %34 enum_to_int<u32, reason=promotion>(read<@type2>(%30))
// IR-NEXT:             {
// IR-NEXT:                 case %34 const<u32>(0):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 default %34:
// IR-NEXT:                     return const<i32>(0);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT:     fn %31 @nested_block(%32 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         {
// IR-NEXT:             let %33 s: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(deref(read<ptr<@type0>>(%32))));
// IR-NEXT:             return read<i32>(field0(%33));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
