#include "stdafx.h"
#include "mu_buff.h"

void GetTokenBufflist(std::list<eBuffState>& outtokenbufflist, const eBuffState curbufftype)
{
	if (curbufftype >= eBuff_CastleRegimentDefense && curbufftype <= eBuff_CastleRegimentAttack3)
	{
		outtokenbufflist.push_back(eBuff_CastleRegimentDefense); outtokenbufflist.push_back(eBuff_CastleRegimentAttack1);
		outtokenbufflist.push_back(eBuff_CastleRegimentAttack2); outtokenbufflist.push_back(eBuff_CastleRegimentAttack3);
	}

	if (curbufftype >= eBuff_CrywolfAltarEnable && curbufftype <= eBuff_CrywolfNPCHide)
	{
		outtokenbufflist.push_back(eBuff_CrywolfAltarEnable); outtokenbufflist.push_back(eBuff_CrywolfAltarDisable);
		outtokenbufflist.push_back(eBuff_CrywolfAltarContracted); outtokenbufflist.push_back(eBuff_CrywolfAltarAttempt);
		outtokenbufflist.push_back(eBuff_CrywolfAltarOccufied); outtokenbufflist.push_back(eBuff_CrywolfHeroContracted);
		outtokenbufflist.push_back(eBuff_CrywolfNPCHide);
	}

	if ((curbufftype >= eBuff_PcRoomSeal1 && curbufftype <= eBuff_PcRoomSeal3) || curbufftype == eBuff_NewWealthSeal)
	{
		outtokenbufflist.push_back(eBuff_NewWealthSeal);
		outtokenbufflist.push_back(eBuff_PcRoomSeal1); outtokenbufflist.push_back(eBuff_PcRoomSeal2);
		outtokenbufflist.push_back(eBuff_PcRoomSeal3);
	}

	if ((curbufftype >= eBuff_Seal1 && curbufftype <= eBuff_Seal4)
		|| curbufftype == eBuff_AscensionSealMaster || curbufftype == eBuff_WealthSealMaster)
	{
		outtokenbufflist.push_back(eBuff_Seal1);
		outtokenbufflist.push_back(eBuff_Seal2);
		outtokenbufflist.push_back(eBuff_Seal3);
		outtokenbufflist.push_back(eBuff_Seal4);
		outtokenbufflist.push_back(eBuff_Seal_HpRecovery);
		outtokenbufflist.push_back(eBuff_Seal_MpRecovery);
		outtokenbufflist.push_back(eBuff_AscensionSealMaster);
		outtokenbufflist.push_back(eBuff_WealthSealMaster);
	}
	if (curbufftype >= eBuff_EliteScroll1 && curbufftype <= eBuff_EliteScroll6)
	{
		outtokenbufflist.push_back(eBuff_EliteScroll1); outtokenbufflist.push_back(eBuff_EliteScroll2);
		outtokenbufflist.push_back(eBuff_EliteScroll3); outtokenbufflist.push_back(eBuff_EliteScroll4);
		outtokenbufflist.push_back(eBuff_EliteScroll5); outtokenbufflist.push_back(eBuff_EliteScroll6);
		outtokenbufflist.push_back(eBuff_Scroll_Battle);
		outtokenbufflist.push_back(eBuff_Scroll_Strengthen);
	}
	if (curbufftype >= eBuff_SecretPotion1 && curbufftype <= eBuff_SecretPotion5)
	{
		outtokenbufflist.push_back(eBuff_SecretPotion1); outtokenbufflist.push_back(eBuff_SecretPotion2);
		outtokenbufflist.push_back(eBuff_SecretPotion3); outtokenbufflist.push_back(eBuff_SecretPotion4);
		outtokenbufflist.push_back(eBuff_SecretPotion5);
	}
}

MUBuffPtr MUBuff::Make()
{
	MUBuffPtr buff(new MUBuff());
	return buff;
}

MUBuff::MUBuff()
{

}

MUBuff::~MUBuff()
{
	ClearBuff();
}

mu_boolean MUBuff::isBuff()
{
	return m_Buff.size() != 0;
}

mu_boolean MUBuff::isBuff(eBuffState buffstate)
{
	if (!isBuff()) return false;

	MUBuffStateMap::iterator iter = m_Buff.find(buffstate);

	if (iter != m_Buff.end())
	{
		return true;
	}

	return false;
}

