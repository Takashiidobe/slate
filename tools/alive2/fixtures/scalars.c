unsigned int add(unsigned int a, unsigned int b) {
    return a + b;
}

int choose(int a, int b) {
    return a < b ? a : b;
}

unsigned int rotate(unsigned int value) {
    return (value << 7) | (value >> 25);
}
