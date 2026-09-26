/* Test that __builtin_prefetch does no harm.

   Prefetch data using a variety of storage classes and address
   expressions with volatile variables and pointers.  */

void exit(int);

int          glob_int_arr[100];
int          glob_int = 4;
volatile int glob_vol_int_arr[100];
int *volatile glob_vol_ptr_int              = glob_int_arr;
volatile int *glob_ptr_vol_int              = glob_vol_int_arr;
volatile int *volatile glob_vol_ptr_vol_int = glob_vol_int_arr;
volatile int glob_vol_int;

static int          stat_int_arr[100];
static volatile int stat_vol_int_arr[100];
static int *volatile stat_vol_ptr_int              = stat_int_arr;
static volatile int *stat_ptr_vol_int              = stat_vol_int_arr;
static volatile int *volatile stat_vol_ptr_vol_int = stat_vol_int_arr;
static volatile int stat_vol_int;

struct S {
  int       a;
  short     b, c;
  char      d[8];
  struct S *next;
};

struct S          str;
volatile struct S vol_str;
struct S *volatile vol_ptr_str              = &str;
volatile struct S *ptr_vol_str              = &vol_str;
volatile struct S *volatile vol_ptr_vol_str = &vol_str;

/* Prefetch volatile global variables using the address of the variable.  */

void simple_vol_global() {
  __builtin_prefetch(glob_vol_int_arr, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&glob_vol_int, 0, 0);
}

/* Prefetch volatile static variables using the address of the variable.  */

void simple_vol_file() {
  __builtin_prefetch(stat_vol_int_arr, 0, 0);
  __builtin_prefetch(stat_vol_ptr_int, 0, 0);
  __builtin_prefetch(stat_ptr_vol_int, 0, 0);
  __builtin_prefetch(stat_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&stat_vol_int, 0, 0);
}

/* Prefetch using address expressions involving volatile global variables.  */

void expr_vol_global(void) {
  __builtin_prefetch(&vol_str, 0, 0);
  __builtin_prefetch(ptr_vol_str, 0, 0);
  __builtin_prefetch(vol_ptr_str, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str, 0, 0);
  __builtin_prefetch(&vol_str.b, 0, 0);
  __builtin_prefetch(&ptr_vol_str->b, 0, 0);
  __builtin_prefetch(&vol_ptr_str->b, 0, 0);
  __builtin_prefetch(&vol_ptr_vol_str->b, 0, 0);
  __builtin_prefetch(&vol_str.d, 0, 0);
  __builtin_prefetch(&vol_ptr_str->d, 0, 0);
  __builtin_prefetch(&ptr_vol_str->d, 0, 0);
  __builtin_prefetch(&vol_ptr_vol_str->d, 0, 0);
  __builtin_prefetch(vol_str.next, 0, 0);
  __builtin_prefetch(vol_ptr_str->next, 0, 0);
  __builtin_prefetch(ptr_vol_str->next, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str->next, 0, 0);
  __builtin_prefetch(vol_str.next->d, 0, 0);
  __builtin_prefetch(vol_ptr_str->next->d, 0, 0);
  __builtin_prefetch(ptr_vol_str->next->d, 0, 0);
  __builtin_prefetch(vol_ptr_vol_str->next->d, 0, 0);

  __builtin_prefetch(&glob_vol_int_arr, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int, 0, 0);
  __builtin_prefetch(&glob_vol_int_arr[2], 0, 0);
  __builtin_prefetch(&glob_vol_ptr_int[3], 0, 0);
  __builtin_prefetch(&glob_ptr_vol_int[3], 0, 0);
  __builtin_prefetch(&glob_vol_ptr_vol_int[3], 0, 0);
  __builtin_prefetch(glob_vol_int_arr + 3, 0, 0);
  __builtin_prefetch(glob_vol_int_arr + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int + 5, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int + 5, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int + 5, 0, 0);
  __builtin_prefetch(glob_vol_ptr_int + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_ptr_vol_int + glob_vol_int, 0, 0);
  __builtin_prefetch(glob_vol_ptr_vol_int + glob_vol_int, 0, 0);
}

