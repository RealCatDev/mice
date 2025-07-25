#ifndef   _MICE_DATA_JSON_H_
#define   _MICE_DATA_JSON_H_

#include "mice/data/stringView.h"
#include "mice/data/stringBuilder.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum Mice_Json_Type {
  MICE_JSON_TYPE_STRING = 0,
  MICE_JSON_TYPE_NUMBER,
  MICE_JSON_TYPE_BOOLEAN,
  MICE_JSON_TYPE_NULL,
  MICE_JSON_TYPE_OBJECT,
  MICE_JSON_TYPE_ARRAY,

  COUNT_MICE_JSON_TYPES
} Mice_Json_Type;

typedef struct Mice_Json Mice_Json;
struct Mice_Json {
  Mice_Json *prev, *next;
  Mice_Json *child;

  Mice_Json_Type type;
  char *name;

  union {
    char *string;
    double number;
    bool boolean;
  } value;
};

Mice_Json *mice_json_create_string(const char *string);
Mice_Json *mice_json_create_string_sv(Mice_String_View sv);
Mice_Json *mice_json_create_number(double number);
Mice_Json *mice_json_create_boolean(bool boolean);
Mice_Json *mice_json_create_null(void);
Mice_Json *mice_json_create_object(void);
Mice_Json *mice_json_create_array(void);

void mice_json_destroy(Mice_Json *json);

void mice_json_add_child(Mice_Json *parent, Mice_Json *child, const char *name);
void mice_json_add_child_sv(Mice_Json *parent, Mice_Json *child, Mice_String_View sv);

void mice_json_set_string_cstr(Mice_Json *json, const char *cstr);
void mice_json_set_string_sv(Mice_Json *json, Mice_String_View sv);

Mice_Json *mice_json_read_from_string_view(Mice_String_View *sv);
void mice_json_write_to_string_builder(Mice_Json *json, Mice_String_Builder *sb);

Mice_Json *mice_json_get(Mice_Json *json, Mice_String_View path);

#endif // _MICE_DATA_JSON_H_