#include "stdafx.h"
#include "mu_uisimplename.h"

extern mu_int32 SommonTable[];

UISimpleName::UISimpleName(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	// Initialize
	{
		_ShowItemName = true;
	}
}

UISimpleName::~UISimpleName()
{

}

void UISimpleName::Update(mu_uint64 CurrentTime)
{

}

void UISimpleName::Draw()
{
	EVector4 Limits = EVector4(MU_VIDEO->_3DViewport.OffsetX,
		MU_VIDEO->_3DViewport.OffsetY,
		MU_VIDEO->_3DViewport.OffsetX + MU_VIDEO->_3DViewport.Width,
		MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height);

#if 0 // FIX
	if (g_bGMObservation == true)
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Live && o->Kind == KIND_PLAYER)
			{
				if (IsShopTitleVisible(c) == false)
				{
					g_pSimpleChat->Create(c->ID, _T(""), c);
				}
			}
		}
	}
#endif

	if (InChaosCastle() == true)
	{
		if (SelectedNpc != -1 || SelectedCharacter != -1)
		{
			return;
		}
	}

	if (SelectedItem != -1 || SelectedNpc != -1 || SelectedCharacter != -1)
	{
		if (SelectedNpc != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedNpc];
			g_pSimpleChat->Create(c->ID, _T(""), c);
		}
		else if (SelectedCharacter != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedCharacter];

			OBJECT *o = &c->Object;
			if (o->Kind == KIND_MONSTER)
			{
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::SIMPLENAME_MONSTERNAME,
					MU_VIDEO->_3DViewport.OffsetX + MU_VIDEO->_3DViewport.Width * 0.5f,
					MU_TO_WINDOW(2.0f, MU_RESOURCE->_640x480Rate[1]),
					1.0f,
					ETA_CENTER, ETA_BOTTOM,
					0.0f,
					MU::MakeARGB(255, 230, 200, 255),
					MU::MakeARGB(100, 0, 0, 255),
					c->ID);
			}
			else
			{
#if 0 // FIX
				if (IsShopTitleVisible(c) == false)
#endif
				{
					g_pSimpleChat->Create(c->ID, _T(""), c);
				}
			}
		}
		else if (SelectedItem != -1)
		{
			RenderItemName(SelectedItem, &Items[SelectedItem].Object, Items[SelectedItem].Item, false, Limits);
		}
	}

	if (_ShowItemName)
	{
		for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
		{
			OBJECT *o = &Items[i].Object;
			if (i != SelectedItem && o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE))
			{
				RenderItemName(i, o, Items[i].Item, true, Limits);
			}
		}
	}
}

