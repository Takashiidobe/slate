// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase resulted in a 'unrecognizeable insn' on powerpc-linux-gnu
   because of a missing trunc_int_for_mode in simplify_and_const_int.  */

struct display {
  struct disphist *hstent;
  int pid;
  int status;
};

struct disphist {
  struct disphist *next;
  char *name;
  int startTries;
  unsigned rLogin:2,
    sd_how:2,
    sd_when:2,
    lock:1,
    goodExit:1;
  char *nuser, *npass, **nargs;
};

void
StartDisplay (struct display *d)
{
  d->pid = 0;
  d->status = 0;
  d->hstent->lock = d->hstent->rLogin = d->hstent->goodExit =
    d->hstent->sd_how = d->hstent->sd_when = 0;
}

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
// DEFAULT-NEXT:     type @type0 display = struct {
// DEFAULT-NEXT:         field0 hstent: ptr<@type1>;
// DEFAULT-NEXT:         field1 pid: i32;
// DEFAULT-NEXT:         field2 status: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type1 disphist = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type1>;
// DEFAULT-NEXT:         field1 name: ptr<i8>;
// DEFAULT-NEXT:         field2 startTries: i32;
// DEFAULT-NEXT:         field3 rLogin: u32 : 2;
// DEFAULT-NEXT:         field4 sd_how: u32 : 2;
// DEFAULT-NEXT:         field5 sd_when: u32 : 2;
// DEFAULT-NEXT:         field6 lock: u32 : 1;
// DEFAULT-NEXT:         field7 goodExit: u32 : 1;
// DEFAULT-NEXT:         field8 nuser: ptr<i8>;
// DEFAULT-NEXT:         field9 npass: ptr<i8>;
// DEFAULT-NEXT:         field10 nargs: ptr<ptr<i8>>;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 20, 20, 20, 20, 20, 24, 32, 40], bit_offsets=[None, None, None, Some(160), Some(162), Some(164), Some(166), Some(167), None, None, None], bit_units=[(20, 1)], field_units=[None, None, None, Some(0), Some(0), Some(0), Some(0), Some(0), None, None, None]];
// DEFAULT-NEXT:     fn %2 @StartDisplay(%3 d: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%3))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type0>>(%3))), const<i32>(0));
// DEFAULT-NEXT:         write<u32>(bitfield5<unit=0, bytes=20..21, bits=4..6>(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%3)))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield4<unit=0, bytes=20..21, bits=2..4>(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%3)))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=0, bytes=20..21, bits=7..8>(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%3)))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=20..21, bits=0..2>(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%3)))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=0, bytes=20..21, bits=6..7>(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%3)))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
