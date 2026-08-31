#include "shared.h"

void init_flags(struct Flags *flags, void *owner, int count) {
  flags->owner = owner;
  flags->count = count;
  // @lowering-begin
  // @rewrite-begin
  flags->ready = 1;
  flags->mode = 5;
  // @rewrite-end
  // @lowering-end
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{arg[0-9]+}}).__bitfield_2.set_ready(({{_v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = 5;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{arg[0-9]+}}).__bitfield_2.set_mode(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 31 >> 31;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*{{arg[0-9]+}}).__bitfield_2.set_ready(({{_v[0-9]+}} as u32) << 31 >> 31);
// REWRITES-DAG: }
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = 5;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*{{arg[0-9]+}}).__bitfield_2.set_mode(({{_v[0-9]+}} as u32) << 29 >> 29);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