void UISimpleName::RenderItemName(mu_int32 i, OBJECT *o, ITEM &Item, mu_boolean Sort, EVector4 &Limits)
{
	mu_text Name[256];

	mu_int32 Level = Item.Level;

	MUTextSimpleRender *Font = FONT_NORMAL;
	mu_uint32 TextColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 BackColor = MU::MakeARGB(0, 0, 0, 192);

	if (o->Type == MODEL_POTION + 15)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" %lld"), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str(), Item.Money);
	}
	else if ((o->Type == MODEL_POTION + 13)
		|| (o->Type == MODEL_POTION + 14)
		|| (o->Type == MODEL_POTION + 16)
		|| (o->Type == MODEL_WING + 15)
		|| (o->Type == MODEL_POTION + 22)
		|| (o->Type == MODEL_POTION + 31)
		|| (o->Type == MODEL_HELPER + 14)
		|| (o->Type == MODEL_POTION + 41)
		|| (o->Type == MODEL_POTION + 42)
		|| (o->Type == MODEL_POTION + 43)
		|| (o->Type == MODEL_POTION + 44)
		)
	{
		Font = FONT_BOLD;
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 11)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), SkillAttribute[30 + Level].Name, GlobalText[102]);
	}
	else if (o->Type == MODEL_WING + 7)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 7].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 21)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 21].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 22)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 22].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 23)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 23].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 46)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 46].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 47)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 47].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 48)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 48].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 54)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 54].Name.c_str());
	}
	else if (o->Type >= MODEL_POTION + 58 && o->Type <= MODEL_POTION + 62)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= MODEL_POTION + 145 && o->Type <= MODEL_POTION + 150)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 125)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 125].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 126)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 126].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 127)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 127].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 53)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 53].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 43 || o->Type == MODEL_HELPER + 44 || o->Type == MODEL_HELPER + 45)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_HELPER].Name.c_str());
	}
	else if (o->Type >= ITEM_POTION + 70 && o->Type <= ITEM_POTION + 71)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= ITEM_POTION + 72 && o->Type <= ITEM_POTION + 77)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 59)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= ITEM_HELPER + 54 && o->Type <= ITEM_HELPER + 58)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= ITEM_POTION + 78 && o->Type <= ITEM_POTION + 82)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 60)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 61)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 83)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 43 || o->Type == MODEL_HELPER + 44 || o->Type == MODEL_HELPER + 45)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_HELPER].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 91)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 91].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 92)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 92].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 93)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 93].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 95)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 95].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 94)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 94].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 84)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 84].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 85)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 85].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 86)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 86].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 87)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 87].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 88)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 88].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 89)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 89].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 90)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 90].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 62)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 62].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 63)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 63].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 97)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 97].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 98)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 98].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 96)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 96].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 64)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 64].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 65)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 65].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 67)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 67].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 80)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 80].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 106)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 106].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 123)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 123].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 68)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 68].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 76)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 76].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 122)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 122].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 128)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 128].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 129)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 129].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 134)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 134].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 130)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 130].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 131)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 131].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 132)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 132].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 133)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 133].Name.c_str());
	}
	else if (o->Type == MODEL_COMPILED_CELE)
	{
		Font = FONT_BOLD;
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), ItemAttribute[ITEM_POTION + 13].Name.c_str());
	}
	else if (o->Type == MODEL_COMPILED_SOUL)
	{
		Font = FONT_BOLD;
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), ItemAttribute[ITEM_POTION + 14].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 17 || o->Type == MODEL_POTION + 18 || o->Type == MODEL_POTION + 19)
	{
		Font = FONT_BOLD;
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		if (Level == 0)
		{
			mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
		}
		else
		{
			mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" +%d"), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str(), Level);
		}
	}
	else if (o->Type == MODEL_POTION + 11 && Level == 7)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[111]);
	}
	else if (o->Type == MODEL_POTION + 12)
	{
		switch (Level)
		{
		case 0:mu_sprintf_s(Name, mu_countof(Name), GlobalText[100]); break;
		case 1:mu_sprintf_s(Name, mu_countof(Name), GlobalText[101]); break;
		case 2:mu_sprintf_s(Name, mu_countof(Name), GlobalText[104]); break;
		}
	}
	else if (o->Type == MODEL_HELPER + 15)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		switch (Level)
		{
		case 0:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[168], ItemAttribute[o->Type - MODEL_ITEM].Name.c_str()); break;
		case 1:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[169], ItemAttribute[o->Type - MODEL_ITEM].Name.c_str()); break;
		case 2:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[167], ItemAttribute[o->Type - MODEL_ITEM].Name.c_str()); break;
		case 3:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[166], ItemAttribute[o->Type - MODEL_ITEM].Name.c_str()); break;
		case 4:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1900], ItemAttribute[o->Type - MODEL_ITEM].Name.c_str()); break;
		}
	}
	else if (o->Type == MODEL_HELPER + 31)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		switch (Level)
		{
		case 0:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" of ") SPF_STRING _T(""), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str(), GlobalText[1187]); break;
		case 1:mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" of ") SPF_STRING _T(""), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str(), GlobalText[1214]); break;
		}
	}
	else if (o->Type == MODEL_EVENT + 16)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[1235]);
	}
	else if (o->Type == MODEL_EVENT + 4)
	{
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[105]);
	}
	else if (o->Type == MODEL_EVENT + 5)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		switch (Level)
		{
		case 14:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[1650]);
			break;

		case 15:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[1651]);
			break;

		default:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[106]);
			break;
		}
	}
	else if (o->Type == MODEL_EVENT + 6)
	{
		if (Level == 13)
		{
			TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
			mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), GlobalText[117]);
		}
		else
		{
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[107]);
		}
	}
	else if (o->Type == MODEL_EVENT + 7)
	{
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[108]);
	}
	else if (o->Type == MODEL_EVENT + 8)
	{
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[109]);
	}
	else if (o->Type == MODEL_EVENT + 9)
	{
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[110]);
	}
	else if (o->Type == MODEL_EVENT + 10)
	{
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" +%d"), GlobalText[115], Level - 7);
	}
	else if (o->Type == MODEL_WING + 32)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.3f, 0.3f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 32].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 33)
	{
		TextColor = MU::Float4ToARGB(0.3f, 1.0f, 0.3f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 33].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 34)
	{
		TextColor = MU::Float4ToARGB(0.3f, 0.3f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 34].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 49)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 49].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 50)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 50].Name.c_str());
	}
	else if (o->Type == ITEM_HELPER + 51)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 51].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 64)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 64].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 48)
	{
		TextColor = MU::Float4ToARGB(1.0f, 1.0f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 48].Name.c_str());
	}
	else if (o->Type == MODEL_WING + 35)
	{
		TextColor = MU::Float4ToARGB(1.0f, 1.0f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_WING + 35].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 45)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 45].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 46)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 46].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 47)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 47].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 48)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 48].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 49)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 49].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 50)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 50].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 32)
	{
		if (Level == 0)
		{
			TextColor = MU::Float4ToARGB(1.0f, 0.3f, 1.0f);
			mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 32].Name.c_str());
		}
		else if (Level == 1)
			{
				TextColor = MU::Float4ToARGB(1.0f, 0.3f, 1.0f);
				mu_sprintf_s(Name, mu_countof(Name), GlobalText[2012]);
			}
	}
	else if (o->Type == MODEL_POTION + 33)
	{
		if (Level == 0)
		{
			TextColor = MU::Float4ToARGB(1.0f, 0.3f, 0.3f);
			mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 33].Name.c_str());
		}
		else if (Level == 1)
			{
				TextColor = MU::Float4ToARGB(1.0f, 0.3f, 0.3f);
				mu_sprintf_s(Name, mu_countof(Name), GlobalText[2013]);
			}
	}
	else if (o->Type == MODEL_POTION + 34)
	{
		if (Level == 0)
		{
			TextColor = MU::Float4ToARGB(0.3f, 0.3f, 1.0f);
			mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 34].Name.c_str());
		}
		else if (Level == 1)
			{
				TextColor = MU::Float4ToARGB(0.3f, 0.3f, 1.0f);
				mu_sprintf_s(Name, mu_countof(Name), GlobalText[2014]);
			}
	}
	else if (o->Type == MODEL_EVENT + 21)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.3f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[2012]);
	}
	else if (o->Type == MODEL_EVENT + 22)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.3f, 0.3f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[2013]);
	}
	else if (o->Type == MODEL_EVENT + 23)
	{
		TextColor = MU::Float4ToARGB(0.3f, 0.3f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[2014]);
	}
	else if (o->Type == MODEL_EVENT + 11)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[810]);
	}
	else if (o->Type == MODEL_EVENT + 12)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[906]);
	}
	else if (o->Type == MODEL_EVENT + 13)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[907]);
	}
	else if (o->Type == MODEL_EVENT + 14)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		switch (Level)
		{
		case 2:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[928]);
			break;
		case 3:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[929]);
			break;
		default:
			mu_sprintf_s(Name, mu_countof(Name), GlobalText[922]);
			break;
		}
	}
	else if (o->Type == MODEL_EVENT + 15)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[925]);
	}
	else if (o->Type == MODEL_WING + 11)
	{
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), SkillAttribute[30 + Level].Name, GlobalText[102]);
	}
	else if (o->Type == MODEL_HELPER + 10)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GetMonsterName(SommonTable[Level]), GlobalText[103]);
	}
	else if (o->Type == MODEL_POTION + 21 && Level == 3)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[1290]);
	}
	else if (o->Type == MODEL_POTION + 7)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1413]); break;
		case 1: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1414]); break;
		}
	}
	else if (o->Type == MODEL_HELPER + 7)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1460]); break;
		case 1: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1461]); break;
		}
	}
	else if (o->Type == MODEL_HELPER + 11)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1416]); break;
		case 1: mu_sprintf_s(Name, mu_countof(Name), GlobalText[1462]); break;
		}
	}
	else if (o->Type == MODEL_EVENT + 18)
	{
		mu_sprintf_s(Name, mu_countof(Name), GlobalText[1462]);
	}
	else if ((o->Type >= MODEL_WING + 60 && o->Type <= MODEL_WING + 65)
		|| (o->Type >= MODEL_WING + 70 && o->Type <= MODEL_WING + 74)
		|| (o->Type >= MODEL_WING + 100 && o->Type <= MODEL_WING + 129))
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.4f, 1.0f);
		mu_strcpy_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 66)
	{
		TextColor = MU::Float4ToARGB(0.6f, 0.4f, 1.0f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 66].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 100)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 100].Name.c_str());
	}
	else if (o->Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN
		&& o->Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 109)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 109].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 110)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 110].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 111)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 111].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 112)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 112].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 113)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 113].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 114)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 114].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 115)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_HELPER + 115].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 112)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 112].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 113)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[ITEM_POTION + 113].Name.c_str());
	}
	else
	{
		ItemNameColor(o, Name, Font, TextColor, BackColor, Item);
	}

	if (!Sort)
	{
		mu_float x = MouseX;
		mu_float y = MouseY - 15.0f;

		if (y < 0.0f)
		{
			y = 0.0f;
		}

		Font->DrawStringC2D(TEXTINDEX::BEGIN_ITEMNAME + i,
			MU_TO_WINDOW(x, MU_RESOURCE->_640x480Rate[0]),
			MU_TO_WINDOW(y, MU_RESOURCE->_640x480Rate[1]),
			1.0f,
			ETA_CENTER, ETA_BOTTOM, 0.0f,
			TextColor, BackColor,
			Name);
	}
	else
	{
		mu_float limitBottom = (MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height) - MU_TO_WINDOW(12.0f, MU_RESOURCE->_640x480Rate[1]);

		ITEM_t &item = Items[i];
		EVector3 Position = item.Object.Position;
		Position.z += 60.0f;

		EVector3 Projected = EMath::Project(Position, MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, Limits);

		Projected.y = MU_VIDEO->_3DViewport.Height - Projected.y;

		if (Projected.y < MU_VIDEO->_3DViewport.OffsetY)
		{
			Projected.y = MU_VIDEO->_3DViewport.OffsetY;
		}
		else if (Projected.y > limitBottom)
		{
			Projected.y = limitBottom;
		}

		Font->DrawStringC2D(TEXTINDEX::BEGIN_ITEMNAME + i,
			Projected.x,
			Projected.y,
			1.0f,
			ETA_CENTER, ETA_BOTTOM, 0.0f,
			TextColor, BackColor,
			Name);
	}
}

