/* Test for MS bitfield layout */
/* { dg-do run { target *-*-mingw* *-*-cygwin* i?86-*-* x86_64-*-* } } */

extern void abort();

struct s1_t {
  char a;
  char b __attribute__((aligned(16)));
} __attribute__((ms_struct));
struct s1_t s1;

struct s2_t {
  char a;
  char b;
} __attribute__((ms_struct));
struct s2_t s2;

struct s3_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(16)));
} __attribute__((ms_struct));
struct s3_t s3;

struct s4_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(2)));
} __attribute__((ms_struct));
struct s4_t s4;

struct s5_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(1)));
} __attribute__((ms_struct));
struct s5_t s5;

__extension__ static __PTRDIFF_TYPE__ offs(const void *a, const void *b) {
  return (__PTRDIFF_TYPE__)((const char *)a - (const char *)b);
}

int main() {
  if (offs(&s1.b, &s1) != 16 || offs(&s2.b, &s2) != 1 ||
      offs(&s3.b, &s3) != 16 || offs(&s4.b, &s4) != 2 || offs(&s5.b, &s5) != 1)
    abort();
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
// DEFAULT-NEXT:     type @type0 s1_t = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type1 s2_t = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type2 s3_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type3 s4_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type4 s5_t = struct {
// DEFAULT-NEXT:         field0 a: i8 : 6;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     global %2 s1: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 s2: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 s3: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 s4: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 s5: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @offs(%12 a: ptr<const void>, %13 b: ptr<const void>) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%12)), pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%11, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%2))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%2))), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%11, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%4))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%4))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%11, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%6))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%6))), widen<i64, reason=usual_arith>(const<i32>(16))));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%11, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%8))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%8))), widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i64>(call<i64, signature=fn(ptr<const void>, ptr<const void>) -> i64>(%11, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(field1(%10))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%10))), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
