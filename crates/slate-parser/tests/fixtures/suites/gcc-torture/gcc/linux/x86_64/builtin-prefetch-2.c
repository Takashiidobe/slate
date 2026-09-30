/* Test that __builtin_prefetch does no harm.

   Prefetch data using a variety of storage classes and address
   expressions.  */

void exit(int);

int  glob_int_arr[100];
int *glob_ptr_int = glob_int_arr;
int  glob_int     = 4;

static int  stat_int_arr[100];
static int *stat_ptr_int = stat_int_arr;
static int  stat_int;

struct S {
  int       a;
  short     b, c;
  char      d[8];
  struct S *next;
};

struct S  str;
struct S *ptr_str = &str;

/* Prefetch global variables using the address of the variable.  */

void simple_global() {
  __builtin_prefetch(glob_int_arr, 0, 0);
  __builtin_prefetch(glob_ptr_int, 0, 0);
  __builtin_prefetch(&glob_int, 0, 0);
}

/* Prefetch file-level static variables using the address of the variable.  */

void simple_file() {
  __builtin_prefetch(stat_int_arr, 0, 0);
  __builtin_prefetch(stat_ptr_int, 0, 0);
  __builtin_prefetch(&stat_int, 0, 0);
}

/* Prefetch local static variables using the address of the variable.  */

void simple_static_local() {
  static int  gx[100];
  static int *hx = gx;
  static int  ix;
  __builtin_prefetch(gx, 0, 0);
  __builtin_prefetch(hx, 0, 0);
  __builtin_prefetch(&ix, 0, 0);
}

/* Prefetch local stack variables using the address of the variable.  */

void simple_local() {
  int  gx[100];
  int *hx = gx;
  int  ix;
  __builtin_prefetch(gx, 0, 0);
  __builtin_prefetch(hx, 0, 0);
  __builtin_prefetch(&ix, 0, 0);
}

/* Prefetch arguments using the address of the variable.  */

void simple_arg(int g[100], int *h, int i) {
  __builtin_prefetch(g, 0, 0);
  __builtin_prefetch(h, 0, 0);
  __builtin_prefetch(&i, 0, 0);
}

/* Prefetch using address expressions involving global variables.  */

void expr_global(void) {
  __builtin_prefetch(&str, 0, 0);
  __builtin_prefetch(ptr_str, 0, 0);
  __builtin_prefetch(&str.b, 0, 0);
  __builtin_prefetch(&ptr_str->b, 0, 0);
  __builtin_prefetch(&str.d, 0, 0);
  __builtin_prefetch(&ptr_str->d, 0, 0);
  __builtin_prefetch(str.next, 0, 0);
  __builtin_prefetch(ptr_str->next, 0, 0);
  __builtin_prefetch(str.next->d, 0, 0);
  __builtin_prefetch(ptr_str->next->d, 0, 0);

  __builtin_prefetch(&glob_int_arr, 0, 0);
  __builtin_prefetch(glob_ptr_int, 0, 0);
  __builtin_prefetch(&glob_int_arr[2], 0, 0);
  __builtin_prefetch(&glob_ptr_int[3], 0, 0);
  __builtin_prefetch(glob_int_arr + 3, 0, 0);
  __builtin_prefetch(glob_int_arr + glob_int, 0, 0);
  __builtin_prefetch(glob_ptr_int + 5, 0, 0);
  __builtin_prefetch(glob_ptr_int + glob_int, 0, 0);
}

/* Prefetch using address expressions involving local variables.  */

void expr_local(void) {
  int       b[10];
  int      *pb = b;
  struct S  t;
  struct S *pt = &t;
  int       j  = 4;

  __builtin_prefetch(&t, 0, 0);
  __builtin_prefetch(pt, 0, 0);
  __builtin_prefetch(&t.b, 0, 0);
  __builtin_prefetch(&pt->b, 0, 0);
  __builtin_prefetch(&t.d, 0, 0);
  __builtin_prefetch(&pt->d, 0, 0);
  __builtin_prefetch(t.next, 0, 0);
  __builtin_prefetch(pt->next, 0, 0);
  __builtin_prefetch(t.next->d, 0, 0);
  __builtin_prefetch(pt->next->d, 0, 0);

  __builtin_prefetch(&b, 0, 0);
  __builtin_prefetch(pb, 0, 0);
  __builtin_prefetch(&b[2], 0, 0);
  __builtin_prefetch(&pb[3], 0, 0);
  __builtin_prefetch(b + 3, 0, 0);
  __builtin_prefetch(b + j, 0, 0);
  __builtin_prefetch(pb + 5, 0, 0);
  __builtin_prefetch(pb + j, 0, 0);
}

