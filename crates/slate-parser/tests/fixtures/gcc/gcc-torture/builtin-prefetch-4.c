/* Test that __builtin_prefetch does no harm.

   Check that the expression containing the address to prefetch is
   evaluated if it has side effects, even if the target does not support
   data prefetch.  Check changes to pointers and to array indices that are
   either global variables or arguments.  */

void abort(void);
void exit(int);

#define ARRSIZE 100

int  arr[ARRSIZE];
int *ptr      = &arr[20];
int  arrindex = 4;

/* Check that assignment within a prefetch argument is evaluated.  */

int assign_arg_ptr(int *p) {
  int *q;
  __builtin_prefetch((q = p), 0, 0);
  return q == p;
}

int assign_glob_ptr(void) {
  int *q;
  __builtin_prefetch((q = ptr), 0, 0);
  return q == ptr;
}

int assign_arg_idx(int *p, int i) {
  int j;
  __builtin_prefetch(&p[j = i], 0, 0);
  return j == i;
}

int assign_glob_idx(void) {
  int j;
  __builtin_prefetch(&ptr[j = arrindex], 0, 0);
  return j == arrindex;
}

/* Check that pre/post increment/decrement within a prefetch argument are
   evaluated.  */

int preinc_arg_ptr(int *p) {
  int *q;
  q = p + 1;
  __builtin_prefetch(++p, 0, 0);
  return p == q;
}

int preinc_glob_ptr(void) {
  int *q;
  q = ptr + 1;
  __builtin_prefetch(++ptr, 0, 0);
  return ptr == q;
}

int postinc_arg_ptr(int *p) {
  int *q;
  q = p + 1;
  __builtin_prefetch(p++, 0, 0);
  return p == q;
}

int postinc_glob_ptr(void) {
  int *q;
  q = ptr + 1;
  __builtin_prefetch(ptr++, 0, 0);
  return ptr == q;
}

int predec_arg_ptr(int *p) {
  int *q;
  q = p - 1;
  __builtin_prefetch(--p, 0, 0);
  return p == q;
}

int predec_glob_ptr(void) {
  int *q;
  q = ptr - 1;
  __builtin_prefetch(--ptr, 0, 0);
  return ptr == q;
}

int postdec_arg_ptr(int *p) {
  int *q;
  q = p - 1;
  __builtin_prefetch(p--, 0, 0);
  return p == q;
}

int postdec_glob_ptr(void) {
  int *q;
  q = ptr - 1;
  __builtin_prefetch(ptr--, 0, 0);
  return ptr == q;
}

int preinc_arg_idx(int *p, int i) {
  int j = i + 1;
  __builtin_prefetch(&p[++i], 0, 0);
  return i == j;
}

int preinc_glob_idx(void) {
  int j = arrindex + 1;
  __builtin_prefetch(&ptr[++arrindex], 0, 0);
  return arrindex == j;
}

int postinc_arg_idx(int *p, int i) {
  int j = i + 1;
  __builtin_prefetch(&p[i++], 0, 0);
  return i == j;
}

int postinc_glob_idx(void) {
  int j = arrindex + 1;
  __builtin_prefetch(&ptr[arrindex++], 0, 0);
  return arrindex == j;
}

int predec_arg_idx(int *p, int i) {
  int j = i - 1;
  __builtin_prefetch(&p[--i], 0, 0);
  return i == j;
}

int predec_glob_idx(void) {
  int j = arrindex - 1;
  __builtin_prefetch(&ptr[--arrindex], 0, 0);
  return arrindex == j;
}

int postdec_arg_idx(int *p, int i) {
  int j = i - 1;
  __builtin_prefetch(&p[i--], 0, 0);
  return i == j;
}

int postdec_glob_idx(void) {
  int j = arrindex - 1;
  __builtin_prefetch(&ptr[arrindex--], 0, 0);
  return arrindex == j;
}

/* Check that function calls within the first prefetch argument are
   evaluated.  */

int getptrcnt = 0;

int *getptr(int *p) {
  getptrcnt++;
  return p + 1;
}

int funccall_arg_ptr(int *p) {
  __builtin_prefetch(getptr(p), 0, 0);
  return getptrcnt == 1;
}

int getintcnt = 0;

int getint(int i) {
  getintcnt++;
  return i + 1;
}

int funccall_arg_idx(int *p, int i) {
  __builtin_prefetch(&p[getint(i)], 0, 0);
  return getintcnt == 1;
}

