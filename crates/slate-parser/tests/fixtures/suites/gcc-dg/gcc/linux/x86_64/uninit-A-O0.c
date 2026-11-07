/* Inspired by part of java/parse.y.
   May be a real bug in CSE. */

/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition -Wall" } */

struct tree
{
    struct tree *car, *cdr, *wfl;
    int code;
    struct { unsigned int renp:1;
      unsigned int rtnp:1;
      unsigned int rpnp:1; } flags;
};
typedef struct tree *tree;
#define NULL_TREE ((tree)0)

/* Codes */
enum
{
    CALL_EXPR, NEW_ARRAY_EXPR, NEW_CLASS_EXPR, CONVERT_EXPR,
    ARRAY_REF, CONDITIONAL_EXPR, STRING_CST, EXPR_WITH_FILE_LOCATION
};

/* Flags */
#define RESOLVE_EXPRESSION_NAME_P(t) ((t)->flags.renp)
#define RESOLVE_TYPE_NAME_P(t) ((t)->flags.rtnp)
#define RESOLVE_PACKAGE_NAME_P(t) ((t)->flags.rpnp)

/* Macros */
#define EXPR_WFL_QUALIFICATION(t) ((t)->wfl)
#define QUAL_WFL(t) ((t)->wfl)
#define EXPR_WFL_NODE(t) ((t)->wfl)
#define TREE_CODE(t) ((t)->code)
#define TREE_OPERAND(t,x) ((t)->car)
#define CLASSTYPE_SUPER(t) ((t)->car)
#define IDENTIFIER_LOCAL_VALUE(t) ((t)->car)
#define TREE_CHAIN(t) ((t)->cdr)
#define QUAL_RESOLUTION(t) ((t)->cdr)

extern tree current_class, this_identifier_node;
extern tree super_identifier_node, length_identifier_node;

tree resolve_and_layout (tree, tree);
tree lookup_field_wrapper (tree, tree);

