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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// IR-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_Alias:[0-9]+]] Alias = @type[[TYPE_S]];
// IR-NEXT:     type @type[[TYPE_EnumAlias:[0-9]+]] EnumAlias = @type[[TYPE_E]];
// IR-NEXT:     global %[[VALUE_global_enum:[0-9]+]] global_enum: @type[[TYPE_E]] [storage=static] = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// IR-NEXT:     fn %[[VALUE_record_local:[0-9]+]] @record_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// IR-NEXT:         write<i32>(field0(%[[VALUE_s]]), const<i32>(1));
// IR-NEXT:         return read<i32>(field0(%[[VALUE_s]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_union_local:[0-9]+]] @union_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// IR-NEXT:         write<i32>(field0(%[[VALUE_u]]), const<i32>(2));
// IR-NEXT:         return read<i32>(field0(%[[VALUE_u]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_alias_local:[0-9]+]] @alias_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// IR-NEXT:         write<i32>(field0(%[[VALUE_a]]), const<i32>(3));
// IR-NEXT:         return read<i32>(field0(%[[VALUE_a]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pointer_local:[0-9]+]] @pointer_local(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_S]]> [storage=automatic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]);
// IR-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_local:[0-9]+]] @enum_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_e:[0-9]+]] e: @type[[TYPE_E]] [storage=automatic] = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_alias_local:[0-9]+]] @enum_alias_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: @type[[TYPE_E]] [storage=automatic] = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// IR-NEXT:         return from_bool<i32, reason=return>(ne<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_parameter:[0-9]+]] @enum_parameter(%[[VALUE_e_3:[0-9]+]] e: @type[[TYPE_E]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_assigned:[0-9]+]] @enum_assigned() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_e_4:[0-9]+]] e: @type[[TYPE_E]] [storage=automatic];
// IR-NEXT:         write<@type[[TYPE_E]]>(%[[VALUE_e_4]], int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// IR-NEXT:         return from_bool<i32, reason=return>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_4]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_arithmetic:[0-9]+]] @enum_arithmetic(%[[VALUE_e_5:[0-9]+]] e: @type[[TYPE_E]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(neg<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_5]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(not<bool>(ne<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_5]])), const<u32>(0)))))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_condition:[0-9]+]] @enum_condition(%[[VALUE_e_6:[0-9]+]] e: @type[[TYPE_E]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_6]])), const<u32>(0))
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_switch:[0-9]+]] @enum_switch(%[[VALUE_e_7:[0-9]+]] e: @type[[TYPE_E]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e_7]]))
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<u32>(0):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 default %[[VALUE0]]:
// IR-NEXT:                     return const<i32>(0);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested_block:[0-9]+]] @nested_block(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))));
// IR-NEXT:             return read<i32>(field0(%[[VALUE_s_2]]));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
