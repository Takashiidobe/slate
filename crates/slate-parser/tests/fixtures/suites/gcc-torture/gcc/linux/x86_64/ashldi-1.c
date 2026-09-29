#include <limits.h>

extern void abort(void);
extern void exit(int);

#if __LONG_LONG_MAX__ == 9223372036854775807LL
#define BITS 64

static unsigned long long const data[64] = {
    0x123456789abcdefULL,  0x2468acf13579bdeULL,  0x48d159e26af37bcULL,
    0x91a2b3c4d5e6f78ULL,  0x123456789abcdef0ULL, 0x2468acf13579bde0ULL,
    0x48d159e26af37bc0ULL, 0x91a2b3c4d5e6f780ULL, 0x23456789abcdef00ULL,
    0x468acf13579bde00ULL, 0x8d159e26af37bc00ULL, 0x1a2b3c4d5e6f7800ULL,
    0x3456789abcdef000ULL, 0x68acf13579bde000ULL, 0xd159e26af37bc000ULL,
    0xa2b3c4d5e6f78000ULL, 0x456789abcdef0000ULL, 0x8acf13579bde0000ULL,
    0x159e26af37bc0000ULL, 0x2b3c4d5e6f780000ULL, 0x56789abcdef00000ULL,
    0xacf13579bde00000ULL, 0x59e26af37bc00000ULL, 0xb3c4d5e6f7800000ULL,
    0x6789abcdef000000ULL, 0xcf13579bde000000ULL, 0x9e26af37bc000000ULL,
    0x3c4d5e6f78000000ULL, 0x789abcdef0000000ULL, 0xf13579bde0000000ULL,
    0xe26af37bc0000000ULL, 0xc4d5e6f780000000ULL, 0x89abcdef00000000ULL,
    0x13579bde00000000ULL, 0x26af37bc00000000ULL, 0x4d5e6f7800000000ULL,
    0x9abcdef000000000ULL, 0x3579bde000000000ULL, 0x6af37bc000000000ULL,
    0xd5e6f78000000000ULL, 0xabcdef0000000000ULL, 0x579bde0000000000ULL,
    0xaf37bc0000000000ULL, 0x5e6f780000000000ULL, 0xbcdef00000000000ULL,
    0x79bde00000000000ULL, 0xf37bc00000000000ULL, 0xe6f7800000000000ULL,
    0xcdef000000000000ULL, 0x9bde000000000000ULL, 0x37bc000000000000ULL,
    0x6f78000000000000ULL, 0xdef0000000000000ULL, 0xbde0000000000000ULL,
    0x7bc0000000000000ULL, 0xf780000000000000ULL, 0xef00000000000000ULL,
    0xde00000000000000ULL, 0xbc00000000000000ULL, 0x7800000000000000ULL,
    0xf000000000000000ULL, 0xe000000000000000ULL, 0xc000000000000000ULL,
    0x8000000000000000ULL};

#elif __LONG_LONG_MAX__ == 2147483647LL
#define BITS 32

static unsigned long long const data[32] = {
    0x1234567fULL, 0x2468acfeULL, 0x48d159fcULL, 0x91a2b3f8ULL, 0x234567f0ULL,
    0x468acfe0ULL, 0x8d159fc0ULL, 0x1a2b3f80ULL, 0x34567f00ULL, 0x68acfe00ULL,
    0xd159fc00ULL, 0xa2b3f800ULL, 0x4567f000ULL, 0x8acfe000ULL, 0x159fc000ULL,
    0x2b3f8000ULL, 0x567f0000ULL, 0xacfe0000ULL, 0x59fc0000ULL, 0xb3f80000ULL,
    0x67f00000ULL, 0xcfe00000ULL, 0x9fc00000ULL, 0x3f800000ULL, 0x7f000000ULL,
    0xfe000000ULL, 0xfc000000ULL, 0xf8000000ULL, 0xf0000000ULL, 0xe0000000ULL,
    0xc0000000ULL, 0x80000000ULL};

#else
#error "Update the test case."
#endif

static unsigned long long variable_shift(unsigned long long x, int i) {
  return x << i;
}

