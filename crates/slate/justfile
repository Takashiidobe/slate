# c-corpus -> transpiled workflow (slate-a28e.10). Override with:
#   just corpus=/other/path transpile-redis
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

corpus_projects := "chibicc cjson curl libexpat libpng libuv libyaml lua lz4 mbedtls musl nginx pcre2 quickjs redis sqlite tinycc utf8proc yyjson zlib zstd"

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

# regenerate the lowering FileCheck blocks in every fixture, leaving rewrites blocks frozen
regen-lowering *paths="tests/fixtures/*.c":
    python3 tools/update_filecheck.py --in-place --profile lowering {{paths}}

# regenerate the rewrites FileCheck blocks in every fixture, leaving lowering blocks frozen
regen-rewrites *paths="tests/fixtures/*.c":
    python3 tools/update_filecheck.py --in-place --profile rewrites {{paths}}

# regenerate both FileCheck profiles in every fixture
regen-filecheck *paths="tests/fixtures/*.c":
    python3 tools/update_filecheck.py --in-place --profile both {{paths}}

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
