#include "mice/data/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

static bool isdigit_predicate(char x) {
  return x >= '0' && x <= '9';
}

static Mice_Json *alloc_json(Mice_Json_Type type) {
  Mice_Json *json = malloc(sizeof(*json));
  assert(json);
  memset(json, 0, sizeof(*json));
  json->type = type;
  return json;
}

Mice_Json *mice_json_create_string(const char *string) {
  Mice_Json *json = alloc_json(MICE_JSON_TYPE_STRING);
  if (string) mice_json_set_string_cstr(json, string);
  return json;
}

Mice_Json *mice_json_create_string_sv(Mice_String_View sv) {
  Mice_Json *json = alloc_json(MICE_JSON_TYPE_STRING);
  mice_json_set_string_sv(json, sv);
  return json;
}

Mice_Json *mice_json_create_number(double number) {
  Mice_Json *json = alloc_json(MICE_JSON_TYPE_NUMBER);
  json->value.number = number;
  return json;
}

Mice_Json *mice_json_create_boolean(bool boolean) {
  Mice_Json *json = alloc_json(MICE_JSON_TYPE_BOOLEAN);
  json->value.boolean = boolean;
  return json;
}

Mice_Json *mice_json_create_null(void) {
  return alloc_json(MICE_JSON_TYPE_NULL);
}

Mice_Json *mice_json_create_object(void) {
  return alloc_json(MICE_JSON_TYPE_OBJECT);
}

Mice_Json *mice_json_create_array(void) {
  return alloc_json(MICE_JSON_TYPE_ARRAY);
}

void mice_json_destroy(Mice_Json *json) {
  for (Mice_Json *child = json->child, *next = NULL; child; child = next) {
    next = child->next;
    mice_json_destroy(child);
  }

  if (json->type == MICE_JSON_TYPE_STRING) free(json->value.string);
  free(json->name);
  free(json);
}

void mice_json_add_child(Mice_Json *parent, Mice_Json *child, const char *name) {
  mice_json_add_child_sv(parent, child, mice_string_view_from_cstr(name));
}

void mice_json_add_child_sv(Mice_Json *parent, Mice_Json *child, Mice_String_View sv) {
  assert(parent && child && parent != child);
  assert(parent->type == MICE_JSON_TYPE_OBJECT || parent->type == MICE_JSON_TYPE_ARRAY);

  if (!parent->child) {
    parent->child = child;
  } else {
    Mice_Json *end = parent->child->prev;
    end->next = child;
    child->prev = end;
  }

  child->name = malloc(sv.count);
  assert(child->name);
  child->name[sv.count] = '\0';
  memcpy(child->name, sv.items, sv.count);

  child->next = NULL;
  parent->child->prev = child;
}

void mice_json_set_string_cstr(Mice_Json *json, const char *cstr) {
  mice_json_set_string_sv(json, mice_string_view_from_cstr(cstr));
}

void mice_json_set_string_sv(Mice_Json *json, Mice_String_View sv) {
  assert(json && json->type == MICE_JSON_TYPE_STRING);

  free(json->value.string);
  json->value.string = malloc(sv.count+1);
  assert(json->value.string);
  json->value.string[sv.count] = '\0';
  memcpy(json->value.string, sv.items, sv.count);
}

Mice_Json *mice_json_read_from_string_view(Mice_String_View *sv) {
  assert(sv);

  *sv = mice_string_view_trim(*sv);
  assert(sv->count);

  if (*sv->items == '"') {
    (void)mice_string_view_chop_left(sv, 1);
    return mice_json_create_string_sv(mice_string_view_chop_by_delim(sv, '"'));
  } else if (*sv->items == '-' || isdigit(*sv->items)) {
    bool sign = false;
    if (*sv->items == '-') {
      sign = true;
      (void)mice_string_view_chop_left(sv, 1);
    }

    Mice_String_View numberSv = mice_string_view_chop_while(sv, isdigit_predicate);

    double number = 0;
    for (size_t i = 0; i < numberSv.count; ++i) {
      number = number * 10 + numberSv.items[i] - '0';
    }
    if (sign) number *= -1;

    return mice_json_create_number(number);
  } else if (*sv->items == 't') {
    assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 4), "true"));
    return mice_json_create_boolean(true);
  } else if (*sv->items == 'f') {
    assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 5), "false"));
    return mice_json_create_boolean(false);
  } else if (*sv->items == 'n') {
    assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 4), "null"));
    return mice_json_create_null();
  } else if (*sv->items == '{') {
    (void)mice_string_view_chop_left(sv, 1);

    Mice_Json *json = mice_json_create_object();
    *sv = mice_string_view_trim(*sv);
    if (*sv->items == '}') {
      (void)mice_string_view_chop_left(sv, 1);
      return json;
    }

    while (true) {
      *sv = mice_string_view_trim(*sv);
      assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 1), "\""));
      Mice_String_View name = mice_string_view_chop_by_delim(sv, '"');

      *sv = mice_string_view_trim(*sv);
      assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 1), ":"));
      *sv = mice_string_view_trim(*sv);
      mice_json_add_child_sv(json, mice_json_read_from_string_view(sv), name);
      *sv = mice_string_view_trim(*sv);

      if (*sv->items != ',') break;
      (void)mice_string_view_chop_left(sv, 1);
    }

    assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 1), "}"));
    return json;
  } else if (*sv->items == '[') {
    (void)mice_string_view_chop_left(sv, 1);

    Mice_Json *json = mice_json_create_array();
    *sv = mice_string_view_trim(*sv);
    if (*sv->items == ']') {
      (void)mice_string_view_chop_left(sv, 1);
      return json;
    }

    while (true) {
      *sv = mice_string_view_trim(*sv);
      mice_json_add_child(json, mice_json_read_from_string_view(sv), NULL);
      *sv = mice_string_view_trim(*sv);

      if (*sv->items != ',') break;
      (void)mice_string_view_chop_left(sv, 1);
    }

    assert(mice_string_view_eq_cstr(mice_string_view_chop_left(sv, 1), "]"));
  }

  assert(0);
  return NULL;
}

