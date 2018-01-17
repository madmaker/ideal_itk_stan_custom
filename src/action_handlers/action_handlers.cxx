#include <iostream>
#include <string>
#include <tc/tc.h>
#include <property/prop_msg.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include "action_handlers.hxx"
#include "remove_targets_from_refs.hxx"
#include "remove_references_with_status.hxx"
#include "check_user_group.hxx"

int stan_custom_idealplm_register_action_handlers(int * decision, va_list args)
{
	int erc = ITK_ok;
	try
	{
		*decision = ALL_CUSTOMIZATIONS;

		erc = EPM_register_action_handler("IDEALPLM_REMOVE_TARGETS_FROM_REFS", "", remove_targets_from_refs);
		erc = EPM_register_action_handler("IDEALPLM_REMOVE_REFERENCES_WITH_STATUS", "", remove_references_with_status);
		erc = EPM_register_rule_handler("IDEALPLM_CHECK_USER_GROUP", "Success if all target items are from the same factory group as user", check_user_group);
	}
	catch (int exfail)
	{
		erc = exfail;
	}
	return erc;
}
