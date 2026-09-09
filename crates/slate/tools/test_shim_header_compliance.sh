#!/usr/bin/env fish

for target in glibc-aarch64 glibc-arm glibc-i386 glibc-x86_64 musl-aarch64 musl-arm musl-i386 musl-x86_64
    echo "=== $target: generating fixtures ==="
    env SLATE_LIBC_DECL_TARGET=$target cargo nextest r --release --profile libc \
        --test libc_declaration_matrix_suite \
        -E 'test(generate_declaration_matrix_fixtures)' --run-ignored ignored-only
    or begin
        echo "generation failed for $target"
        break
    end

    echo "=== $target: retesting ==="
    env SLATE_LIBC_DECL_TARGET=$target cargo nextest r --release --profile libc \
        --test libc_declaration_matrix_suite -E 'test(declaration_matrices)'
    or begin
        echo "retest failed for $target"
        break
    end
end
