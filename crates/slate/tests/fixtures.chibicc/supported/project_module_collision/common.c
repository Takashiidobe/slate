struct common { volatile int value; };
int project_get(struct common *p) {
    return __atomic_load_n(&p->value, __ATOMIC_ACQUIRE);
}
