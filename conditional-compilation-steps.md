# Conditional compilation steps

Macro epic `slate-f2zu.9.5.12`, in order.

- [x] 1. Close `9.5.7` and `9.5.10` if the fixtures confirm they are done.
- [x] 2. `.21`: write `merge-report.txt` on every `--merge` run.
- [x] 3. `.18`: drop hole types from the template key.
- [x] 4. `9.5.6`: pass caller locals as hidden arguments, then regenerate zlib.
- [x] 5. `.10`: allow `return`, `break` and `continue` in templates.
- [x] 6. `.20`: keep `sizeof` unfolded in templates.
- [x] 7. `.24`: lower `alignof` to `align_of::<T>()` where usable.
- [x] 8. `.25`: lower `offsetof` to `offset_of!` where usable.
- [x] 9. `.11` and `.13`: keep integer casts and conversions in sema.
- [x] 10. `.19`: `goto` out of a macro does not lift; those expansions stay inline.
- [x] 11. `.14`: audit zlib, with all-features `validate_merge` and both full profiles.

Deferred: `.17` (remeasure after step 4), `.16`.

## Maintainable output

Goal: any rewrite slate can prove keeps behavior and that a person maintaining
the port would make anyway. Readable call sites beat line count, so lifting a
macro is worth it even when the crate grows. Fixes stay generic, with no zlib
knowledge. Beads are `slate-f2zu.9.*` unless they carry another prefix.

Keep the checks passing throughout: `--check-projection`, the merge fixtures and
`tools/corpus/validate_merge.py`. Regenerate `slate-zlib` and review it after
each group.

Guard behavior first:

- [ ] 12. `9.5.12.29`: trace comparison and feature coverage in `validate_merge`.

Recover names, with no control flow change:

- [ ] 13. `9.7`: enumerators as consts (`BAD`, not `16209`).
- [ ] 14. `9.8`: object-like constant macros as consts (`Z_STREAM_ERROR`, `GZ_READ`).
- [ ] 15. `9.9`: anonymous records take their typedef or field name (`code`, `gz_state`).
- [ ] 16. `9.10`: keep the written literal radix (`0xff`).
- [ ] 17. `9.12`: `char[]` from a string prints as a string.
- [ ] 18. `9.11`: C `const` objects become non-mut statics.

Stop spelling differences from splitting code:

- [ ] 19. `9.5.12.27` and `slate-nfq8.14`: spell `size_t` and `ptrdiff_t`. `longest_match` goes from 12 copies to 4.
- [ ] 20. `9.19`: fold compound-assignment temporaries, removing `#[cfg] let` pairs.
- [ ] 21. `9.5.4`: drop feature gates on calls to macros empty otherwise. `longest_match` goes from 4 copies to 2.
- [ ] 22. `9.5.5`, `9.5.8.7`, `9.5.8.8`, `9.5.8.9`: libc spellings. Then reassess `9.3` and close it if nothing is left.

Give each definition one home:

- [ ] 23. `9.13`: one module per C header for types, and path-bound calls in library output.
- [ ] 24. `9.14`: header macros in `__slate_macros`, one template per C macro.
- [ ] 25. `9.5.12.28`: merge templates with identical text.

Recover control flow:

- [ ] 26. `9.17`: `do {} while (0)` becomes a block, and constant conditions fold.
- [ ] 27. `9.18` and `slate-o3gw.24`: `else if`, and no `else` after a diverging branch.
- [ ] 28. `9.16`: switch dispatch in functions with `goto` becomes a `match`.
- [ ] 29. `slate-nfq8.4`: cfg on match arms, and hoisting differing `if` conditions.
- [ ] 30. `9.5.12.22`: lift macros that declare locals in those functions (`DROPBITS`, `BYTEBITS`).
- [ ] 31. `9.5.12.23`, then `9.15`: pass labels so `NEEDBITS`, `PULLBYTE`, `PULL` and `ROOM` lift. Reverses the `.19` decision.
- [ ] 32. `slate-o3gw.10` and `slate-o3gw.25`: structure the remaining `__join` blocks.

Clean up what remains:

- [ ] 33. `slate-o3gw.21` and `slate-o3gw.18`: redundant `unsafe`.
- [ ] 34. `slate-o3gw.20`: sink zeroed locals, with `null_mut()` for pointers that stay.
- [ ] 35. `slate-o3gw.22` and `slate-o3gw.23`: post-increment temporaries and redundant casts.
- [ ] 36. `9.5.13`: re-measure cfg groups and trace why splice leaves the mostly shared ones.

Only if a project needs it: `9.5.12.12` (variadic and `#`/`##` macros).

Not on this path: `9.5.8.5`, `9.5.8.6` and `9.5.12.26` are minor or do not show
up in zlib.
