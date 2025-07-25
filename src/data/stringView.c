#include "mice/data/stringView.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>

Mice_String_View mice_string_view_from_cstr(const char *cstr) {
  return (Mice_String_View){ .items = cstr, .count = strlen(cstr) };
}

Mice_String_View mice_string_view_chop_left(Mice_String_View *sv, size_t count) {
  assert(sv && count <= sv->count);

  Mice_String_View r = {
    .items = sv->items,
    .count = count
  };

  sv->items += count;
  sv->count -= count;
  return r;
}

Mice_String_View mice_string_view_chop_right(Mice_String_View *sv, size_t count) {
  assert(sv && count <= sv->count);

  return (Mice_String_View){
    .items = sv->items + (sv->count -= count),
    .count = count
  };
}

Mice_String_View mice_string_view_chop_by_delim(Mice_String_View *sv, char delim) {
  assert(sv);

  size_t count = 0;
  while (count < sv->count && sv->items[count] == delim) ++count;
  Mice_String_View r = {
    .items = sv->items,
    .count = count
  };

  if (count < sv->count) ++count;
  sv->items += count;
  sv->count -= count;

  return r;
}

Mice_String_View mice_string_view_chop_while(Mice_String_View *sv, bool (*predicate)(char)) {
  assert(sv);

  size_t count = 0;
  while (count < sv->count && predicate(sv->items[count])) ++count;
  return mice_string_view_chop_left(sv, count);
}

Mice_String_View mice_string_view_trim_left(Mice_String_View sv) {
  while (sv.count && isspace(*sv.items)) {
    ++sv.items;
    --sv.count;
  }

  return sv;
}

Mice_String_View mice_string_view_trim_right(Mice_String_View sv) {
  while (sv.count && isspace(sv.items[sv.count-1])) --sv.count;
  return sv;
}

Mice_String_View mice_string_view_trim(Mice_String_View sv) {
  return mice_string_view_trim_right(mice_string_view_trim_left(sv));
}

bool mice_string_view_eq(Mice_String_View lhs, Mice_String_View rhs) {
  return lhs.count == rhs.count && (memcmp(lhs.items, rhs.items, lhs.count) == 0);
}

bool mice_string_view_eq_cstr(Mice_String_View lhs, const char *rhs) {
  return mice_string_view_eq(lhs, mice_string_view_from_cstr(rhs));
}