# Conditional compilation steps

Macro epic `slate-f2zu.9.5.12`, in order.

- [x] 1. Close `9.5.7` and `9.5.10` if the fixtures confirm they are done.
- [x] 2. `.21`: write `merge-report.txt` on every `--merge` run.
- [x] 3. `.18`: drop hole types from the template key.
- [x] 4. `9.5.6`: pass caller locals as hidden arguments, then regenerate zlib.
- [ ] 5. `.10`: allow `return`, `break` and `continue` in templates.
- [ ] 6. `.20`: keep `sizeof` unfolded in templates.
- [ ] 7. `.11` and `.13`: keep integer casts and conversions in sema.
- [ ] 8. `.19`: decide whether `goto` out of a macro can lift.
- [ ] 9. `.14`: audit zlib, with all-features `validate_merge` and both full profiles.
- [ ] 10. `.12`: lift variadic and `#`/`##` macros if a project needs them.

Deferred: `.17` (remeasure after step 4), `.16`.
