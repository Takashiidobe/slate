// SLATE-FILECHECK-DEFINES DEFAULT

enum Lisp_Type
{
  Lisp_Int                     
  ,Lisp_Record                 
  ,Lisp_Cons                   
  ,Lisp_String                 
  ,Lisp_Vector                 
  ,Lisp_Symbol
  ,Lisp_Char                     
};
typedef
union Lisp_Object
  {
    struct
      {
        enum Lisp_Type type: 3L ;
        unsigned long  markbit: 1;
        unsigned long  val: 32;
      } gu;
    long  i;
  }
Lisp_Object;
extern int initialized;
extern void call_critical_lisp_code (Lisp_Object);
void
init_device_faces (int *d)
{
  if (initialized)
    {
      Lisp_Object tdevice;
      do {
          tdevice = (union Lisp_Object)
                        { gu:
                          { markbit: 0,
                            type: Lisp_Record,
                            val: ((unsigned long )d)
                          }
                        };
      } while (0);
      call_critical_lisp_code (tdevice);
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
// DEFAULT-NEXT:     type @type[[TYPE_Lisp_Type:[0-9]+]] Lisp_Type = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_Lisp_Int:[0-9]+]] Lisp_Int = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_Lisp_Record:[0-9]+]] Lisp_Record = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_Lisp_Cons:[0-9]+]] Lisp_Cons = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_Lisp_String:[0-9]+]] Lisp_String = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_Lisp_Vector:[0-9]+]] Lisp_Vector = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_Lisp_Symbol:[0-9]+]] Lisp_Symbol = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_Lisp_Char:[0-9]+]] Lisp_Char = const<i32>(6);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Lisp_Object:[0-9]+]] Lisp_Object = union {
// DEFAULT-NEXT:         field0 gu: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 type: @type[[TYPE_Lisp_Type]] : 3;
// DEFAULT-NEXT:         field1 markbit: u64 : 1;
// DEFAULT-NEXT:         field2 val: u64 : 32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(4)], bit_units=[(0, 5)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_Lisp_Object_2:[0-9]+]] Lisp_Object = @type[[TYPE_Lisp_Object]];
// DEFAULT-NEXT:     extern %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_critical_lisp_code:[0-9]+]] @call_critical_lisp_code(%[[VALUE0:[0-9]+]] <unnamed>: @type[[TYPE_Lisp_Object]]) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_init_device_faces:[0-9]+]] @init_device_faces(%[[VALUE_d:[0-9]+]] d: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_initialized]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_tdevice:[0-9]+]] tdevice: @type[[TYPE_Lisp_Object]] [storage=automatic];
// DEFAULT-NEXT:                 do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<@type[[TYPE_Lisp_Object]]>(%[[VALUE_tdevice]], copy<@type[[TYPE_Lisp_Object]], reason=assign>(read<@type[[TYPE_Lisp_Object]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_Lisp_Object]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = int_to_enum<@type[[TYPE_Lisp_Type]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), field2 = ptr_to_int<u64, reason=explicit>(read<ptr<i32>>(%[[VALUE_d]])))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 call<void, signature=fn(@type[[TYPE_Lisp_Object]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_call_critical_lisp_code]], copy<@type[[TYPE_Lisp_Object]], reason=arg>(read<@type[[TYPE_Lisp_Object]]>(%[[VALUE_tdevice]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
