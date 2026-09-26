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
// DEFAULT-NEXT:     type @type0 epic_rx_desc = struct {
// DEFAULT-NEXT:         field0 next: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 epic_private = struct {
// DEFAULT-NEXT:         field0 rx_ring: ptr<@type0>;
// DEFAULT-NEXT:         field1 rx_skbuff: array<u32, 5>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %6 check_rx_ring: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(12), index1 = const<i32>(14), index2 = const<i32>(16), index3 = const<i32>(18), index4 = const<i32>(10)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @epic_init_ring(%4 ep: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%4)))), read<i32>(%5)))), reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(10), mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%5), const<i32>(1)), const<i32>(2)))));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(deref(read<ptr<@type1>>(%4)))), read<i32>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type1>>(%4)))), sub<i32, overflow=ub>(read<i32>(%5), const<i32>(1))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 ep: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %9 rx_ring: array<@type0, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%9), read<i32>(%10)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(%8)), read<i32>(%10))), reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(%8), array_decay<ptr<@type0>, length=Some(5)>(%9));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%3, addr_of<ptr<@type1>>(%8));
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%9), read<i32>(%10))))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), read<i32>(%10))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(5)>(field1(%8)), read<i32>(%10)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
