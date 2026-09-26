/* Test that __builtin_prefetch does no harm.

   Data prefetch should not fault if used with an invalid address.  */

#include <limits.h>

void exit(int);

#define ARRSIZE 65
int *bad_addr[ARRSIZE];
int  arr_used;

/* Fill bad_addr with a range of values in the hopes that on any target
   some will be invalid addresses.  */
void init_addrs(void) {
  int i;
  int bits_per_ptr = sizeof(void *) * 8;
  for (i = 0; i < bits_per_ptr; i++)
    bad_addr[i] = (void *)(1UL << i);
  arr_used = bits_per_ptr + 1; /* The last element used is zero.  */
}

void prefetch_for_read(void) {
  int i;
  for (i = 0; i < ARRSIZE; i++)
    __builtin_prefetch(bad_addr[i], 0, 0);
}

void prefetch_for_write(void) {
  int i;
  for (i = 0; i < ARRSIZE; i++)
    __builtin_prefetch(bad_addr[i], 1, 0);
}

int main() {
  init_addrs();
  prefetch_for_read();
  prefetch_for_write();
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
// DEFAULT-NEXT:     global %1 bad_addr: array<ptr<i32>, 65> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 arr_used: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @init_addrs() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 bits_per_ptr: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(65)>(%1), read<i32>(%4))), pointer_cast<ptr<i32>, reason=assign>(int_to_ptr<ptr<void>, reason=explicit>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), read<i32>(%4)))));
// DEFAULT-NEXT:         write<i32>(%2, add<i32, overflow=ub>(read<i32>(%5), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_prefetch(%14 <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @prefetch_for_read() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const void>, ...) -> void>(%15, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(65)>(%1), read<i32>(%7))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @prefetch_for_write() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(65))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const void>, ...) -> void>(%15, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(65)>(%1), read<i32>(%9))))), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
