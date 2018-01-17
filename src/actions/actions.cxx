#include <iostream>
#include <string>
#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <property/prop_msg.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include "actions.hxx"
#include "post_set_izgotovitel.hxx"

/**
 *
 */
int stan_custom_idealplm_register_actions(int * decision, va_list args)
{
	int erc = ITK_ok;
	try
	{
		*decision = ALL_CUSTOMIZATIONS;

		METHOD_id_t post_set_izgotovitel_method;

		erc = METHOD_find_method("Pm8_CompanyPartRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		/*erc = METHOD_find_method("SPB5_DetRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_AssyRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_CADModelRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_ComplectRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_ComplexRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_DocRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);*/
		if (post_set_izgotovitel_method.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (M9_Cod1CRel)\n", erc);
		}
	}
	catch (int exfail)
	{
		return exfail;
	}
	return ITK_ok;
}
