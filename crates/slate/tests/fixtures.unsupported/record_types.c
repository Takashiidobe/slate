#include <stdio.h>

struct Opaque;

typedef struct Point {
  int  x;
  long y;
} Point;

struct Segment {
  Point          from;
  Point          to;
  char           tag;
  double         weight;
  struct Opaque *extra;
};

struct Node {
  int          value;
  struct Node *next;
};

static long segment_length(struct Segment *s) {
  return (s->to.x - s->from.x) + (s->to.y - s->from.y);
}

static int sum_list(struct Node *node, int count) {
  int total = 0;
  while (count-- > 0) {
    total += node->value;
    node = node->next;
  }
  return total;
}

int main(void) {
  struct Segment s;
  s.from.x = 1;
  s.from.y = 2;
  s.to.x   = 10;
  s.to.y   = 20;
  s.tag    = 'a';
  s.weight = 0.5;
  printf("%ld %c %.2f\n", segment_length(&s), s.tag, s.weight);

  struct Node third;
  third.value = 3;
  third.next  = &third;
  struct Node second;
  second.value = 2;
  second.next  = &third;
  struct Node first;
  first.value = 1;
  first.next  = &second;
  first.next->next->value += 4;
  printf("%d\n", sum_list(&first, 3));
  return 0;
}
