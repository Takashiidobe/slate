# Slate website

The demo site runs [Slate](https://github.com/Takashiidobe/slate) entirely in
the browser. `build.sh` compiles the Slate CLI to `wasm32-wasip1` and runs it in
a Web Worker through
[browser_wasi_shim](https://github.com/bjorn3/browser_wasi_shim). Each target's
headers are downloaded lazily on use. The same site serves the
[Slate book](../docs/src/SUMMARY.md) at `/docs/`.

## Build

Run from the workspace root. Install the sysroots and compiler headers once
(`slate sysroot install ...`), then run:

```sh
rustup target add wasm32-wasip1
cargo install mdbook --version 0.5.4 --locked
bash www/build.sh
```

`SLATE_HEADERS` overrides the directory containing `compiler-headers/` and
`sysroots/`. By default, the build uses Slate's local data directory:

| Host                            | Directory                                                                |
| ------------------------------- | ------------------------------------------------------------------------ |
| Linux                           | `$XDG_DATA_HOME/slate` if absolute, otherwise `$HOME/.local/share/slate` |
| macOS                           | `$HOME/Library/Application Support/Slate`                                |
| Windows (Git Bash/MSYS2/Cygwin) | `%LOCALAPPDATA%\Slate\data`                                              |

The build requires Bash and GNU utilities (including find, sort, tar, stat, sed,
and sha256sum).

## Run locally

```sh
python3 -m http.server -d www/dist 8000
```

Open `http://localhost:8000/` for the playground or
`http://localhost:8000/docs/` for the book. To preview Markdown changes without
rebuilding the playground, run `mdbook serve docs` from the workspace root.

## Deploy

Build the site, then deploy the combined output to Netlify:

```sh
cd www
ntl deploy --prod --dir=dist --no-build
```
