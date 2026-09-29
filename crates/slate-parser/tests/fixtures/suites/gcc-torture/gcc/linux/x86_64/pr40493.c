extern void abort(void);

typedef union i386_operand_type {
  struct {
    unsigned int reg8         : 1;
    unsigned int reg16        : 1;
    unsigned int reg32        : 1;
    unsigned int reg64        : 1;
    unsigned int floatreg     : 1;
    unsigned int regmmx       : 1;
    unsigned int regxmm       : 1;
    unsigned int regymm       : 1;
    unsigned int control      : 1;
    unsigned int debug        : 1;
    unsigned int test         : 1;
    unsigned int sreg2        : 1;
    unsigned int sreg3        : 1;
    unsigned int imm1         : 1;
    unsigned int imm8         : 1;
    unsigned int imm8s        : 1;
    unsigned int imm16        : 1;
    unsigned int imm32        : 1;
    unsigned int imm32s       : 1;
    unsigned int imm64        : 1;
    unsigned int disp8        : 1;
    unsigned int disp16       : 1;
    unsigned int disp32       : 1;
    unsigned int disp32s      : 1;
    unsigned int disp64       : 1;
    unsigned int acc          : 1;
    unsigned int floatacc     : 1;
    unsigned int baseindex    : 1;
    unsigned int inoutportreg : 1;
    unsigned int shiftcount   : 1;
    unsigned int jumpabsolute : 1;
    unsigned int esseg        : 1;
    unsigned int regmem       : 1;
    unsigned int mem          : 1;
    unsigned int byte         : 1;
    unsigned int word         : 1;
    unsigned int dword        : 1;
    unsigned int fword        : 1;
    unsigned int qword        : 1;
    unsigned int tbyte        : 1;
    unsigned int xmmword      : 1;
    unsigned int ymmword      : 1;
    unsigned int unspecified  : 1;
    unsigned int anysize      : 1;
  } bitfield;
  unsigned int array[2];
} i386_operand_type;

unsigned int x00, x01, y00, y01;

