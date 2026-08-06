/** selects an operating mode */
enum Mode {
  /** disable processing */
  MODE_OFF = 0,
  /// enable processing
  MODE_ON  = 1,
};

/** stores a selected mode */
struct Holder {
  /** current mode value */
  enum Mode mode;
};

int main(void) {
  struct Holder holder = {MODE_ON};
  return holder.mode == MODE_ON ? 0 : 1;
}
