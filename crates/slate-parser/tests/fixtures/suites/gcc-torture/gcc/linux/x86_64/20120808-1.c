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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cp:[0-9]+]] cp: volatile ptr<u8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: array<u8, 32> [storage=static] [align=16] = aggregate<array<u8, 32>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<u8, 32> [storage=automatic] [align=16] = aggregate<array<u8, 32>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<u8> [storage=automatic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_d]]), read<i32, volatile>(%[[VALUE_i]]));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(255);
// DEFAULT-NEXT:                     let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<u8>>(%[[VALUE_p]], read<ptr<u8>>(%[[VALUE5]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_y]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(deref(read<ptr<u8>>(%[[VALUE5]]))))));
// DEFAULT-NEXT:                     switch %[[VALUE6:[0-9]+]] read<i32>(%[[VALUE_j]])
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE6]] const<i32>(1):
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE7]]), const<i32>(2));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                             case %[[VALUE6]] const<i32>(2):
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE9]]), const<i32>(4));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                             case %[[VALUE6]] const<i32>(25):
// DEFAULT-NEXT:                                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                             default %[[VALUE6]]:
// DEFAULT-NEXT:                                 break %[[VALUE6]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), read<i32>(%[[VALUE_j]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                     write<ptr<u8>, volatile>(%[[VALUE_cp]], read<ptr<u8>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(0)))))), const<i32>(255)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(1)))))), const<i32>(253))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(2)))))), const<i32>(251))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(3)))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(4)))))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_c]]), const<i32>(25)))))), const<i32>(254))), ne<ptr<u8>>(read<ptr<u8>, volatile>(%[[VALUE_cp]]), ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(%[[VALUE_d]]), const<i32>(30))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
