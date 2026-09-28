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
// DEFAULT-NEXT:     type @type0 FUNC_P = fn(ptr<void>, ptr<u8>, u16) -> u16;
// DEFAULT-NEXT:     fn %1 @crashIt(%2 id: i32, %3 func: ptr<fn(ptr<void>, ptr<u8>, u16) -> u16>, %4 funcparm: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 buff: array<u8, 5> [storage=automatic];
// DEFAULT-NEXT:         let %6 reverse: array<u8, 4> [storage=automatic];
// DEFAULT-NEXT:         let %7 bp: ptr<u8> [storage=automatic] = array_decay<ptr<u8>, length=Some(5)>(%5);
// DEFAULT-NEXT:         let %8 rp: ptr<u8> [storage=automatic] = array_decay<ptr<u8>, length=Some(4)>(%6);
// DEFAULT-NEXT:         let %9 count: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %10 cnt: u16 [storage=automatic];
// DEFAULT-NEXT:         while %11 gt<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13: ptr<u8> [synthetic] = read<ptr<u8>>(%8);
// DEFAULT-NEXT:                 let %14: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%8, read<ptr<u8>>(%14));
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(%13)), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(and<i32>(read<i32>(%2), const<i32>(127)))));
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%15), const<i32>(7));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:                 let %17: u16 [synthetic] = read<u16>(%9);
// DEFAULT-NEXT:                 let %18: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%17))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u16>(%9, read<u16>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u16>(%10, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%9))), const<i32>(1)))));
// DEFAULT-NEXT:         while %12 {
// DEFAULT-NEXT:             let %19: u16 [synthetic] = read<u16>(%9);
// DEFAULT-NEXT:             let %20: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%19))), const<i32>(1))));
// DEFAULT-NEXT:             write<u16>(%9, read<u16>(%20));
// DEFAULT-NEXT:             yield gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%19))), const<i32>(1));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21: ptr<u8> [synthetic] = read<ptr<u8>>(%8);
// DEFAULT-NEXT:                 let %22: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%8, read<ptr<u8>>(%22));
// DEFAULT-NEXT:                 let %23: ptr<u8> [synthetic] = read<ptr<u8>>(%7);
// DEFAULT-NEXT:                 let %24: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%7, read<ptr<u8>>(%24));
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(%23)), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%22))))), const<i32>(128)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %25: ptr<u8> [synthetic] = read<ptr<u8>>(%8);
// DEFAULT-NEXT:         let %26: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=true, element=u8, overflow=ub>(read<ptr<u8>>(%25), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u8>>(%8, read<ptr<u8>>(%26));
// DEFAULT-NEXT:         let %27: ptr<u8> [synthetic] = read<ptr<u8>>(%7);
// DEFAULT-NEXT:         let %28: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u8>>(%7, read<ptr<u8>>(%28));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%27)), read<u8>(deref(read<ptr<u8>>(%26))));
// DEFAULT-NEXT:         call<u16, signature=fn(ptr<void>, ptr<u8>, u16) -> u16>(read<ptr<fn(ptr<void>, ptr<u8>, u16) -> u16>>(%3), pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%4)), array_decay<ptr<u8>, length=Some(5)>(%5), read<u16>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
