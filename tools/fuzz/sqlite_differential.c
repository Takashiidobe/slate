#define _GNU_SOURCE
#include <ctype.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sqlite3.h"

#define API(X)                                                                 \
  X(initialize)                                                                \
  X(open_v2)                                                                   \
  X(close)                                                                     \
  X(prepare_v2)                                                                \
  X(step)                                                                      \
  X(finalize)                                                                  \
  X(column_count)                                                              \
  X(column_type)                                                               \
  X(column_int64)                                                              \
  X(column_double)                                                             \
  X(column_text)                                                               \
  X(column_blob)                                                               \
  X(column_bytes)                                                              \
  X(errmsg)                                                                    \
  X(extended_errcode)                                                          \
  X(changes64)                                                                 \
  X(progress_handler)                                                          \
  X(limit)                                                                     \
  X(hard_heap_limit64)                                                         \
  X(soft_heap_limit64)                                                         \
  X(set_authorizer)                                                            \
  X(vfs_find)                                                                  \
  X(vfs_register)                                                              \
  X(test_control)                                                              \
  X(strnicmp)                                                                  \
  X(stricmp)                                                                   \
  X(malloc64)                                                                  \
  X(deserialize)                                                               \
  X(db_config)                                                                 \
  X(file_control)                                                              \
  X(config)

#define API_FIELD(name) __typeof__(sqlite3_##name) *name;

typedef struct Api {
  const char *label;
  API(API_FIELD)
  sqlite3_vfs *base;
  sqlite3_vfs vfs;
  sqlite3_mem_methods memory;
} Api;

typedef struct Trace {
  char *bytes;
  size_t length;
  size_t capacity;
  int truncated;
} Trace;

typedef struct Run {
  Api *api;
  unsigned progress;
} Run;

typedef struct Input {
  unsigned char *image;
  int image_size;
  char *sql;
} Input;

enum {
  PROGRESS_INTERVAL = 10,
  PROGRESS_LIMIT = 20000,
  STATEMENT_LIMIT = 256,
  ROW_LIMIT = 2000,
  TRACE_LIMIT = 4 << 20,
  IMAGE_LIMIT = 4000000,
};

static const struct {
  unsigned mask;
  int setting;
} db_config_bits[] = {
    {0x0001, SQLITE_DBCONFIG_ENABLE_FKEY},
    {0x0002, SQLITE_DBCONFIG_ENABLE_TRIGGER},
    {0x0004, SQLITE_DBCONFIG_ENABLE_VIEW},
    {0x0008, SQLITE_DBCONFIG_ENABLE_QPSG},
    {0x0010, SQLITE_DBCONFIG_TRIGGER_EQP},
    {0x0020, SQLITE_DBCONFIG_DEFENSIVE},
    {0x0040, SQLITE_DBCONFIG_WRITABLE_SCHEMA},
    {0x0080, SQLITE_DBCONFIG_LEGACY_ALTER_TABLE},
    {0x0100, SQLITE_DBCONFIG_STMT_SCANSTATUS},
    {0x0200, SQLITE_DBCONFIG_REVERSE_SCANORDER},
    {0x0800, SQLITE_DBCONFIG_DQS_DML},
    {0x1000, SQLITE_DBCONFIG_DQS_DDL},
    {0x2000, SQLITE_DBCONFIG_TRUSTED_SCHEMA},
};

static Api native;
static Api translated;
static Trace native_trace;
static Trace translated_trace;

static void trace_append(Trace *trace, const char *format, ...) {
  if (trace->truncated) {
    return;
  }
  for (;;) {
    va_list arguments;
    va_start(arguments, format);
    size_t room = trace->capacity - trace->length;
    int written = vsnprintf(trace->bytes + trace->length, room, format, arguments);
    va_end(arguments);
    if (written < 0) {
      abort();
    }
    if ((size_t)written < room) {
      trace->length += (size_t)written;
      break;
    }
    if (trace->length + (size_t)written >= TRACE_LIMIT) {
      trace->truncated = 1;
      trace->length = 0;
      trace->bytes[0] = 0;
      trace_append(trace, "TRUNCATED\n");
      return;
    }
    trace->capacity = 2 * (trace->length + (size_t)written + 1);
    trace->bytes = realloc(trace->bytes, trace->capacity);
    if (!trace->bytes) {
      abort();
    }
  }
}

static void trace_bytes(Trace *trace, const unsigned char *bytes, int length) {
  trace_append(trace, "%d:", length);
  for (int i = 0; i < length && !trace->truncated; i++) {
    trace_append(trace, "%02x", bytes[i]);
  }
}

static Api *api_of(sqlite3_vfs *vfs) {
  return (Api *)((char *)vfs - offsetof(Api, vfs));
}

