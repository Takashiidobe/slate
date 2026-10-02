__attribute__((target("rtm"))) void abort_transaction(void) {
  __builtin_ia32_xabort(3 + 4);
}

int main(void) { return 0; }
