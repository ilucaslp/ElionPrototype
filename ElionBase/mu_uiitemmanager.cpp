#include "stdafx.h"
#include "mu_uiitemmanager.h"

std::stack<mu_uint32> g_ItemKeyMapper;
mu_uint32 g_ItemKey = 1000;

ITEM *GenerateItem()
{
	ITEM *Item = new ITEM();

	if (g_ItemKeyMapper.empty() == false)
	{
		Item->Key = g_ItemKeyMapper.top();
		g_ItemKeyMapper.pop();
	}
	else
	{
		Item->Key = g_ItemKey++;
	}

	return Item;
}

void DestroyItem(ITEM *Item)
{
	g_ItemKeyMapper.push(Item->Key);

	delete Item;
}