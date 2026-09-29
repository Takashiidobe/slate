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
// DEFAULT-NEXT:     type @type[[TYPE___dev_t:[0-9]+]] __dev_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uid_t:[0-9]+]] __uid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___gid_t:[0-9]+]] __gid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___ino_t:[0-9]+]] __ino_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___mode_t:[0-9]+]] __mode_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___nlink_t:[0-9]+]] __nlink_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___time_t:[0-9]+]] __time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___blksize_t:[0-9]+]] __blksize_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___blkcnt_t:[0-9]+]] __blkcnt_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___syscall_slong_t:[0-9]+]] __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_timespec:[0-9]+]] timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_stat:[0-9]+]] stat = struct {
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
// DEFAULT-NEXT:         field11 st_atim: @type[[TYPE_timespec]];
// DEFAULT-NEXT:         field12 st_mtim: @type[[TYPE_timespec]];
// DEFAULT-NEXT:         field13 st_ctim: @type[[TYPE_timespec]];
// DEFAULT-NEXT:         field14 __glibc_reserved: array<i64, 3>;
// DEFAULT-NEXT:     } [size=144, align=8, offsets=[0, 8, 16, 24, 28, 32, 36, 40, 48, 56, 64, 72, 88, 104, 120]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 110, 117, 108, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stat:[0-9]+]] @stat(%[[VALUE___file:[0-9]+]] __file: ptr<const i8> [restrict], %[[VALUE___buf:[0-9]+]] __buf: ptr<@type[[TYPE_stat]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_info:[0-9]+]] info: @type[[TYPE_stat]] [storage=automatic] = aggregate<@type[[TYPE_stat]], zero_fill=true>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<@type[[TYPE_stat]]>) -> i32>(%[[VALUE_stat]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), addr_of<ptr<@type[[TYPE_stat]]>>(%[[VALUE_info]])), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), read<i64>(field0(field12(%[[VALUE_info]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
