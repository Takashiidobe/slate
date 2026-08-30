# GitHub-derived Slate probes

These standalone C programs are exploratory inputs, not registered differential
fixtures. Each one reduces a construct found through `gh search code` to a small
runtime probe. They are intended to be promoted into `tests/fixtures/` with
lowering and rewrite FileCheck regions when a Slate defect is fixed.

| Probe                          | Stress point                                                         | Result                                     | Search origin                                                                                                                                                                      |
| ------------------------------ | -------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `computed_goto_label_delta.c`  | relative label addresses in a threaded interpreter                   | generated Rust panics at `unreachable!()`  | [Ruby VM dispatch](https://github.com/ruby/ruby/blob/3aa34522c1b13dfcb7dc8ba459eed90f3d886632/vm_exec.h)                                                                           |
| `statement_expr_local_label.c` | local labels and branches inside a value-producing expression        | generated Rust does not terminate          | [Linux wait macros](https://github.com/raspberrypi/linux/blob/b3aefe19d14cf15f2e41dfd269fa0ca6198dacd2/include/linux/wait.h)                                                       |
| `statement_expr_auto_type.c`   | single evaluation through `__auto_type`                              | matches native C                           | [NVIDIA atomic macros](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/e4a5faa2567f28c8eabe0ebb6422b6d0abcf37eb/src/nvidia/inc/libraries/nvport/inline/atomic_c11.h)        |
| `generic_type_dispatch.c`      | `_Generic` function selection and lvalue conversion                  | matches native C                           | [Ruri format dispatch](https://github.com/RuriOSS/ruri/blob/5aa881689f99d4b30d3d80acf0c93d6a17136fa8/src/info.c)                                                                   |
| `vla_bound_side_effects.c`     | evaluated and unevaluated VLA bounds plus parameter bounds           | matches native C                           | [GCC VLA tests](https://github.com/gcc-mirror/gcc/tree/9c40d80365f334e01f6e864e62ca9965841774cb/gcc/testsuite/gcc.dg)                                                              |
| `packed_mixed_bitfields.c`     | packed signed fields, zero-width alignment, and mixed storage widths | generated Rust fails with E0793            | [Linux packed structures](https://github.com/torvalds/linux/tree/78bb208b99e7d3314f670710fa9ee0a793682bca)                                                                         |
| `asm_goto_statement_expr.c`    | `asm goto` inside a statement expression with local labels           | lowering cannot find the AST labels        | [Linux BPF `may_goto`](https://github.com/SELinuxProject/selinux-kernel/blob/d0db3d89effa1ebff996584671f03cad9409f257/tools/testing/selftests/bpf/libarena/include/bpf_may_goto.h) |
| `asm_dialect_switch.c`         | Intel and AT&T syntax with extended-asm operands                     | matches native C; promoted to the differential suite | [Google AMD erratum reproducer](https://github.com/google/security-research/blob/1a21bff5a66b8f40f16c4a3577d212ff9890709f/pocs/cpus/errata/amd/genoa-evex-rsp/zenrsp.c)            |
| `c23_typeof_unqual.c`          | `typeof_unqual`, `nullptr`, `constexpr`, and `_BitInt` together      | matches native C                           | [PostgreSQL `copyObject`](https://github.com/postgres/postgres/blob/92819e57945d106317eaaa4273180dd8663f91bd/src/include/nodes/nodes.h#L220-L224)                                  |

Compile the C23 probe with `-std=gnu2x`; the other probes use `-std=gnu11`.
Results compare native Clang output and exit status against a generated release
Cargo project. The nontermination result uses a 10-second timeout.
