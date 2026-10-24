/* { dg-do compile } */
/* { dg-options "-O2 -w -fpermissive" } */
/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

/* This file was automatically reduced from tree-ssa-operands.c.  It
   contains many warnings, but it exposes a copy propagation bug that
   is somewhat difficult to reproduce otherwise.  */

__extension__ typedef __SIZE_TYPE__ size_t;
  extern void fancy_abort (const char *, int, const char *) __attribute__ ((__noreturn__));
  typedef union tree_node *tree;
  enum tree_code {
 TREE_LIST, ARRAY_TYPE, RECORD_TYPE, UNION_TYPE, QUAL_UNION_TYPE, STRING_CST, LT_EXPR, GE_EXPR, LABEL_EXPR, ASM_EXPR, SSA_NAME, };
  enum tree_code_class {
   tcc_type,   tcc_reference,   tcc_expression };
  extern const enum tree_code_class tree_code_type[];
  extern const unsigned char tree_code_length[];
  struct tree_common {
   union tree_ann_d *ann;
   __extension__ enum tree_code code : 8;
 };
  struct tree_string {
   char str[1];
 };
  struct tree_list {
   tree purpose;
   tree value;
 };
  struct tree_exp {
   tree     operands[1];
 };
  typedef struct ssa_use_operand_d {
   struct ssa_use_operand_d* prev;
   struct ssa_use_operand_d* next;
   tree stmt;
   tree * use;
 }
  ssa_use_operand_t;
  struct tree_ssa_name {
   struct ssa_use_operand_d imm_uses;
 };
  union tree_node {
   struct tree_common common;
   struct tree_string string;
   struct tree_list list;
   struct tree_exp exp;
   struct tree_ssa_name ssa_name;
 };
  typedef struct bitmap_head_def *bitmap;
  typedef union varray_data_tag {
   char c[1];
   int i[1];
   tree * tp[1];
 }
  varray_data;
  struct varray_head_tag {
   size_t num_elements;
   size_t elements_used;
   varray_data data;
 };
  typedef struct varray_head_tag *varray_type;
  extern void varray_check_failed (varray_type, size_t, const char *, int,      const char *) __attribute__ ((__noreturn__));
  typedef ssa_use_operand_t *use_operand_p;
  struct use_optype_d {
   struct use_optype_d *next;
   struct ssa_use_operand_d use_ptr;
 };
  typedef struct use_optype_d *use_optype_p;
  struct ssa_operand_memory_d {
   char mem[(2048 - sizeof (void *))];
 };
  struct stmt_operands_d {
   struct use_optype_d * use_ops;
 };
  struct ptr_info_def {
   tree name_mem_tag;
 };
  typedef struct subvar *subvar_t;
  struct var_ann_d {
   size_t uid;
 };
  struct stmt_ann_d {
   unsigned modified : 1;
   struct stmt_operands_d operands;
   bitmap addresses_taken;
 };
  typedef struct var_ann_d *var_ann_t;
  typedef struct stmt_ann_d *stmt_ann_t;
  static __inline__ var_ann_t var_ann (tree t) {
 }
  static __inline__ stmt_ann_t stmt_ann (tree t) {
   ((void)(!(is_gimple_stmt (t)) ? fancy_abort ("/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-flow-inline.h", 55, __FUNCTION__), 0 : 0));
   return (stmt_ann_t) t->common.ann;
 }
  static __inline__ unsigned char stmt_modified_p (tree t) {
   stmt_ann_t ann = stmt_ann (t);
   return ann ? ann->modified : 1;
 }
  static __inline__ void delink_imm_use (ssa_use_operand_t *linknode) {
   if (linknode->prev == ((void *)0))     return;
   linknode->prev->next = linknode->next;
   linknode->next->prev = linknode->prev;
   linknode->prev = ((void *)0);
   linknode->next = ((void *)0);
 }
  static __inline__ void link_imm_use_to_list (ssa_use_operand_t *linknode, ssa_use_operand_t *list) {
   linknode->prev = list;
   linknode->next = list->next;
   list->next = linknode;
 }
  static __inline__ void link_imm_use (ssa_use_operand_t *linknode, tree def) {
   ssa_use_operand_t *root;
   if (!def || ((enum tree_code) (def)->common.code) != SSA_NAME)     linknode->prev = ((void *)0);
   else     {
       root = &(__extension__ ({ const tree __t = (def); if (((enum tree_code) (__t)->common.code) != (SSA_NAME)) tree_check_failed (__t, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-flow-inline.h", 222, __FUNCTION__, (SSA_NAME), 0); __t; }
)->ssa_name.imm_uses);
       link_imm_use_to_list (linknode, root);
     }
 }
  static __inline__ void link_imm_use_stmt (ssa_use_operand_t *linknode, tree def, tree stmt) {
     link_imm_use (linknode, def);
 }
  struct ggc_root_tab {
 };
  struct opbuild_list_d {
   varray_type vars;
   varray_type uid;
   varray_type next;
   int first;
   unsigned num;
 };
  static struct opbuild_list_d build_uses;
  static struct opbuild_list_d build_v_may_defs;
  static struct ssa_operand_memory_d *operand_memory = ((void *)0);
  static unsigned operand_memory_index;
  static use_optype_p free_uses = ((void *)0);
  static __inline__ int opbuild_next (struct opbuild_list_d *list, int prev) {
   return __extension__ (*({
 varray_type const _va = (list->next);
 const size_t _n = (prev);
 if (_n >= _va->num_elements) varray_check_failed (_va, _n, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 323, __FUNCTION__);
 &_va->data.i[_n];
 }
));
 }
  static __inline__ tree * opbuild_elem_real (struct opbuild_list_d *list, int elem) {
   return __extension__ (*({
 varray_type const _va = (list->vars);
 const size_t _n = (elem);
 if (_n >= _va->num_elements) varray_check_failed (_va, _n, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 332, __FUNCTION__);
 &_va->data.tp[_n];
 }
));
 }
  static __inline__ void opbuild_clear (struct opbuild_list_d *list) {
   ((list->vars)->elements_used = 0);
   ((list->next)->elements_used = 0);
 }
  static __inline__ void * ssa_operand_alloc (unsigned size) {
   char *ptr;
   if (operand_memory_index + size >= (2048 - sizeof (void *)))     {
       ptr = ggc_alloc_stat (sizeof (struct ssa_operand_memory_d) );
     }
   ptr = &(operand_memory->mem[operand_memory_index]);
   return ptr;
 }
  static __inline__ void correct_use_link (use_operand_p ptr, tree stmt) {
   use_operand_p prev;
   tree root;
   prev = ptr->prev;
   if (prev)     {
       unsigned char stmt_mod = 1;
       while (stmt_mod)  {    while (prev->stmt == stmt || prev->stmt == ((void *)0))      prev = prev->prev;    if (prev->use == ((void *)0))      stmt_mod = 0;    else      if ((stmt_mod = stmt_modified_p (prev->stmt)))        prev = prev->prev;  }
       if (prev->use == ((void *)0))  root = prev->stmt;
       else  root = *(prev->use);
       if (root == *(ptr->use))  return;
     }
   delink_imm_use (ptr);
   link_imm_use (ptr, *(ptr->use));
 }
  static __inline__ struct use_optype_d * alloc_use (void) {
   struct use_optype_d *ret;
     {
       free_uses = free_uses->next;
     }
     ret = (struct use_optype_d *)ssa_operand_alloc (sizeof (struct use_optype_d));
   return ret;
 }
  static __inline__ void finalize_ssa_use_ops (tree stmt) {
   int new_i;
   struct use_optype_d *old_ops, *ptr, *last;
   tree * old_base;
   struct use_optype_d new_list;
   last = &new_list;
   if (old_ops)     old_base = ((old_ops)->use_ptr.use);
   while (old_ops && new_i != -1)     {
       tree * new_base = opbuild_elem_real (&build_uses, (new_i));
       if (old_base == new_base)         {    last->next = old_ops;    last = old_ops;    correct_use_link ((&((last)->use_ptr)), stmt);    old_ops = old_ops->next;    new_i = opbuild_next (&build_uses, new_i);  }
       else         if (old_base < new_base)    {      use_operand_p use_p = (&((old_ops)->use_ptr));      delink_imm_use (use_p);      old_ops = old_ops->next;      ptr->next = free_uses;    }
  else    {      ptr = alloc_use ();      (ptr)->use_ptr.use = (opbuild_elem_real (&build_uses, (new_i))); link_imm_use_stmt (&((ptr)->use_ptr), *(opbuild_elem_real (&build_uses, (new_i))), (stmt));      new_i = opbuild_next (&build_uses, new_i);    }
       if (old_ops)         old_base = ((old_ops)->use_ptr.use);
     }
   for ( ;
  new_i != -1;
  new_i = opbuild_next (&build_uses, new_i))     {
       ptr = alloc_use ();
       (ptr)->use_ptr.use = (opbuild_elem_real (&build_uses, (new_i)));
 link_imm_use_stmt (&((ptr)->use_ptr), *(opbuild_elem_real (&build_uses, (new_i))), (stmt));
     }
     {
  {    use_operand_p use_p = (&((ptr)->use_ptr));    delink_imm_use (use_p);  }
     }
   (stmt_ann (stmt)->operands.use_ops) = new_list.next;
   {
     unsigned x = 0;
     for (ptr = (stmt_ann (stmt)->operands.use_ops);
 ptr;
 ptr = ptr->next)       x++;
   }
 }
  finalize_ssa_uses (tree stmt) {
   {
     unsigned x;
       ((void)(!(*(opbuild_elem_real (&build_uses, x)) != stmt) ? fancy_abort ("/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 581, __FUNCTION__), 0 : 0));
   }
   finalize_ssa_use_ops (stmt);
   opbuild_clear (&build_uses);
 }
  finalize_ssa_v_may_def_ops (tree stmt) {
   int new_i;
     {
     }
   for ( ;
  new_i != -1;
  new_i = opbuild_next (&build_v_may_defs, new_i))     {
     }
   {
   }
 }
  get_expr_operands (tree stmt, tree *expr_p, int flags) {
   enum tree_code code;
   tree expr = *expr_p;
     {
       {  subvar_t svars;  if (var_can_have_subvars (expr)      && (svars = get_subvars_for_var (expr)))    {    }    {    }       }
       {    {      if (code == LT_EXPR   || code == GE_EXPR)        {   swap_tree_operands (stmt,         &__extension__ (*({const tree __t = __extension__ ({ const tree __t = (expr); char const __c = tree_code_type[(int) (((enum tree_code) (__t)->common.code))]; if (!((__c) >= tcc_reference && (__c) <= tcc_expression)) tree_class_check_failed (__t, tcc_expression, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1435, __FUNCTION__); __t; }); const int __i = (0); if (__i < 0 || __i >= tree_code_length[(int) (((enum tree_code) (__t)->common.code))]) tree_operand_check_failed (__i, ((enum tree_code) (__t)->common.code), "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1435, __FUNCTION__); &__t->exp.operands[__i]; })),         &__extension__ (*({const tree __t = __extension__ ({ const tree __t = (expr); char const __c = tree_code_type[(int) (((enum tree_code) (__t)->common.code))]; if (!((__c) >= tcc_reference && (__c) <= tcc_expression)) tree_class_check_failed (__t, tcc_expression, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1436, __FUNCTION__); __t; }); const int __i = (1); if (__i < 0 || __i >= tree_code_length[(int) (((enum tree_code) (__t)->common.code))]) tree_operand_check_failed (__i, ((enum tree_code) (__t)->common.code), "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1436, __FUNCTION__); &__t->exp.operands[__i]; })));        }        {        }    }       }
     }
 }
  get_asm_expr_operands (tree stmt) {
   int noutputs = list_length (__extension__ (*({
const tree __t = __extension__ ({ const tree __t = (__extension__ ({ const tree __t = (stmt); if (((enum tree_code) (__t)->common.code) != (ASM_EXPR)) tree_check_failed (__t, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1495, __FUNCTION__, (ASM_EXPR), 0); __t; })); char const __c = tree_code_type[(int) (((enum tree_code) (__t)->common.code))]; if (!((__c) >= tcc_reference && (__c) <= tcc_expression)) tree_class_check_failed (__t, tcc_expression, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1495, __FUNCTION__); __t; }
);
 const int __i = (1);
 if (__i < 0 || __i >= tree_code_length[(int) (((enum tree_code) (__t)->common.code))]) tree_operand_check_failed (__i, ((enum tree_code) (__t)->common.code), "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1495, __FUNCTION__);
 &__t->exp.operands[__i];
 }
)));
   const char **oconstraints     = (const char **) __builtin_alloca((noutputs) * sizeof (const char *));
   int i;
   tree link;
   const char *constraint;
   unsigned char allows_mem, allows_reg, is_inout;
     {
       oconstraints[i] = constraint  = ((const char *)(__extension__ ({ const tree __t = ((__extension__ ({ const tree __t = ((__extension__ ({ const tree __t = (link); if (((enum tree_code) (__t)->common.code) != (TREE_LIST)) tree_check_failed (__t, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1506, __FUNCTION__, (TREE_LIST), 0); __t; })->list.purpose)); if (((enum tree_code) (__t)->common.code) != (TREE_LIST)) tree_check_failed (__t, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1506, __FUNCTION__, (TREE_LIST), 0); __t; })->list.value)); if (((enum tree_code) (__t)->common.code) != (STRING_CST)) tree_check_failed (__t, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1506, __FUNCTION__, (STRING_CST), 0); __t; }
)->string.str));
       parse_output_constraint (&constraint, i, 0, 0,    &allows_mem, &allows_reg, &is_inout);
       }
 }
  get_indirect_ref_operands (tree stmt, tree expr, int flags) {
   tree *pptr = &__extension__ (*({
const tree __t = __extension__ ({ const tree __t = (expr); char const __c = tree_code_type[(int) (((enum tree_code) (__t)->common.code))]; if (!((__c) >= tcc_reference && (__c) <= tcc_expression)) tree_class_check_failed (__t, tcc_expression, "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1591, __FUNCTION__); __t; }
);
 const int __i = (0);
 if (__i < 0 || __i >= tree_code_length[(int) (((enum tree_code) (__t)->common.code))]) tree_operand_check_failed (__i, ((enum tree_code) (__t)->common.code), "/home/cygnus/dnovillo/gcc/src.ppc64/gcc/tree-ssa-operands.c", 1591, __FUNCTION__);
 &__t->exp.operands[__i];
 }
));
   tree ptr = *pptr;
     {
       struct ptr_info_def *pi = ((void *)0);
       if (((enum tree_code) (ptr)->common.code) == SSA_NAME    && pi->name_mem_tag)  {  }
     }
 }
  note_addressable (tree var, stmt_ann_t s_ann) {
   subvar_t svars;
     {
       if (var_can_have_subvars (var)    && (svars = get_subvars_for_var (var)))  bitmap_set_bit (s_ann->addresses_taken, var_ann (var)->uid);
     }
 }
  const struct ggc_root_tab gt_ggc_r_gt_tree_ssa_operands_h[] = {
   {
     &operand_memory,   }
,   {
   }
,   {
   }
,   {
   }
, };

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_tree_node:[0-9]+]] tree_node = union {
// DEFAULT-NEXT:         field0 common: @type[[TYPE_tree_common:[0-9]+]];
// DEFAULT-NEXT:         field1 string: @type[[TYPE_tree_string:[0-9]+]];
// DEFAULT-NEXT:         field2 list: @type[[TYPE_tree_list:[0-9]+]];
// DEFAULT-NEXT:         field3 exp: @type[[TYPE_tree_exp:[0-9]+]];
// DEFAULT-NEXT:         field4 ssa_name: @type[[TYPE_tree_ssa_name:[0-9]+]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree:[0-9]+]] tree = ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     type @type[[TYPE_tree_code:[0-9]+]] tree_code = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_TREE_LIST:[0-9]+]] TREE_LIST = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_ARRAY_TYPE:[0-9]+]] ARRAY_TYPE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_RECORD_TYPE:[0-9]+]] RECORD_TYPE = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_UNION_TYPE:[0-9]+]] UNION_TYPE = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_QUAL_UNION_TYPE:[0-9]+]] QUAL_UNION_TYPE = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_STRING_CST:[0-9]+]] STRING_CST = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_LT_EXPR:[0-9]+]] LT_EXPR = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE_GE_EXPR:[0-9]+]] GE_EXPR = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE_LABEL_EXPR:[0-9]+]] LABEL_EXPR = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_ASM_EXPR:[0-9]+]] ASM_EXPR = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE_SSA_NAME:[0-9]+]] SSA_NAME = const<i32>(10);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_tree_code_class:[0-9]+]] tree_code_class = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_TREE_LIST]] tcc_type = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_ARRAY_TYPE]] tcc_reference = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_RECORD_TYPE]] tcc_expression = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_tree_common]] tree_common = struct {
// DEFAULT-NEXT:         field0 ann: ptr<@type[[TYPE_tree_ann_d:[0-9]+]]>;
// DEFAULT-NEXT:         field1 code: @type[[TYPE_tree_code]] : 8;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_ann_d]] tree_ann_d = union incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_tree_string]] tree_string = struct {
// DEFAULT-NEXT:         field0 str: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_list]] tree_list = struct {
// DEFAULT-NEXT:         field0 purpose: ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:         field1 value: ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_exp]] tree_exp = struct {
// DEFAULT-NEXT:         field0 operands: array<ptr<@type[[TYPE_tree_node]]>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ssa_use_operand_d:[0-9]+]] ssa_use_operand_d = struct {
// DEFAULT-NEXT:         field0 prev: ptr<@type[[TYPE_ssa_use_operand_d]]>;
// DEFAULT-NEXT:         field1 next: ptr<@type[[TYPE_ssa_use_operand_d]]>;
// DEFAULT-NEXT:         field2 stmt: ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:         field3 use: ptr<ptr<@type[[TYPE_tree_node]]>>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_ssa_use_operand_t:[0-9]+]] ssa_use_operand_t = @type[[TYPE_ssa_use_operand_d]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_ssa_name]] tree_ssa_name = struct {
// DEFAULT-NEXT:         field0 imm_uses: @type[[TYPE_ssa_use_operand_d]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_head_def:[0-9]+]] bitmap_head_def = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_bitmap:[0-9]+]] bitmap = ptr<@type[[TYPE_bitmap_head_def]]>;
// DEFAULT-NEXT:     type @type[[TYPE_varray_data_tag:[0-9]+]] varray_data_tag = union {
// DEFAULT-NEXT:         field0 c: array<i8, 1>;
// DEFAULT-NEXT:         field1 i: array<i32, 1>;
// DEFAULT-NEXT:         field2 tp: array<ptr<ptr<@type[[TYPE_tree_node]]>>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_varray_data:[0-9]+]] varray_data = @type[[TYPE_varray_data_tag]];
// DEFAULT-NEXT:     type @type[[TYPE_varray_head_tag:[0-9]+]] varray_head_tag = struct {
// DEFAULT-NEXT:         field0 num_elements: u64;
// DEFAULT-NEXT:         field1 elements_used: u64;
// DEFAULT-NEXT:         field2 data: @type[[TYPE_varray_data_tag]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_varray_type:[0-9]+]] varray_type = ptr<@type[[TYPE_varray_head_tag]]>;
// DEFAULT-NEXT:     type @type[[TYPE_use_operand_p:[0-9]+]] use_operand_p = ptr<@type[[TYPE_ssa_use_operand_d]]>;
// DEFAULT-NEXT:     type @type[[TYPE_use_optype_d:[0-9]+]] use_optype_d = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_use_optype_d]]>;
// DEFAULT-NEXT:         field1 use_ptr: @type[[TYPE_ssa_use_operand_d]];
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_use_optype_p:[0-9]+]] use_optype_p = ptr<@type[[TYPE_use_optype_d]]>;
// DEFAULT-NEXT:     type @type[[TYPE_ssa_operand_memory_d:[0-9]+]] ssa_operand_memory_d = struct {
// DEFAULT-NEXT:         field0 mem: array<i8, 2040>;
// DEFAULT-NEXT:     } [size=2040, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_stmt_operands_d:[0-9]+]] stmt_operands_d = struct {
// DEFAULT-NEXT:         field0 use_ops: ptr<@type[[TYPE_use_optype_d]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ptr_info_def:[0-9]+]] ptr_info_def = struct {
// DEFAULT-NEXT:         field0 name_mem_tag: ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_subvar:[0-9]+]] subvar = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_subvar_t:[0-9]+]] subvar_t = ptr<@type[[TYPE_subvar]]>;
// DEFAULT-NEXT:     type @type[[TYPE_var_ann_d:[0-9]+]] var_ann_d = struct {
// DEFAULT-NEXT:         field0 uid: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_stmt_ann_d:[0-9]+]] stmt_ann_d = struct {
// DEFAULT-NEXT:         field0 modified: u32 : 1;
// DEFAULT-NEXT:         field1 operands: @type[[TYPE_stmt_operands_d]];
// DEFAULT-NEXT:         field2 addresses_taken: ptr<@type[[TYPE_bitmap_head_def]]>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_var_ann_t:[0-9]+]] var_ann_t = ptr<@type[[TYPE_var_ann_d]]>;
// DEFAULT-NEXT:     type @type[[TYPE_stmt_ann_t:[0-9]+]] stmt_ann_t = ptr<@type[[TYPE_stmt_ann_d]]>;
// DEFAULT-NEXT:     type @type[[TYPE_ggc_root_tab:[0-9]+]] ggc_root_tab = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_opbuild_list_d:[0-9]+]] opbuild_list_d = struct {
// DEFAULT-NEXT:         field0 vars: ptr<@type[[TYPE_varray_head_tag]]>;
// DEFAULT-NEXT:         field1 uid: ptr<@type[[TYPE_varray_head_tag]]>;
// DEFAULT-NEXT:         field2 next: ptr<@type[[TYPE_varray_head_tag]]>;
// DEFAULT-NEXT:         field3 first: i32;
// DEFAULT-NEXT:         field4 num: u32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 28]];
// DEFAULT-NEXT:     extern %[[VALUE_tree_code_type:[0-9]+]] tree_code_type: array<@type[[TYPE_tree_code_class]], incomplete> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_tree_code_length:[0-9]+]] tree_code_length: array<u8, incomplete> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 59> [storage=static] = code_units<array<i8, 59>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 102, 108, 111, 119, 45, 105, 110, 108, 105, 110, 101, 46, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 116, 109, 116, 95, 97, 110, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 59> [storage=static] = code_units<array<i8, 59>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 102, 108, 111, 119, 45, 105, 110, 108, 105, 110, 101, 46, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([108, 105, 110, 107, 95, 105, 109, 109, 95, 117, 115, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_build_uses:[0-9]+]] build_uses: @type[[TYPE_opbuild_list_d]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_build_v_may_defs:[0-9]+]] build_v_may_defs: @type[[TYPE_opbuild_list_d]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_operand_memory:[0-9]+]] operand_memory: ptr<@type[[TYPE_ssa_operand_memory_d]]> [storage=static] = null<ptr<@type[[TYPE_ssa_operand_memory_d]]>> [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_operand_memory_index:[0-9]+]] operand_memory_index: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_free_uses:[0-9]+]] free_uses: ptr<@type[[TYPE_use_optype_d]]> [storage=static] = null<ptr<@type[[TYPE_use_optype_d]]>> [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([111, 112, 98, 117, 105, 108, 100, 95, 110, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([111, 112, 98, 117, 105, 108, 100, 95, 101, 108, 101, 109, 95, 114, 101, 97, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([102, 105, 110, 97, 108, 105, 122, 101, 95, 115, 115, 97, 95, 117, 115, 101, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([103, 101, 116, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([103, 101, 116, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([103, 101, 116, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([103, 101, 116, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([103, 101, 116, 95, 97, 115, 109, 95, 101, 120, 112, 114, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([103, 101, 116, 95, 105, 110, 100, 105, 114, 101, 99, 116, 95, 114, 101, 102, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 60> [storage=static] = code_units<array<i8, 60>>([47, 104, 111, 109, 101, 47, 99, 121, 103, 110, 117, 115, 47, 100, 110, 111, 118, 105, 108, 108, 111, 47, 103, 99, 99, 47, 115, 114, 99, 46, 112, 112, 99, 54, 52, 47, 103, 99, 99, 47, 116, 114, 101, 101, 45, 115, 115, 97, 45, 111, 112, 101, 114, 97, 110, 100, 115, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 26> [storage=static] = code_units<array<i8, 26>>([103, 101, 116, 95, 105, 110, 100, 105, 114, 101, 99, 116, 95, 114, 101, 102, 95, 111, 112, 101, 114, 97, 110, 100, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_gt_ggc_r_gt_tree_ssa_operands_h:[0-9]+]] gt_ggc_r_gt_tree_ssa_operands_h: array<@type[[TYPE_ggc_root_tab]], 4> [storage=static] [const] = aggregate<array<@type[[TYPE_ggc_root_tab]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_ggc_root_tab]], zero_fill=false>(), index1 = aggregate<@type[[TYPE_ggc_root_tab]], zero_fill=false>(), index2 = aggregate<@type[[TYPE_ggc_root_tab]], zero_fill=false>(), index3 = aggregate<@type[[TYPE_ggc_root_tab]], zero_fill=false>()) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ARRAY_TYPE]] @fancy_abort(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_varray_check_failed:[0-9]+]] @varray_check_failed(%[[VALUE3:[0-9]+]] <unnamed>: ptr<@type[[TYPE_varray_head_tag]]>, %[[VALUE4:[0-9]+]] <unnamed>: u64, %[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_var_ann:[0-9]+]] @var_ann(%[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_var_ann_d]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_is_gimple_stmt:[0-9]+]] @is_gimple_stmt(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stmt_ann:[0-9]+]] @stmt_ann(%[[VALUE_t_2:[0-9]+]] t: ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_stmt_ann_d]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_is_gimple_stmt]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_t_2]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>, i32, ptr<const i8>) -> void>(%[[VALUE_ARRAY_TYPE]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(59)>(%[[VALUE_str]])), const<i32>(55), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:             write<i32>(%[[VALUE8]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE8]], const<i32>(0));
// DEFAULT-NEXT:         return pointer_cast<ptr<@type[[TYPE_stmt_ann_d]]>, reason=explicit>(read<ptr<@type[[TYPE_tree_ann_d]]>>(field0(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_t_2]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_stmt_modified_p:[0-9]+]] @stmt_modified_p(%[[VALUE_t_3:[0-9]+]] t: ptr<@type[[TYPE_tree_node]]>) -> u8 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ann:[0-9]+]] ann: ptr<@type[[TYPE_stmt_ann_d]]> [storage=automatic] = call<ptr<@type[[TYPE_stmt_ann_d]]>, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_stmt_ann]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_t_3]]));
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(conditional<i32>(ne<ptr<@type[[TYPE_stmt_ann_d]]>>(read<ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_ann]]), null<ptr<@type[[TYPE_stmt_ann_d]]>>), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(deref(read<ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_ann]]))))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_delink_imm_use:[0-9]+]] @delink_imm_use(%[[VALUE_linknode:[0-9]+]] linknode: ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_ssa_use_operand_d]]>>(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]])))), null<ptr<@type[[TYPE_ssa_use_operand_d]]>>)
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]])))))), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]])))))), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]]))), null<ptr<@type[[TYPE_ssa_use_operand_d]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode]]))), null<ptr<@type[[TYPE_ssa_use_operand_d]]>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_imm_use_to_list:[0-9]+]] @link_imm_use_to_list(%[[VALUE_linknode_2:[0-9]+]] linknode: ptr<@type[[TYPE_ssa_use_operand_d]]>, %[[VALUE_list:[0-9]+]] list: ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_2]]))), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_list]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_2]]))), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_list]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_list]]))), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tree_check_failed:[0-9]+]] @tree_check_failed(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_link_imm_use:[0-9]+]] @link_imm_use(%[[VALUE_linknode_3:[0-9]+]] linknode: ptr<@type[[TYPE_ssa_use_operand_d]]>, %[[VALUE_def:[0-9]+]] def: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_root:[0-9]+]] root: ptr<@type[[TYPE_ssa_use_operand_d]]> [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(ne<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_def]]), null<ptr<@type[[TYPE_tree_node]]>>)), ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_def]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))))
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_3]]))), null<ptr<@type[[TYPE_ssa_use_operand_d]]>>);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___t:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_def]]);
// DEFAULT-NEXT:                     if ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t]]), array_decay<ptr<i8>, length=Some(59)>(%[[VALUE_str_3]]), const<i32>(222), array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_4]]), const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE9]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_root]], addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(field4(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE9]]))))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void>(%[[VALUE_link_imm_use_to_list]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_3]]), read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_root]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_link_imm_use_stmt:[0-9]+]] @link_imm_use_stmt(%[[VALUE_linknode_4:[0-9]+]] linknode: ptr<@type[[TYPE_ssa_use_operand_d]]>, %[[VALUE_def_2:[0-9]+]] def: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_stmt:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_link_imm_use]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_linknode_4]]), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_def_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_opbuild_next:[0-9]+]] @opbuild_next(%[[VALUE_list_2:[0-9]+]] list: ptr<@type[[TYPE_opbuild_list_d]]>, %[[VALUE_prev:[0-9]+]] prev: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<i32> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE__va:[0-9]+]] _va: ptr<@type[[TYPE_varray_head_tag]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_varray_head_tag]]>>(field2(deref(read<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_list_2]]))));
// DEFAULT-NEXT:             let %[[VALUE__n:[0-9]+]] _n: u64 [storage=automatic] [const] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_prev]])));
// DEFAULT-NEXT:             if ge<u64>(read<u64>(%[[VALUE__n]]), read<u64>(field0(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va]])))))
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_varray_head_tag]]>, u64, ptr<const i8>, i32, ptr<const i8>) -> void>(%[[VALUE_varray_check_failed]], read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va]]), read<u64>(%[[VALUE__n]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_5]])), const<i32>(323), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:             write<ptr<i32>>(%[[VALUE10]], addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field1(field2(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va]]))))), read<u64>(%[[VALUE__n]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_opbuild_elem_real:[0-9]+]] @opbuild_elem_real(%[[VALUE_list_3:[0-9]+]] list: ptr<@type[[TYPE_opbuild_list_d]]>, %[[VALUE_elem:[0-9]+]] elem: i32) -> ptr<ptr<@type[[TYPE_tree_node]]>> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: ptr<ptr<ptr<@type[[TYPE_tree_node]]>>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE__va_2:[0-9]+]] _va: ptr<@type[[TYPE_varray_head_tag]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_varray_head_tag]]>>(field0(deref(read<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_list_3]]))));
// DEFAULT-NEXT:             let %[[VALUE__n_2:[0-9]+]] _n: u64 [storage=automatic] [const] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_elem]])));
// DEFAULT-NEXT:             if ge<u64>(read<u64>(%[[VALUE__n_2]]), read<u64>(field0(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va_2]])))))
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_varray_head_tag]]>, u64, ptr<const i8>, i32, ptr<const i8>) -> void>(%[[VALUE_varray_check_failed]], read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va_2]]), read<u64>(%[[VALUE__n_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_7]])), const<i32>(332), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:             write<ptr<ptr<ptr<@type[[TYPE_tree_node]]>>>>(%[[VALUE11]], addr_of<ptr<ptr<ptr<@type[[TYPE_tree_node]]>>>>(deref(ptr_offset<ptr<ptr<ptr<@type[[TYPE_tree_node]]>>>, subtract=false, element=ptr<ptr<@type[[TYPE_tree_node]]>>, overflow=ub>(array_decay<ptr<ptr<ptr<@type[[TYPE_tree_node]]>>>, length=Some(1)>(field2(field2(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(%[[VALUE__va_2]]))))), read<u64>(%[[VALUE__n_2]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(read<ptr<ptr<ptr<@type[[TYPE_tree_node]]>>>>(%[[VALUE11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_opbuild_clear:[0-9]+]] @opbuild_clear(%[[VALUE_list_4:[0-9]+]] list: ptr<@type[[TYPE_opbuild_list_d]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(field0(deref(read<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_list_4]])))))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type[[TYPE_varray_head_tag]]>>(field2(deref(read<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_list_4]])))))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ggc_alloc_stat:[0-9]+]] @ggc_alloc_stat(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ssa_operand_alloc:[0-9]+]] @ssa_operand_alloc(%[[VALUE_size:[0-9]+]] size: u32) -> ptr<void> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ptr:[0-9]+]] ptr: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         if ge<u64>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_operand_memory_index]]), read<u32>(%[[VALUE_size]]))), sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2048))), const<u64>(8)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_ptr]], int_to_ptr<ptr<i8>, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_ggc_alloc_stat]], const<u64>(2040))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_ptr]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2040)>(field0(deref(read<ptr<@type[[TYPE_ssa_operand_memory_d]]>>(%[[VALUE_operand_memory]])))), read<u32>(%[[VALUE_operand_memory_index]])))));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<i8>>(%[[VALUE_ptr]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_correct_use_link:[0-9]+]] @correct_use_link(%[[VALUE_ptr_2:[0-9]+]] ptr: ptr<@type[[TYPE_ssa_use_operand_d]]>, %[[VALUE_stmt_2:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_prev_2:[0-9]+]] prev: ptr<@type[[TYPE_ssa_use_operand_d]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_root_2:[0-9]+]] root: ptr<@type[[TYPE_tree_node]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_ptr_2]])))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_ssa_use_operand_d]]>>(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]]), null<ptr<@type[[TYPE_ssa_use_operand_d]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_stmt_mod:[0-9]+]] stmt_mod: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 while %[[VALUE12:[0-9]+]] ne<u8>(read<u8>(%[[VALUE_stmt_mod]]), const<u8>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         while %[[VALUE13:[0-9]+]] logical_or<bool>(eq<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(field2(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_2]])), eq<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(field2(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))), null<ptr<@type[[TYPE_tree_node]]>>))
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))));
// DEFAULT-NEXT:                         if eq<ptr<ptr<@type[[TYPE_tree_node]]>>>(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))), null<ptr<ptr<@type[[TYPE_tree_node]]>>>)
// DEFAULT-NEXT:                             write<u8>(%[[VALUE_stmt_mod]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             let %[[VALUE14:[0-9]+]]: u8 [synthetic] = call<u8, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> u8>(%[[VALUE_stmt_modified_p]], read<ptr<@type[[TYPE_tree_node]]>>(field2(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))));
// DEFAULT-NEXT:                             write<u8>(%[[VALUE_stmt_mod]], read<u8>(%[[VALUE14]]));
// DEFAULT-NEXT:                             if ne<u8>(read<u8>(%[[VALUE14]]), const<u8>(0))
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field0(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if eq<ptr<ptr<@type[[TYPE_tree_node]]>>>(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))), null<ptr<ptr<@type[[TYPE_tree_node]]>>>)
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_root_2]], read<ptr<@type[[TYPE_tree_node]]>>(field2(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_root_2]], read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_prev_2]])))))));
// DEFAULT-NEXT:                 if eq<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_root_2]]), read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_ptr_2]])))))))
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void>(%[[VALUE_delink_imm_use]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_ptr_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_link_imm_use]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_ptr_2]]), read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(deref(read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_ptr_2]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloc_use:[0-9]+]] @alloc_use() -> ptr<@type[[TYPE_use_optype_d]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: ptr<@type[[TYPE_use_optype_d]]> [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_free_uses]], read<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_free_uses]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ret]], pointer_cast<ptr<@type[[TYPE_use_optype_d]]>, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_ssa_operand_alloc]], truncate<u32, reason=arg, fits=always>(const<u64>(40)))));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finalize_ssa_use_ops:[0-9]+]] @finalize_ssa_use_ops(%[[VALUE_stmt_3:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_new_i:[0-9]+]] new_i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_old_ops:[0-9]+]] old_ops: ptr<@type[[TYPE_use_optype_d]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ptr_3:[0-9]+]] ptr: ptr<@type[[TYPE_use_optype_d]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_last:[0-9]+]] last: ptr<@type[[TYPE_use_optype_d]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_old_base:[0-9]+]] old_base: ptr<ptr<@type[[TYPE_tree_node]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_new_list:[0-9]+]] new_list: @type[[TYPE_use_optype_d]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_last]], addr_of<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_new_list]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_use_optype_d]]>>(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]), null<ptr<@type[[TYPE_use_optype_d]]>>)
// DEFAULT-NEXT:             write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_old_base]], read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]))))));
// DEFAULT-NEXT:         while %[[VALUE15:[0-9]+]] logical_and<bool>(ne<ptr<@type[[TYPE_use_optype_d]]>>(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]), null<ptr<@type[[TYPE_use_optype_d]]>>), ne<i32>(read<i32>(%[[VALUE_new_i]]), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_new_base:[0-9]+]] new_base: ptr<ptr<@type[[TYPE_tree_node]]>> [storage=automatic] = call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]]));
// DEFAULT-NEXT:                 if eq<ptr<ptr<@type[[TYPE_tree_node]]>>>(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_old_base]]), read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_new_base]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_last]]))), read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_last]], read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_correct_use_link]], addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_last]])))), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_3]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]], read<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]])))));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_new_i]], call<i32, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> i32>(%[[VALUE_opbuild_next]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<ptr<ptr<@type[[TYPE_tree_node]]>>>(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_old_base]]), read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_new_base]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_use_p:[0-9]+]] use_p: ptr<@type[[TYPE_ssa_use_operand_d]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]))));
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void>(%[[VALUE_delink_imm_use]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_use_p]]));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]], read<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]])))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]]))), read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_free_uses]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]], call<ptr<@type[[TYPE_use_optype_d]]>, signature=fn() -> ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_alloc_use]]));
// DEFAULT-NEXT:                             write<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]])))), call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])));
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_tree_node]]>, ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_link_imm_use_stmt]], addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]])))), read<ptr<@type[[TYPE_tree_node]]>>(deref(call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])))), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_3]]));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_new_i]], call<i32, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> i32>(%[[VALUE_opbuild_next]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 if ne<ptr<@type[[TYPE_use_optype_d]]>>(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]), null<ptr<@type[[TYPE_use_optype_d]]>>)
// DEFAULT-NEXT:                     write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_old_base]], read<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_old_ops]]))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_new_i]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_new_i]], call<i32, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> i32>(%[[VALUE_opbuild_next]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]], call<ptr<@type[[TYPE_use_optype_d]]>, signature=fn() -> ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_alloc_use]]));
// DEFAULT-NEXT:                     write<ptr<ptr<@type[[TYPE_tree_node]]>>>(field3(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]])))), call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])));
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>, ptr<@type[[TYPE_tree_node]]>, ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_link_imm_use_stmt]], addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]])))), read<ptr<@type[[TYPE_tree_node]]>>(deref(call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), read<i32>(%[[VALUE_new_i]])))), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_3]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_use_p_2:[0-9]+]] use_p: ptr<@type[[TYPE_ssa_use_operand_d]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_ssa_use_operand_d]]>>(field1(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]]))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_ssa_use_operand_d]]>) -> void>(%[[VALUE_delink_imm_use]], read<ptr<@type[[TYPE_ssa_use_operand_d]]>>(%[[VALUE_use_p_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_use_optype_d]]>>(field0(field1(deref(call<ptr<@type[[TYPE_stmt_ann_d]]>, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_stmt_ann]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_3]]))))), read<ptr<@type[[TYPE_use_optype_d]]>>(field0(%[[VALUE_new_list]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]], read<ptr<@type[[TYPE_use_optype_d]]>>(field0(field1(deref(call<ptr<@type[[TYPE_stmt_ann_d]]>, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_stmt_ann]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_3]])))))));
// DEFAULT-NEXT:                 condition: ne<ptr<@type[[TYPE_use_optype_d]]>>(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]]), null<ptr<@type[[TYPE_use_optype_d]]>>)
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]], read<ptr<@type[[TYPE_use_optype_d]]>>(field0(deref(read<ptr<@type[[TYPE_use_optype_d]]>>(%[[VALUE_ptr_3]])))));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     let %[[VALUE18:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE18]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_x]], read<u32>(%[[VALUE19]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finalize_ssa_uses:[0-9]+]] @finalize_ssa_uses(%[[VALUE_stmt_4:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_x_2:[0-9]+]] x: u32 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:             if not<bool>(ne<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(deref(call<ptr<ptr<@type[[TYPE_tree_node]]>>, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_opbuild_elem_real]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_x_2]]))))), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_4]])))
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, i32, ptr<const i8>) -> void>(%[[VALUE_ARRAY_TYPE]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_9]])), const<i32>(581), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE20]], const<i32>(0));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<i32>(%[[VALUE20]], const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> void>(%[[VALUE_finalize_ssa_use_ops]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_4]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>) -> void>(%[[VALUE_opbuild_clear]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_uses]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finalize_ssa_v_may_def_ops:[0-9]+]] @finalize_ssa_v_may_def_ops(%[[VALUE_stmt_5:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_new_i_2:[0-9]+]] new_i: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_new_i_2]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_new_i_2]], call<i32, signature=fn(ptr<@type[[TYPE_opbuild_list_d]]>, i32) -> i32>(%[[VALUE_opbuild_next]], addr_of<ptr<@type[[TYPE_opbuild_list_d]]>>(%[[VALUE_build_v_may_defs]]), read<i32>(%[[VALUE_new_i_2]])));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_var_can_have_subvars:[0-9]+]] @var_can_have_subvars(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_get_subvars_for_var:[0-9]+]] @get_subvars_for_var(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_swap_tree_operands:[0-9]+]] @swap_tree_operands(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tree_class_check_failed:[0-9]+]] @tree_class_check_failed(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tree_operand_check_failed:[0-9]+]] @tree_operand_check_failed(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_get_expr_operands:[0-9]+]] @get_expr_operands(%[[VALUE_stmt_6:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_expr_p:[0-9]+]] expr_p: ptr<ptr<@type[[TYPE_tree_node]]>>, %[[VALUE_flags:[0-9]+]] flags: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_code:[0-9]+]] code: @type[[TYPE_tree_code]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_expr:[0-9]+]] expr: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_expr_p]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_svars:[0-9]+]] svars: ptr<@type[[TYPE_subvar]]> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_var_can_have_subvars]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_expr]])), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE23:[0-9]+]]: ptr<@type[[TYPE_subvar]]> [synthetic] = int_to_ptr<ptr<@type[[TYPE_subvar]]>, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_get_subvars_for_var]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_expr]])));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_subvar]]>>(%[[VALUE_svars]], read<ptr<@type[[TYPE_subvar]]>>(%[[VALUE23]]));
// DEFAULT-NEXT:                     write<bool>(%[[VALUE22]], ne<ptr<@type[[TYPE_subvar]]>>(read<ptr<@type[[TYPE_subvar]]>>(%[[VALUE23]]), null<ptr<@type[[TYPE_subvar]]>>));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE22]], const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE22]])
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_or<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(%[[VALUE_code]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6))), eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(%[[VALUE_code]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE24:[0-9]+]]: ptr<ptr<@type[[TYPE_tree_node]]>> [synthetic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE___t_2:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:                                 let %[[VALUE25:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE___t_3:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_expr]]);
// DEFAULT-NEXT:                                     let %[[VALUE___c:[0-9]+]] __c: i8 [storage=automatic] [const] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code_class]]>(deref(ptr_offset<ptr<const @type[[TYPE_tree_code_class]]>, subtract=false, element=@type[[TYPE_tree_code_class]], overflow=ub>(array_decay<ptr<const @type[[TYPE_tree_code_class]]>, length=None>(%[[VALUE_tree_code_type]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_3]])))))))))))))))));
// DEFAULT-NEXT:                                     if not<bool>(logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c]])), const<i32>(1)), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c]])), const<i32>(2))))
// DEFAULT-NEXT:                                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_class_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_3]]), const<i32>(2), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_11]]), const<i32>(1435), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE25]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_3]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_2]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE25]]));
// DEFAULT-NEXT:                                 let %[[VALUE___i:[0-9]+]] __i: i32 [storage=automatic] [const] = const<i32>(0);
// DEFAULT-NEXT:                                 if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE___i]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE___i]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=None>(%[[VALUE_tree_code_length]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_2]]))))))))))))))))))
// DEFAULT-NEXT:                                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_operand_check_failed]], read<i32>(%[[VALUE___i]]), enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_2]])))))))))), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_13]]), const<i32>(1435), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:                                 write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE24]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field0(field3(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_2]]))))), read<i32>(%[[VALUE___i]])))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             let %[[VALUE26:[0-9]+]]: ptr<ptr<@type[[TYPE_tree_node]]>> [synthetic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE___t_4:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:                                 let %[[VALUE27:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE___t_5:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_expr]]);
// DEFAULT-NEXT:                                     let %[[VALUE___c_2:[0-9]+]] __c: i8 [storage=automatic] [const] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code_class]]>(deref(ptr_offset<ptr<const @type[[TYPE_tree_code_class]]>, subtract=false, element=@type[[TYPE_tree_code_class]], overflow=ub>(array_decay<ptr<const @type[[TYPE_tree_code_class]]>, length=None>(%[[VALUE_tree_code_type]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_5]])))))))))))))))));
// DEFAULT-NEXT:                                     if not<bool>(logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_2]])), const<i32>(1)), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_2]])), const<i32>(2))))
// DEFAULT-NEXT:                                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_class_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_5]]), const<i32>(2), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_15]]), const<i32>(1436), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE27]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_5]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_4]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE27]]));
// DEFAULT-NEXT:                                 let %[[VALUE___i_2:[0-9]+]] __i: i32 [storage=automatic] [const] = const<i32>(1);
// DEFAULT-NEXT:                                 if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE___i_2]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE___i_2]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=None>(%[[VALUE_tree_code_length]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_4]]))))))))))))))))))
// DEFAULT-NEXT:                                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_operand_check_failed]], read<i32>(%[[VALUE___i_2]]), enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_4]])))))))))), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_17]]), const<i32>(1436), array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:                                 write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE26]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field0(field3(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_4]]))))), read<i32>(%[[VALUE___i_2]])))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_swap_tree_operands]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_6]]), addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE24]]))), addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE26]]))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_list_length:[0-9]+]] @list_length(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_parse_output_constraint:[0-9]+]] @parse_output_constraint(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE28:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_get_asm_expr_operands:[0-9]+]] @get_asm_expr_operands(%[[VALUE_stmt_7:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_noutputs:[0-9]+]] noutputs: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: ptr<ptr<@type[[TYPE_tree_node]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___t_6:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___t_7:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___t_8:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_stmt_7]]);
// DEFAULT-NEXT:                     if ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_8]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9)))
// DEFAULT-NEXT:                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_8]]), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_19]]), const<i32>(1495), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_20]]), const<i32>(9), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE31]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_8]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_7]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE31]]));
// DEFAULT-NEXT:                 let %[[VALUE___c_3:[0-9]+]] __c: i8 [storage=automatic] [const] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code_class]]>(deref(ptr_offset<ptr<const @type[[TYPE_tree_code_class]]>, subtract=false, element=@type[[TYPE_tree_code_class]], overflow=ub>(array_decay<ptr<const @type[[TYPE_tree_code_class]]>, length=None>(%[[VALUE_tree_code_type]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_7]])))))))))))))))));
// DEFAULT-NEXT:                 if not<bool>(logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_3]])), const<i32>(1)), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_3]])), const<i32>(2))))
// DEFAULT-NEXT:                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_class_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_7]]), const<i32>(2), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_21]]), const<i32>(1495), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE30]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_6]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE30]]));
// DEFAULT-NEXT:             let %[[VALUE___i_3:[0-9]+]] __i: i32 [storage=automatic] [const] = const<i32>(1);
// DEFAULT-NEXT:             if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE___i_3]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE___i_3]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=None>(%[[VALUE_tree_code_length]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_6]]))))))))))))))))))
// DEFAULT-NEXT:                 call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_operand_check_failed]], read<i32>(%[[VALUE___i_3]]), enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_6]])))))))))), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_23]]), const<i32>(1495), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:             write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE29]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field0(field3(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_6]]))))), read<i32>(%[[VALUE___i_3]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_noutputs]], call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_list_length]], read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE29]])))));
// DEFAULT-NEXT:         let %[[VALUE_oconstraints:[0-9]+]] oconstraints: ptr<ptr<const i8>> [storage=automatic] = pointer_cast<ptr<ptr<const i8>>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_noutputs]]))), const<u64>(8))));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_link:[0-9]+]] link: ptr<@type[[TYPE_tree_node]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_constraint:[0-9]+]] constraint: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_allows_mem:[0-9]+]] allows_mem: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_allows_reg:[0-9]+]] allows_reg: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_is_inout:[0-9]+]] is_inout: u8 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___t_9:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___t_10:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:                     let %[[VALUE34:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE___t_11:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_link]]);
// DEFAULT-NEXT:                         if ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_11]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_11]]), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_25]]), const<i32>(1506), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_26]]), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE34]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_11]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_10]], read<ptr<@type[[TYPE_tree_node]]>>(field0(field2(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE34]]))))));
// DEFAULT-NEXT:                     if ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_10]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_10]]), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_27]]), const<i32>(1506), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_28]]), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE33]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_10]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_9]], read<ptr<@type[[TYPE_tree_node]]>>(field1(field2(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE33]]))))));
// DEFAULT-NEXT:                 if ne<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_9]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))
// DEFAULT-NEXT:                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_9]]), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_29]]), const<i32>(1506), array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_30]]), const<i32>(5), const<i32>(0));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE32]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: ptr<const i8> [synthetic] = pointer_cast<ptr<const i8>, reason=explicit>(array_decay<ptr<i8>, length=Some(1)>(field0(field1(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE32]]))))));
// DEFAULT-NEXT:             write<ptr<const i8>>(%[[VALUE_constraint]], read<ptr<const i8>>(%[[VALUE35]]));
// DEFAULT-NEXT:             write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%[[VALUE_oconstraints]]), read<i32>(%[[VALUE_i]]))), read<ptr<const i8>>(%[[VALUE35]]));
// DEFAULT-NEXT:             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_parse_output_constraint]], addr_of<ptr<ptr<const i8>>>(%[[VALUE_constraint]]), read<i32>(%[[VALUE_i]]), const<i32>(0), const<i32>(0), addr_of<ptr<u8>>(%[[VALUE_allows_mem]]), addr_of<ptr<u8>>(%[[VALUE_allows_reg]]), addr_of<ptr<u8>>(%[[VALUE_is_inout]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_indirect_ref_operands:[0-9]+]] @get_indirect_ref_operands(%[[VALUE_stmt_8:[0-9]+]] stmt: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_expr_2:[0-9]+]] expr: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_flags_2:[0-9]+]] flags: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pptr:[0-9]+]] pptr: ptr<ptr<@type[[TYPE_tree_node]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: ptr<ptr<@type[[TYPE_tree_node]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___t_12:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const];
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: ptr<@type[[TYPE_tree_node]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___t_13:[0-9]+]] __t: ptr<@type[[TYPE_tree_node]]> [storage=automatic] [const] = read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_expr_2]]);
// DEFAULT-NEXT:                 let %[[VALUE___c_4:[0-9]+]] __c: i8 [storage=automatic] [const] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code_class]]>(deref(ptr_offset<ptr<const @type[[TYPE_tree_code_class]]>, subtract=false, element=@type[[TYPE_tree_code_class]], overflow=ub>(array_decay<ptr<const @type[[TYPE_tree_code_class]]>, length=None>(%[[VALUE_tree_code_type]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_13]])))))))))))))))));
// DEFAULT-NEXT:                 if not<bool>(logical_and<bool>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_4]])), const<i32>(1)), le<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE___c_4]])), const<i32>(2))))
// DEFAULT-NEXT:                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_class_check_failed]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_13]]), const<i32>(2), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_31]]), const<i32>(1591), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE37]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_12]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE37]]));
// DEFAULT-NEXT:             let %[[VALUE___i_4:[0-9]+]] __i: i32 [storage=automatic] [const] = const<i32>(0);
// DEFAULT-NEXT:             if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE___i_4]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE___i_4]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=None>(%[[VALUE_tree_code_length]]), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_12]]))))))))))))))))))
// DEFAULT-NEXT:                 call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_tree_operand_check_failed]], read<i32>(%[[VALUE___i_4]]), enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_12]])))))))))), array_decay<ptr<i8>, length=Some(60)>(%[[VALUE_str_33]]), const<i32>(1591), array_decay<ptr<i8>, length=Some(26)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:             write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE36]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_tree_node]]>>, subtract=false, element=ptr<@type[[TYPE_tree_node]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_tree_node]]>>, length=Some(1)>(field0(field3(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE___t_12]]))))), read<i32>(%[[VALUE___i_4]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_pptr]], addr_of<ptr<ptr<@type[[TYPE_tree_node]]>>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE36]]))));
// DEFAULT-NEXT:         let %[[VALUE_ptr_4:[0-9]+]] ptr: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = read<ptr<@type[[TYPE_tree_node]]>>(deref(read<ptr<ptr<@type[[TYPE_tree_node]]>>>(%[[VALUE_pptr]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_pi:[0-9]+]] pi: ptr<@type[[TYPE_ptr_info_def]]> [storage=automatic] = null<ptr<@type[[TYPE_ptr_info_def]]>>;
// DEFAULT-NEXT:             if logical_and<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(int_to_enum<@type[[TYPE_tree_code]], reason=explicit>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_tree_code]]>(bitfield1<unit=0, bytes=8..9, bits=0..8>(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_ptr_4]])))))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))), ne<ptr<@type[[TYPE_tree_node]]>>(read<ptr<@type[[TYPE_tree_node]]>>(field0(deref(read<ptr<@type[[TYPE_ptr_info_def]]>>(%[[VALUE_pi]])))), null<ptr<@type[[TYPE_tree_node]]>>))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bitmap_set_bit:[0-9]+]] @bitmap_set_bit(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_note_addressable:[0-9]+]] @note_addressable(%[[VALUE_var:[0-9]+]] var: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_s_ann:[0-9]+]] s_ann: ptr<@type[[TYPE_stmt_ann_d]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_svars_2:[0-9]+]] svars: ptr<@type[[TYPE_subvar]]> [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<i32>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_var_can_have_subvars]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_var]])), const<i32>(0))
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: ptr<@type[[TYPE_subvar]]> [synthetic] = int_to_ptr<ptr<@type[[TYPE_subvar]]>, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_get_subvars_for_var]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_var]])));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_subvar]]>>(%[[VALUE_svars_2]], read<ptr<@type[[TYPE_subvar]]>>(%[[VALUE39]]));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE38]], ne<ptr<@type[[TYPE_subvar]]>>(read<ptr<@type[[TYPE_subvar]]>>(%[[VALUE39]]), null<ptr<@type[[TYPE_subvar]]>>));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE38]], const<bool>(false));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:                 call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_bitmap_set_bit]], read<ptr<@type[[TYPE_bitmap_head_def]]>>(field2(deref(read<ptr<@type[[TYPE_stmt_ann_d]]>>(%[[VALUE_s_ann]])))), read<u64>(field0(deref(call<ptr<@type[[TYPE_var_ann_d]]>, signature=fn(ptr<@type[[TYPE_tree_node]]>) -> ptr<@type[[TYPE_var_ann_d]]>>(%[[VALUE_var_ann]], read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_var]]))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