static int fuzz_open(sqlite3_vfs *vfs, sqlite3_filename name, sqlite3_file *file,
                     int flags, int *out_flags) {
  if (name) {
    return SQLITE_CANTOPEN;
  }
  Api *api = api_of(vfs);
  return api->base->xOpen(api->base, name, file, flags, out_flags);
}

static int fuzz_randomness(sqlite3_vfs *vfs, int length, char *out) {
  (void)vfs;
  for (int i = 0; i < length; i++) {
    out[i] = (char)(i * 37 + 11);
  }
  return length;
}

static int fuzz_sleep(sqlite3_vfs *vfs, int microseconds) {
  (void)vfs;
  return microseconds;
}

static int fuzz_current_time(sqlite3_vfs *vfs, double *out) {
  (void)vfs;
  *out = 2460000.5;
  return SQLITE_OK;
}

static int fuzz_current_time_int64(sqlite3_vfs *vfs, sqlite3_int64 *out) {
  (void)vfs;
  *out = (sqlite3_int64)2460000.5 * 86400000 + 43200000;
  return SQLITE_OK;
}

#define ZEROED_MEMORY(api)                                                     \
  static void *api##_malloc(int size) {                                        \
    void *memory = api.memory.xMalloc(size);                                   \
    if (memory) {                                                              \
      memset(memory, 0, (size_t)size);                                         \
    }                                                                          \
    return memory;                                                             \
  }                                                                            \
  static void *api##_realloc(void *memory, int size) {                         \
    int old = memory ? api.memory.xSize(memory) : 0;                           \
    void *grown = api.memory.xRealloc(memory, size);                           \
    if (grown && size > old) {                                                 \
      memset((char *)grown + old, 0, (size_t)(size - old));                    \
    }                                                                          \
    return grown;                                                              \
  }

ZEROED_MEMORY(native)
ZEROED_MEMORY(translated)

static void install_memory(Api *api, void *(*zeroed_malloc)(int),
                           void *(*zeroed_realloc)(void *, int)) {
  if (api->config(SQLITE_CONFIG_GETMALLOC, &api->memory) != SQLITE_OK) {
    abort();
  }
  sqlite3_mem_methods zeroed = api->memory;
  zeroed.xMalloc = zeroed_malloc;
  zeroed.xRealloc = zeroed_realloc;
  if (api->config(SQLITE_CONFIG_MALLOC, &zeroed) != SQLITE_OK) {
    abort();
  }
}

static void install_vfs(Api *api) {
  api->initialize();
  api->base = api->vfs_find(0);
  if (!api->base) {
    abort();
  }
  api->vfs = *api->base;
  api->vfs.zName = "slate-fuzz";
  api->vfs.pNext = 0;
  api->vfs.xOpen = fuzz_open;
  api->vfs.xRandomness = fuzz_randomness;
  api->vfs.xSleep = fuzz_sleep;
  api->vfs.xCurrentTime = fuzz_current_time;
  if (api->vfs.iVersion >= 2) {
    api->vfs.xCurrentTimeInt64 = fuzz_current_time_int64;
  }
  if (api->vfs_register(&api->vfs, 1) != SQLITE_OK) {
    abort();
  }
}

