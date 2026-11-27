# Headers and Include Search

Slate resolves each include with the source file's configured search paths. That
lookup produces two inputs for translation: the header's contents and its
provenance. Provenance follows the header's tokens into the AST and IR.

## Resolve the header

- **Quoted includes** try the including file's directory, then `-iquote`, `-I`,
  and the system search paths. A file found beside its includer inherits the
  includer's header kind.
- **Angled includes** start with `-I`, then try the system search paths. Angle
  brackets alone do not mark a header as system. A header found on `-I` is a
  user header.
- **System search paths** include explicit `-isystem` directories, compiler
  builtin headers, and standard headers from the target's sysroot. An explicit
  sysroot replaces the target default. Slate uses that root without falling back
  to host headers.

The preprocessor records the resolved file and header kind. It also tracks the
outermost system header entered from user code. For example, if `<stdio.h>`
includes a private SDK header, a declaration in the private file can still be
attributed to `<stdio.h>`. Slate uses both locations when recognizing libc
declarations. A user header with the same filename does not establish a libc
identity.

## Track macros across headers

The definition and invocation can belong to different files. For example, a
system header can define `CHAR_MAX`, and user code can invoke it. The expanded
token retains the definition origin as well as the use site, so Slate can
recognize the named constant. Calls spelled inside system headers stay outside
the user invocation table. See [Macro Provenance](macro-provenance.md).

Slate processes forced inputs before the main source. `-imacros` contributes
macro definitions and discards declarations. `-include` also contributes
declarations as translation unit roots. Macros from both keep their definition
spans. Include guards and `#pragma once` can suppress repeat reads of the same
header, so a skipped include supplies no second set of expanded tokens.

Start with `CompilerArgs::search_paths` and `pp/include.rs` in
`crates/slate-parser/src/`.
