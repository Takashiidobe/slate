// SLATE-FILECHECK-DEFINES DEFAULT

// Contributed by Alexandre Oliva <aoliva@redhat.com>
// From Red Hat case 106165.

/* { dg-require-effective-target indirect_calls } */

typedef unsigned short (FUNC_P) (void *, unsigned char *, unsigned short);

void crashIt(int id, FUNC_P *func, unsigned char *funcparm)
{
  unsigned char buff[5], reverse[4];
  unsigned char *bp = buff;
  unsigned char *rp = reverse;
  unsigned short int count = 0;
  unsigned short cnt;
  while (id > 0)
    {
      *rp++ = (unsigned char) (id & 0x7F);
      id >>= 7;
      count++;
    }
  cnt = count + 1;
  while ((count--) > 1)
    {
      *bp++ = (unsigned char)(*(--rp) | 0x80);
    }
  *bp++ = *(--rp);
  (void)(*func)(funcparm, buff, cnt);
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
// DEFAULT-NEXT:     type @type[[TYPE_FUNC_P:[0-9]+]] FUNC_P = fn(ptr<void>, ptr<u8>, u16) -> u16;
// DEFAULT-NEXT:     fn %[[VALUE_crashIt:[0-9]+]] @crashIt(%[[VALUE_id:[0-9]+]] id: i32, %[[VALUE_func:[0-9]+]] func: ptr<fn(ptr<void>, ptr<u8>, u16) -> u16>, %[[VALUE_funcparm:[0-9]+]] funcparm: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buff:[0-9]+]] buff: array<u8, 5> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_reverse:[0-9]+]] reverse: array<u8, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bp:[0-9]+]] bp: ptr<u8> [storage=automatic] = array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_buff]]);
// DEFAULT-NEXT:         let %[[VALUE_rp:[0-9]+]] rp: ptr<u8> [storage=automatic] = array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_reverse]]);
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_cnt:[0-9]+]] cnt: u16 [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_id]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_rp]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_rp]], read<ptr<u8>>(%[[VALUE2]]));
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(%[[VALUE1]])), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(and<i32>(read<i32>(%[[VALUE_id]]), const<i32>(127)))));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_id]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE3]]), const<i32>(7));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_id]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_count]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE5]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_count]], read<u16>(%[[VALUE6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u16>(%[[VALUE_cnt]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_count]]))), const<i32>(1)))));
// DEFAULT-NEXT:         while %[[VALUE7:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_count]]);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE8]]))), const<i32>(1))));
// DEFAULT-NEXT:             write<u16>(%[[VALUE_count]], read<u16>(%[[VALUE9]]));
// DEFAULT-NEXT:             yield gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE8]]))), const<i32>(1));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_rp]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_rp]], read<ptr<u8>>(%[[VALUE11]]));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_bp]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%[[VALUE_bp]], read<ptr<u8>>(%[[VALUE13]]));
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(%[[VALUE12]])), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE11]]))))), const<i32>(128)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_rp]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_rp]], read<ptr<u8>>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_bp]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_bp]], read<ptr<u8>>(%[[VALUE17]]));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE16]])), read<u8>(deref(read<ptr<u8>>(%[[VALUE15]]))));
// DEFAULT-NEXT:         call<u16, signature=fn(ptr<void>, ptr<u8>, u16) -> u16>(read<ptr<fn(ptr<void>, ptr<u8>, u16) -> u16>>(%[[VALUE_func]]), pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%[[VALUE_funcparm]])), array_decay<ptr<u8>, length=Some(5)>(%[[VALUE_buff]]), read<u16>(%[[VALUE_cnt]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