const eBuffState MUBuff::isBuff(std::list<eBuffState> buffstatelist)
{
	if (!isBuff()) return eBuffNone;

	for (std::list<eBuffState>::iterator iter = buffstatelist.begin();
		iter != buffstatelist.end(); )
	{
		std::list<eBuffState>::iterator Tempiter = iter;
		++iter;
		eBuffState tempbufftype = (*Tempiter);

		if (isBuff(tempbufftype)) return tempbufftype;
	}

	return eBuffNone;
}

void MUBuff::TokenBuff(eBuffState curbufftype)
{
	std::list<eBuffState> tokenbufflist;

	GetTokenBufflist(tokenbufflist, curbufftype);

	UnRegisterBuff(tokenbufflist);
	RegisterBuff(curbufftype);
}

const mu_uint32 MUBuff::GetBuffCount(eBuffState buffstate)
{
	mu_uint32 tempcount = 0;

	if (!isBuff()) return tempcount;

	MUBuffStateMap::iterator iter = m_Buff.find(buffstate);

	if (iter != m_Buff.end())
	{
		tempcount = (*iter).second;
		return tempcount;
	}

	return tempcount;
}

const mu_uint32 MUBuff::GetBuffSize()
{
	return m_Buff.size();
}

const eBuffState MUBuff::GetBuff(mu_int32 iterindex)
{
	if (iterindex >= (mu_int32)GetBuffSize()) return eBuffNone;

	mu_int32 i = 0;

	for (MUBuffStateMap::iterator iter = m_Buff.begin(); iter != m_Buff.end(); )
	{
		MUBuffStateMap::iterator tempiter = iter;
		++iter;

		if (i == iterindex)
		{
			return (*tempiter).first;
		}

		i += 1;
	}

	return eBuffNone;
}

void MUBuff::RegisterBuff(eBuffState buffstate)
{
	m_Buff.insert(std::make_pair(buffstate, 1));
	MUBuffStateMap::iterator iter = m_Buff.find(buffstate);

	if (iter == m_Buff.end())
	{
		m_Buff.insert(std::make_pair(buffstate, 1));
	}
}

void MUBuff::RegisterBuff(std::list<eBuffState> buffstate)
{
	for (std::list<eBuffState>::iterator iter = buffstate.begin(); iter != buffstate.end();++iter)
	{
		RegisterBuff(*iter);
	}
}

void MUBuff::UnRegisterBuff(eBuffState buffstate)
{
	if (!isBuff())
	{
		return;
	}

	m_Buff.erase(buffstate);
}

void MUBuff::UnRegisterBuff(std::list<eBuffState> buffstate)
{
	for (std::list<eBuffState>::iterator iter = buffstate.begin(); iter != buffstate.end();++iter)
	{
		UnRegisterBuff(*iter);
	}
}

void MUBuff::ClearBuff()
{
	m_Buff.clear();
}

