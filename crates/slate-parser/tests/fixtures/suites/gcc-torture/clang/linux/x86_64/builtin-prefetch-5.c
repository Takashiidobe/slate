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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: array<i8, 8>;
// DEFAULT-NEXT:     } [size=12, align=2, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i8, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr:[0-9]+]] ptr: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_arr]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_idx:[0-9]+]] idx: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_prefetch:[0-9]+]] @__builtin_prefetch(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_arg_ptr:[0-9]+]] @arg_ptr(%[[VALUE_p:[0-9]+]] p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_p]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_arg_idx:[0-9]+]] @arg_idx(%[[VALUE_p_2:[0-9]+]] p: ptr<i8>, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE_i]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_glob_ptr:[0-9]+]] @glob_ptr() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%[[VALUE_ptr]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_glob_idx:[0-9]+]] @glob_idx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_ptr]]), read<i32>(%[[VALUE_idx]]))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i16>>(field1(%[[VALUE_s]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field2(%[[VALUE_s]])), const<i32>(1))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_arg_ptr]], addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field2(%[[VALUE_s]])), const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_arg_ptr]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_ptr]]), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, i32) -> void>(%[[VALUE_arg_idx]], read<ptr<i8>>(%[[VALUE_ptr]]), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, i32) -> void>(%[[VALUE_arg_idx]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_ptr]]), const<i32>(1)), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_idx]], const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_glob_ptr]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_glob_idx]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_ptr]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_idx]], const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_glob_ptr]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_glob_idx]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
