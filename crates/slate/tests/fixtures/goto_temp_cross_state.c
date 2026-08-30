#include <stdio.h>

typedef struct {
    int pad;
    int x;
    int y;
} inner_t;

typedef struct {
    int lead;
    inner_t in;
} outer_t;

typedef struct {
    outer_t *dict;
} state_t;

static int compute(const state_t *const ms, int flag) {
    const outer_t *const o = ms->dict;
    const inner_t *const q = &o->in;
    int acc = q->x;
    if (flag) {
        goto second;
    }
    acc += 100;
second:
    acc += q->y;
    return acc;
}

int main(void) {
    inner_t inr;
    inr.pad = 0;
    inr.x = 3;
    inr.y = 4;
    outer_t ou;
    ou.lead = 0;
    ou.in = inr;
    state_t s;
    s.dict = &ou;
    printf("%d %d\n", compute(&s, 0), compute(&s, 1));
    return 0;
}
