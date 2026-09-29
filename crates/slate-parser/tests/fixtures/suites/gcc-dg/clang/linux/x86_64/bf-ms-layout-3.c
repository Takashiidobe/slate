/* Test for MS bitfield layout */
/* { dg-do run { target *-*-mingw* *-*-cygwin* i?86-*-* x86_64-*-* } } */

extern void abort();

struct s1_t {
    char a;
    char b __attribute__ ((aligned (16)));
} __attribute__ ((ms_struct));
struct s1_t s1;

struct s2_t {
  char a;
  char b;
} __attribute__ ((ms_struct));
struct s2_t s2;

struct s3_t {
  __extension__ char a : 6;
  char b __attribute__ ((aligned (16)));
} __attribute__ ((ms_struct));
struct s3_t s3;

struct s4_t {
  __extension__ char a : 6;
  char b __attribute__ ((aligned (2)));
} __attribute__ ((ms_struct));
struct s4_t s4;

struct s5_t {
  __extension__ char a : 6;
  char b __attribute__ ((aligned (1)));
} __attribute__ ((ms_struct));
struct s5_t s5;

__extension__
static __PTRDIFF_TYPE__ offs (const void *a, const void *b)
{
  return (__PTRDIFF_TYPE__) ((const char*)a  - (const char*)b);
}

int main()
{
  if (offs (&s1.b, &s1) != 16
      || offs (&s2.b, &s2) != 1
      || offs (&s3.b, &s3) != 16
      || offs (&s4.b, &s4) != 2
      || offs (&s5.b, &s5) != 1)
    abort ();
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE_s1_t:[0-9]+]] s1_t = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_s2_t:[0-9]+]] s2_t = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_s3_t:[0-9]+]] s3_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_s4_t:[0-9]+]] s4_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_s5_t:[0-9]+]] s5_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_s1_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_s2_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s3:[0-9]+]] s3: @type[[TYPE_s3_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s4:[0-9]+]] s4: @type[[TYPE_s4_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s5:[0-9]+]] s5: @type[[TYPE_s5_t]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort(unprototyped) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_offs:[0-9]+]] @offs(%[[VALUE_a:[0-9]+]] a: ptr<const void>, %[[VALUE_b:[0-9]+]] b: ptr<const void>) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%[[VALUE_b]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%[[VALUE_offs]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%[[VALUE_s1]]))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s1_t]]>>(%[[VALUE_s1]]))), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%[[VALUE_offs]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%[[VALUE_s2]]))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s2_t]]>>(%[[VALUE_s2]]))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%[[VALUE_offs]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%[[VALUE_s3]]))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s3_t]]>>(%[[VALUE_s3]]))), widen<i64, reason=usual_arith>(const<i32>(16))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%[[VALUE_offs]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%[[VALUE_s4]]))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s4_t]]>>(%[[VALUE_s4]]))), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%[[VALUE_offs]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%[[VALUE_s5]]))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_s5_t]]>>(%[[VALUE_s5]]))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
