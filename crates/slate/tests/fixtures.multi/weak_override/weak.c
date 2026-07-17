int weak_global __attribute__((weak)) = 13;

int __attribute__((weak)) fallback_value(void) {
    return weak_global;
}
