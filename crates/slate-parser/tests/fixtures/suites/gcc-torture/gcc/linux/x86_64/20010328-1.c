// SLATE-FILECHECK-DEFINES DEFAULT

typedef __SIZE_TYPE__ size_t;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;

typedef struct
  {
    int __val[2];
  } __fsid_t;

typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;
typedef __u_quad_t __ino64_t;

extern void *memcpy (void *__restrict __dest,
                     __const void *__restrict __src, size_t __n) ;

struct statfs
  {
    int f_type;
    int f_bsize;

    __fsblkcnt_t f_blocks;
    __fsblkcnt_t f_bfree;
    __fsblkcnt_t f_bavail;
    __fsfilcnt_t f_files;
    __fsfilcnt_t f_ffree;

    __fsid_t f_fsid;
    int f_namelen;
    int f_spare[6];
  };


struct statfs64
  {
    int f_type;
    int f_bsize;
    __fsblkcnt64_t f_blocks;
    __fsblkcnt64_t f_bfree;
    __fsblkcnt64_t f_bavail;
    __fsfilcnt64_t f_files;
    __fsfilcnt64_t f_ffree;
    __fsid_t f_fsid;
    int f_namelen;
    int f_spare[6];
  };

extern int __statfs (__const char *__file, struct statfs *__buf);
extern int __statfs64 (__const char *__file, struct statfs64 *__buf);


int
__statfs64 (const char *file, struct statfs64 *buf)
{
  struct statfs buf32;

  if (__statfs (file, &buf32) < 0)
    return -1;

  buf->f_type = buf32.f_type;
  buf->f_bsize = buf32.f_bsize;
  buf->f_blocks = buf32.f_blocks;
  buf->f_bfree = buf32.f_bfree;
  buf->f_bavail = buf32.f_bavail;
  buf->f_files = buf32.f_files;
  buf->f_ffree = buf32.f_ffree;
  buf->f_fsid = buf32.f_fsid;
  buf->f_namelen = buf32.f_namelen;
  memcpy (buf->f_spare, buf32.f_spare, sizeof (buf32.f_spare));

  return 0;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __u_int = u32;
// DEFAULT-NEXT:     type @type2 __u_long = u64;
// DEFAULT-NEXT:     type @type3 __u_quad_t = u64;
// DEFAULT-NEXT:     type @type4 __quad_t = i64;
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 __val: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type6 __fsid_t = @type5;
// DEFAULT-NEXT:     type @type7 __blksize_t = i64;
// DEFAULT-NEXT:     type @type8 __blkcnt_t = i64;
// DEFAULT-NEXT:     type @type9 __blkcnt64_t = i64;
// DEFAULT-NEXT:     type @type10 __fsblkcnt_t = u64;
// DEFAULT-NEXT:     type @type11 __fsblkcnt64_t = u64;
// DEFAULT-NEXT:     type @type12 __fsfilcnt_t = u64;
// DEFAULT-NEXT:     type @type13 __fsfilcnt64_t = u64;
// DEFAULT-NEXT:     type @type14 __ino64_t = u64;
// DEFAULT-NEXT:     type @type15 statfs = struct {
// DEFAULT-NEXT:         field0 f_type: i32;
// DEFAULT-NEXT:         field1 f_bsize: i32;
// DEFAULT-NEXT:         field2 f_blocks: u64;
// DEFAULT-NEXT:         field3 f_bfree: u64;
// DEFAULT-NEXT:         field4 f_bavail: u64;
// DEFAULT-NEXT:         field5 f_files: u64;
// DEFAULT-NEXT:         field6 f_ffree: u64;
// DEFAULT-NEXT:         field7 f_fsid: @type5;
// DEFAULT-NEXT:         field8 f_namelen: i32;
// DEFAULT-NEXT:         field9 f_spare: array<i32, 6>;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 4, 8, 16, 24, 32, 40, 48, 56, 60]];
// DEFAULT-NEXT:     type @type16 statfs64 = struct {
// DEFAULT-NEXT:         field0 f_type: i32;
// DEFAULT-NEXT:         field1 f_bsize: i32;
// DEFAULT-NEXT:         field2 f_blocks: u64;
// DEFAULT-NEXT:         field3 f_bfree: u64;
// DEFAULT-NEXT:         field4 f_bavail: u64;
// DEFAULT-NEXT:         field5 f_files: u64;
// DEFAULT-NEXT:         field6 f_ffree: u64;
// DEFAULT-NEXT:         field7 f_fsid: @type5;
// DEFAULT-NEXT:         field8 f_namelen: i32;
// DEFAULT-NEXT:         field9 f_spare: array<i32, 6>;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 4, 8, 16, 24, 32, 40, 48, 56, 60]];
// DEFAULT-NEXT:     fn %18 @memcpy(%30 __dest: ptr<void> [restrict], %31 __src: ptr<const void> [restrict], %32 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @__statfs(%33 __file: ptr<const i8>, %34 __buf: ptr<@type15>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @__statfs64(%27 file: ptr<const i8>, %28 buf: ptr<@type16>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 buf32: @type15 [storage=automatic];
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<const i8>, ptr<@type15>) -> i32>(%23, read<ptr<const i8>>(%27), addr_of<ptr<@type15>>(%29)), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type16>>(%28))), read<i32>(field0(%29)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type16>>(%28))), read<i32>(field1(%29)));
// DEFAULT-NEXT:         write<u64>(field2(deref(read<ptr<@type16>>(%28))), read<u64>(field2(%29)));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type16>>(%28))), read<u64>(field3(%29)));
// DEFAULT-NEXT:         write<u64>(field4(deref(read<ptr<@type16>>(%28))), read<u64>(field4(%29)));
// DEFAULT-NEXT:         write<u64>(field5(deref(read<ptr<@type16>>(%28))), read<u64>(field5(%29)));
// DEFAULT-NEXT:         write<u64>(field6(deref(read<ptr<@type16>>(%28))), read<u64>(field6(%29)));
// DEFAULT-NEXT:         write<@type5>(field7(deref(read<ptr<@type16>>(%28))), copy<@type5, reason=assign>(read<@type5>(field7(%29))));
// DEFAULT-NEXT:         write<i32>(field8(deref(read<ptr<@type16>>(%28))), read<i32>(field8(%29)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(6)>(field9(deref(read<ptr<@type16>>(%28))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(6)>(field9(%29))), const<u64>(24));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
