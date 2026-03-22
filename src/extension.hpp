#ifndef _INCLUDE_SOURCEMOD_EXTENSION_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_H_

#include "smsdk_ext.h"
#include "dt_send.h"

class ClassProxy : public SDKExtension, public IClientListener
{
public: // SDKExtension
	virtual bool SDK_OnLoad(char*, size_t, bool) override;
	virtual void SDK_OnUnload() override;
public: // IClientListener
	virtual void OnClientPutInServer(int) override;
	virtual void OnClientDisconnecting(int) override;
public:
	SendProp* m_iClass;
	SendProp* m_iDClass;
	SendVarProxyFn m_iClass_Proxy;
	SendVarProxyFn m_iDClass_Proxy;
};

void ClassProxy_m_iClass_Fn(const SendProp*, const void*, const void*, DVariant*, int, int);
void ClassProxy_m_iDesiredClass_Fn(const SendProp*, const void*, const void*, DVariant*, int, int);
extern const sp_nativeinfo_t g_NativesInfo[];
#endif