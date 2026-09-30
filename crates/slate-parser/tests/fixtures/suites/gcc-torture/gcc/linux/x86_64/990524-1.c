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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([49, 50, 51, 52, 53, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([49, 50, 51, 52, 53, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_loop:[0-9]+]] @loop(%[[VALUE_pz:[0-9]+]] pz: ptr<i8>, %[[VALUE_pzDta:[0-9]+]] pzDta: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_pzDta]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_pzDta]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_pz]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_pz]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE2]])));
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%[[VALUE4]])), read<i8>(%[[VALUE6]]));
// DEFAULT-NEXT:                     switch %[[VALUE7:[0-9]+]] widen<i32, reason=promotion>(read<i8>(%[[VALUE6]]))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE7]] const<i32>(0):
// DEFAULT-NEXT:                                 goto %[[VALUE_loopDone2:[0-9]+]];
// DEFAULT-NEXT:                             case %[[VALUE7]] const<i32>(34):
// DEFAULT-NEXT:                                 case %[[VALUE7]] const<i32>(92):
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_pz]]), neg<i32, overflow=ub>(const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(92)));
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_pz]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_pz]], read<ptr<i8>>(%[[VALUE9]]));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE8]])), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_pzDta]]), neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         label %[[VALUE_loopDone2]] loopDone2:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_a]]), read<ptr<i8>>(%[[VALUE_pz]])), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_b]]), read<ptr<i8>>(%[[VALUE_pzDta]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>) -> void>(%[[VALUE_loop]], array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_a]]), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
