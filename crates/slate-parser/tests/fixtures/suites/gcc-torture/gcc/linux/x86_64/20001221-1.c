// SLATE-FILECHECK-DEFINES DEFAULT

static void
foo ()
{
  long maplength;
  int type;
  {
    const long nibbles = 8;
    char buf1[nibbles + 1];
    char buf2[nibbles + 1];
    char buf3[nibbles + 1];
    buf1[nibbles] = '\0';
    buf2[nibbles] = '\0';
    buf3[nibbles] = '\0';
    ((nibbles) <= 16
     ? (({
       void *__s = (buf1);
       union
	 {
	   unsigned int __ui;
	   unsigned short int __usi;
	   unsigned char __uc;
	 }
       *__u = __s;
       unsigned char __c = (unsigned char)('0');
       switch ((unsigned int) (nibbles))
	 {
	  case 16:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 12:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 0:
	   break;
	 }
       __s;
     }))
     : 0);
    ((nibbles) <= 16
     ? (({
       void *__s = (buf2);
       union
	 {
	   unsigned int __ui;
	   unsigned short int __usi;
	   unsigned char __uc;
	 }
       *__u = __s;
       unsigned char __c = (unsigned char)('0');
       switch ((unsigned int) (nibbles))
	 {
	  case 16:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 12:
	   __u->__ui = __c * 0x01010101;
	   __u = __extension__ ((void *) __u + 4);
	  case 8:
	   __u->__ui = __c * 0x01010101; 
	   __u = __extension__ ((void *) __u + 4);
	  case 4:
	   __u->__ui = __c * 0x01010101;
	  case 0:
	   break;
	 }
       __s;
     }))
     : 0);
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 __ui: u32;
// DEFAULT-NEXT:         field1 __usi: u16;
// DEFAULT-NEXT:         field2 __uc: u8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 __ui: u32;
// DEFAULT-NEXT:         field1 __usi: u16;
// DEFAULT-NEXT:         field2 __uc: u8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 maplength: i64 [storage=automatic];
// DEFAULT-NEXT:         let %2 type: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %3 nibbles: i64 [storage=automatic] [const] = widen<i64, reason=assign>(const<i32>(8));
// DEFAULT-NEXT:             let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %4 buf1: vla<i8, %15> [storage=automatic];
// DEFAULT-NEXT:             let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %5 buf2: vla<i8, %16> [storage=automatic];
// DEFAULT-NEXT:             let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %6 buf3: vla<i8, %17> [storage=automatic];
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%4), read<i64>(%3))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%5), read<i64>(%3))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%6), read<i64>(%3))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             let %20: ptr<void> [synthetic];
// DEFAULT-NEXT:             if le<i64>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:                 let %21: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 __s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=None>(%4));
// DEFAULT-NEXT:                     let %9 __u: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(read<ptr<void>>(%7));
// DEFAULT-NEXT:                     let %10 __c: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:                     switch %18 reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%3)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %18 const<u32>(16):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type0>>(%9))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type0>>(%9, pointer_cast<ptr<@type0>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type0>>(%9)), const<i32>(4))));
// DEFAULT-NEXT:                             case %18 const<u32>(12):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type0>>(%9))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%10))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type0>>(%9, pointer_cast<ptr<@type0>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type0>>(%9)), const<i32>(4))));
// DEFAULT-NEXT:                             case %18 const<u32>(0):
// DEFAULT-NEXT:                                 break %18;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<ptr<void>>(%21, read<ptr<void>>(%7));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%20, read<ptr<void>>(%21));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<void>>(%20, null<ptr<void>>);
// DEFAULT-NEXT:             let %22: ptr<void> [synthetic];
// DEFAULT-NEXT:             if le<i64>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:                 let %23: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 __s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=None>(%5));
// DEFAULT-NEXT:                     let %13 __u: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%11));
// DEFAULT-NEXT:                     let %14 __c: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:                     switch %19 reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%3)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %19 const<u32>(16):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type1>>(%13))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type1>>(%13, pointer_cast<ptr<@type1>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type1>>(%13)), const<i32>(4))));
// DEFAULT-NEXT:                             case %19 const<u32>(12):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type1>>(%13))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type1>>(%13, pointer_cast<ptr<@type1>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type1>>(%13)), const<i32>(4))));
// DEFAULT-NEXT:                             case %19 const<u32>(8):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type1>>(%13))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type1>>(%13, pointer_cast<ptr<@type1>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type1>>(%13)), const<i32>(4))));
// DEFAULT-NEXT:                             case %19 const<u32>(4):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type1>>(%13))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(16843009))));
// DEFAULT-NEXT:                             case %19 const<u32>(0):
// DEFAULT-NEXT:                                 break %19;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<ptr<void>>(%23, read<ptr<void>>(%11));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%22, read<ptr<void>>(%23));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<void>>(%22, null<ptr<void>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
