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
// DEFAULT-NEXT:     type @type0 node_type = enum : u32 {
// DEFAULT-NEXT:         %0 INITIAL = const<i32>(0);
// DEFAULT-NEXT:         %1 FREE = const<i32>(1);
// DEFAULT-NEXT:         %2 PRECOLORED = const<i32>(2);
// DEFAULT-NEXT:         %3 SIMPLIFY = const<i32>(3);
// DEFAULT-NEXT:         %4 SIMPLIFY_SPILL = const<i32>(4);
// DEFAULT-NEXT:         %5 SIMPLIFY_FAT = const<i32>(5);
// DEFAULT-NEXT:         %6 FREEZE = const<i32>(6);
// DEFAULT-NEXT:         %7 SPILL = const<i32>(7);
// DEFAULT-NEXT:         %8 SELECT = const<i32>(8);
// DEFAULT-NEXT:         %9 SPILLED = const<i32>(9);
// DEFAULT-NEXT:         %10 COALESCED = const<i32>(10);
// DEFAULT-NEXT:         %11 COLORED = const<i32>(11);
// DEFAULT-NEXT:         %12 LAST_NODE_TYPE = const<i32>(12);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @baz() -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @put_web(%18 type: @type0) -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %20 enum_to_int<u32, reason=promotion>(read<@type0>(%18))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %20 const<u32>(0):
// DEFAULT-NEXT:                     case %20 const<u32>(1):
// DEFAULT-NEXT:                         case %20 const<u32>(6):
// DEFAULT-NEXT:                             case %20 const<u32>(7):
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 break %20;
// DEFAULT-NEXT:                 case %20 const<u32>(2):
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 break %20;
// DEFAULT-NEXT:                 default %20:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @reset_lists() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%17, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
