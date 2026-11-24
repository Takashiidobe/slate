static int __attribute__((swiftcall))
check_context(int value, void *context __attribute__((swift_context))) {
    return context == 0 ? 0 : value;
}

int main(void) {
    int value = 42;
    return check_context(value, &value) != 42;
}
