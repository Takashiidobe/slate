# SQLite lowering barriers

- Target: x86_64 Linux, Clang flavor, gnu17.
- Input: `~/c-corpus/sqlite/build-clang/compile_commands.json`.
- Snapshot: `67cb738f2`; 94 TUs reach lowering, 30 clean, 64 with barriers.
- Probe each entry with `slate lowering-barriers --flavor=clang`, retaining
  preprocessing flags, resolving relative paths against its build directory,
  and pinning `-std=gnu17` when absent.
- Counts are first reported barriers; fixing one can expose another.
- Epic: `slate-wcf7`. Historical CIR failures are not current reproductions.

| Core sqlite3.c barrier | Count | Bead |
| --- | ---: | --- |
| Flexible-array record layouts | 2,061 | `slate-wcf7.3` |
| Gotos and labels | 16 | `slate-wcf7.4` |
| Variadic function pointers | 8 | `slate-wcf7.5` |
| Atomic reads and thread fence | 6 | `slate-wcf7.6` |
| Static pointer-offset initializers | 3 | `slate-wcf7.7` |
| Floating-point classification | 1 | `slate-wcf7.8` |

- Flexible-array layouts include `ExprList` (2,039 barriers), `SrcList`,
  `IdList`, `With`, and `WalIterator`; shell/recovery also needs `RecoverBitmap`.
- Shell has 72 record barriers, 29 goto/label barriers, and the `seenInterrupt`
  global attribute barrier (`slate-wcf7.9`).
- Corpus utilities/testfixture also expose static aggregate initialization,
  incomplete arrays, and unprototyped functions (`slate-p58o.7.11`).
- Generated-Rust build/link and runtime parity are unverified until lowering
  clears (`slate-wcf7.10`). Build core, shell, utilities, and Tcl testfixture
  as separate targets using their respective compilation flags.
