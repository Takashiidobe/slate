typedef enum { REPEAT_NONE, REPEAT_CHECK, REPEAT_VALID } FSE_repeat;

typedef struct {
  FSE_repeat mode;
} Entropy;

static unsigned select_type(FSE_repeat *repeatMode, unsigned count) {
  if (count == 0) {
    *repeatMode = REPEAT_NONE;
    return 0;
  }
  return (unsigned)*repeatMode + count;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int run(Entropy *e, unsigned count) {
  return (int)select_type(&e->mode, count);
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  Entropy e;
  e.mode = REPEAT_VALID;
  return run(&e, 0) + run(&e, 5);
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn select_type({{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: u32) -> u32 {
// LOWERING-DAG: let mut repeatMode: *mut u32 = std::ptr::null_mut();
// LOWERING-DAG: let mut count: u32 = 0;
// LOWERING-DAG: let mut __retval: u32 = 0;
// LOWERING-DAG: repeatMode = {{arg[0-9]+}};
// LOWERING-DAG: count = {{arg[0-9]+}};
// LOWERING-DAG: {
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = count;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG: if {{_v[0-9]+}} {
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = FSE_repeat::REPEAT_NONE as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut u32 = repeatMode;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 0;
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: *mut u32 = repeatMode;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = count;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn run({{arg[0-9]+}}: *mut Entropy, {{arg[0-9]+}}: u32) -> i32 {
// LOWERING-DAG: let mut e: *mut Entropy = std::ptr::null_mut();
// LOWERING-DAG: let mut count: u32 = 0;
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: e = {{arg[0-9]+}};
// LOWERING-DAG: count = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Entropy = e;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = count;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = select_type((unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).mode) }) as *mut u32, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn select_type({{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: u32) -> u32 {
// REWRITES-DAG: let mut count: u32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: u32 = 0;
// REWRITES-DAG: {
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-DAG: if count == {{_v[0-9]+}} {
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = FSE_repeat::REPEAT_NONE as u32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: *{{arg[0-9]+}} = {{_v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: __retval = 0;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// REWRITES-DAG: }
// REWRITES-DAG: __retval = (unsafe { *{{arg[0-9]+}} }) + count;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// REWRITES-DAG: fn run({{arg[0-9]+}}: &mut Entropy, {{arg[0-9]+}}: u32) -> i32 {
// REWRITES-DAG: let mut count: u32 = {{arg[0-9]+}};
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut Entropy = {{arg[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = select_type((unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).mode) }) as *mut u32, count);
// REWRITES-DAG: __retval = {{_v[0-9]+}} as i32;
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