#define BIND_NATIVE(name) native.name = sqlite3_##name;
#define BIND_TRANSLATED(name)                                                  \
  translated.name = (__typeof__(sqlite3_##name) *)dlsym(handle, "sqlite3_" #name); \
  if (!translated.name) {                                                      \
    fprintf(stderr, "missing sqlite3_" #name " in %s\n", path);                \
    abort();                                                                   \
  }

int LLVMFuzzerInitialize(int *argc, char ***argv) {
  (void)argc;
  (void)argv;
  const char *path = getenv("SLATE_SQLITE_TRANSLATED");
  if (!path) {
    fprintf(stderr, "set SLATE_SQLITE_TRANSLATED to the translated libsqlite3.so\n");
    abort();
  }
  void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
  if (!handle) {
    fprintf(stderr, "dlopen %s: %s\n", path, dlerror());
    abort();
  }
  native.label = "native";
  translated.label = "translated";
  API(BIND_NATIVE)
  API(BIND_TRANSLATED)
  if ((void *)translated.open_v2 == (void *)native.open_v2) {
    fprintf(stderr, "%s resolved to the native library\n", path);
    abort();
  }
  install_memory(&native, native_malloc, native_realloc);
  install_memory(&translated, translated_malloc, translated_realloc);
  install_vfs(&native);
  install_vfs(&translated);
  return 0;
}

static int progress(void *context) {
  Run *run = context;
  return ++run->progress > PROGRESS_LIMIT;
}

static int authorize(void *context, int action, const char *first, const char *second,
                     const char *third, const char *fourth) {
  Run *run = context;
  (void)second;
  (void)third;
  (void)fourth;
  if (action == SQLITE_PRAGMA && first &&
      (run->api->strnicmp("vdbe_", first, 5) == 0 ||
       run->api->stricmp("parser_trace", first) == 0 ||
       run->api->stricmp("temp_store_directory", first) == 0 ||
       run->api->stricmp("data_store_directory", first) == 0 ||
       run->api->stricmp("soft_heap_limit", first) == 0 ||
       run->api->stricmp("hard_heap_limit", first) == 0 ||
       run->api->stricmp("compile_options", first) == 0)) {
    return SQLITE_DENY;
  }
  if (action == SQLITE_FUNCTION && second &&
      run->api->strnicmp("sqlite_compileoption_", second, 21) == 0) {
    return SQLITE_DENY;
  }
  return SQLITE_OK;
}

static void trace_row(Trace *trace, Api *api, sqlite3_stmt *statement) {
  int columns = api->column_count(statement);
  trace_append(trace, "R%d", columns);
  for (int i = 0; i < columns && !trace->truncated; i++) {
    int type = api->column_type(statement, i);
    switch (type) {
    case SQLITE_INTEGER:
      trace_append(trace, " i%lld", (long long)api->column_int64(statement, i));
      break;
    case SQLITE_FLOAT:
      trace_append(trace, " f%a", api->column_double(statement, i));
      break;
    case SQLITE_TEXT: {
      const unsigned char *text = api->column_text(statement, i);
      trace_append(trace, " t");
      trace_bytes(trace, text, text ? api->column_bytes(statement, i) : -1);
      break;
    }
    case SQLITE_BLOB: {
      const void *blob = api->column_blob(statement, i);
      trace_append(trace, " b");
      trace_bytes(trace, blob, blob ? api->column_bytes(statement, i) : 0);
      break;
    }
    default:
      trace_append(trace, " n%d", type);
      break;
    }
  }
  trace_append(trace, "\n");
}

static unsigned char hex_value(unsigned c) {
  c += 9 * (1 & (c >> 6));
  return (unsigned char)(c & 0xf);
}

static int is_db_sql(const unsigned char *data, size_t size) {
  if (size > 4 && memcmp(data, "\n--\n", 4) == 0) {
    return 1;
  }
  while (size > 0 && isspace(*data)) {
    data++;
    size--;
  }
  unsigned char digits[8];
  int count = 0;
  for (; size > 0 && count < 8; size--, data++) {
    if (isxdigit(*data)) {
      digits[count++] = *data;
    }
  }
  return count == 8 && memcmp(digits, "53514c69", 8) == 0;
}

static long decode_image(const unsigned char *data, size_t size, Input *input) {
  size_t capacity = 4096;
  size_t used = 0;
  size_t nibbles = 0;
  unsigned char byte = 0;
  unsigned char *image = calloc(capacity, 1);
  if (!image) {
    abort();
  }
  size_t i;
  for (i = 0; i < size; i++) {
    unsigned char c = data[i];
    if (isxdigit(c)) {
      nibbles++;
      if (nibbles & 1) {
        byte = (unsigned char)(hex_value(c) * 16);
        continue;
      }
      byte += hex_value(c);
      size_t at = nibbles / 2 - 1;
      if (at >= IMAGE_LIMIT) {
        free(image);
        return -1;
      }
      if (at >= used) {
        used = (at + 4096) & ~(size_t)4095;
      }
      if (used > capacity) {
        image = realloc(image, used);
        if (!image) {
          abort();
        }
        memset(image + capacity, 0, used - capacity);
        capacity = used;
      }
      image[at] = byte;
    } else if (c == '[' && i + 3 < size) {
      size_t end = i + 1;
      size_t offset = 0;
      while (end < size && isxdigit(data[end]) && offset < IMAGE_LIMIT) {
        offset = offset * 16 + hex_value(data[end++]);
      }
      if (end < size && data[end] == ']') {
        nibbles = 2 * offset;
        i = end;
      }
    } else if (c == '\n' && i + 4 < size && memcmp(data + i, "\n--\n", 4) == 0) {
      i += 4;
      break;
    }
  }
  input->image = image;
  input->image_size = (int)used;
  return (long)i;
}

static unsigned big_endian(const unsigned char *bytes) {
  return (unsigned)bytes[0] << 24 | (unsigned)bytes[1] << 16 | (unsigned)bytes[2] << 8 |
         (unsigned)bytes[3];
}

static void load_image(Api *api, Trace *trace, sqlite3 *db, const Input *input) {
  unsigned flags = input->image_size >= 76 ? big_endian(input->image + 72) : 0;
  unsigned optimizations = input->image_size >= 80 ? big_endian(input->image + 76) : 0;
  api->test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, optimizations);
  for (size_t i = 0; i < sizeof db_config_bits / sizeof db_config_bits[0]; i++) {
    if (flags & db_config_bits[i].mask) {
      int value = 0;
      api->db_config(db, db_config_bits[i].setting, -1, &value);
      api->db_config(db, db_config_bits[i].setting, !value, (int *)0);
    }
  }
  unsigned char *copy = api->malloc64((sqlite3_uint64)input->image_size);
  if (!copy) {
    trace_append(trace, "M\n");
    return;
  }
  memcpy(copy, input->image, (size_t)input->image_size);
  if (input->image_size >= 20 && copy[18] == 2 && copy[19] == 2) {
    copy[18] = copy[19] = 1;
  }
  int rc = api->deserialize(db, "main", copy, input->image_size, input->image_size,
                            SQLITE_DESERIALIZE_RESIZEABLE | SQLITE_DESERIALIZE_FREEONCLOSE);
  trace_append(trace, "D%d\n", rc);
  sqlite3_int64 limit = IMAGE_LIMIT;
  api->file_control(db, "main", SQLITE_FCNTL_SIZE_LIMIT, &limit);
}

static void execute(Api *api, Trace *trace, const Input *input) {
  const char *sql = input->sql;
  trace->length = 0;
  trace->truncated = 0;
  trace_append(trace, "");
  Run run = {api, 0};
  api->test_control(SQLITE_TESTCTRL_PRNG_SEED, 1, (sqlite3 *)0);
  api->hard_heap_limit64(20000000);
  api->soft_heap_limit64(0);
  sqlite3 *db = 0;
  int rc = api->open_v2("fuzz.db", &db,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 0);
  trace_append(trace, "O%d\n", rc);
  if (rc != SQLITE_OK) {
    api->close(db);
    return;
  }
  api->progress_handler(db, PROGRESS_INTERVAL, progress, &run);
  api->limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
  api->limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
  api->limit(db, SQLITE_LIMIT_LENGTH, 50000);
  if (input->image_size > 0) {
    load_image(api, trace, db, input);
  }
  api->set_authorizer(db, authorize, &run);
  const char *tail = sql;
  for (int statements = 0; *tail && statements < STATEMENT_LIMIT && !trace->truncated;
       statements++) {
    sqlite3_stmt *statement = 0;
    const char *next = 0;
    rc = api->prepare_v2(db, tail, -1, &statement, &next);
    trace_append(trace, "P%d %d %s\n", rc, api->extended_errcode(db), api->errmsg(db));
    if (statement) {
      int rows = 0;
      while ((rc = api->step(statement)) == SQLITE_ROW && rows++ < ROW_LIMIT &&
             !trace->truncated) {
        trace_row(trace, api, statement);
      }
      trace_append(trace, "S%d %d %s %lld\n", rc, api->extended_errcode(db), api->errmsg(db),
                   (long long)api->changes64(db));
      trace_append(trace, "F%d\n", api->finalize(statement));
    }
    if (!next || next == tail) {
      break;
    }
    tail = next;
  }
  trace_append(trace, "C%d %u\n", api->close(db), run.progress);
}

static void report(const Trace *left, const Trace *right) {
  size_t length = left->length < right->length ? left->length : right->length;
  size_t at = 0;
  while (at < length && left->bytes[at] == right->bytes[at]) {
    at++;
  }
  size_t line = at;
  while (line > 0 && left->bytes[line - 1] != '\n') {
    line--;
  }
  fprintf(stderr, "divergence at byte %zu\n--- native\n%.400s\n--- translated\n%.400s\n", at,
          left->bytes + line, right->bytes + line);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  Input input = {0};
  size_t start = 0;
  if (is_db_sql(data, size)) {
    long at = decode_image(data, size, &input);
    if (at < 0) {
      return 0;
    }
    start = (size_t)at;
  }
  input.sql = malloc(size - start + 1);
  if (!input.sql) {
    abort();
  }
  memcpy(input.sql, data + start, size - start);
  input.sql[size - start] = 0;
  execute(&native, &native_trace, &input);
  execute(&translated, &translated_trace, &input);
  free(input.sql);
  free(input.image);
  if (native_trace.length != translated_trace.length ||
      memcmp(native_trace.bytes, translated_trace.bytes, native_trace.length) != 0) {
    report(&native_trace, &translated_trace);
    const char *keep = getenv("SLATE_SQLITE_DIVERGENCES");
    if (!keep) {
      abort();
    }
    static unsigned divergences;
    char path[4096];
    snprintf(path, sizeof path, "%s/divergence-%d-%05u.sql", keep, (int)getpid(),
             divergences++);
    FILE *file = fopen(path, "wb");
    if (!file || fwrite(data, 1, size, file) != size || fclose(file)) {
      abort();
    }
    fprintf(stderr, "saved %s\n", path);
  }
  return 0;
}
