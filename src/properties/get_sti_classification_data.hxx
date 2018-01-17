#ifndef GET_STI_CLASSIFICATION_DATA_HXX_
#define GET_STI_CLASSIFICATION_DATA_HXX_

#include <tc/tc.h>
#include <epm.h>

int read_sti_classification_object_data(tag_t);
int get_sti_classification_data(METHOD_message_t*, va_list);

#endif
