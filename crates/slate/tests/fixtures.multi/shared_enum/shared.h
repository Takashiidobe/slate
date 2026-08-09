typedef enum {
  MODE_IDLE,
  MODE_READY,
} Mode;

struct Holder {
  Mode mode;
};
