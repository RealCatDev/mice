#ifndef   _MICE_DATA_STRING_VIEW_H_
#define   _MICE_DATA_STRING_VIEW_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct Mice_String_View {
  const char *items;
  size_t count;
} Mice_String_View;

Mice_String_View mice_string_view_from_cstr(const char *cstr);

Mice_String_View mice_string_view_chop_left(Mice_String_View *sv, size_t count);
Mice_String_View mice_string_view_chop_right(Mice_String_View *sv, size_t count);
Mice_String_View mice_string_view_chop_by_delim(Mice_String_View *sv, char delim);
Mice_String_View mice_string_view_chop_while(Mice_String_View *sv, bool (*predicate)(char));

Mice_String_View mice_string_view_trim_left(Mice_String_View sv);
Mice_String_View mice_string_view_trim_right(Mice_String_View sv);
Mice_String_View mice_string_view_trim(Mice_String_View sv);

bool mice_string_view_eq(Mice_String_View lhs, Mice_String_View rhs);
bool mice_string_view_eq_cstr(Mice_String_View lhs, const char *rhs);

#endif // _MICE_DATA_STRING_VIEW_H_