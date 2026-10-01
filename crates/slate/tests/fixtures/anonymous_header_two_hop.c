#include "anonymous_header_wrapper.h"

int main(void) {
  anonymous_header_state state = {0};
  state.value.wide             = 7;
  return state.count + (state.value.wide != 7);
}
