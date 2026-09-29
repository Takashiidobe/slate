// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

typedef __SIZE_TYPE__ size_t;
typedef unsigned char Bufbyte;
typedef int Bytecount;
typedef int Charcount;
typedef struct lstream Lstream;
typedef int  Lisp_Object;
extern Lisp_Object Qnil;
extern inline  int
TRUE_LIST_P (Lisp_Object object)
{
  return ((  object  ) == (  Qnil ))  ;
}
struct Lisp_String
{
  Bytecount _size;
  Bufbyte *_data;
};
typedef enum lstream_buffering
{
  LSTREAM_LINE_BUFFERED,
} Lstream_buffering;
struct lstream
{
  Lstream_buffering buffering;  
  unsigned char *out_buffer;  
  size_t out_buffer_size;  
  size_t out_buffer_ind;  
  size_t byte_count;
  long flags;   
  char data[1];
};
typedef struct printf_spec printf_spec;
struct printf_spec
{
};
typedef union printf_arg printf_arg;
union printf_arg
{
};
typedef struct
{
   int cur;
} printf_spec_dynarr;
typedef struct
{
} printf_arg_dynarr;
extern void Lstream_fputc (struct lstream *, int);
extern void Lstream_write (struct lstream *, const Bufbyte *, Bytecount);
extern void Lstream_flush_out (struct lstream *);
extern printf_spec_dynarr *parse_doprnt_spec (Bufbyte *, Bytecount);
static void
doprnt_1 (Lisp_Object stream, const  Bufbyte *string, Bytecount len,
	  Charcount minlen, Charcount maxlen, int minus_flag, int zero_flag)
{
  Charcount cclen;
  Bufbyte pad;
  Lstream *lstr = ((  struct lstream  *) ((void *)((((    stream    ) & ((1UL << ((4   * 8 )  - 4 ) ) - 1UL) ) ) | 0x40000000 )) )  ;
  cclen = (  len ) ;
  if (zero_flag)
    pad = '0';
  pad = ' ';
#if 0
  if (minlen > cclen && !minus_flag)
#endif
    {
      int to_add = minlen - cclen;
      while (to_add > 0)
	{
	  (( lstr )->out_buffer_ind >= ( lstr )->out_buffer_size ?	Lstream_fputc ( lstr ,   pad ) :	(( lstr )->out_buffer[( lstr )->out_buffer_ind++] =	(unsigned char) (  pad ),	( lstr )->byte_count++,	( lstr )->buffering == LSTREAM_LINE_BUFFERED &&	( lstr )->out_buffer[( lstr )->out_buffer_ind - 1] == '\n' ?	Lstream_flush_out ( lstr ) : 0)) ;
	  to_add--;
	}
    }
  if (maxlen >= 0)
    len = (  ((( maxlen ) <= (  cclen )) ? ( maxlen ) : (  cclen ))  ) ;
  Lstream_write (lstr, string, len);
  if (minlen > cclen && minus_flag)
    {
      int to_add = minlen - cclen;
      while (to_add > 0)
	{
	  (( lstr )->out_buffer_ind >= ( lstr )->out_buffer_size ?	Lstream_fputc ( lstr ,   pad ) :	(( lstr )->out_buffer[( lstr )->out_buffer_ind++] =	(unsigned char) (  pad ),	( lstr )->byte_count++,	( lstr )->buffering == LSTREAM_LINE_BUFFERED &&	( lstr )->out_buffer[( lstr )->out_buffer_ind - 1] == '\n' ?	Lstream_flush_out ( lstr ) : 0)) ;
	  to_add--;
	}
    }
}
static Bytecount
emacs_doprnt_1 (Lisp_Object stream, const  Bufbyte *format_nonreloc,
		Lisp_Object format_reloc, Bytecount format_length,
		int nargs,
		const  Lisp_Object *largs)
{
  int i;
  printf_spec_dynarr *specs = 0;
  format_nonreloc = (( ((  struct Lisp_String  *) ((void *)((((     format_reloc     ) & ((1UL << ((4   * 8 )  - 4 ) ) - 1UL) ) ) | 0x40000000 )) )   )->_data + 0)  ;
  format_length = (( ((  struct Lisp_String  *) ((void *)((((     format_reloc     ) & ((1UL << ((4   * 8 )  - 4 ) ) - 1UL) ) ) | 0x40000000 )) )   )->_size)  ;
  specs = parse_doprnt_spec (format_nonreloc, format_length);
  for (i = 0; i < (( specs )->cur) ; i++)
    {
      char ch;
      doprnt_1 (stream, (Bufbyte *) &ch, 1, 0, -1, 0, 0);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 Bufbyte = u8;
// DEFAULT-NEXT:     type @type2 Bytecount = i32;
// DEFAULT-NEXT:     type @type3 Charcount = i32;
// DEFAULT-NEXT:     type @type4 lstream = struct {
// DEFAULT-NEXT:         field0 buffering: @type8;
// DEFAULT-NEXT:         field1 out_buffer: ptr<u8>;
// DEFAULT-NEXT:         field2 out_buffer_size: u64;
// DEFAULT-NEXT:         field3 out_buffer_ind: u64;
// DEFAULT-NEXT:         field4 byte_count: u64;
// DEFAULT-NEXT:         field5 flags: i64;
// DEFAULT-NEXT:         field6 data: array<i8, 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     type @type5 Lstream = @type4;
// DEFAULT-NEXT:     type @type6 Lisp_Object = i32;
// DEFAULT-NEXT:     type @type7 Lisp_String = struct {
// DEFAULT-NEXT:         field0 _size: i32;
// DEFAULT-NEXT:         field1 _data: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type8 lstream_buffering = enum : u32 {
// DEFAULT-NEXT:         %0 LSTREAM_LINE_BUFFERED = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type9 Lstream_buffering = @type8;
// DEFAULT-NEXT:     type @type10 printf_spec = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type11 printf_spec = @type10;
// DEFAULT-NEXT:     type @type12 printf_arg = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type13 printf_arg = @type12;
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 cur: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type15 printf_spec_dynarr = @type14;
// DEFAULT-NEXT:     type @type16 = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type17 printf_arg_dynarr = @type16;
// DEFAULT-NEXT:     extern %7 Qnil: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @TRUE_LIST_P(%9 object: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%9), read<i32>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @Lstream_fputc(%49 <unnamed>: ptr<@type4>, %50 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %23 @Lstream_write(%51 <unnamed>: ptr<@type4>, %52 <unnamed>: ptr<const u8>, %53 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %24 @Lstream_flush_out(%54 <unnamed>: ptr<@type4>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %25 @parse_doprnt_spec(%55 <unnamed>: ptr<u8>, %56 <unnamed>: i32) -> ptr<@type14> [linkage=external];
// DEFAULT-NEXT:     fn %26 @doprnt_1(%27 stream: i32, %28 string: ptr<const u8>, %29 len: i32, %30 minlen: i32, %31 maxlen: i32, %32 minus_flag: i32, %33 zero_flag: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34 cclen: i32 [storage=automatic];
// DEFAULT-NEXT:         let %35 pad: u8 [storage=automatic];
// DEFAULT-NEXT:         let %36 lstr: ptr<@type4> [storage=automatic] = pointer_cast<ptr<@type4>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%27))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))));
// DEFAULT-NEXT:         write<i32>(%34, read<i32>(%29));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%33), const<i32>(0))
// DEFAULT-NEXT:             write<u8>(%35, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(48))));
// DEFAULT-NEXT:         write<u8>(%35, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %37 to_add: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%30), read<i32>(%34));
// DEFAULT-NEXT:             while %57 gt<i32>(read<i32>(%37), const<i32>(0))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ge<u64>(read<u64>(field3(deref(read<ptr<@type4>>(%36)))), read<u64>(field2(deref(read<ptr<@type4>>(%36)))))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type4>, i32) -> void>(%22, read<ptr<@type4>>(%36), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%35))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         let %60: ptr<@type4> [synthetic] = read<ptr<@type4>>(%36);
// DEFAULT-NEXT:                         let %61: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type4>>(%60))));
// DEFAULT-NEXT:                         let %62: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%61), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type4>>(%60))), read<u64>(%62));
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type4>>(%36)))), read<u64>(%61))), read<u8>(%35));
// DEFAULT-NEXT:                         let %63: ptr<@type4> [synthetic] = read<ptr<@type4>>(%36);
// DEFAULT-NEXT:                         let %64: u64 [synthetic] = read<u64>(field4(deref(read<ptr<@type4>>(%63))));
// DEFAULT-NEXT:                         let %65: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%64), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(field4(deref(read<ptr<@type4>>(%63))), read<u64>(%65));
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type8>(field0(deref(read<ptr<@type4>>(%36))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type4>>(%36)))), sub<u64, overflow=wrap>(read<u64>(field3(deref(read<ptr<@type4>>(%36)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))))), const<i32>(10)))
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type4>) -> void>(%24, read<ptr<@type4>>(%36));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             const<i32>(0);
// DEFAULT-NEXT:                     let %66: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:                     let %67: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%37, read<i32>(%67));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%31), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%29, conditional<i32>(le<i32>(read<i32>(%31), read<i32>(%34)), read<i32>(%31), read<i32>(%34)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, ptr<const u8>, i32) -> void>(%23, read<ptr<@type4>>(%36), read<ptr<const u8>>(%28), read<i32>(%29));
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%30), read<i32>(%34)), ne<i32>(read<i32>(%32), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %38 to_add: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%30), read<i32>(%34));
// DEFAULT-NEXT:                 while %58 gt<i32>(read<i32>(%38), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ge<u64>(read<u64>(field3(deref(read<ptr<@type4>>(%36)))), read<u64>(field2(deref(read<ptr<@type4>>(%36)))))
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type4>, i32) -> void>(%22, read<ptr<@type4>>(%36), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%35))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             let %68: ptr<@type4> [synthetic] = read<ptr<@type4>>(%36);
// DEFAULT-NEXT:                             let %69: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type4>>(%68))));
// DEFAULT-NEXT:                             let %70: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%69), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(field3(deref(read<ptr<@type4>>(%68))), read<u64>(%70));
// DEFAULT-NEXT:                             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type4>>(%36)))), read<u64>(%69))), read<u8>(%35));
// DEFAULT-NEXT:                             let %71: ptr<@type4> [synthetic] = read<ptr<@type4>>(%36);
// DEFAULT-NEXT:                             let %72: u64 [synthetic] = read<u64>(field4(deref(read<ptr<@type4>>(%71))));
// DEFAULT-NEXT:                             let %73: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%72), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(field4(deref(read<ptr<@type4>>(%71))), read<u64>(%73));
// DEFAULT-NEXT:                             if logical_and<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type8>(field0(deref(read<ptr<@type4>>(%36))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type4>>(%36)))), sub<u64, overflow=wrap>(read<u64>(field3(deref(read<ptr<@type4>>(%36)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))))), const<i32>(10)))
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<@type4>) -> void>(%24, read<ptr<@type4>>(%36));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 const<i32>(0);
// DEFAULT-NEXT:                         let %74: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:                         let %75: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%38, read<i32>(%75));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @emacs_doprnt_1(%40 stream: i32, %41 format_nonreloc: ptr<const u8>, %42 format_reloc: i32, %43 format_length: i32, %44 nargs: i32, %45 largs: ptr<const i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %46 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %47 specs: ptr<@type14> [storage=automatic] = null<ptr<@type14>>;
// DEFAULT-NEXT:         write<ptr<const u8>>(%41, pointer_cast<ptr<const u8>, reason=assign>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(pointer_cast<ptr<@type7>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%42))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))))))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%43, read<i32>(field0(deref(pointer_cast<ptr<@type7>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%42))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))))))));
// DEFAULT-NEXT:         write<ptr<@type14>>(%47, call<ptr<@type14>, signature=fn(ptr<u8>, i32) -> ptr<@type14>>(%25, pointer_cast<ptr<u8>, reason=arg>(read<ptr<const u8>>(%41)), read<i32>(%43)));
// DEFAULT-NEXT:         call<ptr<@type14>, signature=fn(ptr<u8>, i32) -> ptr<@type14>>(%25, pointer_cast<ptr<u8>, reason=arg>(read<ptr<const u8>>(%41)), read<i32>(%43));
// DEFAULT-NEXT:         for %59
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%46, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%46), read<i32>(field0(deref(read<ptr<@type14>>(%47)))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:                 let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%46, read<i32>(%77));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %48 ch: i8 [storage=automatic];
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ptr<const u8>, i32, i32, i32, i32, i32) -> void>(%26, read<i32>(%40), pointer_cast<ptr<const u8>, reason=arg>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<i8>>(%48))), const<i32>(1), const<i32>(0), neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
