typedef enum { REPEAT_NONE, REPEAT_CHECK, REPEAT_VALID } FSE_repeat;

typedef struct {
  FSE_repeat mode;
} Entropy;

// @lowering-fn-begin
// @rewrite-fn-begin
static unsigned select_type(FSE_repeat *repeatMode, unsigned count) {
  if (count == 0) {
    *repeatMode = REPEAT_NONE;
    return 0;
  }
  return (unsigned)*repeatMode + count;
}
// @rewrite-fn-end
// @lowering-fn-end

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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn select_type({{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: u32) -> u32 {
// COMMON-LOWERING-DAG:     let mut repeatMode: *mut u32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut count: u32 = 0;
// COMMON-LOWERING-DAG:     let mut __retval: u32 = 0;
// COMMON-LOWERING-DAG:     repeatMode = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     count = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = count;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u32 = FSE_repeat::REPEAT_NONE as u32;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut u32 = repeatMode;
// COMMON-LOWERING-DAG:             unsafe {
// COMMON-LOWERING-DAG:                 *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: u32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u32 = repeatMode;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = count;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn run({{arg[0-9]+}}: *mut Entropy, {{arg[0-9]+}}: u32) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = select_type(
// COMMON-LOWERING-DAG:         (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).mode) }) as *mut u32,
// COMMON-LOWERING-DAG:         {{arg[0-9]+}},
// COMMON-LOWERING-DAG:     );
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn select_type(mut repeatMode: *mut u32, mut count: u32) -> u32 {
// COMMON-REWRITES-DAG:     if count == 0 {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: u32 = FSE_repeat::REPEAT_NONE as u32;
// COMMON-REWRITES-DAG:         unsafe {
// COMMON-REWRITES-DAG:             *repeatMode = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         return 0;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     (unsafe { *repeatMode }) + count
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn run({{arg[0-9]+}}: &mut Entropy, {{arg[0-9]+}}: u32) -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = select_type(
// COMMON-REWRITES-DAG:         (unsafe { std::ptr::addr_of_mut!((*({{arg[0-9]+}} as *mut Entropy)).mode) }) as *mut u32,
// COMMON-REWRITES-DAG:         {{arg[0-9]+}},
// COMMON-REWRITES-DAG:     );
// COMMON-REWRITES-DAG:     {{__v[0-9]+}} as i32
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
