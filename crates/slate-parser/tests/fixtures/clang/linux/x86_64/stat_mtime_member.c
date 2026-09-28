#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", (long long)info.st_mtime);
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
// DEFAULT-NEXT:     type @type0 __dev_t = u64;
// DEFAULT-NEXT:     type @type1 __uid_t = u32;
// DEFAULT-NEXT:     type @type2 __gid_t = u32;
// DEFAULT-NEXT:     type @type3 __ino_t = u64;
// DEFAULT-NEXT:     type @type4 __mode_t = u32;
// DEFAULT-NEXT:     type @type5 __nlink_t = u64;
// DEFAULT-NEXT:     type @type6 __off_t = i64;
// DEFAULT-NEXT:     type @type7 __time_t = i64;
// DEFAULT-NEXT:     type @type8 __blksize_t = i64;
// DEFAULT-NEXT:     type @type9 __blkcnt_t = i64;
// DEFAULT-NEXT:     type @type10 __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type11 timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type12 stat = struct {
// DEFAULT-NEXT:         field0 st_dev: u64;
// DEFAULT-NEXT:         field1 st_ino: u64;
// DEFAULT-NEXT:         field2 st_nlink: u64;
// DEFAULT-NEXT:         field3 st_mode: u32;
// DEFAULT-NEXT:         field4 st_uid: u32;
// DEFAULT-NEXT:         field5 st_gid: u32;
// DEFAULT-NEXT:         field6 __pad0: i32;
// DEFAULT-NEXT:         field7 st_rdev: u64;
// DEFAULT-NEXT:         field8 st_size: i64;
// DEFAULT-NEXT:         field9 st_blksize: i64;
// DEFAULT-NEXT:         field10 st_blocks: i64;
// DEFAULT-NEXT:         field11 st_atim: @type11;
// DEFAULT-NEXT:         field12 st_mtim: @type11;
// DEFAULT-NEXT:         field13 st_ctim: @type11;
// DEFAULT-NEXT:         field14 __glibc_reserved: array<i64, 3>;
// DEFAULT-NEXT:     } [size=144, align=8, offsets=[0, 8, 16, 24, 28, 32, 36, 40, 48, 56, 64, 72, 88, 104, 120]];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 110, 117, 108, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @printf(%20 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @stat(%21 __file: ptr<const i8> [restrict], %22 __buf: ptr<@type12> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 info: @type12 [storage=automatic] = aggregate<@type12, zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<@type12>) -> i32>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%23)), addr_of<ptr<@type12>>(%19)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%24)), read<i64>(field0(field12(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
