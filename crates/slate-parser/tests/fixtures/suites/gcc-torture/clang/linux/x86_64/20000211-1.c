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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_Bufbyte:[0-9]+]] Bufbyte = u8;
// DEFAULT-NEXT:     type @type[[TYPE_Bytecount:[0-9]+]] Bytecount = i32;
// DEFAULT-NEXT:     type @type[[TYPE_Charcount:[0-9]+]] Charcount = i32;
// DEFAULT-NEXT:     type @type[[TYPE_lstream:[0-9]+]] lstream = struct {
// DEFAULT-NEXT:         field0 buffering: @type[[TYPE_lstream_buffering:[0-9]+]];
// DEFAULT-NEXT:         field1 out_buffer: ptr<u8>;
// DEFAULT-NEXT:         field2 out_buffer_size: u64;
// DEFAULT-NEXT:         field3 out_buffer_ind: u64;
// DEFAULT-NEXT:         field4 byte_count: u64;
// DEFAULT-NEXT:         field5 flags: i64;
// DEFAULT-NEXT:         field6 data: array<i8, 1>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8, 16, 24, 32, 40, 48]];
// DEFAULT-NEXT:     type @type[[TYPE_Lstream:[0-9]+]] Lstream = @type[[TYPE_lstream]];
// DEFAULT-NEXT:     type @type[[TYPE_Lisp_Object:[0-9]+]] Lisp_Object = i32;
// DEFAULT-NEXT:     type @type[[TYPE_Lisp_String:[0-9]+]] Lisp_String = struct {
// DEFAULT-NEXT:         field0 _size: i32;
// DEFAULT-NEXT:         field1 _data: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_lstream_buffering]] lstream_buffering = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_LSTREAM_LINE_BUFFERED:[0-9]+]] LSTREAM_LINE_BUFFERED = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Lstream_buffering:[0-9]+]] Lstream_buffering = @type[[TYPE_lstream_buffering]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_spec:[0-9]+]] printf_spec = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_spec_2:[0-9]+]] printf_spec = @type[[TYPE_printf_spec]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_arg:[0-9]+]] printf_arg = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_arg_2:[0-9]+]] printf_arg = @type[[TYPE_printf_arg]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 cur: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_spec_dynarr:[0-9]+]] printf_spec_dynarr = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_printf_arg_dynarr:[0-9]+]] printf_arg_dynarr = @type[[TYPE1]];
// DEFAULT-NEXT:     extern %[[VALUE_Qnil:[0-9]+]] Qnil: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_TRUE_LIST_P:[0-9]+]] @TRUE_LIST_P(%[[VALUE_object:[0-9]+]] object: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_object]]), read<i32>(%[[VALUE_Qnil]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Lstream_fputc:[0-9]+]] @Lstream_fputc(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE_lstream]]>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Lstream_write:[0-9]+]] @Lstream_write(%[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_lstream]]>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<const u8>, %[[VALUE4:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Lstream_flush_out:[0-9]+]] @Lstream_flush_out(%[[VALUE5:[0-9]+]] <unnamed>: ptr<@type[[TYPE_lstream]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_parse_doprnt_spec:[0-9]+]] @parse_doprnt_spec(%[[VALUE6:[0-9]+]] <unnamed>: ptr<u8>, %[[VALUE7:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE0]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_doprnt_1:[0-9]+]] @doprnt_1(%[[VALUE_stream:[0-9]+]] stream: i32, %[[VALUE_string:[0-9]+]] string: ptr<const u8>, %[[VALUE_len:[0-9]+]] len: i32, %[[VALUE_minlen:[0-9]+]] minlen: i32, %[[VALUE_maxlen:[0-9]+]] maxlen: i32, %[[VALUE_minus_flag:[0-9]+]] minus_flag: i32, %[[VALUE_zero_flag:[0-9]+]] zero_flag: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_cclen:[0-9]+]] cclen: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pad:[0-9]+]] pad: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lstr:[0-9]+]] lstr: ptr<@type[[TYPE_lstream]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_lstream]]>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_stream]]))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cclen]], read<i32>(%[[VALUE_len]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_zero_flag]]), const<i32>(0))
// DEFAULT-NEXT:             write<u8>(%[[VALUE_pad]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(48))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_pad]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_to_add:[0-9]+]] to_add: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_minlen]]), read<i32>(%[[VALUE_cclen]]));
// DEFAULT-NEXT:             while %[[VALUE8:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_to_add]]), const<i32>(0))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ge<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), read<u64>(field2(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))))
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_lstream]]>, i32) -> void>(%[[VALUE_Lstream_fputc]], read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%[[VALUE_pad]]))));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_lstream]]> [synthetic] = read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE9]]))));
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE10]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE9]]))), read<u64>(%[[VALUE11]]));
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), read<u64>(%[[VALUE10]]))), read<u8>(%[[VALUE_pad]]));
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: ptr<@type[[TYPE_lstream]]> [synthetic] = read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]);
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: u64 [synthetic] = read<u64>(field4(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE12]]))));
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE13]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(field4(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE12]]))), read<u64>(%[[VALUE14]]));
// DEFAULT-NEXT:                         if logical_and<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_lstream_buffering]]>(field0(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), sub<u64, overflow=wrap>(read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))))), const<i32>(10)))
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type[[TYPE_lstream]]>) -> void>(%[[VALUE_Lstream_flush_out]], read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             const<i32>(0);
// DEFAULT-NEXT:                     let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_to_add]]);
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_to_add]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_maxlen]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_len]], conditional<i32>(le<i32>(read<i32>(%[[VALUE_maxlen]]), read<i32>(%[[VALUE_cclen]])), read<i32>(%[[VALUE_maxlen]]), read<i32>(%[[VALUE_cclen]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_lstream]]>, ptr<const u8>, i32) -> void>(%[[VALUE_Lstream_write]], read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]), read<ptr<const u8>>(%[[VALUE_string]]), read<i32>(%[[VALUE_len]]));
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_minlen]]), read<i32>(%[[VALUE_cclen]])), ne<i32>(read<i32>(%[[VALUE_minus_flag]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_to_add_2:[0-9]+]] to_add: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_minlen]]), read<i32>(%[[VALUE_cclen]]));
// DEFAULT-NEXT:                 while %[[VALUE17:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_to_add_2]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ge<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), read<u64>(field2(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))))
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<@type[[TYPE_lstream]]>, i32) -> void>(%[[VALUE_Lstream_fputc]], read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]), reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%[[VALUE_pad]]))));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             let %[[VALUE18:[0-9]+]]: ptr<@type[[TYPE_lstream]]> [synthetic] = read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]);
// DEFAULT-NEXT:                             let %[[VALUE19:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE18]]))));
// DEFAULT-NEXT:                             let %[[VALUE20:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE19]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE18]]))), read<u64>(%[[VALUE20]]));
// DEFAULT-NEXT:                             write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), read<u64>(%[[VALUE19]]))), read<u8>(%[[VALUE_pad]]));
// DEFAULT-NEXT:                             let %[[VALUE21:[0-9]+]]: ptr<@type[[TYPE_lstream]]> [synthetic] = read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]);
// DEFAULT-NEXT:                             let %[[VALUE22:[0-9]+]]: u64 [synthetic] = read<u64>(field4(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE21]]))));
// DEFAULT-NEXT:                             let %[[VALUE23:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE22]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(field4(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE21]]))), read<u64>(%[[VALUE23]]));
// DEFAULT-NEXT:                             if logical_and<bool>(eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_lstream_buffering]]>(field0(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), sub<u64, overflow=wrap>(read<u64>(field3(deref(read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))))), const<i32>(10)))
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<@type[[TYPE_lstream]]>) -> void>(%[[VALUE_Lstream_flush_out]], read<ptr<@type[[TYPE_lstream]]>>(%[[VALUE_lstr]]));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 const<i32>(0);
// DEFAULT-NEXT:                         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_to_add_2]]);
// DEFAULT-NEXT:                         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_to_add_2]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_emacs_doprnt_1:[0-9]+]] @emacs_doprnt_1(%[[VALUE_stream_2:[0-9]+]] stream: i32, %[[VALUE_format_nonreloc:[0-9]+]] format_nonreloc: ptr<const u8>, %[[VALUE_format_reloc:[0-9]+]] format_reloc: i32, %[[VALUE_format_length:[0-9]+]] format_length: i32, %[[VALUE_nargs:[0-9]+]] nargs: i32, %[[VALUE_largs:[0-9]+]] largs: ptr<const i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_specs:[0-9]+]] specs: ptr<@type[[TYPE0]]> [storage=automatic] = null<ptr<@type[[TYPE0]]>>;
// DEFAULT-NEXT:         write<ptr<const u8>>(%[[VALUE_format_nonreloc]], pointer_cast<ptr<const u8>, reason=assign>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(field1(deref(pointer_cast<ptr<@type[[TYPE_Lisp_String]]>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_format_reloc]]))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))))))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_format_length]], read<i32>(field0(deref(pointer_cast<ptr<@type[[TYPE_Lisp_String]]>, reason=explicit>(int_to_ptr<ptr<void>, reason=explicit>(or<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_format_reloc]]))), sub<u64, overflow=wrap>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1073741824))))))))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_specs]], call<ptr<@type[[TYPE0]]>, signature=fn(ptr<u8>, i32) -> ptr<@type[[TYPE0]]>>(%[[VALUE_parse_doprnt_spec]], pointer_cast<ptr<u8>, reason=arg>(read<ptr<const u8>>(%[[VALUE_format_nonreloc]])), read<i32>(%[[VALUE_format_length]])));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE0]]>, signature=fn(ptr<u8>, i32) -> ptr<@type[[TYPE0]]>>(%[[VALUE_parse_doprnt_spec]], pointer_cast<ptr<u8>, reason=arg>(read<ptr<const u8>>(%[[VALUE_format_nonreloc]])), read<i32>(%[[VALUE_format_length]]));
// DEFAULT-NEXT:         for %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_specs]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_ch:[0-9]+]] ch: i8 [storage=automatic];
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ptr<const u8>, i32, i32, i32, i32, i32) -> void>(%[[VALUE_doprnt_1]], read<i32>(%[[VALUE_stream_2]]), pointer_cast<ptr<const u8>, reason=arg>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<i8>>(%[[VALUE_ch]]))), const<i32>(1), const<i32>(0), neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
