void abort(void);
void exit(int);

static char *begfield(int tab, char *ptr, char *lim, int sword, int schar) {
  if (tab) {
    while (ptr < lim && sword--) {
      while (ptr < lim && *ptr != tab)
        ++ptr;
      if (ptr < lim)
        ++ptr;
    }
  } else {
    while (1)
      ;
  }

  if (ptr + schar <= lim)
    ptr += schar;

  return ptr;
}

int main(void) {
  char *s   = ":ab";
  char *lim = s + 3;
  if (begfield(':', s, lim, 1, 1) != s + 2)
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
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([58, 97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @begfield(%3 tab: i32, %4 ptr: ptr<i8>, %5 lim: ptr<i8>, %6 sword: i32, %7 schar: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %12 {
// DEFAULT-NEXT:                     let %16: bool [synthetic];
// DEFAULT-NEXT:                     if lt<ptr<i8>>(read<ptr<i8>>(%4), read<ptr<i8>>(%5))
// DEFAULT-NEXT:                         let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:                         write<bool>(%16, ne<i32>(read<i32>(%17), const<i32>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%16, const<bool>(false));
// DEFAULT-NEXT:                     yield read<bool>(%16);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         while %13 logical_and<bool>(lt<ptr<i8>>(read<ptr<i8>>(%4), read<ptr<i8>>(%5)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%4)))), read<i32>(%3)))
// DEFAULT-NEXT:                             let %19: ptr<i8> [synthetic] = read<ptr<i8>>(%4);
// DEFAULT-NEXT:                             let %20: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%19), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%4, read<ptr<i8>>(%20));
// DEFAULT-NEXT:                         if lt<ptr<i8>>(read<ptr<i8>>(%4), read<ptr<i8>>(%5))
// DEFAULT-NEXT:                             let %21: ptr<i8> [synthetic] = read<ptr<i8>>(%4);
// DEFAULT-NEXT:                             let %22: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%21), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%4, read<ptr<i8>>(%22));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %14 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), read<i32>(%7)), read<ptr<i8>>(%5))
// DEFAULT-NEXT:             let %23: ptr<i8> [synthetic] = read<ptr<i8>>(%4);
// DEFAULT-NEXT:             let %24: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), read<i32>(%7));
// DEFAULT-NEXT:             write<ptr<i8>>(%4, read<ptr<i8>>(%24));
// DEFAULT-NEXT:         return read<ptr<i8>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 s: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(4)>(%15);
// DEFAULT-NEXT:         let %10 lim: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(3));
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(i32, ptr<i8>, ptr<i8>, i32, i32) -> ptr<i8>>(%2, const<i32>(58), read<ptr<i8>>(%9), read<ptr<i8>>(%10), const<i32>(1), const<i32>(1)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
