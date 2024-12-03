#include "stdafx.h"
#include "mu_renderproxy.h"

MUCriticalSectionSpinLock g_RenderProxyLock(5000);
mu_uint32 g_RenderProxyCount = 0;
mu_boolean g_RenderProxySort = false;
MURenderProxy g_RenderProxy[MAX_RENDERPROXY];
mu_uint32 g_RenderProxyIndex[MAX_RENDERPROXY];
std::map<mu_uint64, mu_uint32> g_RenderProxyMap;
std::map<mu_uint64, mu_uint32> g_RenderProxyFlyMap;

RenderProxyFastSort g_RenderProxyFastSort;

mu_uint32 FindRenderProxy(mu_uint8 RenderIndex, mu_uint16 Model, mu_uint8 RenderType, mu_uint8 BlendType, mu_uint16 ExtraFlag, mu_uint16 Texture, mu_uint8 MeshIndex)
{
	mu_uint64 Hash = RetrieveRenderProxyHash(RenderIndex, Model, RenderType, BlendType, ExtraFlag, Texture, MeshIndex);

	mu_uint32 proxyIndex = WAITING_PROXY;

	auto it = g_RenderProxyMap.find(Hash);

	if (it == g_RenderProxyMap.end())
	{
		g_RenderProxyLock.Lock();
		auto it = g_RenderProxyFlyMap.find(Hash);

		if(it == g_RenderProxyFlyMap.end())
		{
			if (g_RenderProxyCount < MAX_RENDERPROXY)
			{
				g_RenderProxySort = true;
				proxyIndex = g_RenderProxyCount++;
				g_RenderProxyIndex[proxyIndex] = proxyIndex;

				g_RenderProxyFlyMap.insert(std::make_pair(Hash, proxyIndex));

				MURenderProxy &proxy = g_RenderProxy[proxyIndex];
				proxy.Hash = Hash;

				proxy.ModelIndex = Model;
				proxy.TextureIndex = Texture;

				proxy.RenderIndex = RenderIndex;
				proxy.RenderType = RenderType;
				proxy.BlendType = BlendType;
				proxy.ExtraFlag = ExtraFlag;
				proxy.MeshIndex = MeshIndex;

				proxy.Enabled = false;
				proxy.ShadowEnabled = false;

				proxy.InstanceCount = 0;
				proxy.InstanceBase = 0;
			}
		}
		else
		{
			proxyIndex = it->second;
		}
		g_RenderProxyLock.Unlock();
	}
	else
	{
		proxyIndex = it->second;
	}

	return proxyIndex;
}

void MergeRenderProxy()
{
	if(g_RenderProxyFlyMap.empty() == false)
	{
		g_RenderProxyMap.insert(std::make_move_iterator(g_RenderProxyFlyMap.begin()), std::make_move_iterator(g_RenderProxyFlyMap.end()));
		g_RenderProxyFlyMap.clear();
	}
}