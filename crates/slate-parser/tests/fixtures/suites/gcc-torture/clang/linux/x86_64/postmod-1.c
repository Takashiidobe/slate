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
// DEFAULT-NEXT:     global %[[VALUE_array0:[0-9]+]] array0: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array1:[0-9]+]] array1: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array2:[0-9]+]] array2: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array3:[0-9]+]] array3: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array4:[0-9]+]] array4: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array5:[0-9]+]] array5: array<f32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter0:[0-9]+]] counter0: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter1:[0-9]+]] counter1: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter2:[0-9]+]] counter2: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter3:[0-9]+]] counter3: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter4:[0-9]+]] counter4: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter5:[0-9]+]] counter5: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stop:[0-9]+]] stop: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vol:[0-9]+]] vol: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_pointer0:[0-9]+]] pointer0: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array0]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_pointer1:[0-9]+]] pointer1: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array1]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_pointer2:[0-9]+]] pointer2: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array2]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_pointer3:[0-9]+]] pointer3: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array3]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_pointer4:[0-9]+]] pointer4: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array4]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_pointer5:[0-9]+]] pointer5: ptr<f32> [storage=automatic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array5]]), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter0]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE1]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer0]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter0]], read<f32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer0]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE3]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer0]], read<ptr<f32>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter1]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE5]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer1]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter1]], read<f32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer1]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE7]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer1]], read<ptr<f32>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter2]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE9]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer2]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter2]], read<f32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer2]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE11]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer2]], read<ptr<f32>>(%[[VALUE12]]));
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE13]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer3]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter3]], read<f32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer3]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE15]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer3]], read<ptr<f32>>(%[[VALUE16]]));
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter4]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE17]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer4]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter4]], read<f32>(%[[VALUE18]]));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer4]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE19]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer4]], read<ptr<f32>>(%[[VALUE20]]));
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter5]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE21]]), read<f32>(deref(read<ptr<f32>>(%[[VALUE_pointer5]]))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter5]], read<f32>(%[[VALUE22]]));
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: ptr<f32> [synthetic] = read<ptr<f32>>(%[[VALUE_pointer5]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: ptr<f32> [synthetic] = ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE23]]), const<i32>(3));
// DEFAULT-NEXT:                 write<ptr<f32>>(%[[VALUE_pointer5]], read<ptr<f32>>(%[[VALUE24]]));
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter0]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE25]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer0]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter0]], read<f32>(%[[VALUE26]]));
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter1]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE27]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer1]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter1]], read<f32>(%[[VALUE28]]));
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter2]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE29]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer2]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter2]], read<f32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter3]]);
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE31]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer3]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter3]], read<f32>(%[[VALUE32]]));
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter4]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE33]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer4]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter4]], read<f32>(%[[VALUE34]]));
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_counter5]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE35]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_pointer5]]), read<i32>(%[[VALUE_x]])))));
// DEFAULT-NEXT:                 write<f32>(%[[VALUE_counter5]], read<f32>(%[[VALUE36]]));
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_addend0:[0-9]+]] addend0: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                     let %[[VALUE_addend1:[0-9]+]] addend1: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                     let %[[VALUE_addend2:[0-9]+]] addend2: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                     let %[[VALUE_addend3:[0-9]+]] addend3: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                     let %[[VALUE_addend4:[0-9]+]] addend4: i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                     for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                             let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), read<i32>(%[[VALUE_addend0]]));
// DEFAULT-NEXT:                             write<i32, volatile>(%[[VALUE_vol]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:                             let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                             let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), read<i32>(%[[VALUE_addend1]]));
// DEFAULT-NEXT:                             write<i32, volatile>(%[[VALUE_vol]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:                             let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                             let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), read<i32>(%[[VALUE_addend2]]));
// DEFAULT-NEXT:                             write<i32, volatile>(%[[VALUE_vol]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:                             let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                             let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), read<i32>(%[[VALUE_addend3]]));
// DEFAULT-NEXT:                             write<i32, volatile>(%[[VALUE_vol]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:                             let %[[VALUE48:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_vol]]);
// DEFAULT-NEXT:                             let %[[VALUE49:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE48]]), read<i32>(%[[VALUE_addend4]]));
// DEFAULT-NEXT:                             write<i32, volatile>(%[[VALUE_vol]], read<i32>(%[[VALUE49]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while not<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_stop]]), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_exit_code:[0-9]+]] exit_code: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array0]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array0]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array1]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array1]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array2]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array2]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array3]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array3]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array4]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array4]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array5]]), const<i32>(1))), const<f32>(1.0));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_array5]]), const<i32>(5))), const<f32>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE50]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter0]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE52]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter1]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE54]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter2]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE56]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter3]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE57]]));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE58]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter4]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE60]]), from_bool<i32, reason=promotion>(ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_counter5]]), const<f32>(3.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exit_code]], read<i32>(%[[VALUE61]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_exit_code]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
