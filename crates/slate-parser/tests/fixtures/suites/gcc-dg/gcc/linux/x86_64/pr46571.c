/* { dg-do compile } */
/* { dg-options "-O2 -fpermissive -fcompare-debug -w" } */

typedef union tree_node *tree;
typedef unsigned int source_location;
enum tree_code
{
  MINUS_EXPR,
  MULT_EXPR,
};
struct tree_omp_clause
{
  union omp_clause_subcode
  {
    enum tree_code reduction_code;
  } subcode;
};
union tree_node
{
  struct tree_omp_clause omp_clause;
};
enum tree_index
{
  TI_ERROR_MARK,
};
typedef struct
{
  unsigned allocatable:1;
  unsigned dimension:1;
  unsigned codimension:1;
  unsigned external:1;
  unsigned optional:1;
  unsigned pointer:1;
  unsigned contiguous:1;
  unsigned referenced:1;
} symbol_attribute;
typedef unsigned int gfc_char_t;
typedef struct gfc_linebuf
{
  source_location location;
} gfc_linebuf;
typedef struct
{
  gfc_char_t *nextc;
  gfc_linebuf *lb;
} locus;
typedef struct
{
  struct gfc_symbol *sym;
  struct gfc_namelist *next;
} gfc_namelist;
enum
{
  OMP_LIST_PLUS,
  OMP_LIST_REDUCTION_FIRST = OMP_LIST_PLUS,
  OMP_LIST_MULT,
  OMP_LIST_SUB,
  OMP_LIST_NUM
};
typedef struct gfc_omp_clauses
{
  gfc_namelist *lists[OMP_LIST_NUM];
} gfc_omp_clauses;
typedef struct gfc_symbol
{
  symbol_attribute attr;
} gfc_symbol;
typedef struct gfc_code
{
  locus loc;
  union
  {
    gfc_omp_clauses *omp_clauses;
  } ext;
} gfc_code;
typedef struct
{
} stmtblock_t;

static tree
gfc_trans_omp_reduction_list (gfc_namelist * namelist, tree list,
			      enum tree_code reduction_code, locus where)
{
  for (; namelist != ((void *) 0); namelist = namelist->next)
    if (namelist->sym->attr.referenced)
      {
	tree node = build_omp_clause (where.lb->location);
	node->omp_clause.subcode.reduction_code = reduction_code;
	gfc_trans_omp_array_reduction (namelist->sym, where);
      }
}

static tree
gfc_trans_omp_clauses (stmtblock_t * block, gfc_omp_clauses * clauses,
		       locus where)
{
  tree omp_clauses = (tree) ((void *) 0);
  int list;
  for (list = 0; list < OMP_LIST_NUM; list++)
    {
      gfc_namelist *n = clauses->lists[list];
      enum tree_code reduction_code;
      if (n == ((void *) 0))
	continue;
      switch (list)
	{
	case OMP_LIST_MULT:
	  reduction_code = MULT_EXPR;
	  break;
	case OMP_LIST_SUB:
	  reduction_code = MINUS_EXPR;
	}
      gfc_trans_omp_reduction_list (n, omp_clauses, reduction_code, where);
    }
}

