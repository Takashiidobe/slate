void abort(void);
void exit(int);

typedef unsigned int dev_t;
typedef unsigned int kdev_t;

static inline kdev_t to_kdev_t(int dev) {
  int major, minor;

  if (sizeof(kdev_t) == 16)
    return (kdev_t)dev;
  major = (dev >> 8);
  minor = (dev & 0xff);
  return (((major) << 22) | (minor));
}

void do_mknod(const char *filename, int mode, kdev_t dev) {
  if (dev == 0x15800078)
    exit(0);
  else
    abort();
}

char *getname(const char *filename) {
  register unsigned int a1, a2, a3, a4, a5, a6, a7, a8, a9;
  a1 = (unsigned int)(filename) * 5 + 1;
  a2 = (unsigned int)(filename) * 6 + 2;
  a3 = (unsigned int)(filename) * 7 + 3;
  a4 = (unsigned int)(filename) * 8 + 4;
  a5 = (unsigned int)(filename) * 9 + 5;
  a6 = (unsigned int)(filename) * 10 + 5;
  a7 = (unsigned int)(filename) * 11 + 5;
  a8 = (unsigned int)(filename) * 12 + 5;
  a9 = (unsigned int)(filename) * 13 + 5;
  return (char *)(a1 * a2 + a3 * a4 + a5 * a6 + a7 * a8 + a9);
}

int sys_mknod(const char *filename, int mode, dev_t dev) {
  int   error;
  char *tmp;

  tmp   = getname(filename);
  error = ((long)(tmp));
  do_mknod(tmp, mode, to_kdev_t(dev));
  return error;
}

int main(void) {
  if (sizeof(int) != 4)
    exit(0);

  return sys_mknod("test", 1, 0x12345678);
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
// DEFAULT-NEXT:     type @type[[TYPE_dev_t:[0-9]+]] dev_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_kdev_t:[0-9]+]] kdev_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_to_kdev_t:[0-9]+]] @to_kdev_t(%[[VALUE_dev:[0-9]+]] dev: i32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_major:[0-9]+]] major: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_minor:[0-9]+]] minor: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             return reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_dev]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_major]], shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_dev]]), const<i32>(8)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_minor]], and<i32>(read<i32>(%[[VALUE_dev]]), const<i32>(255)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_major]]), const<i32>(22)), read<i32>(%[[VALUE_minor]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_do_mknod:[0-9]+]] @do_mknod(%[[VALUE_filename:[0-9]+]] filename: ptr<const i8>, %[[VALUE_mode:[0-9]+]] mode: i32, %[[VALUE_dev_2:[0-9]+]] dev: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_dev_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(360710264)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getname:[0-9]+]] @getname(%[[VALUE_filename_2:[0-9]+]] filename: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a4:[0-9]+]] a4: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a5:[0-9]+]] a5: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a6:[0-9]+]] a6: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a7:[0-9]+]] a7: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a8:[0-9]+]] a8: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a9:[0-9]+]] a9: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a1]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a2]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a3]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a4]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a5]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a6]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a7]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a8]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a9]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%[[VALUE_filename_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(13))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         return int_to_ptr<ptr<i8>, reason=explicit>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%[[VALUE_a1]]), read<u32>(%[[VALUE_a2]])), mul<u32, overflow=wrap>(read<u32>(%[[VALUE_a3]]), read<u32>(%[[VALUE_a4]]))), mul<u32, overflow=wrap>(read<u32>(%[[VALUE_a5]]), read<u32>(%[[VALUE_a6]]))), mul<u32, overflow=wrap>(read<u32>(%[[VALUE_a7]]), read<u32>(%[[VALUE_a8]]))), read<u32>(%[[VALUE_a9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sys_mknod:[0-9]+]] @sys_mknod(%[[VALUE_filename_3:[0-9]+]] filename: ptr<const i8>, %[[VALUE_mode_2:[0-9]+]] mode: i32, %[[VALUE_dev_3:[0-9]+]] dev: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_error:[0-9]+]] error: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_tmp]], call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_getname]], read<ptr<const i8>>(%[[VALUE_filename_3]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_error]], truncate<i32, reason=assign, fits=unknown>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_tmp]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32, u32) -> void>(%[[VALUE_do_mknod]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_tmp]])), read<i32>(%[[VALUE_mode_2]]), call<u32, signature=fn(i32) -> u32>(%[[VALUE_to_kdev_t]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_dev_3]]))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32, u32) -> i32>(%[[VALUE_sys_mknod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])), const<i32>(1), reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
