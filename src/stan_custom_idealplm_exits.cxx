#include <iostream>
#include <string>
#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tccore/custom.h>
#include "misc.hxx"
#include "stan_custom_idealplm_exits.hxx"
#include "action_handlers/action_handlers.hxx"
#include "properties/properties.hxx"
#include "actions/actions.hxx"

//cppcheck-suppress unusedFunction
extern "C" __declspec (dllexport) int stan_custom_idealplm_register_callbacks()
{
	int erc = ITK_ok;

	try
	{
		printf("\Loading idealplm custom library...\n");

		erc = CUSTOM_register_exit("stan_custom_idealplm", "USER_gs_shell_init_module", stan_custom_idealplm_register_action_handlers);
		//erc = CUSTOM_register_exit("stan_custom_idealplm", "USER_register_properties", stan_custom_idealplm_register_properties);
		erc = CUSTOM_register_exit("stan_custom_idealplm", "USER_init_module", stan_custom_idealplm_register_actions);
	}
	catch (...)
	{
		return erc;
	}
	return erc;
}