static unsigned long long constant_shift(unsigned long long x, int i) {
  switch (i) {
  case 0:
    x = x << 0;
    break;
  case 1:
    x = x << 1;
    break;
  case 2:
    x = x << 2;
    break;
  case 3:
    x = x << 3;
    break;
  case 4:
    x = x << 4;
    break;
  case 5:
    x = x << 5;
    break;
  case 6:
    x = x << 6;
    break;
  case 7:
    x = x << 7;
    break;
  case 8:
    x = x << 8;
    break;
  case 9:
    x = x << 9;
    break;
  case 10:
    x = x << 10;
    break;
  case 11:
    x = x << 11;
    break;
  case 12:
    x = x << 12;
    break;
  case 13:
    x = x << 13;
    break;
  case 14:
    x = x << 14;
    break;
  case 15:
    x = x << 15;
    break;
  case 16:
    x = x << 16;
    break;
  case 17:
    x = x << 17;
    break;
  case 18:
    x = x << 18;
    break;
  case 19:
    x = x << 19;
    break;
  case 20:
    x = x << 20;
    break;
  case 21:
    x = x << 21;
    break;
  case 22:
    x = x << 22;
    break;
  case 23:
    x = x << 23;
    break;
  case 24:
    x = x << 24;
    break;
  case 25:
    x = x << 25;
    break;
  case 26:
    x = x << 26;
    break;
  case 27:
    x = x << 27;
    break;
  case 28:
    x = x << 28;
    break;
  case 29:
    x = x << 29;
    break;
  case 30:
    x = x << 30;
    break;
  case 31:
    x = x << 31;
    break;
#if BITS > 32
  case 32:
    x = x << 32;
    break;
  case 33:
    x = x << 33;
    break;
  case 34:
    x = x << 34;
    break;
  case 35:
    x = x << 35;
    break;
  case 36:
    x = x << 36;
    break;
  case 37:
    x = x << 37;
    break;
  case 38:
    x = x << 38;
    break;
  case 39:
    x = x << 39;
    break;
  case 40:
    x = x << 40;
    break;
  case 41:
    x = x << 41;
    break;
  case 42:
    x = x << 42;
    break;
  case 43:
    x = x << 43;
    break;
  case 44:
    x = x << 44;
    break;
  case 45:
    x = x << 45;
    break;
  case 46:
    x = x << 46;
    break;
  case 47:
    x = x << 47;
    break;
  case 48:
    x = x << 48;
    break;
  case 49:
    x = x << 49;
    break;
  case 50:
    x = x << 50;
    break;
  case 51:
    x = x << 51;
    break;
  case 52:
    x = x << 52;
    break;
  case 53:
    x = x << 53;
    break;
  case 54:
    x = x << 54;
    break;
  case 55:
    x = x << 55;
    break;
  case 56:
    x = x << 56;
    break;
  case 57:
    x = x << 57;
    break;
  case 58:
    x = x << 58;
    break;
  case 59:
    x = x << 59;
    break;
  case 60:
    x = x << 60;
    break;
  case 61:
    x = x << 61;
    break;
  case 62:
    x = x << 62;
    break;
  case 63:
    x = x << 63;
    break;
#endif

  default:
    abort();
  }
  return x;
}