int main() {
  simple_global();
  simple_file();
  simple_static_local();
  simple_local();
  simple_arg(glob_int_arr, glob_ptr_int, glob_int);

  str.next = &str;
  expr_global();
  expr_local();

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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:         field3 d: array<i8, 8>;
// DEFAULT-NEXT:         field4 next: ptr<@type[[TYPE_S]]>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 6, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_glob_int_arr:[0-9]+]] glob_int_arr: array<i32, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_glob_ptr_int:[0-9]+]] glob_ptr_int: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_glob_int:[0-9]+]] glob_int: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stat_int_arr:[0-9]+]] stat_int_arr: array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_stat_ptr_int:[0-9]+]] stat_ptr_int: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_stat_int_arr]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_stat_int:[0-9]+]] stat_int: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] str: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr_str:[0-9]+]] ptr_str: ptr<@type[[TYPE_S]]> [storage=static] = addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gx:[0-9]+]] gx: array<i32, 100> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_hx:[0-9]+]] hx: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_gx]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ix:[0-9]+]] ix: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_prefetch:[0-9]+]] @__builtin_prefetch(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_simple_global:[0-9]+]] @simple_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_glob_ptr_int]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_glob_int]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_simple_file:[0-9]+]] @simple_file() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_stat_int_arr]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_stat_ptr_int]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_stat_int]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_simple_static_local:[0-9]+]] @simple_static_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_gx]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_hx]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_ix]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_simple_local:[0-9]+]] @simple_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_gx_2:[0-9]+]] gx: array<i32, 100> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_hx_2:[0-9]+]] hx: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_gx_2]]);
// DEFAULT-NEXT:         let %[[VALUE_ix_2:[0-9]+]] ix: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_gx_2]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_hx_2]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_ix_2]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_simple_arg:[0-9]+]] @simple_arg(%[[VALUE_g:[0-9]+]] g: ptr<i32> [array=100], %[[VALUE_h:[0-9]+]] h: ptr<i32>, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_g]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_h]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_i]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_expr_global:[0-9]+]] @expr_global() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_str]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_ptr_str]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(%[[VALUE_str]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_ptr_str]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 8>>>(field3(%[[VALUE_str]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 8>>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_ptr_str]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(field4(%[[VALUE_str]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_ptr_str]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type[[TYPE_S]]>>(field4(%[[VALUE_str]])))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type[[TYPE_S]]>>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_ptr_str]])))))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i32, 100>>>(%[[VALUE_glob_int_arr]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_glob_ptr_int]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]]), const<i32>(2))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_glob_ptr_int]]), const<i32>(3))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]]), const<i32>(3))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]]), read<i32>(%[[VALUE_glob_int]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_glob_ptr_int]]), const<i32>(5))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_glob_ptr_int]]), read<i32>(%[[VALUE_glob_int]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_expr_local:[0-9]+]] @expr_local() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_pb:[0-9]+]] pb: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pt:[0-9]+]] pt: ptr<@type[[TYPE_S]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]]);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_t]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_pt]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(%[[VALUE_t]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_pt]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 8>>>(field3(%[[VALUE_t]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 8>>>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_pt]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(field4(%[[VALUE_t]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_pt]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type[[TYPE_S]]>>(field4(%[[VALUE_t]])))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field3(deref(read<ptr<@type[[TYPE_S]]>>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_pt]])))))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i32, 10>>>(%[[VALUE_b]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%[[VALUE_pb]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), const<i32>(2))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_pb]]), const<i32>(3))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), const<i32>(3))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_b]]), read<i32>(%[[VALUE_j]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_pb]]), const<i32>(5))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_pb]]), read<i32>(%[[VALUE_j]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_simple_global]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_simple_file]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_simple_static_local]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_simple_local]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>, i32) -> void>(%[[VALUE_simple_arg]], array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_glob_int_arr]]), read<ptr<i32>>(%[[VALUE_glob_ptr_int]]), read<i32>(%[[VALUE_glob_int]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(field4(%[[VALUE_str]]), addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_str]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_expr_global]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_expr_local]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
