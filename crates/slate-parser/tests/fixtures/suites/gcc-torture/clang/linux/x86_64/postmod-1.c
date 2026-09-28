#define DECLARE_ARRAY(A)   array##A[0x10]
#define DECLARE_COUNTER(A) counter##A = 0
#define DECLARE_POINTER(A) *pointer##A = array##A + x
/* Create a loop that allows post-modification of pointerA, followed by
   a use of the post-modified address.  */
#define BEFORE(A)          counter##A += *pointer##A, pointer##A += 3
#define AFTER(A)           counter##A += pointer##A[x]

/* Set up the arrays so that one iteration of the loop sets the counter
   to 3.0f.  */
#define INIT_ARRAY(A) array##A[1] = 1.0f, array##A[5] = 2.0f

/* Check that the loop worked correctly for all values.  */
#define CHECK_ARRAY(A) exit_code |= (counter##A != 3.0f)

/* Having 6 copies triggered the bug for ARM and Thumb.  */
#define MANY(A) A(0), A(1), A(2), A(3), A(4), A(5)

/* Each addendA should be allocated a register.  */
#define INIT_VOLATILE(A) addend##A = vol
#define ADD_VOLATILE(A)  vol += addend##A

/* Having 5 copies triggered the bug for ARM and Thumb.  */
#define MANY2(A) A(0), A(1), A(2), A(3), A(4)

float MANY(DECLARE_ARRAY);
float MANY(DECLARE_COUNTER);

volatile int stop = 1;
volatile int vol;

void __attribute__((noinline)) foo(int x) {
  float MANY(DECLARE_POINTER);
  int   i;

  do {
    MANY(BEFORE);
    MANY(AFTER);
    /* Create an inner loop that should ensure the code above
       has registers free for reload inheritance.  */
    {
      int MANY2(INIT_VOLATILE);
      for (i = 0; i < 10; i++)
        MANY2(ADD_VOLATILE);
    }
  } while (!stop);
}