void mice_json_write_to_string_builder(Mice_Json *json, Mice_String_Builder *sb) {
  assert(json && sb);

  switch (json->type) {
  case MICE_JSON_TYPE_STRING: {
    MICE_DA_APPEND(sb, '"');
    const char *string = json->value.string;
    MICE_DA_APPEND_MANY(sb, string, strlen(string));
    MICE_DA_APPEND(sb, '"');
  } break;
  case MICE_JSON_TYPE_NUMBER: {
    static char buf[64];
    int n = sprintf(buf, "%e", json->value.number);
    assert(n > 0);

    MICE_DA_APPEND_MANY(sb, buf, n);
  } break;
  case MICE_JSON_TYPE_BOOLEAN: {
    if (json->value.boolean) MICE_DA_APPEND_MANY(sb, "true", 4);
    else MICE_DA_APPEND_MANY(sb, "false", 5);
  } break;
  case MICE_JSON_TYPE_NULL: {
    MICE_DA_APPEND_MANY(sb, "null", 4);
  } break;
  case MICE_JSON_TYPE_OBJECT: {
    MICE_DA_APPEND(sb, '{');

    for (Mice_Json *child = json->child; child; child = child->next) {
      if (child != json->child) MICE_DA_APPEND(sb, ',');

      MICE_DA_APPEND(sb, '"');
      const char *name = child->name;
      assert(name);
      MICE_DA_APPEND_MANY(sb, name, strlen(name));
      MICE_DA_APPEND(sb, '"');
      MICE_DA_APPEND(sb, ':');
      mice_json_write_to_string_builder(child, sb);
    }

    MICE_DA_APPEND(sb, '}');
  } break;
  case MICE_JSON_TYPE_ARRAY: {
    MICE_DA_APPEND(sb, '[');

    for (Mice_Json *child = json->child; child; child = child->next) {
      if (child == json->child) MICE_DA_APPEND(sb, ',');
      mice_json_write_to_string_builder(child, sb);
    }

    MICE_DA_APPEND(sb, ']');
  } break;
  case COUNT_MICE_JSON_TYPES:
  default: assert(0);
  }
}

Mice_Json *mice_json_get(Mice_Json *json, Mice_String_View path) {
  while (path.count) {
    Mice_String_View prefix = mice_string_view_chop_left(&path, 1);
    if (mice_string_view_eq_cstr(prefix, ".")) {
      assert(json->type == MICE_JSON_TYPE_OBJECT);

      assert(mice_string_view_eq_cstr(mice_string_view_chop_left(&path, 1), "\""));
      Mice_String_View name = mice_string_view_chop_by_delim(&path, '"');

      Mice_Json *child = json->child;
      for (; child; child = child->next) {
        if (mice_string_view_eq_cstr(name, child->name)) break;
      }

      if (!child) return json;
      json = child;
    } else if (mice_string_view_eq_cstr(prefix, "[")) {
      assert(json->type == MICE_JSON_TYPE_ARRAY);

      Mice_String_View indexSv = mice_string_view_chop_by_delim(&path, ']');

      size_t index = 0;
      for (size_t i = 0; i < indexSv.count; ++i) {
        index = index * 10 + indexSv.items[i]-'0';
      }

      Mice_Json *child = json->child;
      for (; index; child = child->next) {
        if (!child) return json;
        --index;
      }

      json = child;
    } else return json;
  }

  return json;
}