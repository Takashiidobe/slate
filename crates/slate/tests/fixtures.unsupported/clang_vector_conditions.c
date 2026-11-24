typedef int vec __attribute__((ext_vector_type(4)));

static vec choose(vec condition, vec left, vec right) {
  return condition ? left : right;
}

static vec both(vec left, vec right) {
  return left && right;
}

static vec either(vec left, vec right) {
  return left || right;
}

static vec invert(vec value) {
  return !value;
}

int main(void) {
  vec condition = {0, 1, -1, -2};
  vec left = {10, 11, 12, 13};
  vec right = {20, 0, 22, 0};
  vec selected = choose(condition, left, right);
  vec and_mask = both(condition, right);
  vec or_mask = either(condition, right);
  vec not_mask = invert(condition);
  int bad_choice = selected[0] != 20 || selected[1] != 0 ||
                   selected[2] != 12 || selected[3] != 13;
  int bad_and = and_mask[0] != 0 || and_mask[1] != 0 ||
                and_mask[2] != -1 || and_mask[3] != 0;
  int bad_or = or_mask[0] != -1 || or_mask[1] != -1 ||
               or_mask[2] != -1 || or_mask[3] != -1;
  int bad_not = not_mask[0] != -1 || not_mask[1] != 0 ||
                not_mask[2] != 0 || not_mask[3] != 0;
  return bad_choice + 2 * bad_and + 4 * bad_or + 8 * bad_not;
}
