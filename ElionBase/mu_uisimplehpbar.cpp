#include "stdafx.h"
#include "mu_uisimplehpbar.h"

#define SCREEN_WIDTH (640.0f)
#define SCREEN_HEIGHT (480.0f)

#define HPBACK_WIDTH (50.0f)
#define HPBACK_HEIGHT (8.0f)
#define HPBAR_WIDTH (46.0f)
#define HPBAR_HEIGHT (4.0f)
#define HPBAR_EXTRAX (2.0f)
#define HPBAR_EXTRAY (2.0f)

UISimpleHPBar::UISimpleHPBar(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;
	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initialize
	{
		for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
		{
			_HPBack[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_HPBAR_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, HPBACK_WIDTH, HPBACK_HEIGHT);
			_HPBack[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_HPBack[n]->SetColor(MU::MakeARGB(255, 255, 255, 128));

			_HPBar[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_HPBAR_COLOR], SCREEN_WIDTH, SCREEN_HEIGHT, 2.0f, 2.0f, HPBAR_WIDTH, HPBAR_HEIGHT);
			_HPBar[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_HPBar[n]->SetColor(MU::MakeARGB(255, 255, 255, 128));
		}
	}
}

UISimpleHPBar::~UISimpleHPBar()
{

}

void UISimpleHPBar::Update(mu_uint64 CurrentTime)
{

}

void UISimpleHPBar::Draw()
{
	if (PartyNumber > 0)
	{
		mu_uint32 textColor = MU::MakeARGB(255, 230, 210, 255);
		mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 128);

		mu_float limitBottom = (MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height) - MU_TO_WINDOW(HPBACK_HEIGHT, MU_RESOURCE->_640x480Rate[1]);

		EVector4 Limits = EVector4(MU_VIDEO->_3DViewport.OffsetX,
			MU_VIDEO->_3DViewport.OffsetY,
			MU_VIDEO->_3DViewport.OffsetX + MU_VIDEO->_3DViewport.Width,
			MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height);

		mu_int32 mouseX = MU_INPUT->GetMouseX();
		mu_int32 mouseY = MU_INPUT->GetMouseY();

		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			PARTY_t &member = Party[n];

			if (member.index <= -1) continue;

			CHARACTER *c = &CharactersClient[member.index];
			OBJECT *o = &c->Object;

			EVector3 Position = o->Position + EVector3(0.0f, 0.0f, o->BoundingBoxMax.z + 100.0f);

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

			mu_float posy = MU_TO_VIEWPORT(Projected.y, _AspectHeight);
			mu_float backx = MU_TO_VIEWPORT(Projected.x, _AspectWidth);
			mu_float colorx = backx;

			if (backx + HPBACK_WIDTH * 0.5f > SCREEN_WIDTH)
			{
				backx -= HPBACK_WIDTH;
			}
			else if (backx < HPBACK_WIDTH * 0.5f)
			{
				backx = 0.0f;
			}
			else
			{
				backx -= HPBACK_WIDTH * 0.5f;
			}

			colorx = backx + HPBAR_EXTRAX;

			_HPBack[n]->SetPosition(backx, posy);
			_HPBar[n]->SetPosition(colorx, posy + HPBAR_EXTRAY);

			_HPBack[n]->Draw();

			mu_float hpPercent = member.maxHP > 0.0f ? member.currHP / member.maxHP : 0.0f;
			if (hpPercent > 1.0f) hpPercent = 1.0f;
			
			if (hpPercent > 0.0f)
			{
				_HPBar[n]->SetDimensions(HPBAR_WIDTH * hpPercent, HPBAR_HEIGHT);
				_HPBar[n]->Draw();
			}

			if (_HPBack[n]->IsInside(mouseX, mouseY))
			{
				mu_float texty = posy;

				if (texty >= 6.0f)
				{
					texty -= 6.0f;
				}
				else
				{
					texty = 0.0f;
				}

				mu_text hpInfo[256];

				mu_sprintf_s(hpInfo, mu_countof(hpInfo), _T("HP : %d/%d"), (mu_int32)member.currHP, (mu_int32)member.maxHP);

				FONT_NORMAL->DrawStringC2D(TEXTINDEX::SIMPLEHPBAR_LIFE_BEGIN + n,
					MU_TO_WINDOW(backx + HPBACK_WIDTH * 0.5f, _AspectWidth),
					MU_TO_WINDOW(texty, _AspectHeight),
					1.0f,
					ETA_CENTER, ETA_BOTTOM, 0.0f,
					textColor, backColor,
					hpInfo);
			}
		}
	}
}