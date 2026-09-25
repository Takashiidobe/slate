// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

/* This was cut down from reload1.c in May 2001, was observed to cause
   a bootstrap failure for powerpc-apple-darwin1.3.

   Copyright (C) 2001  Free Software Foundation.  */

enum insn_code
{
  CODE_FOR_extendqidi2 = 3,
  CODE_FOR_nothing = 870
};

struct rtx_def;

enum machine_mode
{
  VOIDmode,
  MAX_MACHINE_MODE
};

typedef unsigned long long HARD_REG_ELT_TYPE;
typedef HARD_REG_ELT_TYPE HARD_REG_SET[((77 + (8 * 8) - 1) / (8 * 8))];

enum rtx_code
{
  UNKNOWN,
  NIL,
  REG,
  LAST_AND_UNUSED_RTX_CODE = 256
};

typedef struct
{
  unsigned min_align:8;
  unsigned base_after_vec:1;
  unsigned min_after_vec:1;
  unsigned max_after_vec:1;
  unsigned min_after_base:1;
  unsigned max_after_base:1;
  unsigned offset_unsigned:1;
  unsigned:2;
  unsigned scale:8;
}
addr_diff_vec_flags;
typedef union rtunion_def
{
  long long rtwint;
  int rtint;
  unsigned int rtuint;
  const char *rtstr;
  struct rtx_def *rtx;
  struct rtvec_def *rtvec;
  enum machine_mode rttype;
  addr_diff_vec_flags rt_addr_diff_vec_flags;
  struct cselib_val_struct *rt_cselib;
  struct bitmap_head_def *rtbit;
  union tree_node *rttree;
  struct basic_block_def *bb;
}
rtunion;
typedef struct rtx_def
{
  enum rtx_code code:16;
  enum machine_mode mode:8;
  unsigned int jump:1;
  unsigned int call:1;
  unsigned int unchanging:1;
  unsigned int volatil:1;
  unsigned int in_struct:1;
  unsigned int used:1;
  unsigned integrated:1;
  unsigned frame_related:1;
  rtunion fld[1];
}
 *rtx;

enum reload_type
{
  RELOAD_FOR_INPUT, RELOAD_FOR_OUTPUT, RELOAD_FOR_INSN,
  RELOAD_FOR_INPUT_ADDRESS, RELOAD_FOR_INPADDR_ADDRESS,
  RELOAD_FOR_OUTPUT_ADDRESS, RELOAD_FOR_OUTADDR_ADDRESS,
  RELOAD_FOR_OPERAND_ADDRESS, RELOAD_FOR_OPADDR_ADDR,
  RELOAD_OTHER, RELOAD_FOR_OTHER_ADDRESS
};

struct reload
{
  rtx in;
  rtx out;
  //  enum reg_class class;
  enum machine_mode inmode;
  enum machine_mode outmode;
  enum machine_mode mode;
  unsigned int nregs;
  int inc;
  rtx in_reg;
  rtx out_reg;
  int regno;
  rtx reg_rtx;
  int opnum;
  int secondary_in_reload;
  int secondary_out_reload;
  enum insn_code secondary_in_icode;
  enum insn_code secondary_out_icode;
  enum reload_type when_needed;
  unsigned int optional:1;
  unsigned int nocombine:1;
  unsigned int secondary_p:1;
  unsigned int nongroup:1;
};

struct insn_chain
{
  rtx insn;
};

extern int n_reloads;
static short reload_order[(2 * 10 * (2 + 1))];
int reload_spill_index[(2 * 10 * (2 + 1))];
extern struct reload rld[(2 * 10 * (2 + 1))];
static rtx *reg_last_reload_reg;
static HARD_REG_SET reg_reloaded_valid;
static HARD_REG_SET reg_reloaded_dead;
static HARD_REG_SET reg_reloaded_died;
static HARD_REG_SET reg_is_output_reload;
extern const unsigned int mode_size[];
extern int target_flags;

