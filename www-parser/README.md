# Slate Parser explorer

Runs `slate-parser` entirely in a browser worker through the vendored
browser_wasi_shim. The C editor, examples, compiler flavors, target selection,
flags, key bindings, share links, and resizable panes follow `../www`.
AST and IR tabs display the CLI's `parse` and `ir` output. Each selected target
runs separately; IR failures leave successful AST output available.
AST uses Rust highlighting for its debug dump. IR uses the custom CodeMirror
mode in `frontend/assets/codemirror-slate-ir.js`, based on
[the IR grammar](../wiki/concepts/ir-grammar.md).
Diagnostics appear in the corresponding tab as text, including source locations.
Compiler Explorer opens the input C with the selected compiler and first target.

## Build and serve

Run from the workspace root:

```sh
rustup target add wasm32-wasip1
bash www-parser/build.sh
python3 -m http.server --directory www-parser/dist 8000
```

Install Clang and GCC compiler headers and the Linux sysroots listed in
`build.sh` using `slate sysroot install` first. `SLATE_HEADERS` overrides the
directory containing `compiler-headers/` and `sysroots/`; defaults and GNU
utility requirements match [the Slate demo](../www/README.md#build).
The optional build argument selects an output directory.

The WASI entry point runs directly, with a 16 MiB stack configured by the build.
Native builds retain the dedicated parser thread. No crate dependency changes
are needed for `wasm32-wasip1`. Parsing runs twice per target, once for each tab.
The worker is terminated after 30 seconds to recover from stuck inputs.

## Hosting

Upload `www-parser/dist/` to a static host. `_headers` configures immutable
caching for hashed WASM, worker, and header archives on Netlify.
Assets use relative paths, so the site can live under a subdirectory.
Linux headers download lazily and are cached in the worker.
Windows targets ship without system headers. Source is processed locally;
clicking Compiler Explorer sends the input to Godbolt.
