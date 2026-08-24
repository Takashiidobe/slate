# Typed module alias ownership

_created 2026-08-24_

`clang_ir::model::Module` owns the normalized type and attribute alias tables
used by semantic consumers. It exposes alias-chain resolution directly, so
lowerers do not reach through `Module::generic` for structural information.

The typed module does not retain the generic module or raw function/global
operations. Generic parsing remains an explicit API for normalization-only
operations such as selective CFG flattening, and conversion consumes that tree
after preserving aliases, typed bodies, metadata, and unconverted top-level
operations.