void
qualify_ambiguous_name (id)
     tree id;
{
  tree qual, qual_wfl, decl;
  tree name;	 /* { dg-bogus "name" "uninitialized variable warning" } */
  tree ptr_type; /* { dg-bogus "ptr_type" "uninitialized variable warning" } */
  int again, new_array_found = 0;
  int super_found = 0, this_found = 0;

  qual = EXPR_WFL_QUALIFICATION (id);
  do {
    qual_wfl = QUAL_WFL (qual);
    switch (TREE_CODE (qual_wfl))
      {
      case CALL_EXPR:
	qual_wfl = TREE_OPERAND (qual_wfl, 0);
	if (TREE_CODE (qual_wfl) != EXPR_WITH_FILE_LOCATION)
	  {
	    qual = EXPR_WFL_QUALIFICATION (qual_wfl);
	    qual_wfl = QUAL_WFL (qual);
	  }
	break;
      case NEW_ARRAY_EXPR:
	qual = TREE_CHAIN (qual);
	new_array_found = again = 1;
	continue;
      case NEW_CLASS_EXPR:
      case CONVERT_EXPR:
	qual_wfl = TREE_OPERAND (qual_wfl, 0);
	break;
      case ARRAY_REF:
	while (TREE_CODE (qual_wfl) == ARRAY_REF)
	  qual_wfl = TREE_OPERAND (qual_wfl, 0);
	break;
      default:
	break;
      }

    name = EXPR_WFL_NODE (qual_wfl);
    ptr_type = current_class;
    again = 0;

  } while (again);

  /* If you put straightforward uses of name and ptr_type here
     instead of the if-else sequence below, the warnings go away.
     Therefore I suspect a real bug. */
  
  if (!this_found && !super_found && (decl = IDENTIFIER_LOCAL_VALUE (name)))
    {
      RESOLVE_EXPRESSION_NAME_P (qual_wfl) = 1;
      QUAL_RESOLUTION (qual) = decl;
    }
  else if ((decl = lookup_field_wrapper (ptr_type, name))
	   || (new_array_found && name == length_identifier_node))
    {
      RESOLVE_EXPRESSION_NAME_P (qual_wfl) = 1;
      QUAL_RESOLUTION (qual) = (new_array_found ? NULL_TREE : decl);
    }
  else if ((decl = resolve_and_layout (name, NULL_TREE)))
    {
      RESOLVE_TYPE_NAME_P (qual_wfl) = 1;
      QUAL_RESOLUTION (qual) = decl;
    }
  else if (TREE_CODE (QUAL_WFL (qual)) == CALL_EXPR
	   || TREE_CODE (QUAL_WFL (qual)) == ARRAY_REF)
    RESOLVE_EXPRESSION_NAME_P (qual_wfl) = 1;
  else 
    RESOLVE_PACKAGE_NAME_P (qual_wfl) = 1;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_tree:[0-9]+]] tree = struct {
// DEFAULT-NEXT:         field0 car: ptr<@type[[TYPE_tree]]>;
// DEFAULT-NEXT:         field1 cdr: ptr<@type[[TYPE_tree]]>;
// DEFAULT-NEXT:         field2 wfl: ptr<@type[[TYPE_tree]]>;
// DEFAULT-NEXT:         field3 code: i32;
// DEFAULT-NEXT:         field4 flags: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 28]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 renp: u32 : 1;
// DEFAULT-NEXT:         field1 rtnp: u32 : 1;
// DEFAULT-NEXT:         field2 rpnp: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tree_2:[0-9]+]] tree = ptr<@type[[TYPE_tree]]>;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_CALL_EXPR:[0-9]+]] CALL_EXPR = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_NEW_ARRAY_EXPR:[0-9]+]] NEW_ARRAY_EXPR = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_NEW_CLASS_EXPR:[0-9]+]] NEW_CLASS_EXPR = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_CONVERT_EXPR:[0-9]+]] CONVERT_EXPR = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_ARRAY_REF:[0-9]+]] ARRAY_REF = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_CONDITIONAL_EXPR:[0-9]+]] CONDITIONAL_EXPR = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_STRING_CST:[0-9]+]] STRING_CST = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE_EXPR_WITH_FILE_LOCATION:[0-9]+]] EXPR_WITH_FILE_LOCATION = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     extern %[[VALUE_current_class:[0-9]+]] current_class: ptr<@type[[TYPE_tree]]> [storage=static] [linkage=external] [c="tree"];
// DEFAULT-NEXT:     extern %[[VALUE_this_identifier_node:[0-9]+]] this_identifier_node: ptr<@type[[TYPE_tree]]> [storage=static] [linkage=external] [c="tree"];
// DEFAULT-NEXT:     extern %[[VALUE_super_identifier_node:[0-9]+]] super_identifier_node: ptr<@type[[TYPE_tree]]> [storage=static] [linkage=external] [c="tree"];
// DEFAULT-NEXT:     extern %[[VALUE_length_identifier_node:[0-9]+]] length_identifier_node: ptr<@type[[TYPE_tree]]> [storage=static] [linkage=external] [c="tree"];
// DEFAULT-NEXT:     fn %[[VALUE_resolve_and_layout:[0-9]+]] @resolve_and_layout(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_tree]]> [c="tree"], %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_tree]]> [c="tree"]) -> ptr<@type[[TYPE_tree]]> [linkage=external] [c="tree(tree, tree)"];
// DEFAULT-NEXT:     fn %[[VALUE_lookup_field_wrapper:[0-9]+]] @lookup_field_wrapper(%[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_tree]]> [c="tree"], %[[VALUE3:[0-9]+]] <unnamed>: ptr<@type[[TYPE_tree]]> [c="tree"]) -> ptr<@type[[TYPE_tree]]> [linkage=external] [c="tree(tree, tree)"];
// DEFAULT-NEXT:     fn %[[VALUE_qualify_ambiguous_name:[0-9]+]] @qualify_ambiguous_name(%[[VALUE_id:[0-9]+]] id: ptr<@type[[TYPE_tree]]> [c="tree"]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_qual:[0-9]+]] qual: ptr<@type[[TYPE_tree]]> [storage=automatic] [c="tree"];
// DEFAULT-NEXT:         let %[[VALUE_qual_wfl:[0-9]+]] qual_wfl: ptr<@type[[TYPE_tree]]> [storage=automatic] [c="tree"];
// DEFAULT-NEXT:         let %[[VALUE_decl:[0-9]+]] decl: ptr<@type[[TYPE_tree]]> [storage=automatic] [c="tree"];
// DEFAULT-NEXT:         let %[[VALUE_name:[0-9]+]] name: ptr<@type[[TYPE_tree]]> [storage=automatic] [c="tree"];
// DEFAULT-NEXT:         let %[[VALUE_ptr_type:[0-9]+]] ptr_type: ptr<@type[[TYPE_tree]]> [storage=automatic] [c="tree"];
// DEFAULT-NEXT:         let %[[VALUE_again:[0-9]+]] again: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_new_array_found:[0-9]+]] new_array_found: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_super_found:[0-9]+]] super_found: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_this_found:[0-9]+]] this_found: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]], read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_id]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]], read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                 switch %[[VALUE5:[0-9]+]] read<i32>(field3(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]]))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE5]] const<i32>(0):
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]], read<ptr<@type[[TYPE_tree]]>>(field0(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), const<i32>(7))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]], read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]], read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         break %[[VALUE5]];
// DEFAULT-NEXT:                         case %[[VALUE5]] const<i32>(1):
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]], read<ptr<@type[[TYPE_tree]]>>(field1(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_again]], const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_new_array_found]], const<i32>(1));
// DEFAULT-NEXT:                         continue %[[VALUE4]];
// DEFAULT-NEXT:                         case %[[VALUE5]] const<i32>(2):
// DEFAULT-NEXT:                             case %[[VALUE5]] const<i32>(3):
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]], read<ptr<@type[[TYPE_tree]]>>(field0(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                         break %[[VALUE5]];
// DEFAULT-NEXT:                         case %[[VALUE5]] const<i32>(4):
// DEFAULT-NEXT:                             while %[[VALUE6:[0-9]+]] eq<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), const<i32>(4))
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]], read<ptr<@type[[TYPE_tree]]>>(field0(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                         break %[[VALUE5]];
// DEFAULT-NEXT:                         default %[[VALUE5]]:
// DEFAULT-NEXT:                             break %[[VALUE5]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_name]], read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_ptr_type]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_current_class]]) [c="tree"]) [c="tree"];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_again]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(%[[VALUE_again]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_this_found]]), const<i32>(0))), not<bool>(ne<i32>(read<i32>(%[[VALUE_super_found]]), const<i32>(0))))
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: ptr<@type[[TYPE_tree]]> [synthetic] = read<ptr<@type[[TYPE_tree]]>>(field0(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_name]])))) [c="tree"] [c="tree"];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE8]]) [c="tree"]) [c="tree"] [c="tree"];
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<ptr<@type[[TYPE_tree]]>>(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE8]]) [c="tree"], null<ptr<@type[[TYPE_tree]]>>));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(field4(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree]]>>(field1(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]]))), read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_tree]]> [synthetic] = call<ptr<@type[[TYPE_tree]]>, signature=fn(ptr<@type[[TYPE_tree]]>, ptr<@type[[TYPE_tree]]>) -> ptr<@type[[TYPE_tree]]>>(%[[VALUE_lookup_field_wrapper]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_ptr_type]]) [c="tree"], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_name]]) [c="tree"]) [c="tree"] [c="tree"];
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE9]]) [c="tree"]) [c="tree"] [c="tree"];
// DEFAULT-NEXT:             if logical_or<bool>(ne<ptr<@type[[TYPE_tree]]>>(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE9]]) [c="tree"], null<ptr<@type[[TYPE_tree]]>>), logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_new_array_found]]), const<i32>(0)), eq<ptr<@type[[TYPE_tree]]>>(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_name]]) [c="tree"], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_length_identifier_node]]) [c="tree"])))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(field4(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_tree]]>>(field1(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]]))), conditional<ptr<@type[[TYPE_tree]]>>(ne<i32>(read<i32>(%[[VALUE_new_array_found]]), const<i32>(0)), null<ptr<@type[[TYPE_tree]]>>, read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<@type[[TYPE_tree]]> [synthetic] = call<ptr<@type[[TYPE_tree]]>, signature=fn(ptr<@type[[TYPE_tree]]>, ptr<@type[[TYPE_tree]]>) -> ptr<@type[[TYPE_tree]]>>(%[[VALUE_resolve_and_layout]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_name]]) [c="tree"], null<ptr<@type[[TYPE_tree]]>> [c="tree"]) [c="tree"] [c="tree"];
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]], read<ptr<@type[[TYPE_tree]]>>(%[[VALUE10]]) [c="tree"]) [c="tree"] [c="tree"];
// DEFAULT-NEXT:                 if ne<ptr<@type[[TYPE_tree]]>>(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE10]]) [c="tree"], null<ptr<@type[[TYPE_tree]]>>)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(field4(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_tree]]>>(field1(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]]))), read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_decl]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_or<bool>(eq<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]]))))))), const<i32>(0)), eq<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE_tree]]>>(field2(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual]]))))))), const<i32>(4)))
// DEFAULT-NEXT:                         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(field4(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<u32>(bitfield2<unit=0, bytes=0..1, bits=2..3>(field4(deref(read<ptr<@type[[TYPE_tree]]>>(%[[VALUE_qual_wfl]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
