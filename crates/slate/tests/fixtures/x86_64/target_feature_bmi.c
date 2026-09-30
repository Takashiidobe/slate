__attribute__((target("bmi"))) int bmi_probe(int x) { return x + 1; }

int main(void) { return bmi_probe(41); }
