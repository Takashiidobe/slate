#include "shared.h"

// @lowering-fn-begin
// @rewrite-fn-begin
int mode_value(struct Holder *holder) {
  return holder->mode == MODE_READY ? 42 : 0;
}
// @rewrite-fn-end
// @lowering-fn-end
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub unsafe extern "C" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// LOWERING-DAG: let mut holder: *mut Holder = std::ptr::null_mut();
// LOWERING-DAG: let mut __retval: i32 = 0;
// LOWERING-DAG: holder = {{arg[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut Holder = holder;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = (unsafe { (*{{_v[0-9]+}}).mode }) as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 42;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-DAG: __retval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// REWRITES-DAG: let mut __retval: i32 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 42;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-DAG: __retval = if {{_v[0-9]+}} == {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-DAG: return __retval;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
