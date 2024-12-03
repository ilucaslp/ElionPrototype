#include "stdafx.h"
#include "mu_uimapname.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f

#define IMAGE_WIDTH 166.0f
#define IMAGE_HEIGHT 99.0f
#define IMAGE_POSX ((SCREEN_WIDTH - IMAGE_WIDTH) * 0.5f)
#define IMAGE_POSY ((SCREEN_HEIGHT * 0.9f - IMAGE_HEIGHT) * 0.5f)

#define IMAGE_ALPHA 0.015f
#define IMAGE_TIME 5000.0f // 5 seconds

UIMapName::UIMapName(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN | GFX_FLAG::GFX_NO_FOCUS);

	// Initialize
	{
		_Logo = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WELCOME_LORENCIA], SCREEN_WIDTH, SCREEN_HEIGHT, IMAGE_POSX, IMAGE_POSY, IMAGE_WIDTH, IMAGE_HEIGHT);
		_Logo->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_WorldMap.insert(std::make_pair(WD_0LORENCIA, INTERFACES::WELCOME_LORENCIA));
		_WorldMap.insert(std::make_pair(WD_1DUNGEON, INTERFACES::WELCOME_DUNGEON));
		_WorldMap.insert(std::make_pair(WD_2DEVIAS, INTERFACES::WELCOME_DEVIAS));
		_WorldMap.insert(std::make_pair(WD_3NORIA, INTERFACES::WELCOME_NORIA));
		_WorldMap.insert(std::make_pair(WD_4LOSTTOWER, INTERFACES::WELCOME_LOSTTOWER));

		_WorldMap.insert(std::make_pair(WD_6STADIUM, INTERFACES::WELCOME_STADIUM));
		_WorldMap.insert(std::make_pair(WD_7ATLANSE, INTERFACES::WELCOME_ATLANS));
		_WorldMap.insert(std::make_pair(WD_8TARKAN, INTERFACES::WELCOME_TARKAN));
		_WorldMap.insert(std::make_pair(WD_9DEVILSQUARE, INTERFACES::WELCOME_DEVILSQUARE));
		_WorldMap.insert(std::make_pair(WD_10HEAVEN, INTERFACES::WELCOME_ICARUS));

		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 0, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 1, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 2, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 3, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 4, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 5, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_11BLOODCASTLE1 + 6, INTERFACES::WELCOME_BLOODCASTLE));
		_WorldMap.insert(std::make_pair(WD_52BLOODCASTLE_MASTER_LEVEL, INTERFACES::WELCOME_BLOODCASTLE));

		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 0, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 1, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 2, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 3, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 4, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_18CHAOS_CASTLE + 5, INTERFACES::WELCOME_CHAOSCASTLE));
		_WorldMap.insert(std::make_pair(WD_53CAOSCASTLE_MASTER_LEVEL, INTERFACES::WELCOME_CHAOSCASTLE));

		_WorldMap.insert(std::make_pair(WD_24HELLAS + 0, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS + 1, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS + 2, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS + 3, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS + 4, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS + 5, INTERFACES::WELCOME_KALIMA));
		_WorldMap.insert(std::make_pair(WD_24HELLAS_7, INTERFACES::WELCOME_KALIMA));

		_WorldMap.insert(std::make_pair(WD_30BATTLECASTLE, INTERFACES::WELCOME_VALLEYOFLOREN));
		_WorldMap.insert(std::make_pair(WD_31HUNTING_GROUND, INTERFACES::WELCOME_LANDOFTRIAL));

		_WorldMap.insert(std::make_pair(WD_33AIDA, INTERFACES::WELCOME_AIDA));
		_WorldMap.insert(std::make_pair(WD_34CRYWOLF_1ST, INTERFACES::WELCOME_CRYWOLF));

		_WorldMap.insert(std::make_pair(WD_37KANTURU_1ST, INTERFACES::WELCOME_KANTURU));
		_WorldMap.insert(std::make_pair(WD_38KANTURU_2ND, INTERFACES::WELCOME_KANTURU));
		_WorldMap.insert(std::make_pair(WD_39KANTURU_3RD, INTERFACES::WELCOME_KANTURU));

		_WorldMap.insert(std::make_pair(WD_41CHANGEUP3RD_1ST, INTERFACES::WELCOME_BALGASBARRACK));
		_WorldMap.insert(std::make_pair(WD_42CHANGEUP3RD_2ND, INTERFACES::WELCOME_BALGASREFUGE));

		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV1, INTERFACES::WELCOME_ILLUSIONTEMPLE));
		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV2, INTERFACES::WELCOME_ILLUSIONTEMPLE));
		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV3, INTERFACES::WELCOME_ILLUSIONTEMPLE));
		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV4, INTERFACES::WELCOME_ILLUSIONTEMPLE));
		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV5, INTERFACES::WELCOME_ILLUSIONTEMPLE));
		_WorldMap.insert(std::make_pair(WD_45CURSEDTEMPLE_LV6, INTERFACES::WELCOME_ILLUSIONTEMPLE));

		_WorldMap.insert(std::make_pair(WD_51HOME_6TH_CHAR, INTERFACES::WELCOME_ELBELAND));

		_WorldMap.insert(std::make_pair(WD_56MAP_SWAMP_OF_QUIET, INTERFACES::WELCOME_SWAMPOFCALMNESS));

		_WorldMap.insert(std::make_pair(WD_57ICECITY, INTERFACES::WELCOME_RAKLION));
		_WorldMap.insert(std::make_pair(WD_58ICECITY_BOSS, INTERFACES::WELCOME_RAKLIONBOSS));

		_WorldMap.insert(std::make_pair(WD_62SANTA_TOWN, INTERFACES::WELCOME_SANTATOWN));
		_WorldMap.insert(std::make_pair(WD_63PK_FIELD, INTERFACES::WELCOME_PKFIELD));
		_WorldMap.insert(std::make_pair(WD_64DUELARENA, INTERFACES::WELCOME_DUELARENA));

		_WorldMap.insert(std::make_pair(WD_65DOPPLEGANGER1, INTERFACES::WELCOME_DOPPELGANGER));
		_WorldMap.insert(std::make_pair(WD_66DOPPLEGANGER2, INTERFACES::WELCOME_DOPPELGANGER));
		_WorldMap.insert(std::make_pair(WD_67DOPPLEGANGER3, INTERFACES::WELCOME_DOPPELGANGER));
		_WorldMap.insert(std::make_pair(WD_68DOPPLEGANGER4, INTERFACES::WELCOME_DOPPELGANGER));

		_WorldMap.insert(std::make_pair(WD_69EMPIREGUARDIAN1, INTERFACES::WELCOME_EMPIREGUARDIAN));
		_WorldMap.insert(std::make_pair(WD_70EMPIREGUARDIAN2, INTERFACES::WELCOME_EMPIREGUARDIAN));
		_WorldMap.insert(std::make_pair(WD_71EMPIREGUARDIAN3, INTERFACES::WELCOME_EMPIREGUARDIAN));
		_WorldMap.insert(std::make_pair(WD_72EMPIREGUARDIAN4, INTERFACES::WELCOME_EMPIREGUARDIAN));

		_WorldMap.insert(std::make_pair(WD_79UNITEDMARKETPLACE, INTERFACES::WELCOME_LORENMARKET));
	}
}

