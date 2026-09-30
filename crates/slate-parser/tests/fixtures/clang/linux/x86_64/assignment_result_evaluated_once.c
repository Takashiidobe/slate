// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
int n;
int f(int x) { n++; return x; }
struct S { unsigned bf : 8; int sb : 4; _Bool flag : 1; } s;

int chains(void) {
  int a, b, i = 0;
  b = f(0);
  a = b = f(1);
  b = (i = i + 1);
  a = b = 0;
  return a + b + n;
}

int bitfields(int x) {
  int a;
  a = s.bf = 300;
  a += s.bf = x;
  a += (s.bf += x);
  a += ++s.bf;
  a += s.bf++;
  a += s.sb = 9;
  a += s.flag = x;
  return a;
}

int promoted(int x) {
  return ((s.bf = x) > -1) + (++s.bf > -1) + ((s.bf += 1) > -1) + (s.bf++ > -1);
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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 bf: u32 : 8;
// IR-NEXT:         field1 sb: i32 : 4;
// IR-NEXT:         field2 flag: bool : 1;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), Some(12)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// IR-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_chains:[0-9]+]] @chains() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         write<i32>(%[[VALUE_b]], call<i32>(%[[VALUE_f]], const<i32>(0)));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_f]], const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         write<i32>(%[[VALUE_b]], const<i32>(0));
// IR-NEXT:         write<i32>(%[[VALUE_a]], const<i32>(0));
// IR-NEXT:         return add<i32>(add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(%[[VALUE_n]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bitfields:[0-9]+]] @bitfields(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), reinterpret<u32>(const<i32>(300)));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], reinterpret<i32>(widen<u32>(truncate<u8b>(reinterpret<u32>(const<i32>(300))))));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = reinterpret<u32>(read<i32>(%[[VALUE_x_2]]));
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE4]]), reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE5]])))));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE8]])), read<i32>(%[[VALUE_x_2]])));
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE7]]), reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE9]])))));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE10]]));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE12]])), const<i32>(1)));
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE13]]));
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE11]]), reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE13]])))));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE14]]));
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: u32 [synthetic] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE16]])), const<i32>(1)));
// IR-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE17]]));
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = reinterpret<i32>(add<u32>(reinterpret<u32>(read<i32>(%[[VALUE15]])), read<u32>(%[[VALUE16]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE18]]));
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..2, bits=8..12>(%[[VALUE_s]]), const<i32>(9));
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE19]]), widen<i32>(truncate<i4b>(const<i32>(9))));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE20]]));
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic] = ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0));
// IR-NEXT:         write<bool>(bitfield2<unit=0, bytes=0..2, bits=12..13>(%[[VALUE_s]]), read<bool>(%[[VALUE22]]));
// IR-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE21]]), from_bool<i32>(read<bool>(%[[VALUE22]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE23]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_promoted:[0-9]+]] @promoted(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE24:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(read<i32>(%[[VALUE_x_3]]));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE24]]));
// IR-NEXT:         let %[[VALUE25:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE26:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE25]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE26]]));
// IR-NEXT:         let %[[VALUE27:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE28:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE27]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE28]]));
// IR-NEXT:         let %[[VALUE29:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE30:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE29]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE30]]));
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(from_bool<i32>(gt<i32>(reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE24]])))), neg<i32>(const<i32>(1)))), from_bool<i32>(gt<i32>(reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE26]])))), neg<i32>(const<i32>(1))))), from_bool<i32>(gt<i32>(reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE28]])))), neg<i32>(const<i32>(1))))), from_bool<i32>(gt<u32>(read<u32>(%[[VALUE29]]), reinterpret<u32>(neg<i32>(const<i32>(1))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
