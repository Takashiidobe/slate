extern int seven(void);

static int (*volatile source)(void) = seven;

static int counter;

static void step(int amount) { counter += amount; }

void main(void) {
  step(source());
  step(-3);
}
