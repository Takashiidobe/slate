/* { dg-do run } */
/* { dg-options "-O2 -fno-inline" } */

struct usb_interface_descriptor {
 unsigned short wMaxPacketSize;
  char e;
} __attribute__ ((packed));

struct usb_device {
 int devnum;
 struct usb_interface_descriptor if_desc[2];
};

extern int printf (const char *, ...);

void foo (unsigned short a)
{
  printf ("%d\n", a);
}

struct usb_device ndev;

void usb_set_maxpacket(int n)
{
  int i;

  for(i=0; i<n;i++)
    foo((&ndev)->if_desc[i].wMaxPacketSize);
}

int
main()
{
  usb_set_maxpacket(2);
  return 0;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_usb_interface_descriptor:[0-9]+]] usb_interface_descriptor = struct {
// DEFAULT-NEXT:         field0 wMaxPacketSize: u16;
// DEFAULT-NEXT:         field1 e: i8;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_usb_device:[0-9]+]] usb_device = struct {
// DEFAULT-NEXT:         field0 devnum: i32;
// DEFAULT-NEXT:         field1 if_desc: array<@type[[TYPE_usb_interface_descriptor]], 2>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ndev:[0-9]+]] ndev: @type[[TYPE_usb_device]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_usb_set_maxpacket:[0-9]+]] @usb_set_maxpacket(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(u16) -> void>(%[[VALUE_foo]], read<u16>(field0(deref(ptr_offset<ptr<@type[[TYPE_usb_interface_descriptor]]>, subtract=false, element=@type[[TYPE_usb_interface_descriptor]], overflow=ub>(array_decay<ptr<@type[[TYPE_usb_interface_descriptor]]>, length=Some(2)>(field1(deref(addr_of<ptr<@type[[TYPE_usb_device]]>>(%[[VALUE_ndev]])))), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_usb_set_maxpacket]], const<i32>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