static void
emit_reload_insns (chain)
     struct insn_chain *chain;
{
  rtx insn = chain->insn;
  register int j;
  rtx following_insn = (((insn)->fld[2]).rtx);
  rtx before_insn = (((insn)->fld[1]).rtx);

  for (j = 0; j < n_reloads; j++)
    {
      register int r = reload_order[j];
      register int i = reload_spill_index[r];

	{
	  rtx out = (((enum rtx_code) (rld[r].out)->code) == REG ? rld[r].out : rld[r].out_reg);
	  register int nregno = (((out)->fld[0]).rtuint);

	  if (nregno >= 77)
	    {
	      rtx src_reg, store_insn = (rtx) 0;

	      reg_last_reload_reg[nregno] = 0;
	      if (src_reg && ((enum rtx_code) (src_reg)->code) == REG && (((src_reg)->fld[0]).rtuint) < 77)
		{
		  int src_regno = (((src_reg)->fld[0]).rtuint);
		  int nr =
		    (((src_regno) >= 32
		      && (src_regno) <=
		      63) ? (((mode_size[(int) (rld[r].mode)]) + 8 -
			      1) / 8) : (((mode_size[(int) (rld[r].mode)]) +
					  (!(target_flags & 0x00000020) ? 4 :
					   8) - 1) / (!(target_flags & 0x00000020) ? 4 : 8)));
		  rtx note = 0;

		  while (nr-- > 0)
		    {
		      ((reg_reloaded_dead)
		       [(src_regno + nr) / ((unsigned) (8 * 8))] &=
		       ~(((HARD_REG_ELT_TYPE) (1)) << ((src_regno + nr) % ((unsigned) (8 * 8)))));
		      ((reg_reloaded_valid)
		       [(src_regno + nr) / ((unsigned) (8 * 8))] |=
		       ((HARD_REG_ELT_TYPE) (1)) << ((src_regno + nr) % ((unsigned) (8 * 8))));
		      ((reg_is_output_reload)
		       [(src_regno + nr) / ((unsigned) (8 * 8))] |=
		       ((HARD_REG_ELT_TYPE) (1)) << ((src_regno + nr) % ((unsigned) (8 * 8))));
		      if (note)
			((reg_reloaded_died)
			 [(src_regno) / ((unsigned) (8 * 8))] |=
			 ((HARD_REG_ELT_TYPE) (1)) << ((src_regno) % ((unsigned) (8 * 8))));
		      else
			((reg_reloaded_died)
			 [(src_regno) / ((unsigned) (8 * 8))] &=
			 ~(((HARD_REG_ELT_TYPE) (1)) << ((src_regno) % ((unsigned) (8 * 8)))));
		    }
		  reg_last_reload_reg[nregno] = src_reg;
		}
	    }
	  else
	    {
	      int num_regs =
		(((nregno) >= 32
		  && (nregno) <=
		  63)
		 ? (((mode_size
		      [(int) (((enum machine_mode) (rld[r].out)->mode))]) +
		     8 -
		     1) /
		    8)
		 : (((mode_size
		      [(int) (((enum machine_mode) (rld[r].out)->mode))]) +
		     (!(target_flags & 0x00000020) ? 4 : 8) - 1) / (!(target_flags & 0x00000020) ? 4 : 8)));
	      while (num_regs-- > 0)
		reg_last_reload_reg[nregno + num_regs] = 0;
	    }
	}
    }
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
// DEFAULT-NEXT:     type @type0 insn_code = enum : u32 {
// DEFAULT-NEXT:         %0 CODE_FOR_extendqidi2 = const<i32>(3);
// DEFAULT-NEXT:         %1 CODE_FOR_nothing = const<i32>(870);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 rtx_def = struct {
// DEFAULT-NEXT:         field0 code: @type5 : 16;
// DEFAULT-NEXT:         field1 mode: @type2 : 8;
// DEFAULT-NEXT:         field2 jump: u32 : 1;
// DEFAULT-NEXT:         field3 call: u32 : 1;
// DEFAULT-NEXT:         field4 unchanging: u32 : 1;
// DEFAULT-NEXT:         field5 volatil: u32 : 1;
// DEFAULT-NEXT:         field6 in_struct: u32 : 1;
// DEFAULT-NEXT:         field7 used: u32 : 1;
// DEFAULT-NEXT:         field8 integrated: u32 : 1;
// DEFAULT-NEXT:         field9 frame_related: u32 : 1;
// DEFAULT-NEXT:         field10 fld: array<@type8, 1>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 8], bit_offsets=[Some(0), Some(16), Some(24), Some(25), Some(26), Some(27), Some(28), Some(29), Some(30), Some(31), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type2 machine_mode = enum : u32 {
// DEFAULT-NEXT:         %0 VOIDmode = const<i32>(0);
// DEFAULT-NEXT:         %1 MAX_MACHINE_MODE = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 HARD_REG_ELT_TYPE = u64;
// DEFAULT-NEXT:     type @type4 HARD_REG_SET = array<u64, 2>;
// DEFAULT-NEXT:     type @type5 rtx_code = enum : u32 {
// DEFAULT-NEXT:         %0 UNKNOWN = const<i32>(0);
// DEFAULT-NEXT:         %1 NIL = const<i32>(1);
// DEFAULT-NEXT:         %2 REG = const<i32>(2);
// DEFAULT-NEXT:         %3 LAST_AND_UNUSED_RTX_CODE = const<i32>(256);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 min_align: u32 : 8;
// DEFAULT-NEXT:         field1 base_after_vec: u32 : 1;
// DEFAULT-NEXT:         field2 min_after_vec: u32 : 1;
// DEFAULT-NEXT:         field3 max_after_vec: u32 : 1;
// DEFAULT-NEXT:         field4 min_after_base: u32 : 1;
// DEFAULT-NEXT:         field5 max_after_base: u32 : 1;
// DEFAULT-NEXT:         field6 offset_unsigned: u32 : 1;
// DEFAULT-NEXT:         field7 <anonymous>: u32 : 2;
// DEFAULT-NEXT:         field8 scale: u32 : 8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1, 1, 1, 1, 1, 1, 2], bit_offsets=[Some(0), Some(8), Some(9), Some(10), Some(11), Some(12), Some(13), Some(14), Some(16)], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type7 addr_diff_vec_flags = @type6;
// DEFAULT-NEXT:     type @type8 rtunion_def = union {
// DEFAULT-NEXT:         field0 rtwint: i64;
// DEFAULT-NEXT:         field1 rtint: i32;
// DEFAULT-NEXT:         field2 rtuint: u32;
// DEFAULT-NEXT:         field3 rtstr: ptr<const i8>;
// DEFAULT-NEXT:         field4 rtx: ptr<@type1>;
// DEFAULT-NEXT:         field5 rtvec: ptr<@type9>;
// DEFAULT-NEXT:         field6 rttype: @type2;
// DEFAULT-NEXT:         field7 rt_addr_diff_vec_flags: @type6;
// DEFAULT-NEXT:         field8 rt_cselib: ptr<@type10>;
// DEFAULT-NEXT:         field9 rtbit: ptr<@type11>;
// DEFAULT-NEXT:         field10 rttree: ptr<@type12>;
// DEFAULT-NEXT:         field11 bb: ptr<@type13>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type9 rtvec_def = struct incomplete;
// DEFAULT-NEXT:     type @type10 cselib_val_struct = struct incomplete;
// DEFAULT-NEXT:     type @type11 bitmap_head_def = struct incomplete;
// DEFAULT-NEXT:     type @type12 tree_node = union incomplete;
// DEFAULT-NEXT:     type @type13 basic_block_def = struct incomplete;
// DEFAULT-NEXT:     type @type14 rtunion = @type8;
// DEFAULT-NEXT:     type @type15 rtx = ptr<@type1>;
// DEFAULT-NEXT:     type @type16 reload_type = enum : u32 {
// DEFAULT-NEXT:         %0 RELOAD_FOR_INPUT = const<i32>(0);
// DEFAULT-NEXT:         %1 RELOAD_FOR_OUTPUT = const<i32>(1);
// DEFAULT-NEXT:         %2 RELOAD_FOR_INSN = const<i32>(2);
// DEFAULT-NEXT:         %3 RELOAD_FOR_INPUT_ADDRESS = const<i32>(3);
// DEFAULT-NEXT:         %4 RELOAD_FOR_INPADDR_ADDRESS = const<i32>(4);
// DEFAULT-NEXT:         %5 RELOAD_FOR_OUTPUT_ADDRESS = const<i32>(5);
// DEFAULT-NEXT:         %6 RELOAD_FOR_OUTADDR_ADDRESS = const<i32>(6);
// DEFAULT-NEXT:         %7 RELOAD_FOR_OPERAND_ADDRESS = const<i32>(7);
// DEFAULT-NEXT:         %8 RELOAD_FOR_OPADDR_ADDR = const<i32>(8);
// DEFAULT-NEXT:         %9 RELOAD_OTHER = const<i32>(9);
// DEFAULT-NEXT:         %10 RELOAD_FOR_OTHER_ADDRESS = const<i32>(10);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type17 reload = struct {
// DEFAULT-NEXT:         field0 in: ptr<@type1>;
// DEFAULT-NEXT:         field1 out: ptr<@type1>;
// DEFAULT-NEXT:         field2 inmode: @type2;
// DEFAULT-NEXT:         field3 outmode: @type2;
// DEFAULT-NEXT:         field4 mode: @type2;
// DEFAULT-NEXT:         field5 nregs: u32;
// DEFAULT-NEXT:         field6 inc: i32;
// DEFAULT-NEXT:         field7 in_reg: ptr<@type1>;
// DEFAULT-NEXT:         field8 out_reg: ptr<@type1>;
// DEFAULT-NEXT:         field9 regno: i32;
// DEFAULT-NEXT:         field10 reg_rtx: ptr<@type1>;
// DEFAULT-NEXT:         field11 opnum: i32;
// DEFAULT-NEXT:         field12 secondary_in_reload: i32;
// DEFAULT-NEXT:         field13 secondary_out_reload: i32;
// DEFAULT-NEXT:         field14 secondary_in_icode: @type0;
// DEFAULT-NEXT:         field15 secondary_out_icode: @type0;
// DEFAULT-NEXT:         field16 when_needed: @type16;
// DEFAULT-NEXT:         field17 optional: u32 : 1;
// DEFAULT-NEXT:         field18 nocombine: u32 : 1;
// DEFAULT-NEXT:         field19 secondary_p: u32 : 1;
// DEFAULT-NEXT:         field20 nongroup: u32 : 1;
// DEFAULT-NEXT:     } [size=104, align=8, offsets=[0, 8, 16, 20, 24, 28, 32, 40, 48, 56, 64, 72, 76, 80, 84, 88, 92, 96, 96, 96, 96], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(768), Some(769), Some(770), Some(771)], bit_units=[(96, 1)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type18 insn_chain = struct {
// DEFAULT-NEXT:         field0 insn: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     extern %38 n_reloads: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %39 reload_order: array<i16, 60> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %40 reload_spill_index: array<i32, 60> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %41 rld: array<@type17, 60> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %42 reg_last_reload_reg: ptr<ptr<@type1>> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %43 reg_reloaded_valid: array<u64, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %44 reg_reloaded_dead: array<u64, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %45 reg_reloaded_died: array<u64, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %46 reg_is_output_reload: array<u64, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %47 mode_size: array<u32, incomplete> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %48 target_flags: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %49 @emit_reload_insns(%50 chain: ptr<@type18>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %51 insn: ptr<@type1> [storage=automatic] = read<ptr<@type1>>(field0(deref(read<ptr<@type18>>(%50))));
// DEFAULT-NEXT:         let %52 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %53 following_insn: ptr<@type1> [storage=automatic] = read<ptr<@type1>>(field4(deref(ptr_offset<ptr<@type8>, subtract=false, element=@type8, overflow=ub>(array_decay<ptr<@type8>, length=Some(1)>(field10(deref(read<ptr<@type1>>(%51)))), const<i32>(2)))));
// DEFAULT-NEXT:         let %54 before_insn: ptr<@type1> [storage=automatic] = read<ptr<@type1>>(field4(deref(ptr_offset<ptr<@type8>, subtract=false, element=@type8, overflow=ub>(array_decay<ptr<@type8>, length=Some(1)>(field10(deref(read<ptr<@type1>>(%51)))), const<i32>(1)))));
// DEFAULT-NEXT:         for %65
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%52, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%52), read<i32>(%38))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %68: i32 [synthetic] = read<i32>(%52);
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%52, read<i32>(%69));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %55 r: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(60)>(%39), read<i32>(%52)))));
// DEFAULT-NEXT:                     let %56 i: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(60)>(%40), read<i32>(%55))));
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %57 out: ptr<@type1> [storage=automatic] = conditional<ptr<@type1>>(eq<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type5, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type5>(bitfield0<unit=0, bytes=0..4, bits=0..16>(deref(read<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55))))))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), read<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55))))), read<ptr<@type1>>(field8(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55))))));
// DEFAULT-NEXT:                         let %58 nregno: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(field2(deref(ptr_offset<ptr<@type8>, subtract=false, element=@type8, overflow=ub>(array_decay<ptr<@type8>, length=Some(1)>(field10(deref(read<ptr<@type1>>(%57)))), const<i32>(0))))));
// DEFAULT-NEXT:                         if ge<i32>(read<i32>(%58), const<i32>(77))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %59 src_reg: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:                                 let %60 store_insn: ptr<@type1> [storage=automatic] = null<ptr<@type1>>;
// DEFAULT-NEXT:                                 write<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(%42), read<i32>(%58))), null<ptr<@type1>>);
// DEFAULT-NEXT:                                 if logical_and<bool>(logical_and<bool>(ne<ptr<@type1>>(read<ptr<@type1>>(%59), null<ptr<@type1>>), eq<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type5, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type5>(bitfield0<unit=0, bytes=0..4, bits=0..16>(deref(read<ptr<@type1>>(%59))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))), lt<u32>(read<u32>(field2(deref(ptr_offset<ptr<@type8>, subtract=false, element=@type8, overflow=ub>(array_decay<ptr<@type8>, length=Some(1)>(field10(deref(read<ptr<@type1>>(%59)))), const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(77))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %61 src_regno: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(read<u32>(field2(deref(ptr_offset<ptr<@type8>, subtract=false, element=@type8, overflow=ub>(array_decay<ptr<@type8>, length=Some(1)>(field10(deref(read<ptr<@type1>>(%59)))), const<i32>(0))))));
// DEFAULT-NEXT:                                         let %62 nr: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(conditional<u32>(logical_and<bool>(ge<i32>(read<i32>(%61), const<i32>(32)), le<i32>(read<i32>(%61), const<i32>(63))), div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=None>(%47), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type2>(field4(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55)))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=None>(%47), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type2>(field4(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55)))))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(not<bool>(ne<i32>(and<i32>(read<i32>(%48), const<i32>(32)), const<i32>(0))), const<i32>(4), const<i32>(8)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(not<bool>(ne<i32>(and<i32>(read<i32>(%48), const<i32>(32)), const<i32>(0))), const<i32>(4), const<i32>(8))))));
// DEFAULT-NEXT:                                         let %63 note: ptr<@type1> [storage=automatic] = null<ptr<@type1>>;
// DEFAULT-NEXT:                                         while %66 {
// DEFAULT-NEXT:                                             let %70: i32 [synthetic] = read<i32>(%62);
// DEFAULT-NEXT:                                             let %71: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%70), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%62, read<i32>(%71));
// DEFAULT-NEXT:                                             yield gt<i32>(read<i32>(%70), const<i32>(0));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 let %72: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%44), div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))));
// DEFAULT-NEXT:                                                 let %73: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%72)));
// DEFAULT-NEXT:                                                 let %74: u64 [synthetic] = and<u64>(read<u64>(%73), not<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))));
// DEFAULT-NEXT:                                                 write<u64>(deref(read<ptr<u64>>(%72)), read<u64>(%74));
// DEFAULT-NEXT:                                                 let %75: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%43), div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))));
// DEFAULT-NEXT:                                                 let %76: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%75)));
// DEFAULT-NEXT:                                                 let %77: u64 [synthetic] = or<u64>(read<u64>(%76), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))));
// DEFAULT-NEXT:                                                 write<u64>(deref(read<ptr<u64>>(%75)), read<u64>(%77));
// DEFAULT-NEXT:                                                 let %78: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%46), div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))));
// DEFAULT-NEXT:                                                 let %79: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%78)));
// DEFAULT-NEXT:                                                 let %80: u64 [synthetic] = or<u64>(read<u64>(%79), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62))), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))));
// DEFAULT-NEXT:                                                 write<u64>(deref(read<ptr<u64>>(%78)), read<u64>(%80));
// DEFAULT-NEXT:                                                 if ne<ptr<@type1>>(read<ptr<@type1>>(%63), null<ptr<@type1>>)
// DEFAULT-NEXT:                                                     let %81: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%45), div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%61)), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))));
// DEFAULT-NEXT:                                                     let %82: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%81)));
// DEFAULT-NEXT:                                                     let %83: u64 [synthetic] = or<u64>(read<u64>(%82), shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%61)), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))));
// DEFAULT-NEXT:                                                     write<u64>(deref(read<ptr<u64>>(%81)), read<u64>(%83));
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     let %84: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%45), div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%61)), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))));
// DEFAULT-NEXT:                                                     let %85: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%84)));
// DEFAULT-NEXT:                                                     let %86: u64 [synthetic] = and<u64>(read<u64>(%85), not<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%61)), reinterpret<u32, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))));
// DEFAULT-NEXT:                                                     write<u64>(deref(read<ptr<u64>>(%84)), read<u64>(%86));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         write<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(%42), read<i32>(%58))), read<ptr<@type1>>(%59));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %64 num_regs: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(conditional<u32>(logical_and<bool>(ge<i32>(read<i32>(%58), const<i32>(32)), le<i32>(read<i32>(%58), const<i32>(63))), div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=None>(%47), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type2, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type2>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55))))))))))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=None>(%47), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type2, reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type2>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type17>, subtract=false, element=@type17, overflow=ub>(array_decay<ptr<@type17>, length=Some(60)>(%41), read<i32>(%55))))))))))))))))), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(not<bool>(ne<i32>(and<i32>(read<i32>(%48), const<i32>(32)), const<i32>(0))), const<i32>(4), const<i32>(8)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(not<bool>(ne<i32>(and<i32>(read<i32>(%48), const<i32>(32)), const<i32>(0))), const<i32>(4), const<i32>(8))))));
// DEFAULT-NEXT:                                 while %67 {
// DEFAULT-NEXT:                                     let %87: i32 [synthetic] = read<i32>(%64);
// DEFAULT-NEXT:                                     let %88: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%87), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%64, read<i32>(%88));
// DEFAULT-NEXT:                                     yield gt<i32>(read<i32>(%87), const<i32>(0));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                     write<ptr<@type1>>(deref(ptr_offset<ptr<ptr<@type1>>, subtract=false, element=ptr<@type1>, overflow=ub>(read<ptr<ptr<@type1>>>(%42), add<i32, overflow=ub>(read<i32>(%58), read<i32>(%64)))), null<ptr<@type1>>);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