mu_boolean CheckExceptionBuff(eBuffState buff, OBJECT* o, mu_boolean iserase)
{
	if (iserase)
	{
		switch (buff)
		{
		default: break;

		case eBuff_SoulPotion:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterUnRegisterBuff(o, eBuff_CrywolfAltarEnable);
					return false;
				}
			}
			break;

		case eBuff_RemovalMagic:
			{
				std::list<eBuffState> bufflist;

				//debuff
				bufflist.push_back(eDeBuff_Poison);
				bufflist.push_back(eDeBuff_Freeze);
				bufflist.push_back(eDeBuff_Harden);
				bufflist.push_back(eDeBuff_Defense);
				bufflist.push_back(eDeBuff_Stun);
				bufflist.push_back(eDeBuff_Sleep);
				bufflist.push_back(eDeBuff_BlowOfDestruction);

				//buff
				bufflist.push_back(eBuff_HpRecovery);
				bufflist.push_back(eBuff_Attack);
				bufflist.push_back(eBuff_Defense);
				bufflist.push_back(eBuff_AddAG);
				bufflist.push_back(eBuff_Cloaking);
				bufflist.push_back(eBuff_AddSkill);
				bufflist.push_back(eBuff_PhysDefense);
				bufflist.push_back(eBuff_AddCriticalDamage);
				bufflist.push_back(eBuff_CrywolfAltarOccufied);

				g_CharacterUnRegisterBuffList(o, bufflist);
			}
			break;
		}

		return true;
	}
	else
	{
		switch (buff)
		{
		default: break;

		case eBuff_CastleRegimentAttack3:
			{
				g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);
			}
			break;

		case eBuff_SoulPotion:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterRegisterBuff(o, eBuff_CrywolfAltarEnable);
					return false;
				}
			}
			break;

		case eBuff_CastleGateIsOpen:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterRegisterBuff(o, eBuff_CrywolfAltarDisable);
					return false;
				}
			}
			break;

		case eBuff_CastleRegimentDefense:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterRegisterBuff(o, eBuff_CrywolfAltarContracted);
					return false;
				}
				else
				{
					g_TokenCharacterBuff(o, eBuff_CastleRegimentDefense);
					return false;
				}
			}
			break;

		case eBuff_CastleRegimentAttack1:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterRegisterBuff(o, eBuff_CrywolfAltarAttempt);
					return false;
				}
				else if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);
				}
			}
			break;

		case eBuff_RemovalMagic:
			{
				if (o->Type >= MODEL_CRYWOLF_ALTAR1 && o->Type <= MODEL_CRYWOLF_ALTAR5)
				{
					g_CharacterRegisterBuff(o, eBuff_CrywolfAltarOccufied);
					return false;
				}
			}
			break;

		case eBuff_CastleRegimentAttack2:
			{
				if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
					g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);
			}
			break;
		}

		return true;
	}
}

