#include <atomic>
#include <cstdint>
#include <cstring>

#include "extension.hpp"

#if SOURCE_ENGINE != SE_TF2
#error "This extension can only be built for Team Fortress 2"
#endif

ClassProxy g_Ext;
SMEXT_LINK(&g_Ext);

std::atomic<std::uint8_t> g_playerClass[102];
std::atomic<std::uint8_t> g_playerDClass[102];

bool ClassProxy::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	sm_sendprop_info_t info;
	if (!gamehelpers->FindSendPropInfo("CTFPlayer", "m_iClass", &info))
	{
		std::strncpy(error, "Couldn't find network property CTFPlayer::m_iClass!", maxlen);
		return false;
	}
	this->m_iClass = info.prop;
	this->m_iClass_Proxy = this->m_iClass->GetProxyFn();
	this->m_iClass->SetProxyFn(ClassProxy_m_iClass_Fn);

	info;
	if (!gamehelpers->FindSendPropInfo("CTFPlayer", "m_iDesiredPlayerClass", &info))
	{
		std::strncpy(error, "Couldn't find network property CTFPlayer::m_iDesiredPlayerClass!", maxlen);
		return false;
	}
	this->m_iDClass = info.prop;
	this->m_iDClass_Proxy = this->m_iDClass->GetProxyFn();
	this->m_iDClass->SetProxyFn(ClassProxy_m_iDesiredClass_Fn);

	playerhelpers->AddClientListener(this);
	sharesys->AddNatives(myself, g_NativesInfo);
	return true;
}

void ClassProxy::SDK_OnUnload()
{
	this->m_iClass->SetProxyFn(this->m_iClass_Proxy);
	this->m_iDClass->SetProxyFn(this->m_iDClass_Proxy);
	playerhelpers->RemoveClientListener(this);
}

void ClassProxy::OnClientPutInServer(int client)
{
	g_playerClass[client] = 0;
	g_playerDClass[client] = 0;
}

void ClassProxy::OnClientDisconnecting(int client)
{
	g_playerClass[client] = 0;
	g_playerDClass[client] = 0;
}

void ClassProxy_m_iClass_Fn(const SendProp* pProp, const void* pStructBase, const void* pData, DVariant* pOut, int iElement, int objectID)
{
	if (0 < objectID && objectID <= 101)
	{
		std::int32_t classValue = g_playerClass[objectID].load(std::memory_order_relaxed);
		if (classValue != 0)
		{
			g_Ext.m_iClass_Proxy(pProp, pStructBase, &classValue, pOut, iElement, objectID);
			return;
		}
	}
	g_Ext.m_iClass_Proxy(pProp, pStructBase, pData, pOut, iElement, objectID);
}

cell_t ClassProxy_m_iClass_Set(IPluginContext* pContext, const cell_t* params)
{
	IGamePlayer* player = playerhelpers->GetGamePlayer(params[1]);
	if (player == nullptr || params[1] <= 0 || params[1] > 101)
	{
		return pContext->ThrowNativeError("Invalid client index (%i)!", params[1]);
	}
	if (!player->IsInGame())
	{
		return pContext->ThrowNativeError("Client (%i) isn't in game!", params[1]);
	}
	g_playerClass[params[1]].store(params[2], std::memory_order_relaxed);
	return 0;
}

void ClassProxy_m_iDesiredClass_Fn(const SendProp* pProp, const void* pStructBase, const void* pData, DVariant* pOut, int iElement, int objectID)
{
	if (0 < objectID && objectID <= 101)
	{
		std::int32_t classValue = g_playerDClass[objectID].load(std::memory_order_relaxed);
		if (classValue != 0)
		{
			g_Ext.m_iDClass_Proxy(pProp, pStructBase, &classValue, pOut, iElement, objectID);
			return;
		}
	}
	g_Ext.m_iDClass_Proxy(pProp, pStructBase, pData, pOut, iElement, objectID);
}

cell_t ClassProxy_m_iDesiredClass_Set(IPluginContext* pContext, const cell_t* params)
{
	IGamePlayer* player = playerhelpers->GetGamePlayer(params[1]);
	if (player == nullptr || params[1] <= 0 || params[1] > 101)
	{
		return pContext->ThrowNativeError("Invalid client index (%i)!", params[1]);
	}
	if (!player->IsInGame())
	{
		return pContext->ThrowNativeError("Client (%i) isn't in game!", params[1]);
	}
	g_playerDClass[params[1]].store(params[2], std::memory_order_relaxed);
	return 0;
}

const sp_nativeinfo_t g_NativesInfo[] =
{
	{"ClassProxy_m_iClass_Set", ClassProxy_m_iClass_Set},
	{"ClassProxy_m_iDesiredPlayerClass_Set", ClassProxy_m_iDesiredPlayerClass_Set},
	{nullptr, nullptr}
};