// SLATE-FILECHECK-DEFINES DEFAULT

// Contributed by Alexandre Oliva <aoliva@redhat.com>
// From Red Hat case 106165.

/* { dg-require-effective-target indirect_calls } */

typedef struct s1
{
  unsigned short v1;
  unsigned char *v2;
} S1;

extern void bar(const struct s1 *const hdb);
extern unsigned char* foo ();

unsigned int sn;
S1 *hdb;
S1 *pb;
unsigned short len;

unsigned int crashIt()
{
  unsigned char *p;
  unsigned int nsn;
  unsigned short cnt;

  if (sn != 0) return 1;

  if ((len < 12) || ((p = (((pb->v1) >= 8) ? pb->v2 : foo() )) == 0))
    return 1;

  nsn = (
	 (((*(unsigned int*)p) & 0x000000ff) << 24) |
	 (((*(unsigned int*)p) & 0x0000ff00) << 8)  |
	 (((*(unsigned int*)p) & 0x00ff0000) >> 8)  |
	 (((*(unsigned int*)p) & 0xff000000) >> 24)  );
  p += 4;

  cnt = (unsigned short) ((
			   (((*(unsigned int*)p) & 0x000000ff) << 24) |
			   (((*(unsigned int*)p) & 0x0000ff00) << 8)  |
			   (((*(unsigned int*)p) & 0x00ff0000) >> 8)  |
			   (((*(unsigned int*)p) & 0xff000000) >> 24)  ) &
			  0xffff);

  if ((len != 12 + (cnt * 56)) || (nsn == 0))
    {
      bar(hdb);
      return 1;
    }

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
// DEFAULT-NEXT:     type @type0 s1 = struct {
// DEFAULT-NEXT:         field0 v1: u16;
// DEFAULT-NEXT:         field1 v2: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 S1 = @type0;
// DEFAULT-NEXT:     global %4 sn: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 hdb: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 pb: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 len: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%12 hdb: ptr<const @type0> [const]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> ptr<u8> [linkage=external];
// DEFAULT-NEXT:     fn %8 @crashIt() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %10 nsn: u32 [storage=automatic];
// DEFAULT-NEXT:         let %11 cnt: u16 [storage=automatic];
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%7))), const<i32>(12))
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %14: ptr<u8> [synthetic];
// DEFAULT-NEXT:             if ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type0>>(%6)))))), const<i32>(8))
// DEFAULT-NEXT:                 write<ptr<u8>>(%14, read<ptr<u8>>(field1(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<u8>>(%14, call<ptr<u8>, signature=fn() -> ptr<u8>>(%3));
// DEFAULT-NEXT:             write<ptr<u8>>(%9, read<ptr<u8>>(%14));
// DEFAULT-NEXT:             write<bool>(%13, eq<ptr<u8>>(read<ptr<u8>>(%14), null<ptr<u8>>));
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         write<u32>(%10, or<u32>(or<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), const<i32>(24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), const<u32>(4278190080)), const<i32>(24))));
// DEFAULT-NEXT:         let %15: ptr<u8> [synthetic] = read<ptr<u8>>(%9);
// DEFAULT-NEXT:         let %16: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%15), const<i32>(4));
// DEFAULT-NEXT:         write<ptr<u8>>(%9, read<ptr<u8>>(%16));
// DEFAULT-NEXT:         write<u16>(%11, truncate<u16, reason=explicit, fits=unknown>(and<u32>(or<u32>(or<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), const<i32>(24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%9)))), const<u32>(4278190080)), const<i32>(24))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65535)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%7))), add<i32, overflow=ub>(const<i32>(12), mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%11))), const<i32>(56)))), eq<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const @type0>) -> void>(%2, pointer_cast<ptr<const @type0>, reason=arg>(read<ptr<@type0>>(%5)));
// DEFAULT-NEXT:                 return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
