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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/11044
// CHECK-NEXT: //!
// CHECK-NEXT: //! Originator: Tim McGrath <misty-@charter.net>
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase contributed by Eric Botcazou <ebotcazou@libertysurf.fr>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Testcase copied from gcc.target/i386/loop-3.c
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     colormod: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     num_entities: i32,
// CHECK-NEXT:     entities: *mut [[SLATERECORD_0]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     senttime: f64,
// CHECK-NEXT:     ping_time: f32,
// CHECK-NEXT:     entities: [[SLATERECORD_1]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct client_s {
// CHECK-NEXT:     state: u32,
// CHECK-NEXT:     ping: i32,
// CHECK-NEXT:     frames: [[[SLATERECORD_2]]; 64],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn CalcPing(mut cl: &mut client_s) -> i32 {
// CHECK-NEXT:     let mut ping: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut count: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut frame: *mut [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if ((unsafe { (*(cl as *mut client_s)).state }) as u32) == ((1 as i32) as u32) {
// CHECK-NEXT:         return unsafe { (*(cl as *mut client_s)).ping };
// CHECK-NEXT:     }
// CHECK-NEXT:     frame = unsafe { (*(cl as *mut client_s)).frames.as_mut_ptr() as *mut [[SLATERECORD_2]] };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         if (unsafe { (*frame).ping_time }) > ((0 as i32) as f32) {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: f32 = ping;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: f32 = [[V_3]] + unsafe { (*frame).ping_time };
// CHECK-NEXT:             ping = [[V_4]];
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:             count = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut [[SLATERECORD_2]] = frame;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut [[SLATERECORD_2]] = unsafe { [[V_9]].offset((1 as i32) as isize) };
// CHECK-NEXT:         frame = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(count != (0 as i32)) {
// CHECK-NEXT:         return 9999 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: f32 = ping;
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: f32 = [[V_11]] / (count as f32);
// CHECK-NEXT:     ping = [[V_12]];
// CHECK-NEXT:     (ping * ((1000 as i32) as f32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cl: client_s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(cl) as *mut (), 0 as i32, 2056 as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (cl.frames.as_mut_ptr() as *mut [[SLATERECORD_2]]).offset((0 as i32) as isize) })
// CHECK-NEXT:             .ping_time = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if CalcPing(unsafe { &mut (*std::ptr::addr_of_mut!(cl)) }) != (1000 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