int main() {
  simple_vol_global();
  simple_vol_file();

  str.next     = &str;
  vol_str.next = &str;
  expr_vol_global();

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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:         field3 d: array<i8, 8>;
// DEFAULT-NEXT:         field4 next: ptr<@type0>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 6, 8, 16]];
// DEFAULT-NEXT:     global %1 glob_int_arr: array<i32, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 glob_int: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %3 glob_vol_int_arr: volatile array<i32, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 glob_vol_ptr_int: volatile ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(100)>(%1) [linkage=external];
// DEFAULT-NEXT:     global %5 glob_ptr_vol_int: ptr<volatile i32> [storage=static] = array_decay<ptr<volatile i32>, length=Some(100)>(%3) [linkage=external];
// DEFAULT-NEXT:     global %6 glob_vol_ptr_vol_int: volatile ptr<volatile i32> [storage=static] = array_decay<ptr<volatile i32>, length=Some(100)>(%3) [linkage=external];
// DEFAULT-NEXT:     global %7 glob_vol_int: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 stat_int_arr: array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %9 stat_vol_int_arr: volatile array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %10 stat_vol_ptr_int: volatile ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(100)>(%8) [linkage=internal];
// DEFAULT-NEXT:     global %11 stat_ptr_vol_int: ptr<volatile i32> [storage=static] = array_decay<ptr<volatile i32>, length=Some(100)>(%9) [linkage=internal];
// DEFAULT-NEXT:     global %12 stat_vol_ptr_vol_int: volatile ptr<volatile i32> [storage=static] = array_decay<ptr<volatile i32>, length=Some(100)>(%9) [linkage=internal];
// DEFAULT-NEXT:     global %13 stat_vol_int: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %15 str: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 vol_str: volatile @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 vol_ptr_str: volatile ptr<@type0> [storage=static] = addr_of<ptr<@type0>>(%15) [linkage=external];
// DEFAULT-NEXT:     global %18 ptr_vol_str: ptr<volatile @type0> [storage=static] = addr_of<ptr<volatile @type0>>(%16) [linkage=external];
// DEFAULT-NEXT:     global %19 vol_ptr_vol_str: volatile ptr<volatile @type0> [storage=static] = addr_of<ptr<volatile @type0>>(%16) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%24 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %26 @__builtin_prefetch(%25 <unnamed>: ptr<const void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %20 @simple_vol_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<volatile i32>, length=Some(100)>(%3)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>, volatile>(%4)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>>(%5)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>, volatile>(%6)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i32>>(%7)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @simple_vol_file() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<volatile i32>, length=Some(100)>(%9)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>, volatile>(%10)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>>(%11)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>, volatile>(%12)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i32>>(%13)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @expr_vol_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile @type0>>(%16)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile @type0>>(%18)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>, volatile>(%17)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile @type0>, volatile>(%19)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i16>>(field1(%16))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i16>>(field1(deref(read<ptr<volatile @type0>>(%18))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(deref(read<ptr<@type0>, volatile>(%17))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i16>>(field1(deref(read<ptr<volatile @type0>, volatile>(%19))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile array<i8, 8>>>(field3(%16))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 8>>>(field3(deref(read<ptr<@type0>, volatile>(%17))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile array<i8, 8>>>(field3(deref(read<ptr<volatile @type0>>(%18))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile array<i8, 8>>>(field3(deref(read<ptr<volatile @type0>, volatile>(%19))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>, volatile>(field4(%16))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>>(field4(deref(read<ptr<@type0>, volatile>(%17))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>, volatile>(field4(deref(read<ptr<volatile @type0>>(%18))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>, volatile>(field4(deref(read<ptr<volatile @type0>, volatile>(%19))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type0>, volatile>(field4(%16)))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type0>>(field4(deref(read<ptr<@type0>, volatile>(%17)))))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type0>, volatile>(field4(deref(read<ptr<volatile @type0>>(%18)))))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type0>, volatile>(field4(deref(read<ptr<volatile @type0>, volatile>(%19)))))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile array<i32, 100>>>(%3)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>, volatile>(%4)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>>(%5)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<volatile i32>, volatile>(%6)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(100)>(%3), const<i32>(2))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>, volatile>(%4), const<i32>(3))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%5), const<i32>(3))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>, volatile>(%6), const<i32>(3))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(100)>(%3), const<i32>(3))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(100)>(%3), read<i32, volatile>(%7))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>, volatile>(%4), const<i32>(5))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%5), const<i32>(5))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>, volatile>(%6), const<i32>(5))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>, volatile>(%4), read<i32, volatile>(%7))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>>(%5), read<i32, volatile>(%7))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%26, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(read<ptr<volatile i32>, volatile>(%6), read<i32, volatile>(%7))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         write<ptr<@type0>>(field4(%15), addr_of<ptr<@type0>>(%15));
// DEFAULT-NEXT:         write<ptr<@type0>, volatile>(field4(%16), addr_of<ptr<@type0>>(%15));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