template<mu_size Length>
void UISimpleName::ItemNameColor(OBJECT *o, mu_text (&Name)[Length], MUTextSimpleRender* &Font, mu_uint32 &TextColor, mu_uint32 &BackColor, ITEM &Item)
{
	mu_int32 Level = Item.Level;

	if (o->Type == MODEL_STAFF + 10 || o->Type == MODEL_SWORD + 19 || o->Type == MODEL_BOW + 18 || o->Type == MODEL_MACE + 13)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.1f, 1.0f);
	}
	else if (g_SocketItemMgr.IsSocketItem(o))
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.4f, 1.0f);
	}
	else if (Item.ExcOption > 0 && (o->Type<MODEL_WING + 3 || o->Type>MODEL_WING + 6) && o->Type != MODEL_HELPER + 30
		&& (o->Type<MODEL_WING + 36 || o->Type>MODEL_WING + 40)
		&& (o->Type<MODEL_WING + 42 || o->Type>MODEL_WING + 43)
		)
	{
		TextColor = MU::Float4ToARGB(0.1f, 1.0f, 0.5f);
	}
	else if (MODEL_POTION + 101 <= o->Type && o->Type <= MODEL_POTION + 109)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 111)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 120)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 121)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 122)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 123)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_POTION + 124)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (MODEL_POTION + 134 <= o->Type && o->Type <= MODEL_POTION + 139)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type == MODEL_HELPER + 116)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (ITEM_WING + 130 <= o->Type &&
		o->Type <= ITEM_WING + 134
		)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (MODEL_POTION + 114 <= o->Type && o->Type <= MODEL_POTION + 119)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (MODEL_POTION + 126 <= o->Type && o->Type <= MODEL_POTION + 129)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (MODEL_POTION + 130 <= o->Type && o->Type <= MODEL_POTION + 132)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (MODEL_HELPER + 121 == o->Type)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= MODEL_POTION + 141 && o->Type <= MODEL_POTION + 144)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.53f, 0.13f);
		mu_sprintf_s(Name, mu_countof(Name), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else if (o->Type >= MODEL_POTION + 157 && o->Type <= MODEL_POTION + 159)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
	}
	else if (o->Type == MODEL_WING + 25)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
	}
	else if (o->Type == MODEL_POTION + 28 || o->Type == MODEL_POTION + 29)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
	}
	else if (Level >= 7)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.8f, 0.1f);
	}
	else if (Item.Luck == true || Item.Skill == true || Item.Option > 0)
	{
		TextColor = MU::Float4ToARGB(0.4f, 0.7f, 1.0f);
	}
	else if (Level == 0)
	{
		TextColor = MU::Float4ToARGB(0.7f, 0.7f, 0.7f);
	}
	else if (Level < 3)
	{
		TextColor = MU::Float4ToARGB(0.9f, 0.9f, 0.9f);
	}
	else if (Level < 5)
	{
		TextColor = MU::Float4ToARGB(1.0f, 0.5f, 0.2f);
	}
	else if (Level < 7)
	{
		TextColor = MU::Float4ToARGB(0.4f, 0.7f, 1.0f);
	}

	mu_text TextName[256];
	if (g_csItemOption.GetSetItemName(TextName, o->Type - MODEL_ITEM, Item.SetOption))
	{
		TextColor = MU::Float4ToARGB(1.0f, 1.0f, 1.0f);
		Font = FONT_BOLD;
		TextColor = MU::MakeARGB(0, 255, 0, 255);
		BackColor = MU::MakeARGB(60, 60, 200, 192);

		mu_strcat_s(TextName, mu_countof(TextName), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}
	else
	{
		mu_strcpy_s(TextName, mu_countof(TextName), ItemAttribute[o->Type - MODEL_ITEM].Name.c_str());
	}

	if (Level == 0)
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), TextName);
	else
		mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(" +%d"), TextName, Level);

	if (Item.Skill == true)
	{
		if (o->Type != MODEL_HELPER + 3)
		{
			mu_strcat_s(Name, mu_countof(Name), GlobalText[176]);
		}
		else
		{
			mu_strcat_s(Name, mu_countof(Name), _T(" +"));
			mu_strcat_s(Name, mu_countof(Name), GlobalText[179]);
		}
	}

	if (Item.Option > 0)
	{
		mu_strcat_s(Name, mu_countof(Name), GlobalText[177]);
	}

	if (Item.Luck == true)
	{
		mu_strcat_s(Name, mu_countof(Name), GlobalText[178]);
	}
}

mu_uint32 UISimpleName::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		{
			if (MU_INPUT->IsShiftPressing() == false &&
				MU_INPUT->IsCtrlPressing() == false)
			{
				switch (event->key.keysym.scancode)
				{
				case SDL_SCANCODE_MENU:
					{
						if (MU_INPUT->IsKeyHolding(SDL_SCANCODE_MENU) == false)
						{
							_ShowItemName = !_ShowItemName;
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UISimpleName::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return nullptr;
}