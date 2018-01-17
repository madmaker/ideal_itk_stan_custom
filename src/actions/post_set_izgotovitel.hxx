#ifndef POST_SET_IZGOTOVITEL_HXX
#define POST_SET_IZGOTOVITEL_HXX
#include <tccore/method.h>
#include <aom.h>
#include <item.h>
#include <preferences.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/tctype.h>
#include <iostream>
#ifdef __cplusplus
         extern "C"{
#endif

int is_subgroup_of_group(tag_t inner_group, tag_t outer_group, bool* isSubgroupOfGroup);
int post_set_izgotovitel(METHOD_message_t* msg, va_list args);

#ifdef __cplusplus
                   }
#endif

#endif