int main(void) {
  int exit_code = 0;

  MANY(INIT_ARRAY);
  foo(1);
  MANY(CHECK_ARRAY);
  return exit_code;
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
// DEFAULT-NEXT:     global %0 array0: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 array1: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 array2: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %3 array3: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 array4: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %5 array5: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %6 counter0: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %7 counter1: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %8 counter2: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %9 counter3: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %10 counter4: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %11 counter5: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %12 stop: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %13 vol: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %14 @foo(%15 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 pointer0: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%0), read<i32>(%15));
// DEFAULT-NEXT:         let %17 pointer1: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%1), read<i32>(%15));
// DEFAULT-NEXT:         let %18 pointer2: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%2), read<i32>(%15));
// DEFAULT-NEXT:         let %19 pointer3: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%3), read<i32>(%15));
// DEFAULT-NEXT:         let %20 pointer4: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%4), read<i32>(%15));
// DEFAULT-NEXT:         let %21 pointer5: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%5), read<i32>(%15));
// DEFAULT-NEXT:         let %22 i: i32 [storage=automatic];
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %32: f32 [synthetic] = read<f32>(%6);
// DEFAULT-NEXT:                 let %33: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%32), read<f32>(deref(read<ptr<f32>>(%16))));
// DEFAULT-NEXT:                 write<f32>(%6, read<f32>(%33));
// DEFAULT-NEXT:                 let %34: ptr<f32> [synthetic] = read<ptr<f32>>(%16);
// DEFAULT-NEXT:                 let %35: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%34), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%16, read<ptr<f32>>(%35));
// DEFAULT-NEXT:                 let %36: f32 [synthetic] = read<f32>(%7);
// DEFAULT-NEXT:                 let %37: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%36), read<f32>(deref(read<ptr<f32>>(%17))));
// DEFAULT-NEXT:                 write<f32>(%7, read<f32>(%37));
// DEFAULT-NEXT:                 let %38: ptr<f32> [synthetic] = read<ptr<f32>>(%17);
// DEFAULT-NEXT:                 let %39: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%38), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%17, read<ptr<f32>>(%39));
// DEFAULT-NEXT:                 let %40: f32 [synthetic] = read<f32>(%8);
// DEFAULT-NEXT:                 let %41: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%40), read<f32>(deref(read<ptr<f32>>(%18))));
// DEFAULT-NEXT:                 write<f32>(%8, read<f32>(%41));
// DEFAULT-NEXT:                 let %42: ptr<f32> [synthetic] = read<ptr<f32>>(%18);
// DEFAULT-NEXT:                 let %43: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%42), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%18, read<ptr<f32>>(%43));
// DEFAULT-NEXT:                 let %44: f32 [synthetic] = read<f32>(%9);
// DEFAULT-NEXT:                 let %45: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%44), read<f32>(deref(read<ptr<f32>>(%19))));
// DEFAULT-NEXT:                 write<f32>(%9, read<f32>(%45));
// DEFAULT-NEXT:                 let %46: ptr<f32> [synthetic] = read<ptr<f32>>(%19);
// DEFAULT-NEXT:                 let %47: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%46), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%19, read<ptr<f32>>(%47));
// DEFAULT-NEXT:                 let %48: f32 [synthetic] = read<f32>(%10);
// DEFAULT-NEXT:                 let %49: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%48), read<f32>(deref(read<ptr<f32>>(%20))));
// DEFAULT-NEXT:                 write<f32>(%10, read<f32>(%49));
// DEFAULT-NEXT:                 let %50: ptr<f32> [synthetic] = read<ptr<f32>>(%20);
// DEFAULT-NEXT:                 let %51: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%50), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%20, read<ptr<f32>>(%51));
// DEFAULT-NEXT:                 let %52: f32 [synthetic] = read<f32>(%11);
// DEFAULT-NEXT:                 let %53: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%52), read<f32>(deref(read<ptr<f32>>(%21))));
// DEFAULT-NEXT:                 write<f32>(%11, read<f32>(%53));
// DEFAULT-NEXT:                 let %54: ptr<f32> [synthetic] = read<ptr<f32>>(%21);
// DEFAULT-NEXT:                 let %55: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%54), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%21, read<ptr<f32>>(%55));
// DEFAULT-NEXT:                 let %56: f32 [synthetic] = read<f32>(%6);
// DEFAULT-NEXT:                 let %57: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%56), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%16), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%6, read<f32>(%57));
// DEFAULT-NEXT:                 let %58: f32 [synthetic] = read<f32>(%7);
// DEFAULT-NEXT:                 let %59: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%58), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%17), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%7, read<f32>(%59));
// DEFAULT-NEXT:                 let %60: f32 [synthetic] = read<f32>(%8);
// DEFAULT-NEXT:                 let %61: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%60), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%18), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%8, read<f32>(%61));
// DEFAULT-NEXT:                 let %62: f32 [synthetic] = read<f32>(%9);
// DEFAULT-NEXT:                 let %63: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%62), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%19), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%9, read<f32>(%63));
// DEFAULT-NEXT:                 let %64: f32 [synthetic] = read<f32>(%10);
// DEFAULT-NEXT:                 let %65: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%64), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%20), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%10, read<f32>(%65));
// DEFAULT-NEXT:                 let %66: f32 [synthetic] = read<f32>(%11);
// DEFAULT-NEXT:                 let %67: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%66), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%21), read<i32>(%15)))));
// DEFAULT-NEXT:                 write<f32>(%11, read<f32>(%67));
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %23 addend0: i32 [storage=automatic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                     let %24 addend1: i32 [storage=automatic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                     let %25 addend2: i32 [storage=automatic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                     let %26 addend3: i32 [storage=automatic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                     let %27 addend4: i32 [storage=automatic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                     for %31
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%22, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%22), const<i32>(10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %68: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                             let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%22, read<i32>(%69));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %70: i32 [synthetic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                             let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), read<i32>(%23));
// DEFAULT-NEXT:                             write<i32, volatile>(%13, read<i32>(%71));
// DEFAULT-NEXT:                             let %72: i32 [synthetic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                             let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), read<i32>(%24));
// DEFAULT-NEXT:                             write<i32, volatile>(%13, read<i32>(%73));
// DEFAULT-NEXT:                             let %74: i32 [synthetic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                             let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), read<i32>(%25));
// DEFAULT-NEXT:                             write<i32, volatile>(%13, read<i32>(%75));
// DEFAULT-NEXT:                             let %76: i32 [synthetic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                             let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), read<i32>(%26));
// DEFAULT-NEXT:                             write<i32, volatile>(%13, read<i32>(%77));
// DEFAULT-NEXT:                             let %78: i32 [synthetic] = read<i32, volatile>(%13);
// DEFAULT-NEXT:                             let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), read<i32>(%27));
// DEFAULT-NEXT:                             write<i32, volatile>(%13, read<i32>(%79));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while not<bool>(ne<i32>(read<i32, volatile>(%12), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %29 exit_code: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%0), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%0), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%1), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%1), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%2), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%2), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%3), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%3), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%4), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%4), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%5), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%5), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%14, const<i32>(1));
// DEFAULT-NEXT:         let %80: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %81: i32 [synthetic] = or<i32>(read<i32>(%80), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%6), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%81));
// DEFAULT-NEXT:         let %82: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = or<i32>(read<i32>(%82), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%7), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%83));
// DEFAULT-NEXT:         let %84: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %85: i32 [synthetic] = or<i32>(read<i32>(%84), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%8), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%85));
// DEFAULT-NEXT:         let %86: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %87: i32 [synthetic] = or<i32>(read<i32>(%86), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%9), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%87));
// DEFAULT-NEXT:         let %88: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %89: i32 [synthetic] = or<i32>(read<i32>(%88), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%10), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%89));
// DEFAULT-NEXT:         let %90: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:         let %91: i32 [synthetic] = or<i32>(read<i32>(%90), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%11), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%29, read<i32>(%91));
// DEFAULT-NEXT:         return read<i32>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
