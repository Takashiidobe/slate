/* Test that __builtin_prefetch does no harm.

   Use addresses that are unlikely to be word-aligned.  Some targets
   have alignment requirements for prefetch addresses, so make sure the
   compiler takes care of that.  This fails if it aborts, anything else
   is OK.  */

void exit(int);

struct S {
  short a;
  short b;
  char  c[8];
} s;

char  arr[100];
char *ptr = arr;
int   idx = 3;

void arg_ptr(char *p) { __builtin_prefetch(p, 0, 0); }

void arg_idx(char *p, int i) { __builtin_prefetch(&p[i], 0, 0); }

void glob_ptr(void) { __builtin_prefetch(ptr, 0, 0); }

void glob_idx(void) { __builtin_prefetch(&ptr[idx], 0, 0); }

int main() {
  __builtin_prefetch(&s.b, 0, 0);
  __builtin_prefetch(&s.c[1], 0, 0);

  arg_ptr(&s.c[1]);
  arg_ptr(ptr + 3);
  arg_idx(ptr, 3);
  arg_idx(ptr + 1, 2);
  idx = 3;
  glob_ptr();
  glob_idx();
  ptr++;
  idx = 2;
  glob_ptr();
  glob_idx();
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
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: array<i8, 8>;
// DEFAULT-NEXT:     } [size=12, align=2, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     global %2 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 arr: array<i8, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 ptr: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(100)>(%3) [linkage=external];
// DEFAULT-NEXT:     global %5 idx: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%14 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @__builtin_prefetch(%15 <unnamed>: ptr<const void>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @arg_ptr(%7 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%7)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @arg_idx(%9 p: ptr<i8>, %10 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), read<i32>(%10))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @glob_ptr() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%4)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @glob_idx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), read<i32>(%5))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(%2))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%16, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field2(%2)), const<i32>(1))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%6, addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field2(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%6, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, i32) -> void>(%8, read<ptr<i8>>(%4), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, i32) -> void>(%8, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), const<i32>(1)), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         let %17: ptr<i8> [synthetic] = read<ptr<i8>>(%4);
// DEFAULT-NEXT:         let %18: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%17), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%4, read<ptr<i8>>(%18));
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
