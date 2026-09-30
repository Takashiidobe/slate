/* Test case contributed by Ingo Rohloff <rohloff@in.tum.de>.
   Code distilled from Linux kernel.  */

/* Compile this program with a gcc-2.95.2 using
   "gcc -O2" and run it. The result will be that
   rx_ring[1].next == 0   (it should be == 14)
   and
   ep.skbuff[4] == 5      (it should be 0)
*/

extern void abort(void);

struct epic_rx_desc {
  unsigned int next;
};

struct epic_private {
  struct epic_rx_desc *rx_ring;
  unsigned int         rx_skbuff[5];
};

static void epic_init_ring(struct epic_private *ep) {
  int i;

  for (i = 0; i < 5; i++) {
    ep->rx_ring[i].next = 10 + (i + 1) * 2;
    ep->rx_skbuff[i]    = 0;
  }
  ep->rx_ring[i - 1].next = 10;
}

static int check_rx_ring[5] = {12, 14, 16, 18, 10};

int main(void) {
  struct epic_private ep;
  struct epic_rx_desc rx_ring[5];
  int                 i;

  for (i = 0; i < 5; i++) {
    rx_ring[i].next = 0;
    ep.rx_skbuff[i] = 5;
  }

  ep.rx_ring = rx_ring;
  epic_init_ring(&ep);

  for (i = 0; i < 5; i++) {
    if (rx_ring[i].next != check_rx_ring[i])
      abort();
    if (ep.rx_skbuff[i] != 0)
      abort();
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_epic_rx_desc:[0-9]+]] epic_rx_desc = struct {
// DEFAULT-NEXT:         field0 next: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_epic_private:[0-9]+]] epic_private = struct {
// DEFAULT-NEXT:         field0 rx_ring: ptr<@type[[TYPE_epic_rx_desc]]>;
// DEFAULT-NEXT:         field1 rx_skbuff: array<u32, 5>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_check_rx_ring:[0-9]+]] check_rx_ring: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(12), index1 = const<i32>(14), index2 = const<i32>(16), index3 = const<i32>(18), index4 = const<i32>(10)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_epic_init_ring:[0-9]+]] @epic_init_ring(%[[VALUE_ep:[0-9]+]] ep: ptr<@type[[TYPE_epic_private]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_epic_rx_desc]]>, subtract=false, element=@type[[TYPE_epic_rx_desc]], overflow=ub>(read<ptr<@type[[TYPE_epic_rx_desc]]>>(field0(deref(read<ptr<@type[[TYPE_epic_private]]>>(%[[VALUE_ep]])))), read<i32>(%[[VALUE_i]])))), reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(10), mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)), const<i32>(2)))));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(deref(read<ptr<@type[[TYPE_epic_private]]>>(%[[VALUE_ep]])))), read<i32>(%[[VALUE_i]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_epic_rx_desc]]>, subtract=false, element=@type[[TYPE_epic_rx_desc]], overflow=ub>(read<ptr<@type[[TYPE_epic_rx_desc]]>>(field0(deref(read<ptr<@type[[TYPE_epic_private]]>>(%[[VALUE_ep]])))), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ep_2:[0-9]+]] ep: @type[[TYPE_epic_private]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rx_ring:[0-9]+]] rx_ring: array<@type[[TYPE_epic_rx_desc]], 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_epic_rx_desc]]>, subtract=false, element=@type[[TYPE_epic_rx_desc]], overflow=ub>(array_decay<ptr<@type[[TYPE_epic_rx_desc]]>, length=Some(5)>(%[[VALUE_rx_ring]]), read<i32>(%[[VALUE_i_2]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(%[[VALUE_ep_2]])), read<i32>(%[[VALUE_i_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_epic_rx_desc]]>>(field0(%[[VALUE_ep_2]]), array_decay<ptr<@type[[TYPE_epic_rx_desc]]>, length=Some(5)>(%[[VALUE_rx_ring]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_epic_private]]>) -> void>(%[[VALUE_epic_init_ring]], addr_of<ptr<@type[[TYPE_epic_private]]>>(%[[VALUE_ep_2]]));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_epic_rx_desc]]>, subtract=false, element=@type[[TYPE_epic_rx_desc]], overflow=ub>(array_decay<ptr<@type[[TYPE_epic_rx_desc]]>, length=Some(5)>(%[[VALUE_rx_ring]]), read<i32>(%[[VALUE_i_2]]))))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_check_rx_ring]]), read<i32>(%[[VALUE_i_2]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(%[[VALUE_ep_2]])), read<i32>(%[[VALUE_i_2]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
