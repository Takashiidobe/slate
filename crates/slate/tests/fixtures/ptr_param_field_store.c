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
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{arg[0-9]+}}).a = {{arg[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{arg[0-9]+}}).b = {{arg[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*({{arg[0-9]+}} as *mut pair_t)).a = {{arg[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*({{arg[0-9]+}} as *mut pair_t)).b = {{arg[0-9]+}};
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
