









case_00209: `int (*fp)()` called with one argument — pre-C23 K&R "unspecified parameters" semantics; slate always parses with `-std=c23`, under which `()` means zero parameters, so clang itself rejects the call before slate ever sees CIR. Not fixable without also supporting an older `-std=` mode.

case_00216: `anon_0` — nested anonymous struct/union field naming resolution bug; same family as the already-tracked slate-nk3.52.1 anonymous struct/union naming issue.
