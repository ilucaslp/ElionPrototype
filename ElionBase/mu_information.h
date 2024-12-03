#ifndef __MU_INFORMATION_H__
#define __MU_INFORMATION_H__

#pragma once

#define MAX_FILTERS 1000
#define MAX_NAMEFILTERS 500

extern mu_boolean EnableGuildWar;
extern mu_int32 GuildWarIndex;
extern mu_text GuildWarName[16 + 1];
extern mu_int32 GuildWarScore[2];

extern mu_boolean EnableSoccer;
extern mu_uint8 HeroSoccerTeam;
extern mu_int32 SoccerTime;
extern mu_text SoccerTeamName[2][16 + 1];
extern mu_boolean SoccerObserver;

extern std::vector<mu_string> AbuseFilter;
extern std::vector<mu_string> AbuseNameFilter;

mu_boolean IsStrifeMap(mu_int32 nMapIndex);

void GetSkillInformation(mu_int32 iType, mu_int32 iLevel, mu_text *lpszName, const mu_size NameLength, mu_int32 *piMana, mu_int32 *piDistance, mu_int32 *piSkillMana = nullptr);
void GetSkillInformation_Energy(mu_int32 iType, mu_int32 *piEnergy);
void GetSkillInformation_Charisma(mu_int32 iType, mu_int32 *piCharisma);
void GetSkillInformation_Damage(mu_int32 iType, mu_int32 *piDamage);
mu_float GetSkillDistance(mu_int32 Index, CHARACTER* c = nullptr);
mu_boolean CheckSkillDelay(mu_int32 SkillIndex);
void CalcSkillDelay(mu_int32 time);
mu_boolean IsGuildMaster();
mu_boolean IsSubGuildMaster();
mu_boolean IsBattleMaster();

extern mu_boolean g_bTimeCheck;
extern mu_uint64 g_iBackupTime;

mu_boolean GetTimeCheck(mu_uint64 DelayTime);

mu_boolean OpenFilterFile(const mu_text *filename);
mu_boolean OpenNameFilterFile(const mu_text *filename);

#endif