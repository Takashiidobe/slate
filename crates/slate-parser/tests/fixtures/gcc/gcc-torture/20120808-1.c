extern void exit(int);
extern void abort(void);

volatile int i;
unsigned char *volatile cp;
unsigned char d[32] = {0};

int main(void) {
  unsigned char  c[32] = {0};
  unsigned char *p     = d + i;
  int            j;
  for (j = 0; j < 30; j++) {
    int x = 0xff;
    int y = *++p;
    switch (j) {
    case 1:
      x ^= 2;
      break;
    case 2:
      x ^= 4;
      break;
    case 25:
      x ^= 1;
      break;
    default:
      break;
    }
    c[j] = y | x;
    cp   = p;
  }
  if (c[0] != 0xff || c[1] != 0xfd || c[2] != 0xfb || c[3] != 0xff ||
      c[4] != 0xff || c[25] != 0xfe || cp != d + 30)
    abort();
  exit(0);
}



// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     global %2 i: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 cp: volatile ptr<u8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: array<u8, 32> [storage=static] [align=16] = aggregate<array<u8, 32>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 c: array<u8, 32> [storage=automatic] [align=16] = aggregate<array<u8, 32>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %7 p: ptr<u8> [storage=automatic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%4), read<i32, volatile>(%2));
// DEFAULT-NEXT:         let %8 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %9 x: i32 [storage=automatic] = const<i32>(255);
// DEFAULT-NEXT:                     let %10 y: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %16: ptr<u8> [synthetic] = read<ptr<u8>>(%7);
// DEFAULT-NEXT:                     let %17: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%16), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%7, read<ptr<u8>>(%17));
// DEFAULT-NEXT:                     write<i32>(%10, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(deref(read<ptr<u8>>(%17))))));
// DEFAULT-NEXT:                     switch %13 read<i32>(%8)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %13 const<i32>(1):
// DEFAULT-NEXT:                                 let %18: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                 let %19: i32 [synthetic] = xor<i32>(read<i32>(%18), const<i32>(2));
// DEFAULT-NEXT:                                 write<i32>(%9, read<i32>(%19));
// DEFAULT-NEXT:                             break %13;
// DEFAULT-NEXT:                             case %13 const<i32>(2):
// DEFAULT-NEXT:                                 let %20: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                 let %21: i32 [synthetic] = xor<i32>(read<i32>(%20), const<i32>(4));
// DEFAULT-NEXT:                                 write<i32>(%9, read<i32>(%21));
// DEFAULT-NEXT:                             break %13;
// DEFAULT-NEXT:                             case %13 const<i32>(25):
// DEFAULT-NEXT:                                 let %22: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                 let %23: i32 [synthetic] = xor<i32>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%9, read<i32>(%23));
// DEFAULT-NEXT:                             break %13;
// DEFAULT-NEXT:                             default %13:
// DEFAULT-NEXT:                                 break %13;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), read<i32>(%8))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%10), read<i32>(%9)))));
// DEFAULT-NEXT:                     write<ptr<u8>, volatile>(%3, read<ptr<u8>>(%7));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(0)))))), const<i32>(255)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(1)))))), const<i32>(253))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(2)))))), const<i32>(251))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(3)))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(4)))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%6), const<i32>(25)))))), const<i32>(254))), ne<ptr<u8>>(read<ptr<u8>, volatile>(%3), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%4), const<i32>(30))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
