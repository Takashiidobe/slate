# Demo site

The demo site runs [Slate](https://github.com/Takashiidobe/slate)
entirely in the browser. `build.sh` compiles the Slate CLI to `wasm32-wasip1`
and runs it in a Web Worker through [browser_wasi_shim](https://github.com/bjorn3/browser_wasi_shim).
Each target's headers are downloaded lazily on use.

## Build

Install the sysroots and compiler headers once (`slate sysroot install ...`), then:

```sh
rustup target add wasm32-wasip1
./build.sh
```

`SLATE_HEADERS` overrides the directory containing `compiler-headers/` and
`sysroots/`. By default, the build uses Slate's local data directory:

| Host | Directory |
| --- | --- |
| Linux | `$XDG_DATA_HOME/slate` if absolute, otherwise `$HOME/.local/share/slate` |
| macOS | `$HOME/Library/Application Support/Slate` |
| Windows (Git Bash/MSYS2/Cygwin) | `%LOCALAPPDATA%\Slate\data` |

The build requires Bash and GNU utilities (including find, sort, tar, stat,
sed, and sha256sum).

## Run locally

```sh
python3 -m http.server -d dist 8000
```

## Deploy

Deploy on netlify:

```sh
ntl deploy --prod
```