int main() {
  if (!assign_arg_ptr(ptr))
    abort();
  if (!assign_glob_ptr())
    abort();
  if (!assign_arg_idx(ptr, 4))
    abort();
  if (!assign_glob_idx())
    abort();
  if (!preinc_arg_ptr(ptr))
    abort();
  if (!preinc_glob_ptr())
    abort();
  if (!postinc_arg_ptr(ptr))
    abort();
  if (!postinc_glob_ptr())
    abort();
  if (!predec_arg_ptr(ptr))
    abort();
  if (!predec_glob_ptr())
    abort();
  if (!postdec_arg_ptr(ptr))
    abort();
  if (!postdec_glob_ptr())
    abort();
  if (!preinc_arg_idx(ptr, 3))
    abort();
  if (!preinc_glob_idx())
    abort();
  if (!postinc_arg_idx(ptr, 3))
    abort();
  if (!postinc_glob_idx())
    abort();
  if (!predec_arg_idx(ptr, 3))
    abort();
  if (!predec_glob_idx())
    abort();
  if (!postdec_arg_idx(ptr, 3))
    abort();
  if (!postdec_glob_idx())
    abort();
  if (!funccall_arg_ptr(ptr))
    abort();
  if (!funccall_arg_idx(ptr, 3))
    abort();
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
// DEFAULT-NEXT:     global %2 arr: array<i32, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %3 ptr: ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(100)>(%2), const<i32>(20)))) [linkage=external];
// DEFAULT-NEXT:     global %4 arrindex: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %60 getptrcnt: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %65 getintcnt: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%72 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @assign_arg_ptr(%6 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%7, read<ptr<i32>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%6)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%7), read<ptr<i32>>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @assign_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%9, read<ptr<i32>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%3)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%9), read<ptr<i32>>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @assign_arg_idx(%11 p: ptr<i32>, %12 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%12));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), read<i32>(%12))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%13), read<i32>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @assign_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%4))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%15), read<i32>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @preinc_arg_ptr(%17 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%18, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%17), const<i32>(1)));
// DEFAULT-NEXT:         let %73: ptr<i32> [synthetic] = read<ptr<i32>>(%17);
// DEFAULT-NEXT:         let %74: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%73), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%17, read<ptr<i32>>(%74));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%74)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%17), read<ptr<i32>>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @preinc_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%20, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), const<i32>(1)));
// DEFAULT-NEXT:         let %75: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %76: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%75), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%76));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%76)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%3), read<ptr<i32>>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @postinc_arg_ptr(%22 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%23, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%22), const<i32>(1)));
// DEFAULT-NEXT:         let %77: ptr<i32> [synthetic] = read<ptr<i32>>(%22);
// DEFAULT-NEXT:         let %78: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%77), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%22, read<ptr<i32>>(%78));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%77)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%22), read<ptr<i32>>(%23)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @postinc_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%25, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), const<i32>(1)));
// DEFAULT-NEXT:         let %79: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %80: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%79), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%80));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%79)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%3), read<ptr<i32>>(%25)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @predec_arg_ptr(%27 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%28, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%27), const<i32>(1)));
// DEFAULT-NEXT:         let %81: ptr<i32> [synthetic] = read<ptr<i32>>(%27);
// DEFAULT-NEXT:         let %82: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%81), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%27, read<ptr<i32>>(%82));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%82)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%27), read<ptr<i32>>(%28)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @predec_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%30, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%3), const<i32>(1)));
// DEFAULT-NEXT:         let %83: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %84: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%83), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%84));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%84)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%3), read<ptr<i32>>(%30)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @postdec_arg_ptr(%32 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%33, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%32), const<i32>(1)));
// DEFAULT-NEXT:         let %85: ptr<i32> [synthetic] = read<ptr<i32>>(%32);
// DEFAULT-NEXT:         let %86: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%85), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%32, read<ptr<i32>>(%86));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%85)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%32), read<ptr<i32>>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @postdec_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%35, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%3), const<i32>(1)));
// DEFAULT-NEXT:         let %87: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %88: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%87), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%88));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i32>>(%87)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%3), read<ptr<i32>>(%35)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @preinc_arg_idx(%37 p: ptr<i32>, %38 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39 j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:         let %89: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:         let %90: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%89), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%38, read<i32>(%90));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%37), read<i32>(%90))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%38), read<i32>(%39)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @preinc_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %41 j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:         let %91: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %92: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%91), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%92));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%92))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%4), read<i32>(%41)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @postinc_arg_idx(%43 p: ptr<i32>, %44 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %45 j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:         let %93: i32 [synthetic] = read<i32>(%44);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%93), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%44, read<i32>(%94));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%43), read<i32>(%93))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%44), read<i32>(%45)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @postinc_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:         let %95: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%95), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%96));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%95))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%4), read<i32>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @predec_arg_idx(%49 p: ptr<i32>, %50 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %51 j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:         let %97: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:         let %98: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%97), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%50, read<i32>(%98));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%49), read<i32>(%98))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%50), read<i32>(%51)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @predec_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %53 j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:         let %99: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %100: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%99), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%100));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%100))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%4), read<i32>(%53)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @postdec_arg_idx(%55 p: ptr<i32>, %56 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %57 j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:         let %101: i32 [synthetic] = read<i32>(%56);
// DEFAULT-NEXT:         let %102: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%101), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%56, read<i32>(%102));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%55), read<i32>(%101))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%56), read<i32>(%57)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @postdec_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %59 j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:         let %103: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %104: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%103), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%104));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%103))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%4), read<i32>(%59)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @getptr(%62 p: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %105: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:         let %106: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%105), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%60, read<i32>(%106));
// DEFAULT-NEXT:         return ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%62), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @funccall_arg_ptr(%64 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%61, read<ptr<i32>>(%64))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%60), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @getint(%67 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %107: i32 [synthetic] = read<i32>(%65);
// DEFAULT-NEXT:         let %108: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%107), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%65, read<i32>(%108));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%67), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @funccall_arg_idx(%69 p: ptr<i32>, %70 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%69), call<i32, signature=fn(i32) -> i32>(%66, read<i32>(%70)))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%65), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%5, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%8), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%10, read<ptr<i32>>(%3), const<i32>(4)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%16, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%19), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%21, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%24), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%26, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%29), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%31, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%34), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%36, read<ptr<i32>>(%3), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%42, read<ptr<i32>>(%3), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%46), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%48, read<ptr<i32>>(%3), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%52), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%54, read<ptr<i32>>(%3), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%58), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%63, read<ptr<i32>>(%3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%68, read<ptr<i32>>(%3), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
