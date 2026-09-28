extern void abort(void);

typedef unsigned long my_uintptr_t;

int check_a(my_uintptr_t tagged_ptr);

int __attribute__((noinline)) try_a(my_uintptr_t x) {
  my_uintptr_t  heap[2];
  my_uintptr_t *hp = heap;

  hp[0] = x;
  hp[1] = 0;
  return check_a((my_uintptr_t)(void *)((char *)hp + 1));
}

int __attribute__((noinline)) check_a(my_uintptr_t tagged_ptr) {
  my_uintptr_t *hp = (my_uintptr_t *)(void *)((char *)tagged_ptr - 1);

  if (hp[0] == 42 && hp[1] == 0)
    return 0;
  return -1;
}

int main(void) {
  if (try_a(42) < 0)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 my_uintptr_t = u64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @check_a(%8 tagged_ptr: u64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 hp: ptr<u64> [storage=automatic] = pointer_cast<ptr<u64>, reason=explicit>(pointer_cast<ptr<void>, reason=explicit>(ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(int_to_ptr<ptr<i8>, reason=explicit>(read<u64>(%8)), const<i32>(1))));
// DEFAULT-NEXT:         if logical_and<bool>(eq<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(42)))), eq<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%9), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @try_a(%5 x: u64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 heap: array<u64, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 hp: ptr<u64> [storage=automatic] = array_decay<ptr<u64>, length=Some(2)>(%6);
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(0))), read<u64>(%5));
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%7), const<i32>(1))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         return call<i32, signature=fn(u64) -> i32>(%3, ptr_to_int<u64, reason=explicit>(pointer_cast<ptr<void>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<u64>>(%7)), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(u64) -> i32>(%4, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(42)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
