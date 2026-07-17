int counter __attribute__((section(".slate_data"))) = 0;

void __attribute__((section(".slate_fn"))) bump(int by) {
    counter += by;
}
