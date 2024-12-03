#ifndef __MU_ITEMCATEGORIESDB_H__
#define __MU_ITEMCATEGORIESDB_H__

#pragma once

class MUItemCategoryItemData
{
public:
	mu_int32 ItemIndex;
	mu_int16 ItemLevel;
};

class MUItemCategoriesData
{
public:
	mu_int32 CategoryName;
	std::vector<MUItemCategoryItemData> Items;
};

class MUItemCategoriesDatabase
{
public:
	MUItemCategoriesDatabase();

	mu_boolean Load(const mu_text *filename);

public:
	std::vector<MUItemCategoriesData> _Categories;
};

extern MUItemCategoriesDatabase g_ItemCategoriesDatabase;

#endif