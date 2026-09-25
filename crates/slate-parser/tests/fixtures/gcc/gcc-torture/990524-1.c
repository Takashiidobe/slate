void abort(void);
void exit(int);

char a[] = "12345";
char b[] = "12345";

void loop(char *pz, char *pzDta) {
  for (;;) {
    switch (*(pz++) = *(pzDta++)) {
    case 0:
      goto loopDone2;

    case '"':
    case '\\':
      pz[-1]  = '\\';
      *(pz++) = pzDta[-1];
    }
  }
loopDone2:;

  if (a - pz != b - pzDta)
    abort();
}

int main(void) {
  loop(a, b);
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
// DEFAULT-NEXT:     global %2 a: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([49, 50, 51, 52, 53, 0]) [linkage=external];
// DEFAULT-NEXT:     global %3 b: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([49, 50, 51, 52, 53, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @loop(%6 pz: ptr<i8>, %7 pzDta: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12: ptr<i8> [synthetic] = read<ptr<i8>>(%7);
// DEFAULT-NEXT:                     let %13: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%7, read<ptr<i8>>(%13));
// DEFAULT-NEXT:                     let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%6);
// DEFAULT-NEXT:                     let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%6, read<ptr<i8>>(%15));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%14)), read<i8>(deref(read<ptr<i8>>(%12))));
// DEFAULT-NEXT:                     switch %11 widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %11 const<i32>(0):
// DEFAULT-NEXT:                                 goto %5;
// DEFAULT-NEXT:                             case %11 const<i32>(34):
// DEFAULT-NEXT:                                 case %11 const<i32>(92):
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), neg<i32, overflow=ub>(const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(92)));
// DEFAULT-NEXT:                             let %16: ptr<i8> [synthetic] = read<ptr<i8>>(%6);
// DEFAULT-NEXT:                             let %17: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%6, read<ptr<i8>>(%17));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%16)), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         label %5 loopDone2:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%2), read<ptr<i8>>(%6)), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%3), read<ptr<i8>>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>) -> void>(%4, array_decay<ptr<i8>, length=Some(6)>(%2), array_decay<ptr<i8>, length=Some(6)>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
