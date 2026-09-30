void abort(void);
void exit(int);

int ns_name_skip(unsigned char **x, unsigned char *y) {
  *x = 0;
  return 0;
}

unsigned char a[2];

int dn_skipname(unsigned char *ptr, unsigned char *eom) {
  unsigned char *saveptr = ptr;

  if (ns_name_skip(&ptr, eom) == -1)
    return (-1);
  return (ptr - saveptr);
}

int main(void) {
  if (dn_skipname(&a[0], &a[1]) == 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<u8, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_ns_name_skip:[0-9]+]] @ns_name_skip(%[[VALUE_x:[0-9]+]] x: ptr<ptr<u8>>, %[[VALUE_y:[0-9]+]] y: ptr<u8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<u8>>(deref(read<ptr<ptr<u8>>>(%[[VALUE_x]])), null<ptr<u8>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dn_skipname:[0-9]+]] @dn_skipname(%[[VALUE_ptr:[0-9]+]] ptr: ptr<u8>, %[[VALUE_eom:[0-9]+]] eom: ptr<u8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_saveptr:[0-9]+]] saveptr: ptr<u8> [storage=automatic] = read<ptr<u8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<ptr<u8>>, ptr<u8>) -> i32>(%[[VALUE_ns_name_skip]], addr_of<ptr<ptr<u8>>>(%[[VALUE_ptr]]), read<ptr<u8>>(%[[VALUE_eom]])), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(ptr_diff<i64, element=u8, same_array=required, overflow=ub>(read<ptr<u8>>(%[[VALUE_ptr]]), read<ptr<u8>>(%[[VALUE_saveptr]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<u8>, ptr<u8>) -> i32>(%[[VALUE_dn_skipname]], addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(%[[VALUE_a]]), const<i32>(0)))), addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(%[[VALUE_a]]), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