void InsertBuffLogicalEffect(eBuffState buff, OBJECT* o, const mu_int32 bufftime)
{
	if (o && o == &Hero->Object)
	{
		switch (buff)
		{
		default: break;

		case eBuff_Hellowin1:
		case eBuff_Hellowin2:
		case eBuff_Hellowin3:
		case eBuff_Hellowin4:
		case eBuff_Hellowin5:
			{
				g_RegisterBuffTime(buff, bufftime);

				if (buff == eBuff_Hellowin1)
				{
					CharacterMachine->CalculateAttackSpeed();
				}
				if (buff == eBuff_Hellowin2)
				{
					CharacterMachine->CalculateDamage();
				}
				if (buff == eBuff_Hellowin3)
				{
					CharacterMachine->CalculateDefense();
				}
			}
			break;
		case eBuff_PcRoomSeal1:
		case eBuff_PcRoomSeal2:
		case eBuff_PcRoomSeal3:
			{
				g_RegisterBuffTime(buff, bufftime);
			}
			break;
		case eBuff_Seal1:
		case eBuff_Seal2:
		case eBuff_Seal3:
		case eBuff_Seal4:
		case eBuff_Seal_HpRecovery:
		case eBuff_Seal_MpRecovery:
		case eBuff_GuardCharm:
		case eBuff_ItemGuardCharm:
		case eBuff_AscensionSealMaster:
		case eBuff_WealthSealMaster:
		case eBuff_NewWealthSeal:
		case eBuff_AG_Addition:
		case eBuff_SD_Addition:
		case eBuff_PartyExpBonus:
			{
				g_RegisterBuffTime(buff, bufftime);
			}
			break;
		case eBuff_EliteScroll1:
		case eBuff_EliteScroll2:
		case eBuff_EliteScroll3:
		case eBuff_EliteScroll4:
		case eBuff_EliteScroll5:
		case eBuff_EliteScroll6:
		case eBuff_Scroll_Battle:
		case eBuff_Scroll_Strengthen:
		case eBuff_Scroll_Healing:
			{
				g_RegisterBuffTime(buff, bufftime);

				if (buff == eBuff_EliteScroll1)
				{
					CharacterMachine->CalculateAttackSpeed();
				}
				else if (buff == eBuff_EliteScroll2)
				{
					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_EliteScroll3)
				{
					CharacterMachine->CalculateDamage();
				}
				else if (buff == eBuff_EliteScroll4)
				{
					CharacterMachine->CalculateMagicDamage();
				}
			}
			break;
		case eBuff_SecretPotion1:
		case eBuff_SecretPotion2:
		case eBuff_SecretPotion3:
		case eBuff_SecretPotion4:
		case eBuff_SecretPotion5:
			{
				g_RegisterBuffTime(buff, bufftime);

				CharacterMachine->CalculateAll();
			}
			break;
		case eBuff_CherryBlossom_Liguor:
		case eBuff_CherryBlossom_RiceCake:
		case eBuff_CherryBlossom_Petal:
			{
				g_RegisterBuffTime(buff, bufftime);

				if (buff == eBuff_CherryBlossom_Petal)
				{
					CharacterMachine->CalculateDamage();
				}
			}
			break;
		case eBuff_SwellOfMagicPower:
			{
				g_RegisterBuffTime(buff, bufftime);

				if (buff == eBuff_SwellOfMagicPower)
				{
					CharacterMachine->CalculateMagicDamage();
				}
			}
			break;
		case eBuff_BlessingOfXmax:
		case eBuff_CureOfSanta:
		case eBuff_SafeGuardOfSanta:
		case eBuff_StrengthOfSanta:
		case eBuff_DefenseOfSanta:
		case eBuff_QuickOfSanta:
		case eBuff_LuckOfSanta:
			{
				g_RegisterBuffTime(buff, bufftime);

				mu_text _Temp[256] = { 0, };

				if (buff == eBuff_BlessingOfXmax)
				{
					g_pChatListBox->AddText(_T(""), GlobalText[2591], TYPE_SYSTEM_MESSAGE);

					CharacterMachine->CalculateDamage();
					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_StrengthOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2594], 30);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);

					CharacterMachine->CalculateDamage();
				}
				else if (buff == eBuff_DefenseOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2595], 100);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);

					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_QuickOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2598], 15);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);

					CharacterMachine->CalculateAttackSpeed();
				}
				else if (buff == eBuff_LuckOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2599], 10);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);
				}
				else if (buff == eBuff_CureOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2592], 500);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);
				}
				else if (buff == eBuff_SafeGuardOfSanta)
				{
					mu_sprintf_s(_Temp, mu_countof(_Temp), GlobalText[2593], 500);
					g_pChatListBox->AddText(_T(""), _Temp, TYPE_SYSTEM_MESSAGE);
				}
			}
			break;
		case eBuff_DuelWatch:
			{
				g_pNewUISystem->HideAll();

				g_pNewUISystem->Hide(GAMEUI::INTERFACE_MAINFRAME);
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_BUFF_WINDOW);
				g_pNewUISystem->Show(GAMEUI::INTERFACE_DUELWATCH_MAINFRAME);
				g_pNewUISystem->Show(GAMEUI::INTERFACE_DUELWATCH_USERLIST);
			}
			break;
		case eBuff_HonorOfGladiator:
			{
				g_RegisterBuffTime(buff, bufftime);
			}
			break;
		}
	}
}

