#if !defined _CLASSPROXY_EXTENSION_INC_
#define _CLASSPROXY_EXTENSION_INC_

#include <tf2>

/**
	* Sets the client's tf2 class to proxy to clients.
	* 
	* @param classType        TF2 Class to proxy, set to TFClass_Unknown to disable.	
	* @error                  Client index is invalid, or not in game
*/
void ClassProxy_m_iClass_Set(int client, TFClassType classType);
void ClassProxy_m_iDesiredPlayerClass_Set(int client, TFClassType classType);

public Extension __ext_classproxy = 
{
	name = "classproxy",
	file = "classproxy.ext",
	autoload = 1,
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#endif