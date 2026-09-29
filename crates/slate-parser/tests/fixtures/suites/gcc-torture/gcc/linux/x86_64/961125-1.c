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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([58, 97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_begfield:[0-9]+]] @begfield(%[[VALUE_tab:[0-9]+]] tab: i32, %[[VALUE_ptr:[0-9]+]] ptr: ptr<i8>, %[[VALUE_lim:[0-9]+]] lim: ptr<i8>, %[[VALUE_sword:[0-9]+]] sword: i32, %[[VALUE_schar:[0-9]+]] schar: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_tab]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE1:[0-9]+]] {
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if lt<ptr<i8>>(read<ptr<i8>>(%[[VALUE_ptr]]), read<ptr<i8>>(%[[VALUE_lim]]))
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sword]]);
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_sword]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                         write<bool>(%[[VALUE2]], ne<i32>(read<i32>(%[[VALUE3]]), const<i32>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE2]], const<bool>(false));
// DEFAULT-NEXT:                     yield read<bool>(%[[VALUE2]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         while %[[VALUE5:[0-9]+]] logical_and<bool>(lt<ptr<i8>>(read<ptr<i8>>(%[[VALUE_ptr]]), read<ptr<i8>>(%[[VALUE_lim]])), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_ptr]])))), read<i32>(%[[VALUE_tab]])))
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_ptr]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                         if lt<ptr<i8>>(read<ptr<i8>>(%[[VALUE_ptr]]), read<ptr<i8>>(%[[VALUE_lim]]))
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_ptr]], read<ptr<i8>>(%[[VALUE9]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE10:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if le<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_ptr]]), read<i32>(%[[VALUE_schar]])), read<ptr<i8>>(%[[VALUE_lim]]))
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE11]]), read<i32>(%[[VALUE_schar]]));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_ptr]], read<ptr<i8>>(%[[VALUE12]]));
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]);
// DEFAULT-NEXT:         let %[[VALUE_lim_2:[0-9]+]] lim: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_s]]), const<i32>(3));
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(i32, ptr<i8>, ptr<i8>, i32, i32) -> ptr<i8>>(%[[VALUE_begfield]], const<i32>(58), read<ptr<i8>>(%[[VALUE_s]]), read<ptr<i8>>(%[[VALUE_lim_2]]), const<i32>(1), const<i32>(1)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_s]]), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