void ClearBuffLogicalEffect(eBuffState buff, OBJECT* o)
{
	if (o && o == &Hero->Object)
	{
		switch (buff)
		{
		default: break;

		case eBuff_Hellowin1:
		case eBuff_Hellowin2:
		case eBuff_Hellowin3:
		case eBuff_Hellowin4:
		case eBuff_Hellowin5:
			{
				g_UnRegisterBuffTime(buff);

				if (buff == eBuff_Hellowin1)
				{
					CharacterMachine->CalculateAttackSpeed();
				}
				else if (buff == eBuff_Hellowin2)
				{
					mu_int32 iBaseClass = GetBaseClass(Hero->Class);

					if (iBaseClass == GAME::CLASS_WIZARD || iBaseClass == GAME::CLASS_DARK || iBaseClass == GAME::CLASS_SUMMONER)
					{
						CharacterMachine->CalculateMagicDamage();
					}
					else
					{
						CharacterMachine->CalculateDamage();
					}
				}
				else if (buff == eBuff_Hellowin3)
				{
					CharacterMachine->CalculateDefense();
				}
			}
			break;
		case eBuff_PcRoomSeal1:
		case eBuff_PcRoomSeal2:
		case eBuff_PcRoomSeal3:
			{
				g_UnRegisterBuffTime(buff);
			}
			break;
		case eBuff_Seal1:
		case eBuff_Seal2:
		case eBuff_Seal3:
		case eBuff_Seal4:
		case eBuff_Seal_HpRecovery:
		case eBuff_Seal_MpRecovery:
		case eBuff_GuardCharm:
		case eBuff_ItemGuardCharm:
		case eBuff_AscensionSealMaster:
		case eBuff_WealthSealMaster:
		case eBuff_NewWealthSeal:
		case eBuff_AG_Addition:
		case eBuff_SD_Addition:
		case eBuff_PartyExpBonus:
			{
				g_UnRegisterBuffTime(buff);
			}
			break;
		case eBuff_EliteScroll1:
		case eBuff_EliteScroll2:
		case eBuff_EliteScroll3:
		case eBuff_EliteScroll4:
		case eBuff_EliteScroll5:
		case eBuff_EliteScroll6:
		case eBuff_Scroll_Battle:
		case eBuff_Scroll_Strengthen:
		case eBuff_Scroll_Healing:
			{
				g_UnRegisterBuffTime(buff);

				if (buff == eBuff_EliteScroll1)
				{
					CharacterMachine->CalculateAttackSpeed();
				}
				else if (buff == eBuff_EliteScroll2)
				{
					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_EliteScroll3)
				{
					CharacterMachine->CalculateDamage();
				}
				else if (buff == eBuff_EliteScroll4)
				{
					CharacterMachine->CalculateMagicDamage();
				}
			}
			break;
		case eBuff_SecretPotion1:
		case eBuff_SecretPotion2:
		case eBuff_SecretPotion3:
		case eBuff_SecretPotion4:
		case eBuff_SecretPotion5:
			{
				g_UnRegisterBuffTime(buff);

				CharacterMachine->CalculateAll();
			}
			break;
		case eBuff_CherryBlossom_Liguor:
		case eBuff_CherryBlossom_RiceCake:
		case eBuff_CherryBlossom_Petal:
			{
				g_UnRegisterBuffTime(buff);

				if (buff == eBuff_CherryBlossom_Petal)
				{
					CharacterMachine->CalculateDamage();
				}
			}
			break;
		case eBuff_SwellOfMagicPower:
			{
				g_UnRegisterBuffTime(buff);

				CharacterMachine->CalculateMagicDamage();
			}
			break;
		case eBuff_BlessingOfXmax:
		case eBuff_CureOfSanta:
		case eBuff_SafeGuardOfSanta:
		case eBuff_StrengthOfSanta:
		case eBuff_DefenseOfSanta:
		case eBuff_QuickOfSanta:
		case eBuff_LuckOfSanta:
			{
				g_UnRegisterBuffTime(buff);

				if (buff == eBuff_BlessingOfXmax)
				{
					CharacterMachine->CalculateDamage();
					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_StrengthOfSanta)
				{
					CharacterMachine->CalculateDamage();
				}
				else if (buff == eBuff_DefenseOfSanta)
				{
					CharacterMachine->CalculateDefense();
				}
				else if (buff == eBuff_QuickOfSanta)
				{
					CharacterMachine->CalculateAttackSpeed();
				}
			}
			break;
		case eBuff_DuelWatch:
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_DUELWATCH_MAINFRAME);
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_DUELWATCH_USERLIST);
				g_pNewUISystem->Show(GAMEUI::INTERFACE_MAINFRAME);
				g_pNewUISystem->Show(GAMEUI::INTERFACE_BUFF_WINDOW);
			}
			break;
		case eBuff_HonorOfGladiator:
			{
				g_UnRegisterBuffTime(buff);
			}
			break;
		}
	}
}