int main() {
  int i;

  for (i = 0; i < BITS; ++i) {
    unsigned long long y = variable_shift(data[0], i);
    if (y != data[i])
      abort();
  }
  for (i = 0; i < BITS; ++i) {
    unsigned long long y = constant_shift(data[0], i);
    if (y != data[i])
      abort();
  }

  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_data:[0-9]+]] data: array<u64, 64> [storage=static] [const] [align=16] = aggregate<array<u64, 64>, zero_fill=false>(index0 =
// DEFAULT-SAME: const<u64>(81985529216486895), index1 = const<u64>(163971058432973790), index2 = const<u64>(327942116865947580), index3 = const<u64>(655884233731895160), index4 = const<u64>(1311768467463790320), index5 = const<u64>(2623536934927580640), index6 = const<u64>(5247073869855161280), index7 = const<u64>(10494147739710322560), index8 = const<u64>(2541551405711093504), index9 = const<u64>(5083102811422187008), index10 = const<u64>(10166205622844374016), index11 = const<u64>(1885667171979196416), index12 = const<u64>(3771334343958392832), index13 = const<u64>(7542668687916785664), index14 = const<u64>(15085337375833571328), index15 = const<u64>(11723930677957591040), index16 = const<u64>(5001117282205630464), index17 = const<u64>(10002234564411260928), index18 = const<u64>(1557725055112970240), index19 = const<u64>(3115450110225940480), index20 = const<u64>(6230900220451880960), index21 = const<u64>(12461800440903761920), index22 = const<u64>(6476856808097972224), index23 = const<u64>(12953713616195944448), index24 = const<u64>(7460683158682337280), index25 = const<u64>(14921366317364674560), index26 = const<u64>(11395988561019797504), index27 = const<u64>(4345233048330043392), index28 = const<u64>(8690466096660086784), index29 = const<u64>(17380932193320173568), index30 = const<u64>(16315120312930795520), index31 = const<u64>(14183496552152039424), index32 = const<u64>(9920249030594527232), index33 = const<u64>(1393753987479502848), index34 = const<u64>(2787507974959005696), index35 = const<u64>(5575015949918011392), index36 = const<u64>(11150031899836022784), index37 = const<u64>(3853319725962493952), index38 = const<u64>(7706639451924987904), index39 = const<u64>(15413278903849975808), index40 = const<u64>(12379813733990400000), index41 = const<u64>(6312883394271248384), index42 = const<u64>(12625766788542496768), index43 = const<u64>(6804789503375441920), index44 = const<u64>(13609579006750883840), index45 = const<u64>(8772413939792216064), index46 = const<u64>(17544827879584432128), index47 = const<u64>(16642911685459312640), index48 = const<u64>(14839079297209073664), index49 = const<u64>(11231414520708595712), index50 = const<u64>(4016084967707639808), index51 = const<u64>(8032169935415279616), index52 = const<u64>(16064339870830559232), index53 = const<u64>(13681935667951566848), index54 = const<u64>(8917127262193582080), index55 = const<u64>(17834254524387164160), index56 = const<u64>(17221764975064776704), index57 = const<u64>(15996785876420001792), index58 = const<u64>(13546827679130451968), index59 = const<u64>(8646911284551352320), index60 = const<u64>(17293822569102704640), index61 = const<u64>(16140901064495857664), index62 = const<u64>(13835058055282163712), index63 = const<u64>(9223372036854775808)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_variable_shift:[0-9]+]] @variable_shift(%[[VALUE_x:[0-9]+]] x: u64, %[[VALUE_i:[0-9]+]] i: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_constant_shift:[0-9]+]] @constant_shift(%[[VALUE_x_2:[0-9]+]] x: u64, %[[VALUE_i_2:[0-9]+]] i: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_i_2]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(0):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(0)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(1):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(1)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(2)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(3):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(3)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(4):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(4)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(5):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(5)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(6):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(6)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(7):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(7)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(8):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(8)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(9):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(9)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(10):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(10)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(11):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(11)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(12):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(12)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(13):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(13)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(14):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(14)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(15):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(15)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(16):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(16)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(17):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(17)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(18):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(18)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(19):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(19)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(20):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(20)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(21):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(21)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(22):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(22)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(23):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(23)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(24):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(24)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(25):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(25)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(26):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(26)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(27):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(27)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(28):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(28)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(29):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(29)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(30):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(30)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(31):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(31)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(32):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(32)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(33):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(33)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(34):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(34)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(35):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(35)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(36):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(36)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(37):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(37)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(38):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(38)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(39):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(39)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(40):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(40)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(41):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(41)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(42):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(42)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(43):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(43)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(44):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(44)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(45):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(45)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(46):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(46)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(47):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(47)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(48):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(48)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(49):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(49)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(50):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(50)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(51):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(51)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(52):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(52)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(53):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(53)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(54):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(54)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(55):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(55)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(56):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(56)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(57):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(57)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(58):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(58)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(59):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(59)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(60):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(60)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(61):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(61)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(62):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(62)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(63):
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_x_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_x_2]]), const<i32>(63)));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_y:[0-9]+]] y: u64 [storage=automatic] = call<u64, signature=fn(u64, i32) -> u64>(%[[VALUE_variable_shift]], read<u64>(deref(ptr_offset<ptr<const u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<const u64>, length=Some(64)>(%[[VALUE_data]]), const<i32>(0)))), read<i32>(%[[VALUE_i_3]]));
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(%[[VALUE_y]]), read<u64>(deref(ptr_offset<ptr<const u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<const u64>, length=Some(64)>(%[[VALUE_data]]), read<i32>(%[[VALUE_i_3]])))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(64))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_y_2:[0-9]+]] y: u64 [storage=automatic] = call<u64, signature=fn(u64, i32) -> u64>(%[[VALUE_constant_shift]], read<u64>(deref(ptr_offset<ptr<const u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<const u64>, length=Some(64)>(%[[VALUE_data]]), const<i32>(0)))), read<i32>(%[[VALUE_i_3]]));
// DEFAULT-NEXT:                     if ne<u64>(read<u64>(%[[VALUE_y_2]]), read<u64>(deref(ptr_offset<ptr<const u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<const u64>, length=Some(64)>(%[[VALUE_data]]), read<i32>(%[[VALUE_i_3]])))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
