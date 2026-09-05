#include "shared.h"

void init_flags(struct Flags *flags, void *owner, int count) {
  flags->owner = owner;
  flags->count = count;
  // @lowering-begin
  // @rewrite-begin
  flags->ready = 1;
  flags->mode  = 5;
  // @rewrite-end
  // @lowering-end
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = 1;
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready(({{__v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = 5;
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode(({{__v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready((1 as u32) << 31 >> 31);
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode((5 as u32) << 29 >> 29);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32 = 1;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready(({{__v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-X86_64-GNU-DAG: }
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32 = 5;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode(({{__v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u32 = 1;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready(({{__v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u32 = 5;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode(({{__v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready((1 as u32) << 31 >> 31);
// REWRITES-X86_64-GNU-DAG: }
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode((5 as u32) << 29 >> 29);
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_ready((1 as u32) << 31 >> 31);
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     (*{{arg[0-9]+}}).__bitfield_2.set_mode((5 as u32) << 29 >> 29);
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
