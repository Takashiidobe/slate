# c-corpus -> transpiled workflow (slate-a28e.10). Override with:
#   just corpus=/other/path transpile-redis
set unstable
set lists

corpus := env_var_or_default("SLATE_C_CORPUS", "/home/takashi/c-corpus")

default:
    @just --list

# core: translate a project from its compile_commands.json into transpiled/<name>
_transpile name src cc=(src / "compile_commands.json"):
    cargo run --release -- translate-project --lib --compile-commands {{cc}} {{src}} transpiled/{{name}}

# core: cargo check the already-transpiled crate, without re-translating it
_check name:
    cargo check --manifest-path transpiled/{{name}}/Cargo.toml

transpile-chibicc: (_transpile "chibicc" (corpus / "chibicc"))
transpile-cjson: (_transpile "cJSON" (corpus / "cJSON"))
transpile-curl: (_transpile "curl" (corpus / "curl"))
transpile-giflib: (_transpile "giflib" (corpus / "giflib"))
transpile-libexpat: (_transpile "libexpat" (corpus / "libexpat/expat"))
transpile-libpng: (_transpile "libpng" (corpus / "libpng"))
transpile-libuv: (_transpile "libuv" (corpus / "libuv"))
transpile-libyaml: (_transpile "libyaml" (corpus / "libyaml"))
transpile-lua: (_transpile "lua" (corpus / "lua"))
transpile-lz4: (_transpile "lz4" (corpus / "lz4"))
transpile-mbedtls: (_transpile "mbedtls" (corpus / "mbedtls"))
transpile-musl: (_transpile "musl" (corpus / "musl"))
transpile-nginx: (_transpile "nginx" (corpus / "nginx"))
transpile-pcre2: (_transpile "pcre2" (corpus / "pcre2"))
transpile-quickjs: (_transpile "quickjs" (corpus / "quickjs"))
transpile-redis: (_transpile "redis" (corpus / "redis"))
transpile-sqlite: (_transpile "sqlite" (corpus / "sqlite"))
transpile-tinycc: (_transpile "tinycc" (corpus / "tinycc"))
transpile-utf8proc: (_transpile "utf8proc" (corpus / "utf8proc"))
transpile-yyjson: (_transpile "yyjson" (corpus / "yyjson"))
transpile-zlib: (_transpile "zlib" (corpus / "zlib"))
transpile-zstd: (_transpile "zstd" (corpus / "zstd"))

check-chibicc: (_check "chibicc")
check-cjson: (_check "cJSON")
check-curl: (_check "curl")
check-giflib: (_check "giflib")
check-libexpat: (_check "libexpat")
check-libpng: (_check "libpng")
check-libuv: (_check "libuv")
check-libyaml: (_check "libyaml")
check-lua: (_check "lua")
check-lz4: (_check "lz4")
check-mbedtls: (_check "mbedtls")
check-musl: (_check "musl")
check-nginx: (_check "nginx")
check-pcre2: (_check "pcre2")
check-quickjs: (_check "quickjs")
check-redis: (_check "redis")
check-sqlite: (_check "sqlite")
check-tinycc: (_check "tinycc")
check-utf8proc: (_check "utf8proc")
check-yyjson: (_check "yyjson")
check-zlib: (_check "zlib")
check-zstd: (_check "zstd")

corpus_projects := "chibicc cjson curl giflib libexpat libpng libuv libyaml lua lz4 mbedtls musl nginx pcre2 quickjs redis sqlite tinycc utf8proc yyjson zlib zstd"

# transpile every c-corpus project; keeps going past per-project failures and reports them at the end
transpile-all:
    #!/usr/bin/env bash
    set -u
    failed=()
    for p in {{corpus_projects}}; do
        echo "== transpile-$p =="
        just transpile-$p || failed+=("$p")
    done
    if [ "${#failed[@]}" -gt 0 ]; then
        echo "failed: ${failed[*]}"
        exit 1
    fi

