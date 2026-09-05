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
// LOWERING-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 42;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// LOWERING-AARCH64-GNU-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 42;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:     return {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 42;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-DAG:     if {{__v[0-9]+}} == {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub unsafe extern "C-unwind" fn mode_value({{arg[0-9]+}}: *mut Holder) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: u32 = (unsafe { (*{{arg[0-9]+}}).mode }) as u32;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: u32 = Mode::MODE_READY as u32;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 42;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-AARCH64-GNU-DAG:     if {{__v[0-9]+}} == {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
