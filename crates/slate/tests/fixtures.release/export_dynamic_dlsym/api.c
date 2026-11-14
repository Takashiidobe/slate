int api_sum(const int *values, int count) {
  int sum = 0;
  for (int i = 0; i < count; i++)
    sum += values[i];
  return sum;
}

int api_scale(int value) { return value * 3; }
