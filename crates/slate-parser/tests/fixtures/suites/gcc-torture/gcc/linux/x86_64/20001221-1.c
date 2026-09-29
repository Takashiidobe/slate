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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __ui: u32;
// DEFAULT-NEXT:         field1 __usi: u16;
// DEFAULT-NEXT:         field2 __uc: u8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __ui: u32;
// DEFAULT-NEXT:         field1 __usi: u16;
// DEFAULT-NEXT:         field2 __uc: u8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_maplength:[0-9]+]] maplength: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_type:[0-9]+]] type: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_nibbles:[0-9]+]] nibbles: i64 [storage=automatic] [const] = widen<i64, reason=assign>(const<i32>(8));
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_nibbles]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE_buf1:[0-9]+]] buf1: vla<i8, %[[VALUE0]]> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_nibbles]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE_buf2:[0-9]+]] buf2: vla<i8, %[[VALUE1]]> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(read<i64>(%[[VALUE_nibbles]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE_buf3:[0-9]+]] buf3: vla<i8, %[[VALUE2]]> [storage=automatic];
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%[[VALUE_buf1]]), read<i64>(%[[VALUE_nibbles]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%[[VALUE_buf2]]), read<i64>(%[[VALUE_nibbles]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%[[VALUE_buf3]]), read<i64>(%[[VALUE_nibbles]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:             if le<i64>(read<i64>(%[[VALUE_nibbles]]), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___s:[0-9]+]] __s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=None>(%[[VALUE_buf1]]));
// DEFAULT-NEXT:                     let %[[VALUE___u:[0-9]+]] __u: ptr<@type[[TYPE0]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE0]]>, reason=assign>(read<ptr<void>>(%[[VALUE___s]]));
// DEFAULT-NEXT:                     let %[[VALUE___c:[0-9]+]] __c: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:                     switch %[[VALUE5:[0-9]+]] reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_nibbles]])))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE5]] const<u32>(16):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE___u]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE0]]>>(%[[VALUE___u]], pointer_cast<ptr<@type[[TYPE0]]>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE0]]>>(%[[VALUE___u]])), const<i32>(4))));
// DEFAULT-NEXT:                             case %[[VALUE5]] const<u32>(12):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE___u]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE0]]>>(%[[VALUE___u]], pointer_cast<ptr<@type[[TYPE0]]>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE0]]>>(%[[VALUE___u]])), const<i32>(4))));
// DEFAULT-NEXT:                             case %[[VALUE5]] const<u32>(0):
// DEFAULT-NEXT:                                 break %[[VALUE5]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<ptr<void>>(%[[VALUE4]], read<ptr<void>>(%[[VALUE___s]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE3]], read<ptr<void>>(%[[VALUE4]]));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE3]], null<ptr<void>>);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:             if le<i64>(read<i64>(%[[VALUE_nibbles]]), widen<i64, reason=usual_arith>(const<i32>(16)))
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<void> [synthetic];
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___s_2:[0-9]+]] __s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=None>(%[[VALUE_buf2]]));
// DEFAULT-NEXT:                     let %[[VALUE___u_2:[0-9]+]] __u: ptr<@type[[TYPE1]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE1]]>, reason=assign>(read<ptr<void>>(%[[VALUE___s_2]]));
// DEFAULT-NEXT:                     let %[[VALUE___c_2:[0-9]+]] __c: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:                     switch %[[VALUE8:[0-9]+]] reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_nibbles]])))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(16):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c_2]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]], pointer_cast<ptr<@type[[TYPE1]]>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]])), const<i32>(4))));
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(12):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c_2]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]], pointer_cast<ptr<@type[[TYPE1]]>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]])), const<i32>(4))));
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(8):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c_2]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]], pointer_cast<ptr<@type[[TYPE1]]>, reason=assign>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]])), const<i32>(4))));
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(4):
// DEFAULT-NEXT:                                 write<u32>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE___u_2]]))), reinterpret<u32, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___c_2]]))), const<i32>(16843009))));
// DEFAULT-NEXT:                             case %[[VALUE8]] const<u32>(0):
// DEFAULT-NEXT:                                 break %[[VALUE8]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<ptr<void>>(%[[VALUE7]], read<ptr<void>>(%[[VALUE___s_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE6]], read<ptr<void>>(%[[VALUE7]]));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE6]], null<ptr<void>>);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
