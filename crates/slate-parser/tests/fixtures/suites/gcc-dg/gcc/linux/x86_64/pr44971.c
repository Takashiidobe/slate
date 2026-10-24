/* { dg-do compile } */
/* { dg-options "-O2 -fpermissive -fcompare-debug -w" } */

typedef struct
{
  int default_screen;
} *_XPrivint;
typedef int FcBool;
typedef struct
{
  struct _XftintInfo *next;
} XftintInfo;
int _XftintInfo;

void _XftintInfoGet (int * dpy, FcBool createIfNecessary)
{
  int event_base;
  &event_base
   && dpy[createIfNecessary] != 0;
}

void _XftCloseint (int dpy, XftintInfo * info,XftintInfo  * *prev)
{
  _XftintInfoGet (dpy, 0);
  for (prev = &_XftintInfo; info = *prev; prev = &(*prev)->next)
    ;
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 default_screen: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE__XPrivint:[0-9]+]] _XPrivint = ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:     type @type[[TYPE_FcBool:[0-9]+]] FcBool = i32;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE__XftintInfo:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE__XftintInfo]] _XftintInfo = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_XftintInfo:[0-9]+]] XftintInfo = @type[[TYPE1]];
// DEFAULT-NEXT:     global %[[VALUE__XftintInfo:[0-9]+]] _XftintInfo: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__XftintInfoGet:[0-9]+]] @_XftintInfoGet(%[[VALUE_dpy:[0-9]+]] dpy: ptr<i32>, %[[VALUE_createIfNecessary:[0-9]+]] createIfNecessary: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_event_base:[0-9]+]] event_base: i32 [storage=automatic];
// DEFAULT-NEXT:         logical_and<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_event_base]]), null<ptr<i32>>), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_dpy]]), read<i32>(%[[VALUE_createIfNecessary]])))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE__XftCloseint:[0-9]+]] @_XftCloseint(%[[VALUE_dpy_2:[0-9]+]] dpy: i32, %[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE1]]>, %[[VALUE_prev:[0-9]+]] prev: ptr<ptr<@type[[TYPE1]]>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE__XftintInfoGet]], int_to_ptr<ptr<i32>, reason=arg>(read<i32>(%[[VALUE_dpy_2]])), const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE1]]>>>(%[[VALUE_prev]], pointer_cast<ptr<ptr<@type[[TYPE1]]>>, reason=assign>(addr_of<ptr<i32>>(%[[VALUE__XftintInfo]])));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE1]]> [synthetic] = read<ptr<@type[[TYPE1]]>>(deref(read<ptr<ptr<@type[[TYPE1]]>>>(%[[VALUE_prev]])));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE1]]>>(%[[VALUE_info]], read<ptr<@type[[TYPE1]]>>(%[[VALUE1]]));
// DEFAULT-NEXT:                 yield ne<ptr<@type[[TYPE1]]>>(read<ptr<@type[[TYPE1]]>>(%[[VALUE1]]), null<ptr<@type[[TYPE1]]>>);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<ptr<ptr<@type[[TYPE1]]>>>(%[[VALUE_prev]], pointer_cast<ptr<ptr<@type[[TYPE1]]>>, reason=assign>(addr_of<ptr<ptr<@type[[TYPE__XftintInfo]]>>>(field0(deref(read<ptr<@type[[TYPE1]]>>(deref(read<ptr<ptr<@type[[TYPE1]]>>>(%[[VALUE_prev]]))))))));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
