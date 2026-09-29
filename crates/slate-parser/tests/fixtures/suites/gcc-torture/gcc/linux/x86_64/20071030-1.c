/* PR target/11044 */
/* Originator: Tim McGrath <misty-@charter.net> */
/* Testcase contributed by Eric Botcazou <ebotcazou@libertysurf.fr> */

/* Testcase copied from gcc.target/i386/loop-3.c */

extern void *memset(void *, int, __SIZE_TYPE__);
extern void  abort(void);

typedef struct {
  unsigned char colormod;
} entity_state_t;

typedef struct {
  int             num_entities;
  entity_state_t *entities;
} packet_entities_t;

typedef struct {
  double            senttime;
  float             ping_time;
  packet_entities_t entities;
} client_frame_t;

typedef enum {
  cs_free,
  cs_server,
  cs_zombie,
  cs_connected,
  cs_spawned
} sv_client_state_t;

typedef struct client_s {
  sv_client_state_t state;
  int               ping;
  client_frame_t    frames[64];
} client_t;

int CalcPing(client_t *cl) {
  float                    ping;
  int                      count, i;
  register client_frame_t *frame;

  if (cl->state == cs_server)
    return cl->ping;
  ping  = 0;
  count = 0;
  for (frame = cl->frames, i = 0; i < 64; i++, frame++) {
    if (frame->ping_time > 0) {
      ping += frame->ping_time;
      count++;
    }
  }
  if (!count)
    return 9999;
  ping /= count;

  return ping * 1000;
}

int main(void) {
  client_t cl;

  memset(&cl, 0, sizeof(cl));

  cl.frames[0].ping_time = 1.0f;

  if (CalcPing(&cl) != 1000)
    abort();

  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 colormod: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_entity_state_t:[0-9]+]] entity_state_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 num_entities: i32;
// DEFAULT-NEXT:         field1 entities: ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_packet_entities_t:[0-9]+]] packet_entities_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 senttime: f64;
// DEFAULT-NEXT:         field1 ping_time: f32;
// DEFAULT-NEXT:         field2 entities: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_client_frame_t:[0-9]+]] client_frame_t = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_cs_free:[0-9]+]] cs_free = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_cs_server:[0-9]+]] cs_server = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_cs_zombie:[0-9]+]] cs_zombie = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_cs_connected:[0-9]+]] cs_connected = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_cs_spawned:[0-9]+]] cs_spawned = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_sv_client_state_t:[0-9]+]] sv_client_state_t = @type[[TYPE3]];
// DEFAULT-NEXT:     type @type[[TYPE_client_s:[0-9]+]] client_s = struct {
// DEFAULT-NEXT:         field0 state: @type[[TYPE3]];
// DEFAULT-NEXT:         field1 ping: i32;
// DEFAULT-NEXT:         field2 frames: array<@type[[TYPE2]], 64>;
// DEFAULT-NEXT:     } [size=2056, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_client_t:[0-9]+]] client_t = @type[[TYPE_client_s]];
// DEFAULT-NEXT:     fn %[[VALUE_cs_free]] @memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cs_server]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_CalcPing:[0-9]+]] @CalcPing(%[[VALUE_cl:[0-9]+]] cl: ptr<@type[[TYPE_client_s]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ping:[0-9]+]] ping: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_frame:[0-9]+]] frame: ptr<@type[[TYPE2]]> [storage=automatic];
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE3]]>(field0(deref(read<ptr<@type[[TYPE_client_s]]>>(%[[VALUE_cl]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             return read<i32>(field1(deref(read<ptr<@type[[TYPE_client_s]]>>(%[[VALUE_cl]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_ping]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE2]]>>(%[[VALUE_frame]], array_decay<ptr<@type[[TYPE2]]>, length=Some(64)>(field2(deref(read<ptr<@type[[TYPE_client_s]]>>(%[[VALUE_cl]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE2]]> [synthetic] = read<ptr<@type[[TYPE2]]>>(%[[VALUE_frame]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<@type[[TYPE2]]> [synthetic] = ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(read<ptr<@type[[TYPE2]]>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE2]]>>(%[[VALUE_frame]], read<ptr<@type[[TYPE2]]>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if gt<f32, exceptions=observable>(read<f32>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_frame]])))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_ping]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE8]]), read<f32>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_frame]])))));
// DEFAULT-NEXT:                             write<f32>(%[[VALUE_ping]], read<f32>(%[[VALUE9]]));
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:                             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(9999);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_ping]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: f32 [synthetic] = div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE12]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_count]])));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_ping]], read<f32>(%[[VALUE13]]));
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_ping]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1000))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_cl_2:[0-9]+]] cl: @type[[TYPE_client_s]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_cs_free]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_client_s]]>>(%[[VALUE_cl_2]])), const<i32>(0), const<u64>(2056));
// DEFAULT-NEXT:         write<f32>(field1(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(64)>(field2(%[[VALUE_cl_2]])), const<i32>(0)))), const<f32>(1.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_client_s]]>) -> i32>(%[[VALUE_CalcPing]], addr_of<ptr<@type[[TYPE_client_s]]>>(%[[VALUE_cl_2]])), const<i32>(1000))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_cs_server]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
