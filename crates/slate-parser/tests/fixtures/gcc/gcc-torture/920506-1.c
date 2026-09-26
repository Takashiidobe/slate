void abort(void);
void exit(int);
int  l[] = {0, 1};
int  main(void) {
  int *p = l;
  switch (*p++) {
  case 0:
    exit(0);
  case 1:
    break;
  case 2:
    break;
  case 3:
  case 4:
    break;
  }
  abort();
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
// DEFAULT-NEXT:     global %2 l: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 p: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(2)>(%2);
// DEFAULT-NEXT:         let %7: ptr<i32> [synthetic] = read<ptr<i32>>(%4);
// DEFAULT-NEXT:         let %8: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%4, read<ptr<i32>>(%8));
// DEFAULT-NEXT:         switch %6 read<i32>(deref(read<ptr<i32>>(%7)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %6 const<i32>(0):
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:                 case %6 const<i32>(1):
// DEFAULT-NEXT:                     break %6;
// DEFAULT-NEXT:                 case %6 const<i32>(2):
// DEFAULT-NEXT:                     break %6;
// DEFAULT-NEXT:                 case %6 const<i32>(3):
// DEFAULT-NEXT:                     case %6 const<i32>(4):
// DEFAULT-NEXT:                         break %6;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
