
enum e { a, b };
enum e v, w;

int same_enum(int x) { return x ? v : w; }

typedef enum { MD_NONE, MD_SHA256 } md_type_t;
struct rsa { int hash_id; };

md_type_t cast_arm(struct rsa *ctx, md_type_t md_alg) {
  md_type_t mgf1;
  mgf1 = (ctx->hash_id != MD_NONE) ? (md_type_t)ctx->hash_id : md_alg;
  return mgf1;
}

typedef enum { need_more, block_done, finish_started, finish_done } block_state;
struct state { int level; int strategy; };
typedef block_state (*compress_func)(struct state *, int);
struct config { compress_func func; };
static const struct config table[2];

block_state deflate_stored(struct state *, int);
block_state deflate_huff(struct state *, int);

block_state nested(struct state *s, int flush) {
  block_state bstate;
  bstate = s->level == 0 ? deflate_stored(s, flush)
           : s->strategy == 2 ? deflate_huff(s, flush)
                              : (*(table[s->level].func))(s, flush);
  return bstate;
}

typedef enum { fast = 1, dfast, greedy } strategy;
struct params { strategy strategy; };

void member_arms(struct params *actual, const struct params *requested) {
  actual->strategy = requested->strategy == 0 ? actual->strategy : requested->strategy;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a:[0-9]+]] a = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_b:[0-9]+]] b = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a]] MD_NONE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_b]] MD_SHA256 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_md_type_t:[0-9]+]] md_type_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_rsa:[0-9]+]] rsa = struct {
// DEFAULT-NEXT:         field0 hash_id: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a]] need_more = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_b]] block_done = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_finish_started:[0-9]+]] finish_started = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_finish_done:[0-9]+]] finish_done = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_block_state:[0-9]+]] block_state = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_state:[0-9]+]] state = struct {
// DEFAULT-NEXT:         field0 level: i32;
// DEFAULT-NEXT:         field1 strategy: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_compress_func:[0-9]+]] compress_func = ptr<fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>;
// DEFAULT-NEXT:     type @type[[TYPE_config:[0-9]+]] config = struct {
// DEFAULT-NEXT:         field0 func: ptr<fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a]] fast = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_b]] dfast = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_finish_started]] greedy = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_strategy:[0-9]+]] strategy = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_params:[0-9]+]] params = struct {
// DEFAULT-NEXT:         field0 strategy: @type[[TYPE2]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_finish_done]] v: @type[[TYPE_e]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: @type[[TYPE_e]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<@type[[TYPE_config]], 2> [storage=static] [const] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_same_enum:[0-9]+]] @same_enum(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(conditional<u32>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_e]]>(%[[VALUE_finish_done]])), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_e]]>(%[[VALUE_w]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cast_arm:[0-9]+]] @cast_arm(%[[VALUE_ctx:[0-9]+]] ctx: ptr<@type[[TYPE_rsa]]>, %[[VALUE_md_alg:[0-9]+]] md_alg: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_mgf1:[0-9]+]] mgf1: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_mgf1]], int_to_enum<@type[[TYPE0]], reason=assign>(conditional<u32>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_rsa]]>>(%[[VALUE_ctx]])))), const<i32>(0)), enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE0]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(field0(deref(read<ptr<@type[[TYPE_rsa]]>>(%[[VALUE_ctx]]))))))), enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_md_alg]])))));
// DEFAULT-NEXT:         return read<@type[[TYPE0]]>(%[[VALUE_mgf1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_deflate_stored:[0-9]+]] @deflate_stored(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_state]]>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> @type[[TYPE1]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_deflate_huff:[0-9]+]] @deflate_huff(%[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_state]]>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> @type[[TYPE1]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_state]]>, %[[VALUE_flush:[0-9]+]] flush: i32) -> @type[[TYPE1]] [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_bstate:[0-9]+]] bstate: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]])))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE4]], enum_to_int<u32, reason=promotion>(call<@type[[TYPE1]], signature=fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>(%[[VALUE_deflate_stored]], read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]]), read<i32>(%[[VALUE_flush]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]])))), const<i32>(2))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE5]], enum_to_int<u32, reason=promotion>(call<@type[[TYPE1]], signature=fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>(%[[VALUE_deflate_huff]], read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]]), read<i32>(%[[VALUE_flush]]))));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<u32>(%[[VALUE5]], enum_to_int<u32, reason=promotion>(call<@type[[TYPE1]], signature=fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>(read<ptr<fn(ptr<@type[[TYPE_state]]>, i32) -> @type[[TYPE1]]>>(field0(deref(ptr_offset<ptr<const @type[[TYPE_config]]>, subtract=false, element=@type[[TYPE_config]], overflow=ub>(array_decay<ptr<const @type[[TYPE_config]]>, length=Some(2)>(%[[VALUE_table]]), read<i32>(field0(deref(read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]])))))))), read<ptr<@type[[TYPE_state]]>>(%[[VALUE_s]]), read<i32>(%[[VALUE_flush]]))));
// DEFAULT-NEXT:             write<u32>(%[[VALUE4]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_bstate]], int_to_enum<@type[[TYPE1]], reason=assign>(read<u32>(%[[VALUE4]])));
// DEFAULT-NEXT:         return read<@type[[TYPE1]]>(%[[VALUE_bstate]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_member_arms:[0-9]+]] @member_arms(%[[VALUE_actual:[0-9]+]] actual: ptr<@type[[TYPE_params]]>, %[[VALUE_requested:[0-9]+]] requested: ptr<const @type[[TYPE_params]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE2]]>(field0(deref(read<ptr<@type[[TYPE_params]]>>(%[[VALUE_actual]]))), int_to_enum<@type[[TYPE2]], reason=assign>(conditional<u32>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE2]]>(field0(deref(read<ptr<const @type[[TYPE_params]]>>(%[[VALUE_requested]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), enum_to_int<u32, reason=promotion>(read<@type[[TYPE2]]>(field0(deref(read<ptr<@type[[TYPE_params]]>>(%[[VALUE_actual]]))))), enum_to_int<u32, reason=promotion>(read<@type[[TYPE2]]>(field0(deref(read<ptr<const @type[[TYPE_params]]>>(%[[VALUE_requested]]))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
