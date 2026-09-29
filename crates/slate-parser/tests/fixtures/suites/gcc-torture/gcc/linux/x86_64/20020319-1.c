// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase ICEd in combine.c:do_SUBST() self-test for sign-extended
CONST_INT because expr.c:expand_expr() was not sign-extending array index
into constant strings.  */

typedef unsigned char uch;
extern uch outbuf[];
extern unsigned outcnt;

extern void flush_outbuf (void);

int zip(void)
{
  outcnt = 0;

    {outbuf[outcnt++]=(uch)("\037\213"[0]); if (outcnt==16384) flush_outbuf();};
    {outbuf[outcnt++]=(uch)("\037\213"[1]); if (outcnt==16384) flush_outbuf();};

  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_uch:[0-9]+]] uch = u8;
// DEFAULT-NEXT:     extern %[[VALUE_outbuf:[0-9]+]] outbuf: array<u8, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_outcnt:[0-9]+]] outcnt: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([31, 139, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([31, 139, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_flush_outbuf:[0-9]+]] @flush_outbuf() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_zip:[0-9]+]] @zip() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u32>(%[[VALUE_outcnt]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_outcnt]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE0]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%[[VALUE_outcnt]], read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=None>(%[[VALUE_outbuf]]), read<u32>(%[[VALUE0]]))), reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]]), const<i32>(0))))));
// DEFAULT-NEXT:             if eq<u32>(read<u32>(%[[VALUE_outcnt]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16384)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_flush_outbuf]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_outcnt]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%[[VALUE_outcnt]], read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=None>(%[[VALUE_outbuf]]), read<u32>(%[[VALUE2]]))), reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]]), const<i32>(1))))));
// DEFAULT-NEXT:             if eq<u32>(read<u32>(%[[VALUE_outcnt]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16384)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_flush_outbuf]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
