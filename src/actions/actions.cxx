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

		METHOD_id_t post_set_izgotovitel_method_det;
		METHOD_id_t post_set_izgotovitel_method_assy;
		METHOD_id_t post_set_izgotovitel_method_cad;
		METHOD_id_t post_set_izgotovitel_method_complect;
		METHOD_id_t post_set_izgotovitel_method_complex;
		METHOD_id_t post_set_izgotovitel_method_doc;
		METHOD_id_t post_set_izgotovitel_method_operation;
		METHOD_id_t post_set_izgotovitel_method_process;
		METHOD_id_t post_set_izgotovitel_method_route;

		//erc = METHOD_find_method("Pm8_CompanyPartRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method);
		erc = METHOD_find_method("SPB5_DetRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_det);
		erc = METHOD_find_method("SPB5_AssyRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_assy);
		erc = METHOD_find_method("SPB5_CADModelRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_cad);
		erc = METHOD_find_method("SPB5_ComplectRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_complect);
		erc = METHOD_find_method("SPB5_ComplexRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_complex);
		erc = METHOD_find_method("SPB5_DocRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_doc);
		erc = METHOD_find_method("TPP5_OperationRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_operation);
		erc = METHOD_find_method("TPP5_MEProcessRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_process);
		erc = METHOD_find_method("TPP5_MERouteRevision", "createPost#Teamcenter::CreateInput,*", &post_set_izgotovitel_method_route);

		if (post_set_izgotovitel_method_det.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_det, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_DetRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_assy.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_assy, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_AssyRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_cad.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_cad, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_CADModelRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_complect.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_complect, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_ComplectRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_complex.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_complex, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_ComplexRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_doc.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_doc, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (SPB5_DocRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_operation.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_operation, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (TPP5_OperationRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_process.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_process, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (TPP5_MEProcessRevision)\n", erc);
		}

		if (post_set_izgotovitel_method_route.id != NULLTAG) {
			erc = METHOD_add_action(post_set_izgotovitel_method_route, METHOD_post_action_type, (METHOD_function_t) post_set_izgotovitel, NULL);
		} else {
			fprintf(stdout, "Method NOT found! (createPost#Teamcenter::CreateInput,*) for (TPP5_MERouteRevision)\n", erc);
		}
	}
	catch (int exfail)
	{
		return exfail;
	}
	return ITK_ok;
}
