#ifndef GET_PKI_CLASSIFICATION_DATA_HXX_
#define GET_PKI_CLASSIFICATION_DATA_HXX_

#include <tc/tc.h>
#include <epm.h>

int read_pki_classification_object_data(tag_t);
int get_pki_classification_data(METHOD_message_t*, va_list);

#endif
