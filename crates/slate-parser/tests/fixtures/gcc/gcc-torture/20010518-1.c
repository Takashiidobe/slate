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
// DEFAULT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG0:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Enum,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "insn_code",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Enum {
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "CODE_FOR_extendqidi2",
// DEFAULT-NEXT:                       value: Some(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 3,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "3",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "CODE_FOR_nothing",
// DEFAULT-NEXT:                       value: Some(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 870,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "870",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG1:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Enum,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "machine_mode",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Enum {
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "VOIDmode",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "MAX_MACHINE_MODE",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG2:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Enum,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "rtx_code",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Enum {
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "UNKNOWN",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "NIL",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "REG",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "LAST_AND_UNUSED_RTX_CODE",
// DEFAULT-NEXT:                       value: Some(
// DEFAULT-NEXT:                           IntegerLiteral(
// DEFAULT-NEXT:                               IntegerLiteral {
// DEFAULT-NEXT:                                   value: 256,
// DEFAULT-NEXT:                                   radix: Decimal,
// DEFAULT-NEXT:                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                       unsigned: false,
// DEFAULT-NEXT:                                       size: None,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   spelling: "256",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG3:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: None,
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "min_align",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 8,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "8",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "base_after_vec",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "min_after_vec",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "max_after_vec",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "min_after_base",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "max_after_base",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "offset_unsigned",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Abstract,
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 2,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "2",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "scale",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 8,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "8",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG4:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Union,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "rtunion_def",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: LongLong,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "rtwint",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "rtint",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "rtuint",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Char {
// DEFAULT-NEXT:                                   signed: None,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           qualifiers: Qualifiers {
// DEFAULT-NEXT:                               is_const: true,
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rtstr",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "rtx_def",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rtx",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "rtvec_def",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rtvec",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "machine_mode",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "rttype",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "addr_diff_vec_flags",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "rt_addr_diff_vec_flags",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "cselib_val_struct",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rt_cselib",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "bitmap_head_def",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rtbit",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Union,
// DEFAULT-NEXT:                                   name: "tree_node",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "rttree",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Struct,
// DEFAULT-NEXT:                                   name: "basic_block_def",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Pointer {
// DEFAULT-NEXT:                                   qualifiers: Qualifiers,
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "bb",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG5:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "rtx_def",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "rtx_code",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "code",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 16,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "16",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "machine_mode",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "mode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 8,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "8",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "jump",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "call",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "unchanging",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "volatil",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "in_struct",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "used",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "integrated",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "frame_related",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtunion",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Array {
// DEFAULT-NEXT:                                   inner: Name(
// DEFAULT-NEXT:                                       "fld",
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   size: Expression(
// DEFAULT-NEXT:                                       IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 1,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "1",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG6:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Enum,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "reload_type",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Enum {
// DEFAULT-NEXT:           enumerators: [
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_INPUT",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OUTPUT",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_INSN",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_INPUT_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_INPADDR_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OUTPUT_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OUTADDR_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OPERAND_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OPADDR_ADDR",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_OTHER",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Enumerator(
// DEFAULT-NEXT:                   Enumerator {
// DEFAULT-NEXT:                       name: "RELOAD_FOR_OTHER_ADDRESS",
// DEFAULT-NEXT:                       value: None,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG7:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "reload",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "in",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "out",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "machine_mode",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "inmode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "machine_mode",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "outmode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "machine_mode",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "mode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "nregs",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "inc",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "in_reg",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "out_reg",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "regno",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "reg_rtx",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "opnum",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "secondary_in_reload",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "secondary_out_reload",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "insn_code",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "secondary_in_icode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "insn_code",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "secondary_out_icode",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Tag(
// DEFAULT-NEXT:                               Reference {
// DEFAULT-NEXT:                                   kind: Enum,
// DEFAULT-NEXT:                                   name: "reload_type",
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "when_needed",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "optional",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "nocombine",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "secondary_p",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: false,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "nongroup",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               bit_width: Some(
// DEFAULT-NEXT:                                   IntegerLiteral(
// DEFAULT-NEXT:                                       IntegerLiteral {
// DEFAULT-NEXT:                                           value: 1,
// DEFAULT-NEXT:                                           radix: Decimal,
// DEFAULT-NEXT:                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                               unsigned: false,
// DEFAULT-NEXT:                                               size: None,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           spelling: "1",
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: tag[{{[0-9]+}}]: TagDefinition {
// DEFAULT-NEXT:       id: TagId(
// DEFAULT-NEXT:           [[#TAG8:]],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       kind: Struct,
// DEFAULT-NEXT:       name: Some(
// DEFAULT-NEXT:           "insn_chain",
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:       body: Record(
// DEFAULT-NEXT:           [
// DEFAULT-NEXT:               Field(
// DEFAULT-NEXT:                   FieldDecl {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           FieldDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "insn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       ),
// DEFAULT-NEXT:   }
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG0]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Reference {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: "rtx_def",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG1]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: LongLong,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "HARD_REG_SET",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Div,
// DEFAULT-NEXT:                                   left: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Sub,
// DEFAULT-NEXT:                                           left: Binary {
// DEFAULT-NEXT:                                               op: Add,
// DEFAULT-NEXT:                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                       value: 77,
// DEFAULT-NEXT:                                                       radix: Decimal,
// DEFAULT-NEXT:                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                           unsigned: false,
// DEFAULT-NEXT:                                                           size: None,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       spelling: "77",
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               right: Paren(
// DEFAULT-NEXT:                                                   Binary {
// DEFAULT-NEXT:                                                       op: Mul,
// DEFAULT-NEXT:                                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 8,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "8",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 8,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "8",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Mul,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 8,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "8",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 8,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "8",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG2]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG3]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "addr_diff_vec_flags",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG4]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "rtunion",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG5]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Typedef,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "rtx",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG6]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG7]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Definition(
// DEFAULT-NEXT:                       TagId(
// DEFAULT-NEXT:                           [[#TAG8]],
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "n_reloads",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Short,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "reload_order",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 10,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "10",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "reload_spill_index",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 10,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "10",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Tag(
// DEFAULT-NEXT:                   Reference {
// DEFAULT-NEXT:                       kind: Struct,
// DEFAULT-NEXT:                       name: "reload",
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "rld",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Expression(
// DEFAULT-NEXT:                           Paren(
// DEFAULT-NEXT:                               Binary {
// DEFAULT-NEXT:                                   op: Mul,
// DEFAULT-NEXT:                                   left: Binary {
// DEFAULT-NEXT:                                       op: Mul,
// DEFAULT-NEXT:                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 2,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "2",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                           IntegerLiteral {
// DEFAULT-NEXT:                                               value: 10,
// DEFAULT-NEXT:                                               radix: Decimal,
// DEFAULT-NEXT:                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                   unsigned: false,
// DEFAULT-NEXT:                                                   size: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                               spelling: "10",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   right: Paren(
// DEFAULT-NEXT:                                       Binary {
// DEFAULT-NEXT:                                           op: Add,
// DEFAULT-NEXT:                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 2,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "2",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 1,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "1",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "rtx",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Pointer {
// DEFAULT-NEXT:                       qualifiers: Qualifiers,
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "reg_last_reload_reg",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "HARD_REG_SET",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "reg_reloaded_valid",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "HARD_REG_SET",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "reg_reloaded_dead",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "HARD_REG_SET",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "reg_reloaded_died",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Named(
// DEFAULT-NEXT:                   "HARD_REG_SET",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "reg_is_output_reload",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: false,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               qualifiers: Qualifiers {
// DEFAULT-NEXT:                   is_const: true,
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Array {
// DEFAULT-NEXT:                       inner: Name(
// DEFAULT-NEXT:                           "mode_size",
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                       size: Unspecified,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Declaration(
// DEFAULT-NEXT:       Declaration {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Integer(
// DEFAULT-NEXT:                   Ranked {
// DEFAULT-NEXT:                       rank: Int,
// DEFAULT-NEXT:                       signed: true,
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               storage: Extern,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarators: [
// DEFAULT-NEXT:               InitDeclaratorKind {
// DEFAULT-NEXT:                   declarator: Name(
// DEFAULT-NEXT:                       "target_flags",
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// DEFAULT-NEXT: decl[{{[0-9]+}}]: Function(
// DEFAULT-NEXT:       FunctionDefinition {
// DEFAULT-NEXT:           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:               ty: Void,
// DEFAULT-NEXT:               storage: Static,
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           declarator: Function {
// DEFAULT-NEXT:               inner: Name(
// DEFAULT-NEXT:                   "emit_reload_insns",
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               parameters: Prototype {
// DEFAULT-NEXT:                   parameters: [
// DEFAULT-NEXT:                       ParameterDeclarationKind {
// DEFAULT-NEXT:                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                               ty: Tag(
// DEFAULT-NEXT:                                   Reference {
// DEFAULT-NEXT:                                       kind: Struct,
// DEFAULT-NEXT:                                       name: "insn_chain",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                           declarator: Pointer {
// DEFAULT-NEXT:                               qualifiers: Qualifiers,
// DEFAULT-NEXT:                               inner: Name(
// DEFAULT-NEXT:                                   "chain",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ],
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           },
// DEFAULT-NEXT:           body: [
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "insn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Member {
// DEFAULT-NEXT:                                           base: Identifier(
// DEFAULT-NEXT:                                               "chain",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           field: "insn",
// DEFAULT-NEXT:                                           arrow: true,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Integer(
// DEFAULT-NEXT:                               Ranked {
// DEFAULT-NEXT:                                   rank: Int,
// DEFAULT-NEXT:                                   signed: true,
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           storage: Register,
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "j",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "following_insn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Member {
// DEFAULT-NEXT:                                               base: Paren(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "insn",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "fld",
// DEFAULT-NEXT:                                                           arrow: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 2,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "2",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "rtx",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               Decl(
// DEFAULT-NEXT:                   Declaration {
// DEFAULT-NEXT:                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                           ty: Named(
// DEFAULT-NEXT:                               "rtx",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                       declarators: [
// DEFAULT-NEXT:                           InitDeclaratorKind {
// DEFAULT-NEXT:                               declarator: Name(
// DEFAULT-NEXT:                                   "before_insn",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               initializer: Some(
// DEFAULT-NEXT:                                   Expr(
// DEFAULT-NEXT:                                       Paren(
// DEFAULT-NEXT:                                           Member {
// DEFAULT-NEXT:                                               base: Paren(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Member {
// DEFAULT-NEXT:                                                           base: Paren(
// DEFAULT-NEXT:                                                               Identifier(
// DEFAULT-NEXT:                                                                   "insn",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           field: "fld",
// DEFAULT-NEXT:                                                           arrow: true,
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       index: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 1,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "1",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               field: "rtx",
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   },
// DEFAULT-NEXT:               ),
// DEFAULT-NEXT:               For {
// DEFAULT-NEXT:                   init: Some(
// DEFAULT-NEXT:                       Expr(
// DEFAULT-NEXT:                           Assign {
// DEFAULT-NEXT:                               op: Assign,
// DEFAULT-NEXT:                               target: Identifier(
// DEFAULT-NEXT:                                   "j",
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                               value: IntegerLiteral(
// DEFAULT-NEXT:                                   IntegerLiteral {
// DEFAULT-NEXT:                                       value: 0,
// DEFAULT-NEXT:                                       radix: Decimal,
// DEFAULT-NEXT:                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                           unsigned: false,
// DEFAULT-NEXT:                                           size: None,
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       spelling: "0",
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ),
// DEFAULT-NEXT:                           },
// DEFAULT-NEXT:                       ),
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   condition: Some(
// DEFAULT-NEXT:                       Binary {
// DEFAULT-NEXT:                           op: Less,
// DEFAULT-NEXT:                           left: Identifier(
// DEFAULT-NEXT:                               "j",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           right: Identifier(
// DEFAULT-NEXT:                               "n_reloads",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   increment: Some(
// DEFAULT-NEXT:                       Postfix {
// DEFAULT-NEXT:                           op: Increment,
// DEFAULT-NEXT:                           operand: Identifier(
// DEFAULT-NEXT:                               "j",
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       },
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:                   body: Block(
// DEFAULT-NEXT:                       [
// DEFAULT-NEXT:                           Decl(
// DEFAULT-NEXT:                               Declaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       storage: Register,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarators: [
// DEFAULT-NEXT:                                       InitDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "r",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           initializer: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "reload_order",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "j",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Decl(
// DEFAULT-NEXT:                               Declaration {
// DEFAULT-NEXT:                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                       ty: Integer(
// DEFAULT-NEXT:                                           Ranked {
// DEFAULT-NEXT:                                               rank: Int,
// DEFAULT-NEXT:                                               signed: true,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       storage: Register,
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                                   declarators: [
// DEFAULT-NEXT:                                       InitDeclaratorKind {
// DEFAULT-NEXT:                                           declarator: Name(
// DEFAULT-NEXT:                                               "i",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           initializer: Some(
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Index {
// DEFAULT-NEXT:                                                       base: Identifier(
// DEFAULT-NEXT:                                                           "reload_spill_index",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                       index: Identifier(
// DEFAULT-NEXT:                                                           "r",
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ],
// DEFAULT-NEXT:                               },
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                           Block(
// DEFAULT-NEXT:                               [
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Named(
// DEFAULT-NEXT:                                                   "rtx",
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclaratorKind {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "out",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Conditional {
// DEFAULT-NEXT:                                                                   condition: Binary {
// DEFAULT-NEXT:                                                                       op: Equal,
// DEFAULT-NEXT:                                                                       left: Paren(
// DEFAULT-NEXT:                                                                           Cast {
// DEFAULT-NEXT:                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                       ty: Tag(
// DEFAULT-NEXT:                                                                                           Reference {
// DEFAULT-NEXT:                                                                                               kind: Enum,
// DEFAULT-NEXT:                                                                                               name: "rtx_code",
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               value: Member {
// DEFAULT-NEXT:                                                                                   base: Paren(
// DEFAULT-NEXT:                                                                                       Member {
// DEFAULT-NEXT:                                                                                           base: Index {
// DEFAULT-NEXT:                                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                                   "rld",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               index: Identifier(
// DEFAULT-NEXT:                                                                                                   "r",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           field: "out",
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   field: "code",
// DEFAULT-NEXT:                                                                                   arrow: true,
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "REG",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   then_value: Some(
// DEFAULT-NEXT:                                                                       Member {
// DEFAULT-NEXT:                                                                           base: Index {
// DEFAULT-NEXT:                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                   "rld",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               index: Identifier(
// DEFAULT-NEXT:                                                                                   "r",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           field: "out",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   else_value: Member {
// DEFAULT-NEXT:                                                                       base: Index {
// DEFAULT-NEXT:                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                               "rld",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           index: Identifier(
// DEFAULT-NEXT:                                                                               "r",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       field: "out_reg",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   Decl(
// DEFAULT-NEXT:                                       Declaration {
// DEFAULT-NEXT:                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                               ty: Integer(
// DEFAULT-NEXT:                                                   Ranked {
// DEFAULT-NEXT:                                                       rank: Int,
// DEFAULT-NEXT:                                                       signed: true,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               storage: Register,
// DEFAULT-NEXT:                                           },
// DEFAULT-NEXT:                                           declarators: [
// DEFAULT-NEXT:                                               InitDeclaratorKind {
// DEFAULT-NEXT:                                                   declarator: Name(
// DEFAULT-NEXT:                                                       "nregno",
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   initializer: Some(
// DEFAULT-NEXT:                                                       Expr(
// DEFAULT-NEXT:                                                           Paren(
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Paren(
// DEFAULT-NEXT:                                                                       Index {
// DEFAULT-NEXT:                                                                           base: Member {
// DEFAULT-NEXT:                                                                               base: Paren(
// DEFAULT-NEXT:                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                       "out",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               field: "fld",
// DEFAULT-NEXT:                                                                               arrow: true,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 0,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "rtuint",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                   ),
// DEFAULT-NEXT:                                   If {
// DEFAULT-NEXT:                                       condition: Binary {
// DEFAULT-NEXT:                                           op: GreaterEqual,
// DEFAULT-NEXT:                                           left: Identifier(
// DEFAULT-NEXT:                                               "nregno",
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                               IntegerLiteral {
// DEFAULT-NEXT:                                                   value: 77,
// DEFAULT-NEXT:                                                   radix: Decimal,
// DEFAULT-NEXT:                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                       unsigned: false,
// DEFAULT-NEXT:                                                       size: None,
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   spelling: "77",
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       },
// DEFAULT-NEXT:                                       then_branch: Block(
// DEFAULT-NEXT:                                           [
// DEFAULT-NEXT:                                               Decl(
// DEFAULT-NEXT:                                                   Declaration {
// DEFAULT-NEXT:                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                           ty: Named(
// DEFAULT-NEXT:                                                               "rtx",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       declarators: [
// DEFAULT-NEXT:                                                           InitDeclaratorKind {
// DEFAULT-NEXT:                                                               declarator: Name(
// DEFAULT-NEXT:                                                                   "src_reg",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           InitDeclaratorKind {
// DEFAULT-NEXT:                                                               declarator: Name(
// DEFAULT-NEXT:                                                                   "store_insn",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               initializer: Some(
// DEFAULT-NEXT:                                                                   Expr(
// DEFAULT-NEXT:                                                                       Cast {
// DEFAULT-NEXT:                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                   ty: Named(
// DEFAULT-NEXT:                                                                                       "rtx",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           value: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 0,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               Expr(
// DEFAULT-NEXT:                                                   Assign {
// DEFAULT-NEXT:                                                       op: Assign,
// DEFAULT-NEXT:                                                       target: Index {
// DEFAULT-NEXT:                                                           base: Identifier(
// DEFAULT-NEXT:                                                               "reg_last_reload_reg",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           index: Identifier(
// DEFAULT-NEXT:                                                               "nregno",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       value: IntegerLiteral(
// DEFAULT-NEXT:                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                               value: 0,
// DEFAULT-NEXT:                                                               radix: Decimal,
// DEFAULT-NEXT:                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                   size: None,
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               spelling: "0",
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ),
// DEFAULT-NEXT:                                               If {
// DEFAULT-NEXT:                                                   condition: Binary {
// DEFAULT-NEXT:                                                       op: And,
// DEFAULT-NEXT:                                                       left: Binary {
// DEFAULT-NEXT:                                                           op: And,
// DEFAULT-NEXT:                                                           left: Identifier(
// DEFAULT-NEXT:                                                               "src_reg",
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: Binary {
// DEFAULT-NEXT:                                                               op: Equal,
// DEFAULT-NEXT:                                                               left: Paren(
// DEFAULT-NEXT:                                                                   Cast {
// DEFAULT-NEXT:                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                               ty: Tag(
// DEFAULT-NEXT:                                                                                   Reference {
// DEFAULT-NEXT:                                                                                       kind: Enum,
// DEFAULT-NEXT:                                                                                       name: "rtx_code",
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       value: Member {
// DEFAULT-NEXT:                                                                           base: Paren(
// DEFAULT-NEXT:                                                                               Identifier(
// DEFAULT-NEXT:                                                                                   "src_reg",
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           field: "code",
// DEFAULT-NEXT:                                                                           arrow: true,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                               right: Identifier(
// DEFAULT-NEXT:                                                                   "REG",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       right: Binary {
// DEFAULT-NEXT:                                                           op: Less,
// DEFAULT-NEXT:                                                           left: Paren(
// DEFAULT-NEXT:                                                               Member {
// DEFAULT-NEXT:                                                                   base: Paren(
// DEFAULT-NEXT:                                                                       Index {
// DEFAULT-NEXT:                                                                           base: Member {
// DEFAULT-NEXT:                                                                               base: Paren(
// DEFAULT-NEXT:                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                       "src_reg",
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                               field: "fld",
// DEFAULT-NEXT:                                                                               arrow: true,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           index: IntegerLiteral(
// DEFAULT-NEXT:                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                   value: 0,
// DEFAULT-NEXT:                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                       size: None,
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   spelling: "0",
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   field: "rtuint",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 77,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "77",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                                   then_branch: Block(
// DEFAULT-NEXT:                                                       [
// DEFAULT-NEXT:                                                           Decl(
// DEFAULT-NEXT:                                                               Declaration {
// DEFAULT-NEXT:                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                           Ranked {
// DEFAULT-NEXT:                                                                               rank: Int,
// DEFAULT-NEXT:                                                                               signed: true,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   declarators: [
// DEFAULT-NEXT:                                                                       InitDeclaratorKind {
// DEFAULT-NEXT:                                                                           declarator: Name(
// DEFAULT-NEXT:                                                                               "src_regno",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           initializer: Some(
// DEFAULT-NEXT:                                                                               Expr(
// DEFAULT-NEXT:                                                                                   Paren(
// DEFAULT-NEXT:                                                                                       Member {
// DEFAULT-NEXT:                                                                                           base: Paren(
// DEFAULT-NEXT:                                                                                               Index {
// DEFAULT-NEXT:                                                                                                   base: Member {
// DEFAULT-NEXT:                                                                                                       base: Paren(
// DEFAULT-NEXT:                                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                                               "src_reg",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       field: "fld",
// DEFAULT-NEXT:                                                                                                       arrow: true,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   index: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 0,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           field: "rtuint",
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Decl(
// DEFAULT-NEXT:                                                               Declaration {
// DEFAULT-NEXT:                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                           Ranked {
// DEFAULT-NEXT:                                                                               rank: Int,
// DEFAULT-NEXT:                                                                               signed: true,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   declarators: [
// DEFAULT-NEXT:                                                                       InitDeclaratorKind {
// DEFAULT-NEXT:                                                                           declarator: Name(
// DEFAULT-NEXT:                                                                               "nr",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           initializer: Some(
// DEFAULT-NEXT:                                                                               Expr(
// DEFAULT-NEXT:                                                                                   Paren(
// DEFAULT-NEXT:                                                                                       Conditional {
// DEFAULT-NEXT:                                                                                           condition: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: And,
// DEFAULT-NEXT:                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                       op: GreaterEqual,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                                               "src_regno",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 32,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "32",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   right: Binary {
// DEFAULT-NEXT:                                                                                                       op: LessEqual,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                                               "src_regno",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 63,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "63",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           then_value: Some(
// DEFAULT-NEXT:                                                                                               Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: Div,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Sub,
// DEFAULT-NEXT:                                                                                                               left: Binary {
// DEFAULT-NEXT:                                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                                       Index {
// DEFAULT-NEXT:                                                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                                                               "mode_size",
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           index: Cast {
// DEFAULT-NEXT:                                                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                                                                               signed: true,
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                                                                   Member {
// DEFAULT-NEXT:                                                                                                                                       base: Index {
// DEFAULT-NEXT:                                                                                                                                           base: Identifier(
// DEFAULT-NEXT:                                                                                                                                               "rld",
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                           index: Identifier(
// DEFAULT-NEXT:                                                                                                                                               "r",
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                       field: "mode",
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 1,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "1",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 8,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           else_value: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Div,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: Sub,
// DEFAULT-NEXT:                                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                                               op: Add,
// DEFAULT-NEXT:                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                   Index {
// DEFAULT-NEXT:                                                                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                                                                           "mode_size",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       index: Cast {
// DEFAULT-NEXT:                                                                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                                                                           rank: Int,
// DEFAULT-NEXT:                                                                                                                                           signed: true,
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                                                               Member {
// DEFAULT-NEXT:                                                                                                                                   base: Index {
// DEFAULT-NEXT:                                                                                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                                                                                           "rld",
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       index: Identifier(
// DEFAULT-NEXT:                                                                                                                                           "r",
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   field: "mode",
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                                   Conditional {
// DEFAULT-NEXT:                                                                                                                       condition: Unary {
// DEFAULT-NEXT:                                                                                                                           op: Not,
// DEFAULT-NEXT:                                                                                                                           operand: Paren(
// DEFAULT-NEXT:                                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                                   op: BitAnd,
// DEFAULT-NEXT:                                                                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                                                                       "target_flags",
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                                           value: 32,
// DEFAULT-NEXT:                                                                                                                                           radix: Hex,
// DEFAULT-NEXT:                                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                           spelling: "0x00000020",
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       then_value: Some(
// DEFAULT-NEXT:                                                                                                                           IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                                   value: 4,
// DEFAULT-NEXT:                                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   spelling: "4",
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       else_value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                               value: 8,
// DEFAULT-NEXT:                                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Paren(
// DEFAULT-NEXT:                                                                                                       Conditional {
// DEFAULT-NEXT:                                                                                                           condition: Unary {
// DEFAULT-NEXT:                                                                                                               op: Not,
// DEFAULT-NEXT:                                                                                                               operand: Paren(
// DEFAULT-NEXT:                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                       op: BitAnd,
// DEFAULT-NEXT:                                                                                                                       left: Identifier(
// DEFAULT-NEXT:                                                                                                                           "target_flags",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                               value: 32,
// DEFAULT-NEXT:                                                                                                                               radix: Hex,
// DEFAULT-NEXT:                                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               spelling: "0x00000020",
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           then_value: Some(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 4,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "4",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           else_value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           Decl(
// DEFAULT-NEXT:                                                               Declaration {
// DEFAULT-NEXT:                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                       ty: Named(
// DEFAULT-NEXT:                                                                           "rtx",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   declarators: [
// DEFAULT-NEXT:                                                                       InitDeclaratorKind {
// DEFAULT-NEXT:                                                                           declarator: Name(
// DEFAULT-NEXT:                                                                               "note",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           initializer: Some(
// DEFAULT-NEXT:                                                                               Expr(
// DEFAULT-NEXT:                                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                           value: 0,
// DEFAULT-NEXT:                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                               size: None,
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                           While {
// DEFAULT-NEXT:                                                               condition: Binary {
// DEFAULT-NEXT:                                                                   op: Greater,
// DEFAULT-NEXT:                                                                   left: Postfix {
// DEFAULT-NEXT:                                                                       op: Decrement,
// DEFAULT-NEXT:                                                                       operand: Identifier(
// DEFAULT-NEXT:                                                                           "nr",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                           value: 0,
// DEFAULT-NEXT:                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                               size: None,
// DEFAULT-NEXT:                                                                           },
// DEFAULT-NEXT:                                                                           spelling: "0",
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               body: Block(
// DEFAULT-NEXT:                                                                   [
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Assign {
// DEFAULT-NEXT:                                                                                   op: BitAndAssign,
// DEFAULT-NEXT:                                                                                   target: Index {
// DEFAULT-NEXT:                                                                                       base: Paren(
// DEFAULT-NEXT:                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                               "reg_reloaded_dead",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       index: Binary {
// DEFAULT-NEXT:                                                                                           op: Div,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                                       "src_regno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                       "nr",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Paren(
// DEFAULT-NEXT:                                                                                               Cast {
// DEFAULT-NEXT:                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                                                               Ranked {
// DEFAULT-NEXT:                                                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                                                   signed: false,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   value: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: Mul,
// DEFAULT-NEXT:                                                                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   value: Unary {
// DEFAULT-NEXT:                                                                                       op: BitNot,
// DEFAULT-NEXT:                                                                                       operand: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: ShiftLeft,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Cast {
// DEFAULT-NEXT:                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                               ty: Named(
// DEFAULT-NEXT:                                                                                                                   "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 1,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "1",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: Rem,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Add,
// DEFAULT-NEXT:                                                                                                               left: Identifier(
// DEFAULT-NEXT:                                                                                                                   "src_regno",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Identifier(
// DEFAULT-NEXT:                                                                                                                   "nr",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Paren(
// DEFAULT-NEXT:                                                                                                           Cast {
// DEFAULT-NEXT:                                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                       ty: Integer(
// DEFAULT-NEXT:                                                                                                                           Ranked {
// DEFAULT-NEXT:                                                                                                                               rank: Int,
// DEFAULT-NEXT:                                                                                                                               signed: false,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                                       op: Mul,
// DEFAULT-NEXT:                                                                                                                       left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                               value: 8,
// DEFAULT-NEXT:                                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                               value: 8,
// DEFAULT-NEXT:                                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               spelling: "8",
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Assign {
// DEFAULT-NEXT:                                                                                   op: BitOrAssign,
// DEFAULT-NEXT:                                                                                   target: Index {
// DEFAULT-NEXT:                                                                                       base: Paren(
// DEFAULT-NEXT:                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                               "reg_reloaded_valid",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       index: Binary {
// DEFAULT-NEXT:                                                                                           op: Div,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                                       "src_regno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                       "nr",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Paren(
// DEFAULT-NEXT:                                                                                               Cast {
// DEFAULT-NEXT:                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                                                               Ranked {
// DEFAULT-NEXT:                                                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                                                   signed: false,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   value: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: Mul,
// DEFAULT-NEXT:                                                                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Cast {
// DEFAULT-NEXT:                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                       ty: Named(
// DEFAULT-NEXT:                                                                                                           "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 1,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "1",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: Rem,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                                       left: Identifier(
// DEFAULT-NEXT:                                                                                                           "src_regno",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                                           "nr",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                   Cast {
// DEFAULT-NEXT:                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Assign {
// DEFAULT-NEXT:                                                                                   op: BitOrAssign,
// DEFAULT-NEXT:                                                                                   target: Index {
// DEFAULT-NEXT:                                                                                       base: Paren(
// DEFAULT-NEXT:                                                                                           Identifier(
// DEFAULT-NEXT:                                                                                               "reg_is_output_reload",
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       index: Binary {
// DEFAULT-NEXT:                                                                                           op: Div,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Add,
// DEFAULT-NEXT:                                                                                                   left: Identifier(
// DEFAULT-NEXT:                                                                                                       "src_regno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Identifier(
// DEFAULT-NEXT:                                                                                                       "nr",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Paren(
// DEFAULT-NEXT:                                                                                               Cast {
// DEFAULT-NEXT:                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                                                               Ranked {
// DEFAULT-NEXT:                                                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                                                   signed: false,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   value: Paren(
// DEFAULT-NEXT:                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                           op: Mul,
// DEFAULT-NEXT:                                                                                                           left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                                   value: Binary {
// DEFAULT-NEXT:                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                           Cast {
// DEFAULT-NEXT:                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                       ty: Named(
// DEFAULT-NEXT:                                                                                                           "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 1,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "1",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                       right: Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: Rem,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                                       left: Identifier(
// DEFAULT-NEXT:                                                                                                           "src_regno",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                                                           "nr",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                   Cast {
// DEFAULT-NEXT:                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       If {
// DEFAULT-NEXT:                                                                           condition: Identifier(
// DEFAULT-NEXT:                                                                               "note",
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           then_branch: Expr(
// DEFAULT-NEXT:                                                                               Paren(
// DEFAULT-NEXT:                                                                                   Assign {
// DEFAULT-NEXT:                                                                                       op: BitOrAssign,
// DEFAULT-NEXT:                                                                                       target: Index {
// DEFAULT-NEXT:                                                                                           base: Paren(
// DEFAULT-NEXT:                                                                                               Identifier(
// DEFAULT-NEXT:                                                                                                   "reg_reloaded_died",
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           index: Binary {
// DEFAULT-NEXT:                                                                                               op: Div,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "src_regno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                   Cast {
// DEFAULT-NEXT:                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                       value: Binary {
// DEFAULT-NEXT:                                                                                           op: ShiftLeft,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Cast {
// DEFAULT-NEXT:                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                           ty: Named(
// DEFAULT-NEXT:                                                                                                               "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   value: Paren(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Rem,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                           "src_regno",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Paren(
// DEFAULT-NEXT:                                                                                                       Cast {
// DEFAULT-NEXT:                                                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                                                           rank: Int,
// DEFAULT-NEXT:                                                                                                                           signed: false,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                                                   left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   },
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                           else_branch: Some(
// DEFAULT-NEXT:                                                                               Expr(
// DEFAULT-NEXT:                                                                                   Paren(
// DEFAULT-NEXT:                                                                                       Assign {
// DEFAULT-NEXT:                                                                                           op: BitAndAssign,
// DEFAULT-NEXT:                                                                                           target: Index {
// DEFAULT-NEXT:                                                                                               base: Paren(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "reg_reloaded_died",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               index: Binary {
// DEFAULT-NEXT:                                                                                                   op: Div,
// DEFAULT-NEXT:                                                                                                   left: Paren(
// DEFAULT-NEXT:                                                                                                       Identifier(
// DEFAULT-NEXT:                                                                                                           "src_regno",
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   right: Paren(
// DEFAULT-NEXT:                                                                                                       Cast {
// DEFAULT-NEXT:                                                                                                           ty: TypeName {
// DEFAULT-NEXT:                                                                                                               specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                   ty: Integer(
// DEFAULT-NEXT:                                                                                                                       Ranked {
// DEFAULT-NEXT:                                                                                                                           rank: Int,
// DEFAULT-NEXT:                                                                                                                           signed: false,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               declarator: Abstract,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           value: Paren(
// DEFAULT-NEXT:                                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                                   op: Mul,
// DEFAULT-NEXT:                                                                                                                   left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           value: Unary {
// DEFAULT-NEXT:                                                                                               op: BitNot,
// DEFAULT-NEXT:                                                                                               operand: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: ShiftLeft,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Cast {
// DEFAULT-NEXT:                                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                       ty: Named(
// DEFAULT-NEXT:                                                                                                                           "HARD_REG_ELT_TYPE",
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               value: Paren(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 1,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "1",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: Rem,
// DEFAULT-NEXT:                                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                                       "src_regno",
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: Paren(
// DEFAULT-NEXT:                                                                                                                   Cast {
// DEFAULT-NEXT:                                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                                                                       signed: false,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                                               op: Mul,
// DEFAULT-NEXT:                                                                                                                               left: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               ),
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                   ],
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           Expr(
// DEFAULT-NEXT:                                                               Assign {
// DEFAULT-NEXT:                                                                   op: Assign,
// DEFAULT-NEXT:                                                                   target: Index {
// DEFAULT-NEXT:                                                                       base: Identifier(
// DEFAULT-NEXT:                                                                           "reg_last_reload_reg",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       index: Identifier(
// DEFAULT-NEXT:                                                                           "nregno",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   value: Identifier(
// DEFAULT-NEXT:                                                                       "src_reg",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       ],
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   else_branch: None,
// DEFAULT-NEXT:                                               },
// DEFAULT-NEXT:                                           ],
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                       else_branch: Some(
// DEFAULT-NEXT:                                           Block(
// DEFAULT-NEXT:                                               [
// DEFAULT-NEXT:                                                   Decl(
// DEFAULT-NEXT:                                                       Declaration {
// DEFAULT-NEXT:                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                               ty: Integer(
// DEFAULT-NEXT:                                                                   Ranked {
// DEFAULT-NEXT:                                                                       rank: Int,
// DEFAULT-NEXT:                                                                       signed: true,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           declarators: [
// DEFAULT-NEXT:                                                               InitDeclaratorKind {
// DEFAULT-NEXT:                                                                   declarator: Name(
// DEFAULT-NEXT:                                                                       "num_regs",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   initializer: Some(
// DEFAULT-NEXT:                                                                       Expr(
// DEFAULT-NEXT:                                                                           Paren(
// DEFAULT-NEXT:                                                                               Conditional {
// DEFAULT-NEXT:                                                                                   condition: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: And,
// DEFAULT-NEXT:                                                                                           left: Binary {
// DEFAULT-NEXT:                                                                                               op: GreaterEqual,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "nregno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 32,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "32",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                           right: Binary {
// DEFAULT-NEXT:                                                                                               op: LessEqual,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Identifier(
// DEFAULT-NEXT:                                                                                                       "nregno",
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 63,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "63",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   then_value: Some(
// DEFAULT-NEXT:                                                                                       Paren(
// DEFAULT-NEXT:                                                                                           Binary {
// DEFAULT-NEXT:                                                                                               op: Div,
// DEFAULT-NEXT:                                                                                               left: Paren(
// DEFAULT-NEXT:                                                                                                   Binary {
// DEFAULT-NEXT:                                                                                                       op: Sub,
// DEFAULT-NEXT:                                                                                                       left: Binary {
// DEFAULT-NEXT:                                                                                                           op: Add,
// DEFAULT-NEXT:                                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                                               Index {
// DEFAULT-NEXT:                                                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                                                       "mode_size",
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                   index: Cast {
// DEFAULT-NEXT:                                                                                                                       ty: TypeName {
// DEFAULT-NEXT:                                                                                                                           specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                               ty: Integer(
// DEFAULT-NEXT:                                                                                                                                   Ranked {
// DEFAULT-NEXT:                                                                                                                                       rank: Int,
// DEFAULT-NEXT:                                                                                                                                       signed: true,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       value: Paren(
// DEFAULT-NEXT:                                                                                                                           Paren(
// DEFAULT-NEXT:                                                                                                                               Cast {
// DEFAULT-NEXT:                                                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                                           ty: Tag(
// DEFAULT-NEXT:                                                                                                                                               Reference {
// DEFAULT-NEXT:                                                                                                                                                   kind: Enum,
// DEFAULT-NEXT:                                                                                                                                                   name: "machine_mode",
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   value: Member {
// DEFAULT-NEXT:                                                                                                                                       base: Paren(
// DEFAULT-NEXT:                                                                                                                                           Member {
// DEFAULT-NEXT:                                                                                                                                               base: Index {
// DEFAULT-NEXT:                                                                                                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                                                                                                       "rld",
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                                   index: Identifier(
// DEFAULT-NEXT:                                                                                                                                                       "r",
// DEFAULT-NEXT:                                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                                               field: "out",
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                       field: "mode",
// DEFAULT-NEXT:                                                                                                                                       arrow: true,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                   value: 8,
// DEFAULT-NEXT:                                                                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   spelling: "8",
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           ),
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 1,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "1",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                               ),
// DEFAULT-NEXT:                                                                                           },
// DEFAULT-NEXT:                                                                                       ),
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                                   else_value: Paren(
// DEFAULT-NEXT:                                                                                       Binary {
// DEFAULT-NEXT:                                                                                           op: Div,
// DEFAULT-NEXT:                                                                                           left: Paren(
// DEFAULT-NEXT:                                                                                               Binary {
// DEFAULT-NEXT:                                                                                                   op: Sub,
// DEFAULT-NEXT:                                                                                                   left: Binary {
// DEFAULT-NEXT:                                                                                                       op: Add,
// DEFAULT-NEXT:                                                                                                       left: Paren(
// DEFAULT-NEXT:                                                                                                           Index {
// DEFAULT-NEXT:                                                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                                                   "mode_size",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               index: Cast {
// DEFAULT-NEXT:                                                                                                                   ty: TypeName {
// DEFAULT-NEXT:                                                                                                                       specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                           ty: Integer(
// DEFAULT-NEXT:                                                                                                                               Ranked {
// DEFAULT-NEXT:                                                                                                                                   rank: Int,
// DEFAULT-NEXT:                                                                                                                                   signed: true,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                   value: Paren(
// DEFAULT-NEXT:                                                                                                                       Paren(
// DEFAULT-NEXT:                                                                                                                           Cast {
// DEFAULT-NEXT:                                                                                                                               ty: TypeName {
// DEFAULT-NEXT:                                                                                                                                   specifiers: DeclarationSpecifiers {
// DEFAULT-NEXT:                                                                                                                                       ty: Tag(
// DEFAULT-NEXT:                                                                                                                                           Reference {
// DEFAULT-NEXT:                                                                                                                                               kind: Enum,
// DEFAULT-NEXT:                                                                                                                                               name: "machine_mode",
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   declarator: Abstract,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                               value: Member {
// DEFAULT-NEXT:                                                                                                                                   base: Paren(
// DEFAULT-NEXT:                                                                                                                                       Member {
// DEFAULT-NEXT:                                                                                                                                           base: Index {
// DEFAULT-NEXT:                                                                                                                                               base: Identifier(
// DEFAULT-NEXT:                                                                                                                                                   "rld",
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                               index: Identifier(
// DEFAULT-NEXT:                                                                                                                                                   "r",
// DEFAULT-NEXT:                                                                                                                                               ),
// DEFAULT-NEXT:                                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                                           field: "out",
// DEFAULT-NEXT:                                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                                                   field: "mode",
// DEFAULT-NEXT:                                                                                                                                   arrow: true,
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                       ),
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                       right: Paren(
// DEFAULT-NEXT:                                                                                                           Conditional {
// DEFAULT-NEXT:                                                                                                               condition: Unary {
// DEFAULT-NEXT:                                                                                                                   op: Not,
// DEFAULT-NEXT:                                                                                                                   operand: Paren(
// DEFAULT-NEXT:                                                                                                                       Binary {
// DEFAULT-NEXT:                                                                                                                           op: BitAnd,
// DEFAULT-NEXT:                                                                                                                           left: Identifier(
// DEFAULT-NEXT:                                                                                                                               "target_flags",
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                                   value: 32,
// DEFAULT-NEXT:                                                                                                                                   radix: Hex,
// DEFAULT-NEXT:                                                                                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                                                                                       size: None,
// DEFAULT-NEXT:                                                                                                                                   },
// DEFAULT-NEXT:                                                                                                                                   spelling: "0x00000020",
// DEFAULT-NEXT:                                                                                                                               },
// DEFAULT-NEXT:                                                                                                                           ),
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               then_value: Some(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                           value: 4,
// DEFAULT-NEXT:                                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                                           },
// DEFAULT-NEXT:                                                                                                                           spelling: "4",
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                   ),
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               else_value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 8,
// DEFAULT-NEXT:                                                                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "8",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 1,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "1",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                           right: Paren(
// DEFAULT-NEXT:                                                                                               Conditional {
// DEFAULT-NEXT:                                                                                                   condition: Unary {
// DEFAULT-NEXT:                                                                                                       op: Not,
// DEFAULT-NEXT:                                                                                                       operand: Paren(
// DEFAULT-NEXT:                                                                                                           Binary {
// DEFAULT-NEXT:                                                                                                               op: BitAnd,
// DEFAULT-NEXT:                                                                                                               left: Identifier(
// DEFAULT-NEXT:                                                                                                                   "target_flags",
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                               right: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                                                                       value: 32,
// DEFAULT-NEXT:                                                                                                                       radix: Hex,
// DEFAULT-NEXT:                                                                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                                                                           size: None,
// DEFAULT-NEXT:                                                                                                                       },
// DEFAULT-NEXT:                                                                                                                       spelling: "0x00000020",
// DEFAULT-NEXT:                                                                                                                   },
// DEFAULT-NEXT:                                                                                                               ),
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   },
// DEFAULT-NEXT:                                                                                                   then_value: Some(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral(
// DEFAULT-NEXT:                                                                                                           IntegerLiteral {
// DEFAULT-NEXT:                                                                                                               value: 4,
// DEFAULT-NEXT:                                                                                                               radix: Decimal,
// DEFAULT-NEXT:                                                                                                               suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                                   unsigned: false,
// DEFAULT-NEXT:                                                                                                                   size: None,
// DEFAULT-NEXT:                                                                                                               },
// DEFAULT-NEXT:                                                                                                               spelling: "4",
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                       ),
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                                   else_value: IntegerLiteral(
// DEFAULT-NEXT:                                                                                                       IntegerLiteral {
// DEFAULT-NEXT:                                                                                                           value: 8,
// DEFAULT-NEXT:                                                                                                           radix: Decimal,
// DEFAULT-NEXT:                                                                                                           suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                                                               unsigned: false,
// DEFAULT-NEXT:                                                                                                               size: None,
// DEFAULT-NEXT:                                                                                                           },
// DEFAULT-NEXT:                                                                                                           spelling: "8",
// DEFAULT-NEXT:                                                                                                       },
// DEFAULT-NEXT:                                                                                                   ),
// DEFAULT-NEXT:                                                                                               },
// DEFAULT-NEXT:                                                                                           ),
// DEFAULT-NEXT:                                                                                       },
// DEFAULT-NEXT:                                                                                   ),
// DEFAULT-NEXT:                                                                               },
// DEFAULT-NEXT:                                                                           ),
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ],
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                   ),
// DEFAULT-NEXT:                                                   While {
// DEFAULT-NEXT:                                                       condition: Binary {
// DEFAULT-NEXT:                                                           op: Greater,
// DEFAULT-NEXT:                                                           left: Postfix {
// DEFAULT-NEXT:                                                               op: Decrement,
// DEFAULT-NEXT:                                                               operand: Identifier(
// DEFAULT-NEXT:                                                                   "num_regs",
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                           right: IntegerLiteral(
// DEFAULT-NEXT:                                                               IntegerLiteral {
// DEFAULT-NEXT:                                                                   value: 0,
// DEFAULT-NEXT:                                                                   radix: Decimal,
// DEFAULT-NEXT:                                                                   suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                       unsigned: false,
// DEFAULT-NEXT:                                                                       size: None,
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                                   spelling: "0",
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                           ),
// DEFAULT-NEXT:                                                       },
// DEFAULT-NEXT:                                                       body: Expr(
// DEFAULT-NEXT:                                                           Assign {
// DEFAULT-NEXT:                                                               op: Assign,
// DEFAULT-NEXT:                                                               target: Index {
// DEFAULT-NEXT:                                                                   base: Identifier(
// DEFAULT-NEXT:                                                                       "reg_last_reload_reg",
// DEFAULT-NEXT:                                                                   ),
// DEFAULT-NEXT:                                                                   index: Binary {
// DEFAULT-NEXT:                                                                       op: Add,
// DEFAULT-NEXT:                                                                       left: Identifier(
// DEFAULT-NEXT:                                                                           "nregno",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                       right: Identifier(
// DEFAULT-NEXT:                                                                           "num_regs",
// DEFAULT-NEXT:                                                                       ),
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               },
// DEFAULT-NEXT:                                                               value: IntegerLiteral(
// DEFAULT-NEXT:                                                                   IntegerLiteral {
// DEFAULT-NEXT:                                                                       value: 0,
// DEFAULT-NEXT:                                                                       radix: Decimal,
// DEFAULT-NEXT:                                                                       suffix: IntegerSuffix {
// DEFAULT-NEXT:                                                                           unsigned: false,
// DEFAULT-NEXT:                                                                           size: None,
// DEFAULT-NEXT:                                                                       },
// DEFAULT-NEXT:                                                                       spelling: "0",
// DEFAULT-NEXT:                                                                   },
// DEFAULT-NEXT:                                                               ),
// DEFAULT-NEXT:                                                           },
// DEFAULT-NEXT:                                                       ),
// DEFAULT-NEXT:                                                   },
// DEFAULT-NEXT:                                               ],
// DEFAULT-NEXT:                                           ),
// DEFAULT-NEXT:                                       ),
// DEFAULT-NEXT:                                   },
// DEFAULT-NEXT:                               ],
// DEFAULT-NEXT:                           ),
// DEFAULT-NEXT:                       ],
// DEFAULT-NEXT:                   ),
// DEFAULT-NEXT:               },
// DEFAULT-NEXT:           ],
// DEFAULT-NEXT:       },
// DEFAULT-NEXT:   )
// SLATE-FILECHECK-END DEFAULT
