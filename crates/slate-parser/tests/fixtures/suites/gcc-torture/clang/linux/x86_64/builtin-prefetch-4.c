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
// DEFAULT-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i32, 100> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr:[0-9]+]] ptr: ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(100)>(%[[VALUE_arr]]), const<i32>(20)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arrindex:[0-9]+]] arrindex: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_getptrcnt:[0-9]+]] getptrcnt: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_getintcnt:[0-9]+]] getintcnt: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_prefetch:[0-9]+]] @__builtin_prefetch(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_assign_arg_ptr:[0-9]+]] @assign_arg_ptr(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q]], read<ptr<i32>>(%[[VALUE2]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_q]]), read<ptr<i32>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_assign_glob_ptr:[0-9]+]] @assign_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_2]], read<ptr<i32>>(%[[VALUE3]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_q_2]]), read<ptr<i32>>(%[[VALUE_ptr]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_assign_arg_idx:[0-9]+]] @assign_arg_idx(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>, %[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_assign_glob_idx:[0-9]+]] @assign_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arrindex]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_j_2]]), read<i32>(%[[VALUE_arrindex]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_preinc_arg_ptr:[0-9]+]] @preinc_arg_ptr(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_3:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_3]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_3]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p_3]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_3]], read<ptr<i32>>(%[[VALUE7]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_3]]), read<ptr<i32>>(%[[VALUE_q_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_preinc_glob_ptr:[0-9]+]] @preinc_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_4:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_4]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr]], read<ptr<i32>>(%[[VALUE9]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ptr]]), read<ptr<i32>>(%[[VALUE_q_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postinc_arg_ptr:[0-9]+]] @postinc_arg_ptr(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_5:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_5]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_4]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p_4]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_4]], read<ptr<i32>>(%[[VALUE11]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_4]]), read<ptr<i32>>(%[[VALUE_q_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postinc_glob_ptr:[0-9]+]] @postinc_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_6:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_6]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr]], read<ptr<i32>>(%[[VALUE13]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ptr]]), read<ptr<i32>>(%[[VALUE_q_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_predec_arg_ptr:[0-9]+]] @predec_arg_ptr(%[[VALUE_p_5:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_7:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_7]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_5]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p_5]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_5]], read<ptr<i32>>(%[[VALUE15]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_5]]), read<ptr<i32>>(%[[VALUE_q_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_predec_glob_ptr:[0-9]+]] @predec_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_8:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_8]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr]], read<ptr<i32>>(%[[VALUE17]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ptr]]), read<ptr<i32>>(%[[VALUE_q_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postdec_arg_ptr:[0-9]+]] @postdec_arg_ptr(%[[VALUE_p_6:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_9:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_9]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_6]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p_6]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_6]], read<ptr<i32>>(%[[VALUE19]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p_6]]), read<ptr<i32>>(%[[VALUE_q_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postdec_glob_ptr:[0-9]+]] @postdec_glob_ptr() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_10:[0-9]+]] q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_q_10]], ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_ptr]], read<ptr<i32>>(%[[VALUE21]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ptr]]), read<ptr<i32>>(%[[VALUE_q_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_preinc_arg_idx:[0-9]+]] @preinc_arg_idx(%[[VALUE_p_7:[0-9]+]] p: ptr<i32>, %[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_3:[0-9]+]] j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_j_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_preinc_glob_idx:[0-9]+]] @preinc_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_4:[0-9]+]] j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_arrindex]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arrindex]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arrindex]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_arrindex]]), read<i32>(%[[VALUE_j_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postinc_arg_idx:[0-9]+]] @postinc_arg_idx(%[[VALUE_p_8:[0-9]+]] p: ptr<i32>, %[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_5:[0-9]+]] j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_i_3]]), read<i32>(%[[VALUE_j_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postinc_glob_idx:[0-9]+]] @postinc_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_6:[0-9]+]] j: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_arrindex]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arrindex]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arrindex]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_arrindex]]), read<i32>(%[[VALUE_j_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_predec_arg_idx:[0-9]+]] @predec_arg_idx(%[[VALUE_p_9:[0-9]+]] p: ptr<i32>, %[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_7:[0-9]+]] j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_4]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_i_4]]), read<i32>(%[[VALUE_j_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_predec_glob_idx:[0-9]+]] @predec_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_8:[0-9]+]] j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_arrindex]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arrindex]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arrindex]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_arrindex]]), read<i32>(%[[VALUE_j_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postdec_arg_idx:[0-9]+]] @postdec_arg_idx(%[[VALUE_p_10:[0-9]+]] p: ptr<i32>, %[[VALUE_i_5:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_9:[0-9]+]] j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_5]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_i_5]]), read<i32>(%[[VALUE_j_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_postdec_glob_idx:[0-9]+]] @postdec_glob_idx() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j_10:[0-9]+]] j: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_arrindex]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arrindex]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arrindex]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_arrindex]]), read<i32>(%[[VALUE_j_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getptr:[0-9]+]] @getptr(%[[VALUE_p_11:[0-9]+]] p: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_getptrcnt]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_getptrcnt]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:         return ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_11]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_funccall_arg_ptr:[0-9]+]] @funccall_arg_ptr(%[[VALUE_p_12:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%[[VALUE_getptr]], read<ptr<i32>>(%[[VALUE_p_12]]))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_getptrcnt]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getint:[0-9]+]] @getint(%[[VALUE_i_6:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_getintcnt]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_getintcnt]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i_6]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_funccall_arg_idx:[0-9]+]] @funccall_arg_idx(%[[VALUE_p_13:[0-9]+]] p: ptr<i32>, %[[VALUE_i_7:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_13]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_getint]], read<i32>(%[[VALUE_i_7]])))))), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_getintcnt]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_assign_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_assign_glob_ptr]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_assign_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(4)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_assign_glob_idx]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_preinc_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_preinc_glob_ptr]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_postinc_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_postinc_glob_ptr]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_predec_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_predec_glob_ptr]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_postdec_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_postdec_glob_ptr]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_preinc_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_preinc_glob_idx]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_postinc_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_postinc_glob_idx]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_predec_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_predec_glob_idx]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_postdec_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_postdec_glob_idx]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_funccall_arg_ptr]], read<ptr<i32>>(%[[VALUE_ptr]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, i32) -> i32>(%[[VALUE_funccall_arg_idx]], read<ptr<i32>>(%[[VALUE_ptr]]), const<i32>(3)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