void InsertBuffPhysicalEffect(const mu_uint32 ThreadIndex, eBuffState buff, OBJECT* o)
{
	switch (buff)
	{
	default: break;

	case eBuff_CursedTempleProdection:
		{
			if (o->Type == MODEL_PLAYER)
			{
				DeleteEffect(MODEL_CURSEDTEMPLE_PRODECTION_SKILL, o);
				DeleteEffect(MODEL_SHIELD_CRASH, o);
				DeleteEffect(BITMAP_SHOCK_WAVE, o);
				EVector3  Light;
				Vector(0.3f, 0.3f, 0.8f, Light);
				CreateEffect(ThreadIndex, MODEL_CURSEDTEMPLE_PRODECTION_SKILL, o->Position, o->Angle, Light, 0, o);
				CreateEffect(ThreadIndex, MODEL_SHIELD_CRASH, o->Position, o->Angle, Light, 1, o);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 10, o);
			}
		}
		break;

	case eBuff_PhysDefense:
		{
			if (o->Type == MODEL_PLAYER)
			{
				PlayBackground(SOUND_SOULBARRIER);
				DeleteJoint(MODEL_SPEARSKILL, o, 0);
				for (mu_int32 j = 0; j < 5; ++j)
				{
					CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 0, o, 50.0f);
				}
			}
		}
		break;

	case eBuff_AddAG:
		{
			DeleteEffect(BITMAP_LIGHT, o, 2);
			CreateEffect(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, o->Light, 2, o);
		}
		break;

	case eBuff_HpRecovery:
		{
			DeleteEffect(BITMAP_LIGHT, o, 1);
			CreateEffect(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, o->Light, 1, o);
		}
		break;
	case eDeBuff_Harden:
		{
			EVector3 Angle;
			VectorCopy(o->Angle, Angle);

			DeleteEffect(MODEL_ICE, o, 1);
			CreateEffect(ThreadIndex, MODEL_ICE, o->Position, Angle, o->Light, 1, o);

			Angle[2] += 180.0f;
			CreateEffect(ThreadIndex, MODEL_ICE, o->Position, Angle, o->Light, 2, o);
		}
		break;

	case eDeBuff_Defense:
		{
			EVector3 Light = { 1.0f, 1.0f, 1.0f };
			DeleteEffect(BITMAP_SKULL, o, 0);
			CreateEffect(ThreadIndex, BITMAP_SKULL, o->Position, o->Angle, Light, 0, o);

			PlayObject(SOUND_BLOODATTACK, o);
		}
		break;

	case eDeBuff_Stun:
		{
			DeleteEffect(BITMAP_SKULL, o, 5);

			EVector3 Angle;
			EVector3 Position;
			VectorCopy(o->Position, Position);
			Vector(0.0f, 0.0f, 90.0f, Angle);
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 8, o, 30.0f);
			Position[2] -= 10.0f;
			Vector(0.0f, 0.0f, 180.0f, Angle);
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 8, o, 30.0f);
			Position[2] -= 10.0f;
			Vector(0.0f, 0.0f, 270.0f, Angle);
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 8, o, 30.0f);
		}
		break;
	case eDeBuff_NeilDOT:
		{
			g_SummonSystem.CreateDamageOfTimeEffect(ThreadIndex, AT_SKILL_SUMMON_REQUIEM, o);
		}
		break;
	case eDeBuff_SahamuttDOT:
		{
			g_SummonSystem.CreateDamageOfTimeEffect(ThreadIndex, AT_SKILL_SUMMON_EXPLOSION, o);
		}
		break;
	case eBuff_Thorns:
		{
			EVector3 vLight;
			Vector(0.9f, 0.6f, 0.1f, vLight);
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 43, o, 50.0f, 0, 0, 0, 0, &vLight);
		}
		break;

	case eDeBuff_Blind:
		{
			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);
			CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, o->Position, o->Angle, vLight, 3, o);
		}
		break;

	case eDeBuff_Sleep:
		{
			EVector3 vLight;
			Vector(0.7f, 0.1f, 0.9f, vLight);
			CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, o->Position, o->Angle, vLight, 4, o);
		}
		break;

	case eDeBuff_AttackDown:
		{
			EVector3 vLight;
			Vector(1.4f, 0.2f, 0.2f, vLight);
			CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, vLight, 1, o, -1, 0, 0, 0, 0.5f);
			CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, vLight, 1, o, -1, 0, 0, 0, 1.0f);
		}
		break;

	case eDeBuff_DefenseDown:
		{
			EVector3 vLight;
			Vector(0.25f, 1.0f, 0.7f, vLight);
			CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, vLight, 2, o, -1, 0, 0, 0, 0.5f);
			CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, vLight, 2, o, -1, 0, 0, 0, 1.0f);
		}
		break;

	case eBuff_Berserker:
		{
			EVector3 vLight[2];
			Vector(0.9f, 0.0f, 0.1f, vLight[0]);
			Vector(1.0f, 1.0f, 1.0f, vLight[1]);
			for (mu_int32 i = 0; i < 4; ++i)
			{
				DeleteEffect(BITMAP_ORORA, o, i);
				//CreateEffect(ThreadIndex, BITMAP_ORORA, o->Position, o->Angle, vLight[0], i, o);
				if (i == 2 || i == 3)
				{
					DeleteEffect(BITMAP_SPARK + 2, o, i);
					//CreateEffect(ThreadIndex, BITMAP_SPARK + 2, o->Position, o->Angle, vLight[1], i, o);
				}
			}
			DeleteEffect(BITMAP_LIGHT_MARKS, o);
			//CreateEffect(ThreadIndex, BITMAP_LIGHT_MARKS, o->Position, o->Angle, vLight[0], 0, o);
		}
		break;

	case eDeBuff_FlameStrikeDamage:
		{
			CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, o->Light, 2, o);
		}
		break;
	case eDeBuff_GiganticStormDamage:
		{
			DeleteEffect(BITMAP_JOINT_THUNDER, o, 0);
			CreateEffect(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Angle, o->Light, 0, o);
		}
		break;
	case eDeBuff_LightningShockDamage:
		{
			CreateEffect(ThreadIndex, MODEL_LIGHTNING_SHOCK, o->Position, o->Angle, o->Light, 2, o);
		}
		break;
	case eDeBuff_BlowOfDestruction:
		{
			CreateEffect(ThreadIndex, MODEL_ICE, o->Position, o->Angle, o->Light);
		}
		break;
	case eBuff_Doppelganger_Ascension:
		{
			PlayBackground(SOUND_SKILL_BERSERKER);
			CreateEffect(ThreadIndex, MODEL_CHANGE_UP_EFF, o->Position, o->Angle, o->Light, 2, o);
			CreateEffect(ThreadIndex, MODEL_CHANGE_UP_CYLINDER, o->Position, o->Angle, o->Light, 2, o);
			CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW2, o->Position, o->Angle, o->Light, 1, o);
		}
		break;
	case eBuff_SD_Addition:
		{
			DeleteEffect(MODEL_EFFECT_SD_AURA, o);
			CreateEffect(ThreadIndex, MODEL_EFFECT_SD_AURA, o->Position, o->Angle, o->Light, 0, o);
		}
		break;
	case eBuff_AG_Addition:
		{
			DeleteParticle(BITMAP_AG_ADDITION_EFFECT);

			for (mu_int32 i = 0; i < 5; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, o->Position, o->Angle, o->Light, 0, 1.0f, o);
				CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, o->Position, o->Angle, o->Light, 1, 1.0f, o);
				CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, o->Position, o->Angle, o->Light, 2, 1.0f, o);
			}
		}
		break;
	}
}

