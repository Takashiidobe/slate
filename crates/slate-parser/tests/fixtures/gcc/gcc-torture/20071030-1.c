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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 colormod: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 entity_state_t = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 num_entities: i32;
// DEFAULT-NEXT:         field1 entities: ptr<@type0>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 packet_entities_t = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 senttime: f64;
// DEFAULT-NEXT:         field1 ping_time: f32;
// DEFAULT-NEXT:         field2 entities: @type2;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type5 client_frame_t = @type4;
// DEFAULT-NEXT:     type @type6 = enum : u32 {
// DEFAULT-NEXT:         %0 cs_free = const<i32>(0);
// DEFAULT-NEXT:         %1 cs_server = const<i32>(1);
// DEFAULT-NEXT:         %2 cs_zombie = const<i32>(2);
// DEFAULT-NEXT:         %3 cs_connected = const<i32>(3);
// DEFAULT-NEXT:         %4 cs_spawned = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type7 sv_client_state_t = @type6;
// DEFAULT-NEXT:     type @type8 client_s = struct {
// DEFAULT-NEXT:         field0 state: @type6;
// DEFAULT-NEXT:         field1 ping: i32;
// DEFAULT-NEXT:         field2 frames: array<@type4, 64>;
// DEFAULT-NEXT:     } [size=2056, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type9 client_t = @type8;
// DEFAULT-NEXT:     fn %0 @memset(%25 <unnamed>: ptr<void>, %26 <unnamed>: i32, %27 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @CalcPing(%18 cl: ptr<@type8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 ping: f32 [storage=automatic];
// DEFAULT-NEXT:         let %20 count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %22 frame: ptr<@type4> [storage=automatic];
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type6>(field0(deref(read<ptr<@type8>>(%18))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             return read<i32>(field1(deref(read<ptr<@type8>>(%18))));
// DEFAULT-NEXT:         write<f32>(%19, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<@type4>>(%22, array_decay<ptr<@type4>, length=Some(64)>(field2(deref(read<ptr<@type8>>(%18)))));
// DEFAULT-NEXT:                 write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%30));
// DEFAULT-NEXT:                 let %31: ptr<@type4> [synthetic] = read<ptr<@type4>>(%22);
// DEFAULT-NEXT:                 let %32: ptr<@type4> [synthetic] = ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(read<ptr<@type4>>(%31), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type4>>(%22, read<ptr<@type4>>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if gt<f32, exceptions=ignore>(read<f32>(field1(deref(read<ptr<@type4>>(%22)))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %33: f32 [synthetic] = read<f32>(%19);
// DEFAULT-NEXT:                             let %34: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%33), read<f32>(field1(deref(read<ptr<@type4>>(%22)))));
// DEFAULT-NEXT:                             write<f32>(%19, read<f32>(%34));
// DEFAULT-NEXT:                             let %35: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                             let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%20, read<i32>(%36));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%20), const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(9999);
// DEFAULT-NEXT:         let %37: f32 [synthetic] = read<f32>(%19);
// DEFAULT-NEXT:         let %38: f32 [synthetic] = div<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%37), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%20)));
// DEFAULT-NEXT:         write<f32>(%19, read<f32>(%38));
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(mul<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%19), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1000))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %24 cl: @type8 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type8>>(%24)), const<i32>(0), const<u64>(2056));
// DEFAULT-NEXT:         write<f32>(field1(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(array_decay<ptr<@type4>, length=Some(64)>(field2(%24)), const<i32>(0)))), const<f32>(1.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type8>) -> i32>(%17, addr_of<ptr<@type8>>(%24)), const<i32>(1000))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
