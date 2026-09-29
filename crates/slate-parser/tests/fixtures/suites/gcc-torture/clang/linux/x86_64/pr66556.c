/* { dg-do run } */
/* { dg-require-effective-target int32plus } */

extern void abort(void);

struct {
  unsigned f2;
  unsigned f3 : 15;
  unsigned f5 : 3;
  short    f6;
} b = {0x7f8000, 6, 5, 0}, g = {8, 0, 5, 0};

short         d, l;
int           a, c, h = 8;
volatile char e[237] = {4};
short        *f      = &d;
short         i[5]   = {3};
char          j;
int          *k = &c;

int fn1(unsigned p1) { return -p1; }

void fn2(char p1) {
  a = p1;
  e[0];
}

short fn3() {
  *k = 4;
  return *f;
}

int main() {

  unsigned m;
  short   *n = &i[4];

  m  = fn1((h && j) <= b.f5);
  l  = m > g.f3;
  *n = 3;
  fn2(b.f2 >> 15);
  if ((a & 0xff) != 0xff)
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 f2: u32;
// DEFAULT-NEXT:         field1 f3: u32 : 15;
// DEFAULT-NEXT:         field2 f5: u32 : 3;
// DEFAULT-NEXT:         field3 f6: i16;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 5, 8], bit_offsets=[None, Some(32), Some(47), None], bit_units=[(4, 3)], field_units=[None, Some(0), Some(0), None]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(8355840)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(5)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(8)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(5)), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: volatile array<i8, 237> [storage=static] [align=16] = aggregate<array<i8, 237>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<i16> [storage=static] = addr_of<ptr<i16>>(%[[VALUE_d]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: array<i16, 5> [storage=static] = aggregate<array<i16, 5>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(neg<u32, overflow=wrap>(read<u32>(%[[VALUE_p1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p1_2:[0-9]+]] p1: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], widen<i32, reason=assign>(read<i8>(%[[VALUE_p1_2]])));
// DEFAULT-NEXT:         read<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(237)>(%[[VALUE_e]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_k]])), const<i32>(4));
// DEFAULT-NEXT:         return read<i16>(deref(read<ptr<i16>>(%[[VALUE_f]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(5)>(%[[VALUE_i]]), const<i32>(4))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_m]], reinterpret<u32, reason=assign, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_fn1]], from_bool<u32, reason=arg>(le<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_j]]), const<i8>(0)))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..7, bits=15..18>(%[[VALUE_b]]))))))));
// DEFAULT-NEXT:         reinterpret<u32, reason=assign, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_fn1]], from_bool<u32, reason=arg>(le<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_j]]), const<i8>(0)))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..7, bits=15..18>(%[[VALUE_b]])))))));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_l]], from_bool<i16, reason=assign>(gt<u32>(read<u32>(%[[VALUE_m]]), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..7, bits=0..15>(%[[VALUE_g]])))))));
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_n]])), truncate<i16, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_fn2]], reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(field0(%[[VALUE_b]])), const<i32>(15)))));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%[[VALUE_a]]), const<i32>(255)), const<i32>(255))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
