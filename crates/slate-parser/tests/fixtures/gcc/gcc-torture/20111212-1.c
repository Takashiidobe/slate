/* PR tree-optimization/50569 */
/* Reported by Paul Koning <pkoning@gcc.gnu.org> */
/* Reduced testcase by Mikael Pettersson <mikpe@it.uu.se> */

struct event {
  struct {
    unsigned int sec;
  } sent __attribute__((packed));
};

void __attribute__((noinline, noclone)) frob_entry(char *buf) {
  struct event event;

  __builtin_memcpy(&event, buf, sizeof(event));
  if (event.sent.sec < 64) {
    event.sent.sec = -1U;
    __builtin_memcpy(buf, &event, sizeof(event));
  }
}

int main(void) {
  union {
    char buf[1 + sizeof(struct event)];
    int  align;
  } u;

  __builtin_memset(&u, 0, sizeof u);

  frob_entry(&u.buf[1]);

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
// DEFAULT-NEXT:     type @type0 event = struct {
// DEFAULT-NEXT:         field0 sent: @type1;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 sec: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 5>;
// DEFAULT-NEXT:         field1 align: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %2 @frob_entry(%3 buf: ptr<i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 event: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%4)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%3)), const<u64>(4));
// DEFAULT-NEXT:         if lt<u32>(read<u32>(field0(field0(%4))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(field0(field0(%4)), neg<u32, overflow=wrap>(const<u32>(1)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%3)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%4)), const<u64>(4));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 u: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%7)), const<i32>(0), const<u64>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%2, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%7)), const<i32>(1)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
