// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

typedef __SIZE_TYPE__ size_t;
typedef unsigned long int reg_syntax_t;
struct re_pattern_buffer
{
  unsigned char *buffer;
};
typedef enum
{
  jump,
  jump_n,
} re_opcode_t;
static int
foo (bufp)
     struct re_pattern_buffer *bufp;
{
  int mcnt;
  unsigned char *p = bufp->buffer;
  switch (((re_opcode_t) * p++))
    {
    unconditional_jump:
      ;
      /* This test case caused an ICE because the statement insertion
	 routines were failing to update basic block boundaries.  */
    case jump:
      do
        {
          (mcnt) = *(p) & 0377;
        }
      while (0);
      (p) += 2;
      p += mcnt;
    case jump_n:
      (mcnt) = *(p + 2) & 0377;
      if (mcnt)
        goto unconditional_jump;
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
// DEFAULT-NEXT:     type @type1 reg_syntax_t = u64;
// DEFAULT-NEXT:     type @type2 re_pattern_buffer = struct {
// DEFAULT-NEXT:         field0 buffer: ptr<u8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 jump = const<i32>(0);
// DEFAULT-NEXT:         %1 jump_n = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 re_opcode_t = @type3;
// DEFAULT-NEXT:     fn %7 @foo(%9 bufp: ptr<@type2>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 mcnt: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 p: ptr<u8> [storage=automatic] = read<ptr<u8>>(field0(deref(read<ptr<@type2>>(%9))));
// DEFAULT-NEXT:         let %14: ptr<u8> [synthetic] = read<ptr<u8>>(%11);
// DEFAULT-NEXT:         let %15: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u8>>(%11, read<ptr<u8>>(%15));
// DEFAULT-NEXT:         switch %12 enum_to_int<u32, reason=promotion>(int_to_enum<@type3, reason=explicit>(widen<u32, reason=explicit>(read<u8>(deref(read<ptr<u8>>(%14))))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %8 unconditional_jump:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                 case %12 const<u32>(0):
// DEFAULT-NEXT:                     do %13
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%10, and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%11))))), const<i32>(255)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 let %16: ptr<u8> [synthetic] = read<ptr<u8>>(%11);
// DEFAULT-NEXT:                 let %17: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%16), const<i32>(2));
// DEFAULT-NEXT:                 write<ptr<u8>>(%11, read<ptr<u8>>(%17));
// DEFAULT-NEXT:                 let %18: ptr<u8> [synthetic] = read<ptr<u8>>(%11);
// DEFAULT-NEXT:                 let %19: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%18), read<i32>(%10));
// DEFAULT-NEXT:                 write<ptr<u8>>(%11, read<ptr<u8>>(%19));
// DEFAULT-NEXT:                 case %12 const<u32>(1):
// DEFAULT-NEXT:                     write<i32>(%10, and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%11), const<i32>(2)))))), const<i32>(255)));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                     goto %8;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
