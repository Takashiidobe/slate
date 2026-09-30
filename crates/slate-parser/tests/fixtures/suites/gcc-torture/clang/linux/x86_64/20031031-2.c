// SLATE-FILECHECK-DEFINES DEFAULT

/* PR/10239 */

void foo ();
void bar ();
void baz ();

enum node_type
{
  INITIAL = 0, FREE,
  PRECOLORED,
  SIMPLIFY, SIMPLIFY_SPILL, SIMPLIFY_FAT, FREEZE, SPILL,
  SELECT,
  SPILLED, COALESCED, COLORED,
  LAST_NODE_TYPE
};

inline void
put_web (enum node_type type)
{
  switch (type)
    {
    case INITIAL:
    case FREE:
    case FREEZE:
    case SPILL:
      foo ();
      break;
    case PRECOLORED:
      bar ();
      break;
    default:
      baz ();
    }
}

void
reset_lists ()
{
  put_web (INITIAL);
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
// DEFAULT-NEXT:     type @type[[TYPE_node_type:[0-9]+]] node_type = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_INITIAL:[0-9]+]] INITIAL = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_FREE:[0-9]+]] FREE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_PRECOLORED:[0-9]+]] PRECOLORED = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_SIMPLIFY:[0-9]+]] SIMPLIFY = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_SIMPLIFY_SPILL:[0-9]+]] SIMPLIFY_SPILL = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_SIMPLIFY_FAT:[0-9]+]] SIMPLIFY_FAT = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_FREEZE:[0-9]+]] FREEZE = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE_SPILL:[0-9]+]] SPILL = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE_SELECT:[0-9]+]] SELECT = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_SPILLED:[0-9]+]] SPILLED = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE_COALESCED:[0-9]+]] COALESCED = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE_COLORED:[0-9]+]] COLORED = const<i32>(11);
// DEFAULT-NEXT:         %[[VALUE_LAST_NODE_TYPE:[0-9]+]] LAST_NODE_TYPE = const<i32>(12);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_INITIAL]] @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_FREE]] @bar() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_PRECOLORED]] @baz() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_put_web:[0-9]+]] @put_web(%[[VALUE_type:[0-9]+]] type: @type[[TYPE_node_type]]) -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] enum_to_int<u32, reason=promotion>(read<@type[[TYPE_node_type]]>(%[[VALUE_type]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<u32>(0):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<u32>(1):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<u32>(6):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<u32>(7):
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_INITIAL]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<u32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_FREE]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_PRECOLORED]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reset_lists:[0-9]+]] @reset_lists() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_node_type]]) -> void>(%[[VALUE_put_web]], int_to_enum<@type[[TYPE_node_type]], reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
