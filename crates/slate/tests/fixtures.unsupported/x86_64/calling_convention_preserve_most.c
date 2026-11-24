typedef __attribute__((preserve_most)) int callback(int);

static int __attribute__((preserve_most)) increment(int value) {
    return value + 1;
}

int main(void) {
    callback *f = increment;
    return f(41) != 42;
}