void ClearBuffPhysicalEffect(eBuffState buff, OBJECT* o)
{
	switch (buff)
	{
	default: break;

	case eBuff_CursedTempleProdection:
		{
			if (g_isCharacterBuff(o, eBuff_CursedTempleProdection))
			{
				DeleteEffect(MODEL_CURSEDTEMPLE_PRODECTION_SKILL, o);
			}
		}
		break;

	case eBuff_Defense:
		{
			DeleteJoint(MODEL_SPEARSKILL, o, 4);
			DeleteJoint(MODEL_SPEARSKILL, o, 9);
		}
		break;

	case eBuff_AddAG:
		{
			DeleteEffect(BITMAP_LIGHT, o, 2);
		}
		break;

	case eBuff_HpRecovery:
		{
			DeleteEffect(BITMAP_LIGHT, o, 1);
		}
		break;

	case eBuff_PhysDefense:
		{
			if (o->Type == MODEL_PLAYER)
			{
				DeleteJoint(MODEL_SPEARSKILL, o, 0);
			}
		}
		break;

	case eBuff_HelpNpc:
		{
			DeleteJoint(MODEL_SPEARSKILL, o, 4);
			DeleteJoint(MODEL_SPEARSKILL, o, 9);
		}
		break;

	case eBuff_RemovalMagic:
		{
			DeleteEffect(BITMAP_LIGHT, o, 2);
			DeleteEffect(BITMAP_LIGHT, o, 1);
			DeleteEffect(BITMAP_SKULL, o, 0);

			DeleteJoint(MODEL_SPEARSKILL, o, 4);
			DeleteJoint(MODEL_SPEARSKILL, o, 9);
			DeleteJoint(MODEL_SPEARSKILL, o, 0);
		}
		break;
	case eDeBuff_CursedTempleRestraint:
		{
			if (g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint))
			{
				DeleteEffect(MODEL_CURSEDTEMPLE_RESTRAINT_SKILL, o);
			}
		}
		break;

	case eDeBuff_Defense:
		{
			DeleteEffect(BITMAP_SKULL, o, 0);
		}
		break;

	case eDeBuff_Stun:
		{
			DeleteEffect(BITMAP_SKULL, o, 5);
		}
		break;

	case eDeBuff_NeilDOT:
		{
			g_SummonSystem.RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_REQUIEM, o);
		}
		break;

	case eDeBuff_SahamuttDOT:
		{
			g_SummonSystem.RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_EXPLOSION, o);
		}
		break;

	case eBuff_Thorns:
		{
			DeleteJoint(BITMAP_FLARE, o, 43);
		}
		break;

	case eDeBuff_Sleep:
		{
			DeleteEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 4);
		}
		break;

	case eDeBuff_Blind:
		{
			DeleteEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 3);
		}
		break;

	case eDeBuff_AttackDown:
		DeleteEffect(BITMAP_SHINY + 6, o, 1);
		DeleteEffect(BITMAP_PIN_LIGHT, o, 1);
		break;

	case eDeBuff_DefenseDown:
		DeleteEffect(BITMAP_SHINY + 6, o, 2);
		DeleteEffect(BITMAP_PIN_LIGHT, o, 2);
		break;

	case eBuff_Berserker:
		{
			for (mu_int32 i = 0; i < 4; ++i)
			{
				DeleteEffect(BITMAP_ORORA, o, i);
				if (i == 2 || i == 3)
					DeleteEffect(BITMAP_SPARK + 2, o, i);
			}
			DeleteEffect(BITMAP_LIGHT_MARKS, o);
		}
		break;

	case eDeBuff_FlameStrikeDamage:
		{
		}
		break;

	case eDeBuff_GiganticStormDamage:
		{
		}
		break;

	case eDeBuff_LightningShockDamage:
		{
		}
		break;

	case eDeBuff_BlowOfDestruction:
		{

		}
		break;

	case eBuff_SwellOfMagicPower:
		{
			DeleteEffect(MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF, o, 0);
		}break;

	case eBuff_Doppelganger_Ascension:
		{

		}
		break;

	case eBuff_SD_Addition:
		{
			DeleteEffect(MODEL_EFFECT_SD_AURA, o);
		}
		break;
	case eBuff_AG_Addition:
		{
			DeleteParticle(BITMAP_AG_ADDITION_EFFECT);
		}
		break;
	}
}

void RegisterBuff(const mu_uint32 ThreadIndex, eBuffState buff, mu_boolean addEffect, OBJECT* o, const mu_int32 bufftime)
{
	eBuffClass buffclasstype = g_IsBuffClass(buff);

	if (buffclasstype == eBuffClass_Count) return;

// 	if (InChaosCastle() && (o && o != &Hero->Object)) // FIX this shouldn't exists...
// 	{
// 		return;
// 	}

	if (addEffect == true)
	{
		InsertBuffPhysicalEffect(ThreadIndex, buff, o);
	}

	if (CheckExceptionBuff(buff, o, false))
	{
		g_CharacterRegisterBuff(o, buff);
		InsertBuffLogicalEffect(buff, o, bufftime);
	}
}

void UnRegisterBuff(eBuffState buff, OBJECT* o)
{
	eBuffClass buffclasstype = g_IsBuffClass(buff);

	if (buffclasstype == eBuffClass_Count) return;

	ClearBuffPhysicalEffect(buff, o);

	if (CheckExceptionBuff(buff, o, true))
	{
		g_CharacterUnRegisterBuff(o, buff);
		ClearBuffLogicalEffect(buff, o);
	}
}