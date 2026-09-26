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
// DEFAULT-NEXT:     type @type0 dev_t = u32;
// DEFAULT-NEXT:     type @type1 kdev_t = u32;
// DEFAULT-NEXT:     global %31 .str31: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%30 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @to_kdev_t(%5 dev: i32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 major: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 minor: i32 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             return reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%5));
// DEFAULT-NEXT:         write<i32>(%6, shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%5), const<i32>(8)));
// DEFAULT-NEXT:         write<i32>(%7, and<i32>(read<i32>(%5), const<i32>(255)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%6), const<i32>(22)), read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @do_mknod(%9 filename: ptr<const i8>, %10 mode: i32, %11 dev: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(360710264)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @getname(%13 filename: ptr<const i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 a1: u32 [storage=automatic];
// DEFAULT-NEXT:         let %15 a2: u32 [storage=automatic];
// DEFAULT-NEXT:         let %16 a3: u32 [storage=automatic];
// DEFAULT-NEXT:         let %17 a4: u32 [storage=automatic];
// DEFAULT-NEXT:         let %18 a5: u32 [storage=automatic];
// DEFAULT-NEXT:         let %19 a6: u32 [storage=automatic];
// DEFAULT-NEXT:         let %20 a7: u32 [storage=automatic];
// DEFAULT-NEXT:         let %21 a8: u32 [storage=automatic];
// DEFAULT-NEXT:         let %22 a9: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%14, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%15, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u32>(%16, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(%17, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         write<u32>(%18, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%19, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%20, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%21, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<u32>(%22, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(ptr_to_int<u32, reason=explicit>(read<ptr<const i8>>(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(13))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         return int_to_ptr<ptr<i8>, reason=explicit>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%14), read<u32>(%15)), mul<u32, overflow=wrap>(read<u32>(%16), read<u32>(%17))), mul<u32, overflow=wrap>(read<u32>(%18), read<u32>(%19))), mul<u32, overflow=wrap>(read<u32>(%20), read<u32>(%21))), read<u32>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @sys_mknod(%24 filename: ptr<const i8>, %25 mode: i32, %26 dev: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 error: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28 tmp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%28, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%12, read<ptr<const i8>>(%24)));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%12, read<ptr<const i8>>(%24));
// DEFAULT-NEXT:         write<i32>(%27, truncate<i32, reason=assign, fits=unknown>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%28))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32, u32) -> void>(%8, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%28)), read<i32>(%25), call<u32, signature=fn(i32) -> u32>(%4, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%26))));
// DEFAULT-NEXT:         return read<i32>(%27);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32, u32) -> i32>(%23, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%31)), const<i32>(1), reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