UIMapName::~UIMapName()
{
}

void UIMapName::Update(mu_uint64 CurrentTime)
{
	mu_float alpha = IMAGE_ALPHA * (MU_ELAPSEDTIME / GAME_FPS);

	switch (_State)
	{
	case ST_FADEIN:
		{
			_Alpha += alpha;

			if (_Alpha >= 1.0f)
			{
				_State = ST_SHOW;
				_Alpha = 1.0f;
				_DeltaTime = 0.0f;
			}
		}
		break;

	case ST_FADEOUT:
		{
			_Alpha -= alpha;

			if (_Alpha <= 0.0f)
			{
				_State = ST_HIDE;
				_Alpha = 0.0f;
				MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}
		}
		break;

	case ST_SHOW:
		{
			_DeltaTime += MU_ELAPSEDTIME;
			if (_DeltaTime >= IMAGE_TIME)
			{
				_State = ST_FADEOUT;
			}
		}
		break;
	}

	_Logo->SetColor(MU::Float4ToARGB(1.0f, 1.0f, 1.0f, _Alpha));
}

void UIMapName::PostDraw()
{
	if (_Alpha > 0.0f)
	{
		_Logo->Draw();
	}
}

UIBase *UIMapName::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return nullptr;
}

void UIMapName::Show()
{
	if (GET_WORLD == WD_40AREA_FOR_GM ||
		GET_WORLD == _World)
	{
		return;
	}

	_Strife = IsStrifeMap(GET_WORLD);

	if (_Strife == true)
	{
		_Logo->SetTexture(&INTERFACES::Data[INTERFACES::WELCOME_STRIFE]);
		UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
	else
	{
		auto it = _WorldMap.find(GET_WORLD);

		if (it != _WorldMap.end())
		{
			_Logo->SetTexture(&INTERFACES::Data[it->second]);
			UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
	}

	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
	{
		_DeltaTime = 0.0f;
		_State = ST_FADEIN;
		_World = GET_WORLD;
		_Alpha = 0.0f;
	}
}