int main(int argc, char *argv[]) {
  i386_operand_type a, b, c, d;

  a.bitfield.reg16 = 1;
  a.bitfield.imm16 = 0;
  a.array[1]       = 22;

  b   = a;
  x00 = b.array[0];
  x01 = b.array[1];

  c   = b;
  y00 = c.array[0];
  y01 = c.array[1];

  d = c;
  if (d.bitfield.reg16 != 1)
    abort();
  if (d.bitfield.imm16 != 0)
    abort();
  if (d.array[1] != 22)
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
// DEFAULT-NEXT:     type @type[[TYPE_i386_operand_type:[0-9]+]] i386_operand_type = union {
// DEFAULT-NEXT:         field0 bitfield: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 array: array<u32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 reg8: u32 : 1;
// DEFAULT-NEXT:         field1 reg16: u32 : 1;
// DEFAULT-NEXT:         field2 reg32: u32 : 1;
// DEFAULT-NEXT:         field3 reg64: u32 : 1;
// DEFAULT-NEXT:         field4 floatreg: u32 : 1;
// DEFAULT-NEXT:         field5 regmmx: u32 : 1;
// DEFAULT-NEXT:         field6 regxmm: u32 : 1;
// DEFAULT-NEXT:         field7 regymm: u32 : 1;
// DEFAULT-NEXT:         field8 control: u32 : 1;
// DEFAULT-NEXT:         field9 debug: u32 : 1;
// DEFAULT-NEXT:         field10 test: u32 : 1;
// DEFAULT-NEXT:         field11 sreg2: u32 : 1;
// DEFAULT-NEXT:         field12 sreg3: u32 : 1;
// DEFAULT-NEXT:         field13 imm1: u32 : 1;
// DEFAULT-NEXT:         field14 imm8: u32 : 1;
// DEFAULT-NEXT:         field15 imm8s: u32 : 1;
// DEFAULT-NEXT:         field16 imm16: u32 : 1;
// DEFAULT-NEXT:         field17 imm32: u32 : 1;
// DEFAULT-NEXT:         field18 imm32s: u32 : 1;
// DEFAULT-NEXT:         field19 imm64: u32 : 1;
// DEFAULT-NEXT:         field20 disp8: u32 : 1;
// DEFAULT-NEXT:         field21 disp16: u32 : 1;
// DEFAULT-NEXT:         field22 disp32: u32 : 1;
// DEFAULT-NEXT:         field23 disp32s: u32 : 1;
// DEFAULT-NEXT:         field24 disp64: u32 : 1;
// DEFAULT-NEXT:         field25 acc: u32 : 1;
// DEFAULT-NEXT:         field26 floatacc: u32 : 1;
// DEFAULT-NEXT:         field27 baseindex: u32 : 1;
// DEFAULT-NEXT:         field28 inoutportreg: u32 : 1;
// DEFAULT-NEXT:         field29 shiftcount: u32 : 1;
// DEFAULT-NEXT:         field30 jumpabsolute: u32 : 1;
// DEFAULT-NEXT:         field31 esseg: u32 : 1;
// DEFAULT-NEXT:         field32 regmem: u32 : 1;
// DEFAULT-NEXT:         field33 mem: u32 : 1;
// DEFAULT-NEXT:         field34 byte: u32 : 1;
// DEFAULT-NEXT:         field35 word: u32 : 1;
// DEFAULT-NEXT:         field36 dword: u32 : 1;
// DEFAULT-NEXT:         field37 fword: u32 : 1;
// DEFAULT-NEXT:         field38 qword: u32 : 1;
// DEFAULT-NEXT:         field39 tbyte: u32 : 1;
// DEFAULT-NEXT:         field40 xmmword: u32 : 1;
// DEFAULT-NEXT:         field41 ymmword: u32 : 1;
// DEFAULT-NEXT:         field42 unspecified: u32 : 1;
// DEFAULT-NEXT:         field43 anysize: u32 : 1;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5], bit_offsets=[Some(0), Some(1), Some(2), Some(3), Some(4), Some(5), Some(6), Some(7), Some(8), Some(9), Some(10), Some(11), Some(12), Some(13), Some(14), Some(15), Some(16), Some(17), Some(18), Some(19), Some(20), Some(21), Some(22), Some(23), Some(24), Some(25), Some(26), Some(27), Some(28), Some(29), Some(30), Some(31), Some(32), Some(33), Some(34), Some(35), Some(36), Some(37), Some(38), Some(39), Some(40), Some(41), Some(42), Some(43)], bit_units=[(0, 6)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_i386_operand_type_2:[0-9]+]] i386_operand_type = @type[[TYPE_i386_operand_type]];
// DEFAULT-NEXT:     global %[[VALUE_x00:[0-9]+]] x00: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x01:[0-9]+]] x01: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y00:[0-9]+]] y00: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y01:[0-9]+]] y01: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_i386_operand_type]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_i386_operand_type]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_i386_operand_type]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_i386_operand_type]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..6, bits=1..2>(field0(%[[VALUE_a]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield16<unit=0, bytes=0..6, bits=16..17>(field0(%[[VALUE_a]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_a]])), const<i32>(1))), reinterpret<u32, reason=assign, fits=always>(const<i32>(22)));
// DEFAULT-NEXT:         write<@type[[TYPE_i386_operand_type]]>(%[[VALUE_b]], copy<@type[[TYPE_i386_operand_type]], reason=assign>(read<@type[[TYPE_i386_operand_type]]>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x00]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_b]])), const<i32>(0)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x01]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_b]])), const<i32>(1)))));
// DEFAULT-NEXT:         write<@type[[TYPE_i386_operand_type]]>(%[[VALUE_c]], copy<@type[[TYPE_i386_operand_type]], reason=assign>(read<@type[[TYPE_i386_operand_type]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_y00]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_c]])), const<i32>(0)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_y01]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_c]])), const<i32>(1)))));
// DEFAULT-NEXT:         write<@type[[TYPE_i386_operand_type]]>(%[[VALUE_d]], copy<@type[[TYPE_i386_operand_type]], reason=assign>(read<@type[[TYPE_i386_operand_type]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..6, bits=1..2>(field0(%[[VALUE_d]])))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield16<unit=0, bytes=0..6, bits=16..17>(field0(%[[VALUE_d]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_d]])), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
