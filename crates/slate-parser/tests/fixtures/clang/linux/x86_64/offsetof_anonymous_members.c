// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

#include <stddef.h>

struct sqe {
  int fd;
  union {
    long off;
    long addr2;
  };
  union {
    struct {
      short lo;
      struct {
        char tag;
        int deep[4];
      };
    };
    double wide;
  };
  struct {
    char pad;
    union {
      int inner;
      char bytes[8];
    };
  } named;
};

_Static_assert(offsetof(struct sqe, off) == 8, "");
_Static_assert(offsetof(struct sqe, addr2) == 8, "");
_Static_assert(offsetof(struct sqe, lo) == 16, "");
_Static_assert(offsetof(struct sqe, tag) == 20, "");
_Static_assert(offsetof(struct sqe, deep[2]) == 32, "");
_Static_assert(offsetof(struct sqe, wide) == 16, "");
_Static_assert(offsetof(struct sqe, named.inner) == 44, "");
_Static_assert(offsetof(struct sqe, named.bytes[3]) == 47, "");

size_t deep_offset(void) {
  return offsetof(struct sqe, deep[1]);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_sqe:[0-9]+]] sqe = struct {
// IR-NEXT:         field0 fd: i32;
// IR-NEXT:         field1 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:         field2 <anonymous>: @type[[TYPE1:[0-9]+]];
// IR-NEXT:         field3 named: @type[[TYPE4:[0-9]+]];
// IR-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 40]];
// IR-NEXT:     type @type[[TYPE0]] = union {
// IR-NEXT:         field0 off: i64;
// IR-NEXT:         field1 addr2: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE1]] = union {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE2:[0-9]+]];
// IR-NEXT:         field1 wide: f64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE2]] = struct {
// IR-NEXT:         field0 lo: i16;
// IR-NEXT:         field1 <anonymous>: @type[[TYPE3:[0-9]+]];
// IR-NEXT:     } [size=24, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE3]] = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 deep: array<i32, 4>;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE4]] = struct {
// IR-NEXT:         field0 pad: i8;
// IR-NEXT:         field1 <anonymous>: @type[[TYPE5:[0-9]+]];
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE5]] = union {
// IR-NEXT:         field0 inner: i32;
// IR-NEXT:         field1 bytes: array<i8, 8>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// IR-NEXT:     fn %[[VALUE_deep_offset:[0-9]+]] @deep_offset() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(28);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