void
gfc_trans_omp_parallel_workshare (gfc_code * code)
{
  stmtblock_t block;
  gfc_trans_omp_clauses (&block, code->ext.omp_clauses, code->loc);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_tree_node:[0-9]+]] tree_node = union {
// DEFAULT-NEXT:         field0 omp_clause: @type[[TYPE_tree_omp_clause:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree:[0-9]+]] tree = ptr<@type[[TYPE_tree_node]]>;
// DEFAULT-NEXT:     type @type[[TYPE_source_location:[0-9]+]] source_location = u32;
// DEFAULT-NEXT:     type @type[[TYPE_tree_code:[0-9]+]] tree_code = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_MINUS_EXPR:[0-9]+]] MINUS_EXPR = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_MULT_EXPR:[0-9]+]] MULT_EXPR = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_tree_omp_clause]] tree_omp_clause = struct {
// DEFAULT-NEXT:         field0 subcode: @type[[TYPE_omp_clause_subcode:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_omp_clause_subcode]] omp_clause_subcode = union {
// DEFAULT-NEXT:         field0 reduction_code: @type[[TYPE_tree_code]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_index:[0-9]+]] tree_index = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_MINUS_EXPR]] TI_ERROR_MARK = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 allocatable: u32 : 1;
// DEFAULT-NEXT:         field1 dimension: u32 : 1;
// DEFAULT-NEXT:         field2 codimension: u32 : 1;
// DEFAULT-NEXT:         field3 external: u32 : 1;
// DEFAULT-NEXT:         field4 optional: u32 : 1;
// DEFAULT-NEXT:         field5 pointer: u32 : 1;
// DEFAULT-NEXT:         field6 contiguous: u32 : 1;
// DEFAULT-NEXT:         field7 referenced: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 0, 0, 0, 0, 0], bit_offsets=[Some(0), Some(1), Some(2), Some(3), Some(4), Some(5), Some(6), Some(7)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_symbol_attribute:[0-9]+]] symbol_attribute = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_char_t:[0-9]+]] gfc_char_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_gfc_linebuf:[0-9]+]] gfc_linebuf = struct {
// DEFAULT-NEXT:         field0 location: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_linebuf_2:[0-9]+]] gfc_linebuf = @type[[TYPE_gfc_linebuf]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 nextc: ptr<u32>;
// DEFAULT-NEXT:         field1 lb: ptr<@type[[TYPE_gfc_linebuf]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_locus:[0-9]+]] locus = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 sym: ptr<@type[[TYPE_gfc_symbol:[0-9]+]]>;
// DEFAULT-NEXT:         field1 next: ptr<@type[[TYPE_gfc_namelist:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_symbol]] gfc_symbol = struct {
// DEFAULT-NEXT:         field0 attr: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_namelist]] gfc_namelist = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_gfc_namelist_2:[0-9]+]] gfc_namelist = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_MINUS_EXPR]] OMP_LIST_PLUS = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_MULT_EXPR]] OMP_LIST_REDUCTION_FIRST = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_OMP_LIST_MULT:[0-9]+]] OMP_LIST_MULT = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_OMP_LIST_SUB:[0-9]+]] OMP_LIST_SUB = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_OMP_LIST_NUM:[0-9]+]] OMP_LIST_NUM = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_omp_clauses:[0-9]+]] gfc_omp_clauses = struct {
// DEFAULT-NEXT:         field0 lists: array<ptr<@type[[TYPE2]]>, 3>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_omp_clauses_2:[0-9]+]] gfc_omp_clauses = @type[[TYPE_gfc_omp_clauses]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_symbol_2:[0-9]+]] gfc_symbol = @type[[TYPE_gfc_symbol]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_code:[0-9]+]] gfc_code = struct {
// DEFAULT-NEXT:         field0 loc: @type[[TYPE1]];
// DEFAULT-NEXT:         field1 ext: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = union {
// DEFAULT-NEXT:         field0 omp_clauses: ptr<@type[[TYPE_gfc_omp_clauses]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_code_2:[0-9]+]] gfc_code = @type[[TYPE_gfc_code]];
// DEFAULT-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_stmtblock_t:[0-9]+]] stmtblock_t = @type[[TYPE5]];
// DEFAULT-NEXT:     fn %[[VALUE_build_omp_clause:[0-9]+]] @build_omp_clause(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gfc_trans_omp_array_reduction:[0-9]+]] @gfc_trans_omp_array_reduction(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gfc_trans_omp_reduction_list:[0-9]+]] @gfc_trans_omp_reduction_list(%[[VALUE_namelist:[0-9]+]] namelist: ptr<@type[[TYPE2]]>, %[[VALUE_list:[0-9]+]] list: ptr<@type[[TYPE_tree_node]]>, %[[VALUE_reduction_code:[0-9]+]] reduction_code: @type[[TYPE_tree_code]], %[[VALUE_where:[0-9]+]] where: @type[[TYPE1]]) -> ptr<@type[[TYPE_tree_node]]> [linkage=internal] [abi=sysv64(scalar, scalar, scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<ptr<@type[[TYPE2]]>>(read<ptr<@type[[TYPE2]]>>(%[[VALUE_namelist]]), null<ptr<@type[[TYPE2]]>>)
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE2]]>>(%[[VALUE_namelist]], pointer_cast<ptr<@type[[TYPE2]]>, reason=assign>(read<ptr<@type[[TYPE_gfc_namelist]]>>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_namelist]]))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=0, bytes=0..1, bits=7..8>(field0(deref(read<ptr<@type[[TYPE_gfc_symbol]]>>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_namelist]]))))))))), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_node:[0-9]+]] node: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = int_to_ptr<ptr<@type[[TYPE_tree_node]]>, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_build_omp_clause]], read<u32>(field0(deref(read<ptr<@type[[TYPE_gfc_linebuf]]>>(field1(%[[VALUE_where]])))))));
// DEFAULT-NEXT:                         write<@type[[TYPE_tree_code]]>(field0(field0(field0(deref(read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_node]]))))), read<@type[[TYPE_tree_code]]>(%[[VALUE_reduction_code]]));
// DEFAULT-NEXT:                         call<i32, signature=fn(unprototyped) -> i32, abi=sysv64(scalar, native_c) -> scalar>(%[[VALUE_gfc_trans_omp_array_reduction]], read<ptr<@type[[TYPE_gfc_symbol]]>>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_namelist]])))), copy<@type[[TYPE1]], reason=vararg>(read<@type[[TYPE1]]>(%[[VALUE_where]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gfc_trans_omp_clauses:[0-9]+]] @gfc_trans_omp_clauses(%[[VALUE_block:[0-9]+]] block: ptr<@type[[TYPE5]]>, %[[VALUE_clauses:[0-9]+]] clauses: ptr<@type[[TYPE_gfc_omp_clauses]]>, %[[VALUE_where_2:[0-9]+]] where: @type[[TYPE1]]) -> ptr<@type[[TYPE_tree_node]]> [linkage=internal] [abi=sysv64(scalar, scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_omp_clauses:[0-9]+]] omp_clauses: ptr<@type[[TYPE_tree_node]]> [storage=automatic] = null<ptr<@type[[TYPE_tree_node]]>>;
// DEFAULT-NEXT:         let %[[VALUE_list_2:[0-9]+]] list: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_list_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_list_2]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_list_2]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_list_2]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_n:[0-9]+]] n: ptr<@type[[TYPE2]]> [storage=automatic] = read<ptr<@type[[TYPE2]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE2]]>>, subtract=false, element=ptr<@type[[TYPE2]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE2]]>>, length=Some(3)>(field0(deref(read<ptr<@type[[TYPE_gfc_omp_clauses]]>>(%[[VALUE_clauses]])))), read<i32>(%[[VALUE_list_2]]))));
// DEFAULT-NEXT:                     let %[[VALUE_reduction_code_2:[0-9]+]] reduction_code: @type[[TYPE_tree_code]] [storage=automatic];
// DEFAULT-NEXT:                     if eq<ptr<@type[[TYPE2]]>>(read<ptr<@type[[TYPE2]]>>(%[[VALUE_n]]), null<ptr<@type[[TYPE2]]>>)
// DEFAULT-NEXT:                         continue %[[VALUE1]];
// DEFAULT-NEXT:                     switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_list_2]])
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE4]] const<i32>(1):
// DEFAULT-NEXT:                                 write<@type[[TYPE_tree_code]]>(%[[VALUE_reduction_code_2]], int_to_enum<@type[[TYPE_tree_code]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                             break %[[VALUE4]];
// DEFAULT-NEXT:                             case %[[VALUE4]] const<i32>(2):
// DEFAULT-NEXT:                                 write<@type[[TYPE_tree_code]]>(%[[VALUE_reduction_code_2]], int_to_enum<@type[[TYPE_tree_code]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<ptr<@type[[TYPE_tree_node]]>, signature=fn(ptr<@type[[TYPE2]]>, ptr<@type[[TYPE_tree_node]]>, @type[[TYPE_tree_code]], @type[[TYPE1]]) -> ptr<@type[[TYPE_tree_node]]>, abi=sysv64(scalar, scalar, scalar, native_c) -> scalar>(%[[VALUE_gfc_trans_omp_reduction_list]], read<ptr<@type[[TYPE2]]>>(%[[VALUE_n]]), read<ptr<@type[[TYPE_tree_node]]>>(%[[VALUE_omp_clauses]]), read<@type[[TYPE_tree_code]]>(%[[VALUE_reduction_code_2]]), copy<@type[[TYPE1]], reason=arg>(read<@type[[TYPE1]]>(%[[VALUE_where_2]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gfc_trans_omp_parallel_workshare:[0-9]+]] @gfc_trans_omp_parallel_workshare(%[[VALUE_code:[0-9]+]] code: ptr<@type[[TYPE_gfc_code]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_block_2:[0-9]+]] block: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_tree_node]]>, signature=fn(ptr<@type[[TYPE5]]>, ptr<@type[[TYPE_gfc_omp_clauses]]>, @type[[TYPE1]]) -> ptr<@type[[TYPE_tree_node]]>, abi=sysv64(scalar, scalar, native_c) -> scalar>(%[[VALUE_gfc_trans_omp_clauses]], addr_of<ptr<@type[[TYPE5]]>>(%[[VALUE_block_2]]), read<ptr<@type[[TYPE_gfc_omp_clauses]]>>(field0(field1(deref(read<ptr<@type[[TYPE_gfc_code]]>>(%[[VALUE_code]]))))), copy<@type[[TYPE1]], reason=arg>(read<@type[[TYPE1]]>(field0(deref(read<ptr<@type[[TYPE_gfc_code]]>>(%[[VALUE_code]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
