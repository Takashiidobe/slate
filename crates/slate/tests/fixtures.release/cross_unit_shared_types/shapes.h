struct flags {
    unsigned ready : 1;
    unsigned level : 4;
    int id;
};

struct shape {
    struct shape *next;
    struct {
        int x, y;
    } origin;
    struct flags flags;
};

typedef struct {
    const char *label;
    int weight;
} tag_t;

struct registry;

struct registry *registry_new(void);
void registry_add(struct registry *registry, struct shape *shape);
int registry_count(const struct registry *registry);
void registry_free(struct registry *registry);

void shape_move(struct shape *shape, int dx, int dy);
int shape_area_sum(const struct shape *shapes, int count);
int tag_score(const tag_t *tag);
int count_vowels(const char *text);
