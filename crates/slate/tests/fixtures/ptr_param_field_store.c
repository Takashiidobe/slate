#include <stdio.h>

typedef struct {
    int a;
    int b;
} pair_t;

static void fill(pair_t *out, int x, int y) {
    // @lowering-begin
    // @rewrite-begin
    out->a = x;
    out->b = y;
    // @rewrite-end
    // @lowering-end
}

int main(void) {
    pair_t p;
    fill(&p, 3, 4);
    printf("%d %d\n", p.a, p.b);
    return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut pair_t = out;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{_v[0-9]+}}).a = {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = y;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut pair_t = out;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{_v[0-9]+}}).b = {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*{{arg[0-9]+}}).a = x;
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*{{arg[0-9]+}}).b = y;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
