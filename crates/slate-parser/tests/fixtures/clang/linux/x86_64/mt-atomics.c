#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

static atomic_int counter = 0;

static void *worker(void *arg) {
  (void)arg;
  for (int i = 0; i < 100000; i++) {
    atomic_fetch_add(&counter, 1);
  }
  return 0;
}

int main(void) {
  pthread_t threads[4];
  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], 0, worker, 0);
  }
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], 0);
  }
  printf("%d\n", (int)counter);
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
// DEFAULT-NEXT:     type @type0 pthread_t = u64;
// DEFAULT-NEXT:     type @type1 pthread_attr_t = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 56>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 pthread_attr_t = @type1;
// DEFAULT-NEXT:     type @type3 atomic_int = i32;
// DEFAULT-NEXT:     global %14 counter: atomic i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %7 @pthread_create(%22 __newthread: ptr<u64> [restrict], %23 __attr: ptr<const @type1> [restrict], %24 __start_routine: ptr<fn(ptr<void>) -> ptr<void>>, %25 __arg: ptr<void> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @pthread_join(%26 __th: u64, %27 __thread_return: ptr<ptr<void>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @printf(%28 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @worker(%16 arg: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<ptr<void>>(%16);
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %17 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(100000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %35: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%14)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 threads: array<u64, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %20 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%37));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<u64>, ptr<const @type1>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%7, addr_of<ptr<u64>>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%19), read<i32>(%20)))), null<ptr<const @type1>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%15), null<ptr<void>>);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %21 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%10, read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(4)>(%19), read<i32>(%21)))), null<ptr<ptr<void>>>);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%13, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%32)), read<i32, atomic=seq_cst>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