filecheck_files := split(`find tests/fixtures -maxdepth 1 -type f -name '*.c' -printf '%p\n' | sort | tr '\n' ' '`)
filecheck_bionic := split(`find tests/fixtures/bionic -type f -name '*.c' -printf '%p\n' | sort | tr '\n' ' '`)
filecheck_macos := split(`find tests/fixtures/macos -type f -name '*.c' -printf '%p\n' | sort | tr '\n' ' '`)
filecheck_msvc := split(`find tests/fixtures/msvc -type f -name '*.c' -printf '%p\n' | sort | tr '\n' ' '`)
filecheck_link := split(`rg -l '@(lowering|rewrite)(-fn)?-(begin|not-begin)|(LOWERING|REWRITES)-' tests/fixtures.link -g '*.c' | sort | tr '\n' ' '`)
filecheck_cfg := split(`rg -l '@rewrite(-fn)?-(begin|not-begin)' tests/fixtures.cfg -g '*.c' | sort | tr '\n' ' '`)
filecheck_projects := split(`rg -l '@(lowering|rewrite)(-fn)?-(begin|not-begin)' tests/fixtures.multi -g '*.c' | cut -d/ -f1-3 | sort -u | tr '\n' ' '`)
filecheck_libraries := split(`rg -l '@(lowering|rewrite)(-fn)?-(begin|not-begin)' tests/fixtures.library -g '*.c' | cut -d/ -f1-3 | sort -u | tr '\n' ' '`)
filecheck_fixtures := filecheck_files ++ filecheck_bionic ++ filecheck_macos ++ filecheck_msvc ++ filecheck_link ++ filecheck_cfg ++ filecheck_projects ++ filecheck_libraries
filecheck_jobs := if num_jobs() { num_jobs() } else { "8" }

# aarch64/x86_64 linux-gnu --target flags the regen-* recipes pass by default; override with e.g.
#   just filecheck_targets='--target MACOS=aarch64-apple-darwin' regen-filecheck
# or filecheck_targets='' to fall back to update_filecheck.py's own per-path target inference
filecheck_targets := "--target X86_64-GNU=x86_64-unknown-linux-gnu --target AARCH64-GNU=aarch64-unknown-linux-gnu"

# regenerate the lowering FileCheck blocks in every fixture, leaving rewrites blocks frozen
regen-lowering *paths=filecheck_fixtures:
    just --jobs {{filecheck_jobs}} _regen-filechecks lowering {{quote(paths)}}

# regenerate the rewrites FileCheck blocks in every fixture, leaving lowering blocks frozen
regen-rewrites *paths=filecheck_fixtures:
    just --jobs {{filecheck_jobs}} _regen-filechecks rewrites {{quote(paths)}}

# regenerate both FileCheck profiles in every fixture
regen-filecheck *paths=filecheck_fixtures:
    just --jobs {{filecheck_jobs}} _regen-filechecks both {{quote(paths)}}

# regenerate the lowering FileCheck blocks for fixtures matching PATTERN (glob or bare fixture name, e.g. `mem*` or `long_double_layout`)
regen-lowering-match *pattern:
    python3 tools/update_filecheck.py --in-place --profile lowering {{filecheck_targets}} {{pattern}}

# regenerate the rewrites FileCheck blocks for fixtures matching PATTERN, leaving lowering blocks frozen
regen-rewrites-match *pattern:
    python3 tools/update_filecheck.py --in-place --profile rewrites {{filecheck_targets}} {{pattern}}

# regenerate both FileCheck profiles for fixtures matching PATTERN
regen-filecheck-match *pattern:
    python3 tools/update_filecheck.py --in-place --profile both {{filecheck_targets}} {{pattern}}

[private]
[parallel]
_regen-filechecks profile *paths: *(_regen-filecheck profile *paths)

# bionic/macos/msvc fixtures carry their own android/darwin/msvc --target defaults inside
# update_filecheck.py (see default_targets_for_path); forcing the generic linux-gnu targets
# on top of those would overwrite their real cross-target checks, so they're excluded here.
[private]
_regen-filecheck profile path:
    #!/usr/bin/env bash
    set -euo pipefail
    case {{quote(path)}} in
        */fixtures/bionic/*|*/fixtures/macos/*|*/fixtures/msvc/*) target_args=() ;;
        *) target_args=({{filecheck_targets}}) ;;
    esac
    python3 tools/update_filecheck.py --in-place --profile {{quote(profile)}} "${target_args[@]}" {{quote(path)}}

# cargo check every already-transpiled crate; keeps going past per-project failures and reports them at the end
check-all:
    #!/usr/bin/env bash
    set -u
    failed=()
    for p in {{corpus_projects}}; do
        [ -d transpiled/$p ] || continue
        echo "== check-$p =="
        just check-$p || failed+=("$p")
    done
    if [ "${#failed[@]}" -gt 0 ]; then
        echo "failed: ${failed[*]}"
        exit 1
    fi
