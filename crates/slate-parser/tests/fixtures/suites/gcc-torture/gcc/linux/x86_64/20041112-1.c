/* This was failing on Alpha because the comparison (p != -1) was rewritten
   as (p+1 != 0) and p+1 isn't allowed to wrap for pointers.  */

extern void abort(void);

typedef __SIZE_TYPE__ size_t;

int global;

static void *foo(int p) {
  if (p == 0) {
    global++;
    return &global;
  }

  return (void *)(size_t)-1;
}

int bar(void) {
  void *p;

  p = foo(global);
  if (p != (void *)(size_t)-1)
    return 1;

  global++;
  return 0;
}

int main(void) {
  global = 1;
  if (bar() != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: i32) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_p]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_global]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_global]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                 return pointer_cast<ptr<void>, reason=return>(addr_of<ptr<i32>>(%[[VALUE_global]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p_2]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_foo]], read<i32>(%[[VALUE_global]])));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p_2]]), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_global]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_global]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_global]], const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_bar]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
