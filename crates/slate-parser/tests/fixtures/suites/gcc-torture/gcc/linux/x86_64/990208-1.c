/* { dg-require-effective-target label_values } */

/* As a quality of implementation issue, we should not prevent inlining
   of function explicitly marked inline just because a label therein had
   its address taken.  */

void abort(void);
void exit(int);

static void *ptr1, *ptr2;
static int   i = 1;

static __inline__ void doit(void **pptr, int cond) {
  if (cond) {
  here:
    *pptr = &&here;
  }
}

__attribute__((noinline)) static void f(int cond) { doit(&ptr1, cond); }

__attribute__((noinline)) static void g(int cond) { doit(&ptr2, cond); }

__attribute__((noinline)) static void bar(void);

int main() {
  f(i);
  bar();
  g(i);

#ifdef __OPTIMIZE__
  if (ptr1 == ptr2)
    abort();
#endif

  exit(0);
}

void bar(void) {}


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
// DEFAULT-NEXT:     global %[[VALUE_ptr1:[0-9]+]] ptr1: ptr<void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ptr2:[0-9]+]] ptr2: ptr<void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_doit:[0-9]+]] @doit(%[[VALUE_pptr:[0-9]+]] pptr: ptr<ptr<void>>, %[[VALUE_cond:[0-9]+]] cond: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_cond]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_here:[0-9]+]] here:
// DEFAULT-NEXT:                     write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_pptr]])), label_addr<ptr<void>>(%[[VALUE_here]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_cond_2:[0-9]+]] cond: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, i32) -> void>(%[[VALUE_doit]], addr_of<ptr<ptr<void>>>(%[[VALUE_ptr1]]), read<i32>(%[[VALUE_cond_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_cond_3:[0-9]+]] cond: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, i32) -> void>(%[[VALUE_doit]], addr_of<ptr<ptr<void>>>(%[[VALUE_ptr2]]), read<i32>(%[[VALUE_cond_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_g]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
