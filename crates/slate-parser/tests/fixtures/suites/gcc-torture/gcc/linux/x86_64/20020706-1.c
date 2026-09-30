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
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 v1: u16;
// DEFAULT-NEXT:         field1 v2: ptr<u8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = @type[[TYPE_s1]];
// DEFAULT-NEXT:     global %[[VALUE_sn:[0-9]+]] sn: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hdb:[0-9]+]] hdb: ptr<@type[[TYPE_s1]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pb:[0-9]+]] pb: ptr<@type[[TYPE_s1]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_len:[0-9]+]] len: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_hdb_2:[0-9]+]] hdb: ptr<const @type[[TYPE_s1]]> [const]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> ptr<u8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_crashIt:[0-9]+]] @crashIt() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_nsn:[0-9]+]] nsn: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cnt:[0-9]+]] cnt: u16 [storage=automatic];
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_sn]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_len]]))), const<i32>(12))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<u8> [synthetic];
// DEFAULT-NEXT:             if ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_pb]])))))), const<i32>(8))
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE1]], read<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_pb]])))));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE1]], call<ptr<u8>, signature=fn() -> ptr<u8>>(%[[VALUE_foo]]));
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE1]]);
// DEFAULT-NEXT:             write<ptr<u8>>(%[[VALUE_p]], read<ptr<u8>>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], eq<ptr<u8>>(read<ptr<u8>>(%[[VALUE2]]), null<ptr<u8>>));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_nsn]], or<u32>(or<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), const<i32>(24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), const<u32>(4278190080)), const<i32>(24))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE3]]), const<i32>(4));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_p]], read<ptr<u8>>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_cnt]], truncate<u16, reason=explicit, fits=unknown>(and<u32>(or<u32>(or<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), const<i32>(24)), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680))), const<i32>(8))), shr<u32, amount_out_of_range=ub, fill=zero_extend>(and<u32>(read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(read<ptr<u8>>(%[[VALUE_p]])))), const<u32>(4278190080)), const<i32>(24))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65535)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_len]]))), add<i32, overflow=ub>(const<i32>(12), mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_cnt]]))), const<i32>(56)))), eq<u32>(read<u32>(%[[VALUE_nsn]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const @type[[TYPE_s1]]>) -> void>(%[[VALUE_bar]], pointer_cast<ptr<const @type[[TYPE_s1]]>, reason=arg>(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_hdb]])));
// DEFAULT-NEXT:                 return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
