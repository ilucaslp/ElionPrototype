#include "stdafx.h"
#include "mu_soundlist.h"

namespace SOUNDS
{
	const mu_uint8 Type = MULoadingType::LOADING_SOUND;
	const mu_uint32 Counter = LOADING_COUNTER::CT_SOUNDS;
	MUWavData Data[MAX_SOUNDS];

	MULoadingData Load[] =
	{
		SOUND_LOAD(SOUND_WIND01, _T("./data/sound/awind.wav"), 1, false)
		SOUND_LOAD(SOUND_RAIN01, _T("./data/sound/arain.wav"), 1, false)
		SOUND_LOAD(SOUND_FOREST01, _T("./data/sound/aforest.wav"), 1, false)
		SOUND_LOAD(SOUND_DUNGEON01, _T("./data/sound/adungeon.wav"), 1, false)
		SOUND_LOAD(SOUND_TITLE01, _T("./data/sound/ititle.wav"), 1, false)
		SOUND_LOAD(SOUND_TOWER01, _T("./data/sound/atower.wav"), 1, false)
		SOUND_LOAD(SOUND_WATER01, _T("./data/sound/awater.wav"), 1, false)
		SOUND_LOAD(SOUND_DESERT01, _T("./data/sound/desert.wav"), 1, false)

		SOUND_LOAD(SOUND_HUMAN_WALK_GROUND, _T("./data/sound/pwalk(soil).wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_WALK_GRASS, _T("./data/sound/pwalk(grass).wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_WALK_SNOW, _T("./data/sound/pwalk(snow).wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_WALK_SWIM, _T("./data/sound/pswim.wav"), 2, false)

		SOUND_LOAD(SOUND_BIRD01, _T("./data/sound/abird1.wav"), 1, true)
		SOUND_LOAD(SOUND_BIRD02, _T("./data/sound/abird2.wav"), 1, true)
		SOUND_LOAD(SOUND_BAT01, _T("./data/sound/abat.wav"), 1, true)
		SOUND_LOAD(SOUND_RAT01, _T("./data/sound/amouse.wav"), 1, true)
		SOUND_LOAD(SOUND_TRAP01, _T("./data/sound/agrate.wav"), 1, true)
		SOUND_LOAD(SOUND_DOOR01, _T("./data/sound/adoor.wav"), 1, false)
		SOUND_LOAD(SOUND_DOOR02, _T("./data/sound/acastledoor.wav"), 1, false)
		SOUND_LOAD(SOUND_ASSASSIN, _T("./data/sound/massassin1.wav"), 1, true)

		SOUND_LOAD(SOUND_HEAVEN01, _T("./data/sound/aheaven.wav"), 1, false)
		SOUND_LOAD(SOUND_THUNDERS01, _T("./data/sound/athunder01.wav"), 1, false)
		SOUND_LOAD(SOUND_THUNDERS02, _T("./data/sound/athunder02.wav"), 1, false)
		SOUND_LOAD(SOUND_THUNDERS03, _T("./data/sound/athunder03.wav"), 1, false)

		SOUND_LOAD(SOUND_CLICK01, _T("./data/sound/ibuttonclick.wav"), 1, false)
		SOUND_LOAD(SOUND_ERROR01, _T("./data/sound/ibuttonerror.wav"), 1, false)
		SOUND_LOAD(SOUND_MENU01, _T("./data/sound/ibuttonmove.wav"), 2, false)
		SOUND_LOAD(SOUND_INTERFACE01, _T("./data/sound/icreatewindow.wav"), 1, false)
		SOUND_LOAD(SOUND_GET_ITEM01, _T("./data/sound/pgetitem.wav"), 1, false)
		SOUND_LOAD(SOUND_DROP_ITEM01, _T("./data/sound/pdropitem.wav"), 1, false)
		SOUND_LOAD(SOUND_DROP_GOLD01, _T("./data/sound/pdropmoney.wav"), 1, false)
		SOUND_LOAD(SOUND_DRINK01, _T("./data/sound/pdrink.wav"), 1, false)
		SOUND_LOAD(SOUND_EAT_APPLE01, _T("./data/sound/peatapple.wav"), 1, false)
		SOUND_LOAD(SOUND_HEART, _T("./data/sound/pheartbeat.wav"), 1, false)
		SOUND_LOAD(SOUND_GET_ENERGY, _T("./data/sound/penergy.wav"), 1, false)
		SOUND_LOAD(SOUND_REPAIR, _T("./data/sound/irepair.wav"), 1, false)
		SOUND_LOAD(SOUND_WHISPER, _T("./data/sound/iwhisper.wav"), 1, false)
		SOUND_LOAD(SOUND_FRIEND_CHAT_ALERT, _T("./data/sound/ifmsgalert.wav"), 1, false)
		SOUND_LOAD(SOUND_FRIEND_MAIL_ALERT, _T("./data/sound/ifmailalert.wav"), 1, false)
		SOUND_LOAD(SOUND_FRIEND_LOGIN_ALERT, _T("./data/sound/ifloginalert.wav"), 1, false)

		SOUND_LOAD(SOUND_BRANDISH_SWORD01, _T("./data/sound/eswingweapon1.wav"), 2, false)
		SOUND_LOAD(SOUND_BRANDISH_SWORD02, _T("./data/sound/eswingweapon2.wav"), 2, false)
		SOUND_LOAD(SOUND_BRANDISH_SWORD03, _T("./data/sound/eswinglightsword.wav"), 2, false)
		SOUND_LOAD(SOUND_EXPLOTION01, _T("./data/sound/eexplosion.wav"), 1, true)
		SOUND_LOAD(SOUND_METEORITE01, _T("./data/sound/emeteorite.wav"), 2, true)
		SOUND_LOAD(SOUND_BREAK01, _T("./data/sound/ebreak.wav"), 1, true)
		SOUND_LOAD(SOUND_JEWEL01, _T("./data/sound/egem.wav"), 1, false)
		SOUND_LOAD(SOUND_ATTACK01, _T("./data/sound/emeleehit1.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK02, _T("./data/sound/emeleehit2.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK03, _T("./data/sound/emeleehit3.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK04, _T("./data/sound/emeleehit4.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK05, _T("./data/sound/emeleehit5.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK06, _T("./data/sound/emissilehit1.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK07, _T("./data/sound/emissilehit2.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK08, _T("./data/sound/emissilehit3.wav"), 2, false)
		SOUND_LOAD(SOUND_ATTACK09, _T("./data/sound/emissilehit4.wav"), 2, false)

		SOUND_LOAD(SOUND_THUNDER01, _T("./data/sound/ethunder.wav"), 1, true)
		SOUND_LOAD(SOUND_BOW01, _T("./data/sound/ebow.wav"), 2, false)
		SOUND_LOAD(SOUND_CROSSBOW01, _T("./data/sound/ecrossbow.wav"), 2, false)
		SOUND_LOAD(SOUND_MIX01, _T("./data/sound/emix.wav"), 2, false)
		SOUND_LOAD(SOUND_FIRECRACKER1, _T("./data/sound/efirecracker1.wav"), 1, true)
		SOUND_LOAD(SOUND_FIRECRACKER2, _T("./data/sound/efirecracker2.wav"), 1, true)
		SOUND_LOAD(SOUND_MEDAL, _T("./data/sound/emedal.wav"), 1, true)
		SOUND_LOAD(SOUND_RING_EVENT_READY, _T("./data/sound/ievent3min.wav"), 1, false)
		SOUND_LOAD(SOUND_RING_EVENT_START, _T("./data/sound/ieventstart.wav"), 1, false)
		SOUND_LOAD(SOUND_RING_EVENT_END, _T("./data/sound/ieventend.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOS_MOB_BOOM01, _T("./data/sound/emonsterboom1.wav"), 2, false)
		SOUND_LOAD(SOUND_CHAOS_MOB_BOOM02, _T("./data/sound/emonsterboom2.wav"), 2, false)
		SOUND_LOAD(SOUND_CHAOS_FALLING, _T("./data/sound/pmalescream.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOS_FALLING_STONE, _T("./data/sound/ewallfall.wav"), 1, false)

		SOUND_LOAD(SOUND_CHANGE_UP, _T("./data/sound/nmalonskillmaster.wav"), 1, false)

		SOUND_LOAD(SOUND_PHOENIXEXP, _T("./data/sound/ephoenixexp.wav"), 1, true)

		SOUND_LOAD(SOUND_HUMAN_SCREAM01, _T("./data/sound/pmalescream1.wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_SCREAM02, _T("./data/sound/pmalescream2.wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_SCREAM03, _T("./data/sound/pmalescream3.wav"), 2, false)
		SOUND_LOAD(SOUND_HUMAN_SCREAM04, _T("./data/sound/pmaledie.wav"), 2, false)

		SOUND_LOAD(SOUND_FEMALE_SCREAM01, _T("./data/sound/pfemalescream1.wav"), 2, false)
		SOUND_LOAD(SOUND_FEMALE_SCREAM02, _T("./data/sound/pfemalescream2.wav"), 2, false)

		SOUND_LOAD(SOUND_SKILL_DEFENSE, _T("./data/sound/sknightdefense.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_SWORD1, _T("./data/sound/sknightskill1.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_SWORD2, _T("./data/sound/sknightskill2.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_SWORD3, _T("./data/sound/sknightskill3.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_SWORD4, _T("./data/sound/sknightskill4.wav"), 1, false)
		SOUND_LOAD(SOUND_STORM, _T("./data/sound/stornado.wav"), 2, true)
		SOUND_LOAD(SOUND_EVIL, _T("./data/sound/sevil.wav"), 2, true)
		SOUND_LOAD(SOUND_MAGIC, _T("./data/sound/smagic.wav"), 2, true)
		SOUND_LOAD(SOUND_HELLFIRE, _T("./data/sound/shellfire.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE, _T("./data/sound/sice.wav"), 2, true)
		SOUND_LOAD(SOUND_FLAME, _T("./data/sound/sflame.wav"), 2, true)
		SOUND_LOAD(SOUND_FLASH, _T("./data/sound/saquaflash.wav"), 2, true)
		SOUND_LOAD(SOUND_BONE1, _T("./data/sound/mbone1.wav"), 2, true)
		SOUND_LOAD(SOUND_BONE2, _T("./data/sound/mbone2.wav"), 2, true)

		SOUND_LOAD(SOUND_RIDINGSPEAR, _T("./data/sound/eridingspear.wav"), 1, false)
		SOUND_LOAD(SOUND_RAIDSHOOT, _T("./data/sound/eraidshoot.wav"), 1, false)
		SOUND_LOAD(SOUND_SWELLLIFE, _T("./data/sound/eswelllife.wav"), 1, false)
		SOUND_LOAD(SOUND_PIERCING, _T("./data/sound/epiercing.wav"), 1, false)
		SOUND_LOAD(SOUND_ICEARROW, _T("./data/sound/eicearrow.wav"), 1, false)
		SOUND_LOAD(SOUND_TELEKINESIS, _T("./data/sound/etelekinesis.wav"), 1, false)
		SOUND_LOAD(SOUND_SOULBARRIER, _T("./data/sound/esoulbarrier.wav"), 1, false)
		SOUND_LOAD(SOUND_BLOODATTACK, _T("./data/sound/ebloodattack.wav"), 1, false)

		SOUND_LOAD(SOUND_HIT_GATE, _T("./data/sound/ehitgate.wav"), 1, false)
		SOUND_LOAD(SOUND_HIT_GATE2, _T("./data/sound/ehitgate2.wav"), 1, false)
		SOUND_LOAD(SOUND_HIT_CRISTAL, _T("./data/sound/ehitcristal.wav"), 1, false)
		SOUND_LOAD(SOUND_DOWN_GATE, _T("./data/sound/edowngate.wav"), 1, false)
		SOUND_LOAD(SOUND_CROW, _T("./data/sound/ecrow.wav"), 1, false)
		SOUND_LOAD(SOUND_BLOODCASTLE, _T("./data/sound/ibloodcastle.wav"), 1, false)

		SOUND_LOAD(SOUND_OPEN_DUELWINDOW, _T("./data/sound/iduelwindow.wav"), 1, false)
		SOUND_LOAD(SOUND_START_DUEL, _T("./data/sound/iduelstart.wav"), 1, false)

		SOUND_LOAD(SOUND_DEATH_POISON1, _T("./data/sound/eblastpoison_1.wav"), 2, false)
		SOUND_LOAD(SOUND_DEATH_POISON2, _T("./data/sound/eblastpoison_2.wav"), 2, false)
		SOUND_LOAD(SOUND_SUDDEN_ICE1, _T("./data/sound/esuddenice_1.wav"), 2, false)
		SOUND_LOAD(SOUND_SUDDEN_ICE2, _T("./data/sound/esuddenice_2.wav"), 2, false)
		SOUND_LOAD(SOUND_NUKE1, _T("./data/sound/ehellfire2_1.wav"), 1, false)
		SOUND_LOAD(SOUND_NUKE2, _T("./data/sound/ehellfire2_2.wav"), 1, false)
		SOUND_LOAD(SOUND_COMBO, _T("./data/sound/ecombo.wav"), 1, false)
		SOUND_LOAD(SOUND_FURY_STRIKE1, _T("./data/sound/erageblow_1.wav"), 1, false)
		SOUND_LOAD(SOUND_FURY_STRIKE2, _T("./data/sound/erageblow_2.wav"), 1, false)
		SOUND_LOAD(SOUND_FURY_STRIKE3, _T("./data/sound/erageblow_3.wav"), 1, false)
		SOUND_LOAD(SOUND_LEVEL_UP, _T("./data/sound/plevelup.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOSCASTLE, _T("./data/sound/ichaoscastle.wav"), 1, false)

		SOUND_LOAD(SOUND_CHAOS_THUNDER01, _T("./data/sound/eelec1.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOS_THUNDER02, _T("./data/sound/eelec2.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOS_ENVIR, _T("./data/sound/achaos.wav"), 1, false)
		SOUND_LOAD(SOUND_CHAOS_END, _T("./data/sound/achaosend.wav"), 1, false)

		SOUND_LOAD(SOUND_BLACKSMITH, _T("./data/sound/nblacksmith.wav"), 1, false)
		SOUND_LOAD(SOUND_HARP, _T("./data/sound/nharp.wav"), 1, false)
		SOUND_LOAD(SOUND_GOBLIN_MIX, _T("./data/sound/nmix.wav"), 1, false)

		SOUND_LOAD(SOUND_BULL_ACTION1, _T("./data/sound/mbull1.wav"), 2, true)
		SOUND_LOAD(SOUND_BULL_ACTION2, _T("./data/sound/mbull2.wav"), 2, true)
		SOUND_LOAD(SOUND_BULL_ATTACK1, _T("./data/sound/mbullattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BULL_ATTACK2, _T("./data/sound/mbullattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BULL_DIE, _T("./data/sound/mbulldie.wav"), 2, true)

		SOUND_LOAD(SOUND_HOUND_ACTION1, _T("./data/sound/mhound1.wav"), 2, true)
		SOUND_LOAD(SOUND_HOUND_ACTION2, _T("./data/sound/mhound2.wav"), 2, true)
		SOUND_LOAD(SOUND_HOUND_ATTACK1, _T("./data/sound/mhoundattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_HOUND_ATTACK2, _T("./data/sound/mhoundattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_HOUND_DIE, _T("./data/sound/mhounddie.wav"), 2, true)

		SOUND_LOAD(SOUND_BUDGE_DRAGON_ACTION1, _T("./data/sound/mbudge1.wav"), 2, true)
		SOUND_LOAD(SOUND_BUDGE_DRAGON_ATTACK1, _T("./data/sound/mbudgeattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BUDGE_DRAGON_DIE, _T("./data/sound/mbudgedie.wav"), 2, true)

		SOUND_LOAD(SOUND_SPIDER_ACTION1, _T("./data/sound/mspider1.wav"), 2, true)

		SOUND_LOAD(SOUND_DARK_KNIGHT_ACTION1, _T("./data/sound/mdarkknight1.wav"), 2, true)
		SOUND_LOAD(SOUND_DARK_KNIGHT_ACTION2, _T("./data/sound/mdarkknight2.wav"), 2, true)
		SOUND_LOAD(SOUND_DARK_KNIGHT_ATTACK1, _T("./data/sound/mdarkknightattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_DARK_KNIGHT_ATTACK2, _T("./data/sound/mdarkknightattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_DARK_KNIGHT_DIE, _T("./data/sound/mdarkknightdie.wav"), 2, true)

		SOUND_LOAD(SOUND_LICH_ACTION1, _T("./data/sound/mwizard1.wav"), 2, true)
		SOUND_LOAD(SOUND_LICH_ACTION2, _T("./data/sound/mwizard2.wav"), 2, true)
		SOUND_LOAD(SOUND_LICH_ATTACK1, _T("./data/sound/mwizardattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_LICH_ATTACK2, _T("./data/sound/mwizardattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_LICH_DIE, _T("./data/sound/mwizarddie.wav"), 2, true)

		SOUND_LOAD(SOUND_GIANT_ACTION1, _T("./data/sound/mgiant1.wav"), 2, true)
		SOUND_LOAD(SOUND_GIANT_ACTION2, _T("./data/sound/mgiant2.wav"), 2, true)
		SOUND_LOAD(SOUND_GIANT_ATTACK1, _T("./data/sound/mgiantattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_GIANT_ATTACK2, _T("./data/sound/mgiantattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_GIANT_DIE, _T("./data/sound/mgiantdie.wav"), 2, true)

		SOUND_LOAD(SOUND_LARVA_ACTION1, _T("./data/sound/mlarva1.wav"), 2, true)
		SOUND_LOAD(SOUND_LARVA_ACTION2, _T("./data/sound/mlarva2.wav"), 2, true)

		SOUND_LOAD(SOUND_HELL_SPIDER_ACTION1, _T("./data/sound/mhellspider1.wav"), 2, true)
		SOUND_LOAD(SOUND_HELL_SPIDER_ATTACK1, _T("./data/sound/mhellspiderattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_HELL_SPIDER_DIE, _T("./data/sound/mhellspiderdie.wav"), 2, true)

		SOUND_LOAD(SOUND_GHOST_ACTION1, _T("./data/sound/mghost1.wav"), 2, true)
		SOUND_LOAD(SOUND_GHOST_ACTION2, _T("./data/sound/mghost2.wav"), 2, true)
		SOUND_LOAD(SOUND_GHOST_ATTACK1, _T("./data/sound/mghostattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_GHOST_ATTACK2, _T("./data/sound/mghostattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_GHOST_DIE, _T("./data/sound/mghostdie.wav"), 2, true)

		SOUND_LOAD(SOUND_CYCLOPS_ACTION1, _T("./data/sound/mogre1.wav"), 2, true)
		SOUND_LOAD(SOUND_CYCLOPS_ACTION2, _T("./data/sound/mogre2.wav"), 2, true)
		SOUND_LOAD(SOUND_CYCLOPS_ATTACK1, _T("./data/sound/mogreattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CYCLOPS_ATTACK2, _T("./data/sound/mogreattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CYCLOPS_DIE, _T("./data/sound/mogredie.wav"), 2, true)

		SOUND_LOAD(SOUND_GORGON_ACTION1, _T("./data/sound/mgorgon1.wav"), 2, true)
		SOUND_LOAD(SOUND_GORGON_ACTION2, _T("./data/sound/mgorgon2.wav"), 2, true)
		SOUND_LOAD(SOUND_GORGON_ATTACK1, _T("./data/sound/mgorgonattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_GORGON_ATTACK2, _T("./data/sound/mgorgonattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_GORGON_DIE, _T("./data/sound/mgorgondie.wav"), 2, true)

		SOUND_LOAD(SOUND_ICE_MONSTER_ACTION1, _T("./data/sound/micemonster1.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_MONSTER_ACTION2, _T("./data/sound/micemonster2.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_MONSTER_DIE, _T("./data/sound/micemonsterdie.wav"), 2, true)

		SOUND_LOAD(SOUND_WORM_ACTION1, _T("./data/sound/mworm1.wav"), 2, true)
		SOUND_LOAD(SOUND_WORM_DIE, _T("./data/sound/mwormdie.wav"), 2, true)

		SOUND_LOAD(SOUND_HOMMERD_ACTION1, _T("./data/sound/mhomord1.wav"), 2, true)
		SOUND_LOAD(SOUND_HOMMERD_ACTION2, _T("./data/sound/mhomord2.wav"), 2, true)
		SOUND_LOAD(SOUND_HOMMERD_ATTACK1, _T("./data/sound/mhomordattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_HOMMERD_DIE, _T("./data/sound/mhomorddie.wav"), 2, true)

		SOUND_LOAD(SOUND_ICE_QUEEN_ACTION1, _T("./data/sound/micequeen1.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_QUEEN_ACTION2, _T("./data/sound/micequeen2.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_QUEEN_ATTACK1, _T("./data/sound/micequeenattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_QUEEN_ATTACK2, _T("./data/sound/micequeenattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_ICE_QUEEN_DIE, _T("./data/sound/micequeendie.wav"), 2, true)

		SOUND_LOAD(SOUND_ASSASSIN_ATTACK1, _T("./data/sound/massassinattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_ASSASSIN_ATTACK2, _T("./data/sound/massassinattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_ASSASSIN_DIE, _T("./data/sound/massassindie.wav"), 2, true)

		SOUND_LOAD(SOUND_YETI_ACTION1, _T("./data/sound/myeti1.wav"), 2, true)
		SOUND_LOAD(SOUND_YETI_ACTION2, _T("./data/sound/myeti2.wav"), 2, true)
		SOUND_LOAD(SOUND_YETI_ATTACK1, _T("./data/sound/myetiattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_YETI_DIE, _T("./data/sound/myetidie.wav"), 2, true)

		SOUND_LOAD(SOUND_GOBLIN_ACTION1, _T("./data/sound/mgoblin1.wav"), 2, true)
		SOUND_LOAD(SOUND_GOBLIN_ACTION2, _T("./data/sound/mgoblin2.wav"), 2, true)
		SOUND_LOAD(SOUND_GOBLIN_ATTACK1, _T("./data/sound/mgoblinattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_GOBLIN_ATTACK2, _T("./data/sound/mgoblinattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_GOBLIN_DIE, _T("./data/sound/mgoblindie.wav"), 2, true)

		SOUND_LOAD(SOUND_SCORPION_ACTION1, _T("./data/sound/mscorpion1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCORPION_ACTION2, _T("./data/sound/mscorpion2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCORPION_ATTACK1, _T("./data/sound/mscorpionattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCORPION_ATTACK2, _T("./data/sound/mscorpionattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCORPION_DIE, _T("./data/sound/mscorpiondie.wav"), 2, true)

		SOUND_LOAD(SOUND_BEETLE_MONSTER_ACTION1, _T("./data/sound/mbeetle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BETTLE_MONSTER_ATTACK1, _T("./data/sound/mbeetleattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BETTLE_MONSTER_DIE, _T("./data/sound/mbeetledie.wav"), 2, true)

		SOUND_LOAD(SOUND_HUNTER_ACTION1, _T("./data/sound/mhunter1.wav"), 2, true)
		SOUND_LOAD(SOUND_HUNTER_ACTION2, _T("./data/sound/mhunter2.wav"), 2, true)
		SOUND_LOAD(SOUND_HUNTER_ATTACK1, _T("./data/sound/mhunterattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_HUNTER_ATTACK2, _T("./data/sound/mhunterattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_HUNTER_DIE, _T("./data/sound/mhunterdie.wav"), 2, true)

		SOUND_LOAD(SOUND_FOREST_MONSTER_ACTION1, _T("./data/sound/mwoodmon1.wav"), 2, true)
		SOUND_LOAD(SOUND_FOREST_MONSTER_ACTION2, _T("./data/sound/mwoodmon2.wav"), 2, true)
		SOUND_LOAD(SOUND_FOREST_MONSTER_ATTACK1, _T("./data/sound/mwoodmonattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_FOREST_MONSTER_ATTACK2, _T("./data/sound/mwoodmonattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_FOREST_MONSTER_DIE, _T("./data/sound/mwoodmondie.wav"), 2, true)

		SOUND_LOAD(SOUND_AGON_ACTION1, _T("./data/sound/margon1.wav"), 2, true)
		SOUND_LOAD(SOUND_AGON_ACTION2, _T("./data/sound/margon2.wav"), 2, true)
		SOUND_LOAD(SOUND_AGON_ATTACK1, _T("./data/sound/margonattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AGON_ATTACK2, _T("./data/sound/margonattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AGON_DIE, _T("./data/sound/margondie.wav"), 2, true)

		SOUND_LOAD(SOUND_GOLEM_ACTION1, _T("./data/sound/mgolem1.wav"), 2, true)
		SOUND_LOAD(SOUND_GOLEM_ACTION2, _T("./data/sound/mgolem2.wav"), 2, true)
		SOUND_LOAD(SOUND_GOLEM_ATTACK1, _T("./data/sound/mgolemattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_GOLEM_ATTACK2, _T("./data/sound/mgolemattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_GOLEM_DIE, _T("./data/sound/mgolemdie.wav"), 2, true)

		SOUND_LOAD(SOUND_DEVIL_ATTACK1, _T("./data/sound/msatanattack1.wav"), 2, true)

		SOUND_LOAD(SOUND_BALROG_ACTION1, _T("./data/sound/mbalrog1.wav"), 2, true)
		SOUND_LOAD(SOUND_BALROG_ACTION2, _T("./data/sound/mbalrog2.wav"), 2, true)
		SOUND_LOAD(SOUND_BALROG_DIE, _T("./data/sound/mbalrogdie.wav"), 2, true)

		SOUND_LOAD(SOUND_SHADOW_ACTION1, _T("./data/sound/mshadow1.wav"), 2, true)
		SOUND_LOAD(SOUND_SHADOW_ACTION2, _T("./data/sound/mshadow2.wav"), 2, true)
		SOUND_LOAD(SOUND_SHADOW_ATTACK1, _T("./data/sound/mshadowattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_SHADOW_ATTACK2, _T("./data/sound/mshadowattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_SHADOW_DIE, _T("./data/sound/mshadowdie.wav"), 2, true)

		SOUND_LOAD(SOUND_BALI_ACTION1, _T("./data/sound/mbali1.wav"), 2, true)
		SOUND_LOAD(SOUND_BALI_ACTION2, _T("./data/sound/mbali2.wav"), 2, true)
		SOUND_LOAD(SOUND_BALI_ATTACK1, _T("./data/sound/mbaliattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BALI_ATTACK2, _T("./data/sound/mbaliattack2.wav"), 2, true)

		SOUND_LOAD(SOUND_GREAT_BAHAMUT_ACTION1, _T("./data/sound/mbahamut1.wav"), 2, true)

		SOUND_LOAD(SOUND_VEPAR_ACTION1, _T("./data/sound/mbepar1.wav"), 2, true)
		SOUND_LOAD(SOUND_VEPAR_ACTION2, _T("./data/sound/mbepar2.wav"), 2, true)

		SOUND_LOAD(SOUND_VALKYRIE_ACTION1, _T("./data/sound/mvalkyrie1.wav"), 2, true)
		SOUND_LOAD(SOUND_VALKYRIE_DIE, _T("./data/sound/mvalkyriedie.wav"), 2, true)

		SOUND_LOAD(SOUND_LIZARD_KING_ACTION1, _T("./data/sound/mlizardking1.wav"), 2, true)
		SOUND_LOAD(SOUND_LIZARD_KING_ACTION2, _T("./data/sound/mlizardking2.wav"), 2, true)

		SOUND_LOAD(SOUND_HYDRA_ACTION1, _T("./data/sound/mhydra1.wav"), 2, true)
		SOUND_LOAD(SOUND_HYDRA_ATTACK1, _T("./data/sound/mhydraattack1.wav"), 2, true)

		SOUND_LOAD(SOUND_WHEEL_ACTION1, _T("./data/sound/iron1.wav"), 2, true)
		SOUND_LOAD(SOUND_WHEEL_ATTACK1, _T("./data/sound/iron_attack1.wav"), 2, true)

		SOUND_LOAD(SOUND_TANTALOS_ACTION1, _T("./data/sound/jaikan1.wav"), 2, true)
		SOUND_LOAD(SOUND_TANTALOS_ACTION2, _T("./data/sound/jaikan2.wav"), 2, true)
		SOUND_LOAD(SOUND_TANTALOS_ATTACK1, _T("./data/sound/jaikan_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_TANTALOS_ATTACK2, _T("./data/sound/jaikan_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_TANTALOS_DIE, _T("./data/sound/jaikan_die.wav"), 2, true)

		SOUND_LOAD(SOUND_BLOODY_WOLF_ACTION1, _T("./data/sound/blood1.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_WOLF_ATTACK1, _T("./data/sound/blood_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_WOLF_ATTACK2, _T("./data/sound/blood_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_WOLF_DIE, _T("./data/sound/blood_die.wav"), 2, true)

		SOUND_LOAD(SOUND_BEAM_KNIGHT_ACTION1, _T("./data/sound/death1.wav"), 2, true)
		SOUND_LOAD(SOUND_BEAM_KNIGHT_ATTACK1, _T("./data/sound/death_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BEAM_KNIGHT_DIE, _T("./data/sound/death_die.wav"), 2, true)

		SOUND_LOAD(SOUND_MUTANT_ACTION1, _T("./data/sound/mutant1.wav"), 2, true)
		SOUND_LOAD(SOUND_MUTANT_ACTION2, _T("./data/sound/mutant2.wav"), 2, true)
		SOUND_LOAD(SOUND_MUTANT_ATTACK1, _T("./data/sound/mutant_attack1.wav"), 2, true)

		SOUND_LOAD(SOUND_ORC_CAP_ATTACK1, _T("./data/sound/morccapattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_ORC_ARCHER_ATTACK1, _T("./data/sound/morcarcherattack1.wav"), 2, true)

		SOUND_LOAD(SOUND_CURSED_KING_ACTION1, _T("./data/sound/mcursedking1.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSED_KING_ACTION2, _T("./data/sound/mcursedking2.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSED_KING_DIE, _T("./data/sound/mcursedkingdie1.wav"), 2, true)

		SOUND_LOAD(SOUND_MEGA_CRUST_ACTION1, _T("./data/sound/mmegacrust1.wav"), 2, true)
		SOUND_LOAD(SOUND_MEGA_CRUST_ATTACK1, _T("./data/sound/mmegacrustattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_MEGA_CRUST_DIE, _T("./data/sound/mmegacrustdie.wav"), 2, true)

		SOUND_LOAD(SOUND_ALQUAMOS_ATTACK1, _T("./data/sound/malquamosattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_ALQUAMOS_DIE, _T("./data/sound/malquamosdie.wav"), 2, true)

		SOUND_LOAD(SOUND_QUEEN_RAINNER_ACTION1, _T("./data/sound/mrainner1.wav"), 2, true)
		SOUND_LOAD(SOUND_QUEEN_RAINNER_ATTACK1, _T("./data/sound/mrainnerattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_QUEEN_RAINNER_DIE, _T("./data/sound/mrainnerdie.wav"), 2, true)

		SOUND_LOAD(SOUND_PHANTOM_KNIGHT_ACTION1, _T("./data/sound/mphantom1.wav"), 2, true)
		SOUND_LOAD(SOUND_PHANTOM_KNIGHT_ATTACK1, _T("./data/sound/mphantomattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_PHANTOM_KNIGHT_DIE, _T("./data/sound/mphantomdie.wav"), 2, true)

		SOUND_LOAD(SOUND_DRAKAN_ACTION1, _T("./data/sound/mdrakan1.wav"), 2, true)
		SOUND_LOAD(SOUND_DRAKAN_ATTACK1, _T("./data/sound/mdrakanattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_DRAKAN_DIE, _T("./data/sound/mdrakandie.wav"), 2, true)

		SOUND_LOAD(SOUND_DARKNESS_PHOENIX_ACTION1, _T("./data/sound/mphoenix1.wav"), 2, true)
		SOUND_LOAD(SOUND_DARKNESS_PHOENIX_ATTACK1, _T("./data/sound/mphoenixattack1.wav"), 2, true)

		SOUND_LOAD(SOUND_MAGIC_SKELETON_ACTION1, _T("./data/sound/mmagicskull.wav"), 2, true)

		SOUND_LOAD(SOUND_DARK_SKULL_ATTACK1, _T("./data/sound/mblackskullattack.wav"), 2, true)
		SOUND_LOAD(SOUND_DARK_SKULL_DIE, _T("./data/sound/mblackskulldie.wav"), 2, true)

		SOUND_LOAD(SOUND_GIANT_OGRE_DIE, _T("./data/sound/mghaintorgerdie.wav"), 2, true)

		SOUND_LOAD(SOUND_RED_SKELETON_ACTION1, _T("./data/sound/mredskull.wav"), 2, true)
		SOUND_LOAD(SOUND_RED_SKELETON_ATTACK1, _T("./data/sound/mredskullattack.wav"), 2, true)
		SOUND_LOAD(SOUND_RED_SKELETON_DIE, _T("./data/sound/mredskulldie.wav"), 2, true)

		SOUND_LOAD(SOUND_DEATH_ANGEL_ACTION1, _T("./data/sound/mdangelidle.wav"), 2, true)
		SOUND_LOAD(SOUND_DEATH_ANGEL_ATTACK1, _T("./data/sound/mdangelattack.wav"), 2, true)
		SOUND_LOAD(SOUND_DEATH_ANGEL_DIE, _T("./data/sound/mdangeldeath.wav"), 2, true)

		SOUND_LOAD(SOUND_KUNDUN_ACTION1, _T("./data/sound/mkundunidle.wav"), 2, true)
		SOUND_LOAD(SOUND_KUNDUN_ATTACK1, _T("./data/sound/mkundunattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_KUNDUN_ATTACK2, _T("./data/sound/mkundunattack2.wav"), 2, true)

		SOUND_LOAD(SOUND_BLOODY_SOLDIER_ACTION1, _T("./data/sound/mbsoldieridle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_SOLDIER_ACTION2, _T("./data/sound/mbsoldieridle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_SOLDIER_ATTACK1, _T("./data/sound/mbsoldierattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_SOLDIER_ATTACK2, _T("./data/sound/mbsoldierattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BLOODY_SOLDIER_DIED, _T("./data/sound/mbsoldierdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_AEGIS_ACTION1, _T("./data/sound/mesisidle.wav"), 2, true)
		SOUND_LOAD(SOUND_AEGIS_ATTACK1, _T("./data/sound/mesisattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AEGIS_ATTACK2, _T("./data/sound/mesisattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AEGIS_DIE, _T("./data/sound/mesisdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_CENTURION_ACTION1, _T("./data/sound/mdsidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ACTION2, _T("./data/sound/mdsidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ATTACK1, _T("./data/sound/mdsattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ATTACK2, _T("./data/sound/mdsattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_DIE1, _T("./data/sound/mdsdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_CENTURION_ACTION3, _T("./data/sound/mlsidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ACTION4, _T("./data/sound/mlsidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ATTACK3, _T("./data/sound/mlsattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_ATTACK4, _T("./data/sound/mlsattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CENTURION_DIE2, _T("./data/sound/mlsdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_NECRON_ACTION1, _T("./data/sound/mnecronidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_NECRON_ACTION2, _T("./data/sound/mnecronidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_NECRON_ATTACK1, _T("./data/sound/mnecronattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_NECRON_ATTACK2, _T("./data/sound/mnecronattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_NECRON_DIE, _T("./data/sound/mnecrondeath.wav"), 2, true)

		SOUND_LOAD(SOUND_SCHRIKER_ACTION1, _T("./data/sound/msvidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ACTION2, _T("./data/sound/msvidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ATTACK1, _T("./data/sound/msvattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ATTACK2, _T("./data/sound/msvattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHIKER_DIE1, _T("./data/sound/msvdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_SCHRIKER_ACTION3, _T("./data/sound/mlvidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ACTION4, _T("./data/sound/mlvidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ATTACK3, _T("./data/sound/mlvattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_ATTACK4, _T("./data/sound/mlvattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_SCHRIKER_DIE2, _T("./data/sound/mlvdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_CASTLE_GATE_DESTROYED, _T("./data/sound/battlecastle/ocdoordis.wav"), 2, true)

		SOUND_LOAD(SOUND_BOW_MERCENARY_ATTACK1, _T("./data/sound/battlecastle/mbowmercattack.wav"), 2, true)
		SOUND_LOAD(SOUND_BOW_MERCENARY_DIE, _T("./data/sound/battlecastle/mbowmercdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_SPEAR_MERCENARY_ATTACK1, _T("./data/sound/battlecastle/mspearmercattack.wav"), 2, true)
		SOUND_LOAD(SOUND_SPEAR_MERCENARY_DIE, _T("./data/sound/battlecastle/mspearmercdeath.wav"), 2, true)

		SOUND_LOAD(SOUND_RUN_DARK_HORSE_1, _T("./data/sound/phorsestep1.wav"), 1, false)
		SOUND_LOAD(SOUND_RUN_DARK_HORSE_2, _T("./data/sound/phorsestep2.wav"), 1, false)
		SOUND_LOAD(SOUND_RUN_DARK_HORSE_3, _T("./data/sound/phorsestep3.wav"), 1, false)
		SOUND_LOAD(SOUND_DARKLORD_PAIN, _T("./data/sound/pdarkpain.wav"), 1, false)
		SOUND_LOAD(SOUND_DARKLORD_DEAD, _T("./data/sound/pdarkdeath.wav"), 1, false)
		SOUND_LOAD(SOUND_ATTACK_SPEAR, _T("./data/sound/sdarkspear.wav"), 1, false)
		SOUND_LOAD(SOUND_ATTACK_FIRE_BUST, _T("./data/sound/efirebust.wav"), 1, false)
		SOUND_LOAD(SOUND_ATTACK_FIRE_BUST_EXP, _T("./data/sound/efirebustboom.wav"), 1, false)
		SOUND_LOAD(SOUND_PART_TELEPORT, _T("./data/sound/esummon.wav"), 1, false)
		SOUND_LOAD(SOUND_ELEC_STRIKE_READY, _T("./data/sound/sdarkelecspikeready.wav"), 1, false)
		SOUND_LOAD(SOUND_ELEC_STRIKE, _T("./data/sound/sdarkelecspike.wav"), 1, false)
		SOUND_LOAD(SOUND_EARTH_QUAKE, _T("./data/sound/sdarkearthquake.wav"), 1, false)
		SOUND_LOAD(SOUND_CRITICAL, _T("./data/sound/sdarkcritical.wav"), 1, false)
		SOUND_LOAD(SOUND_DSPIRIT_MISSILE, _T("./data/sound/dspirit_missile.wav"), 4, false)
		SOUND_LOAD(SOUND_DSPIRIT_SHOUT, _T("./data/sound/dspirit_shout.wav"), 1, false)
		SOUND_LOAD(SOUND_DSPIRIT_RUSH, _T("./data/sound/dspirit_rush.wav"), 3, false)

		SOUND_LOAD(SOUND_KALIMA_AMBIENT, _T("./data/sound/akalima.wav"), 1, false)
		SOUND_LOAD(SOUND_KALIMA_AMBIENT2, _T("./data/sound/akalima01.wav"), 1, false)
		SOUND_LOAD(SOUND_KALIMA_AMBIENT3, _T("./data/sound/akalima02.wav"), 1, false)
		SOUND_LOAD(SOUND_KALIMA_WATER_FALL, _T("./data/sound/akalimawaterfall.wav"), 3, false)
		SOUND_LOAD(SOUND_KALIMA_FALLING_STONE, _T("./data/sound/akalimastone.wav"), 3, false)
		SOUND_LOAD(SOUND_DEATH_BUBBLE, _T("./data/sound/mdeathbubble.wav"), 1, false)

		SOUND_LOAD(SOUND_KUNDUN_AMBIENT1, _T("./data/sound/mkundunambient1.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_AMBIENT2, _T("./data/sound/mkundunambient2.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_ROAR, _T("./data/sound/mkundunroar.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_SIGHT, _T("./data/sound/mkundunsight.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_SHUDDER, _T("./data/sound/mkundunshudder.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_DESTROY, _T("./data/sound/mkundundestory.wav"), 1, false)

		SOUND_LOAD(SOUND_SKILL_SKULL, _T("./data/sound/eskull.wav"), 1, false)
		SOUND_LOAD(SOUND_GREAT_POISON, _T("./data/sound/egreatpoison.wav"), 1, false)
		SOUND_LOAD(SOUND_GREAT_SHIELD, _T("./data/sound/egreatshield.wav"), 1, false)

		SOUND_LOAD(SOUND_BC_AMBIENT, _T("./data/sound/battlecastle/asiegeambi.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AMBIENT_BATTLE1, _T("./data/sound/battlecastle/ranambi1.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AMBIENT_BATTLE2, _T("./data/sound/battlecastle/ranambi2.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AMBIENT_BATTLE3, _T("./data/sound/battlecastle/ranambi3.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AMBIENT_BATTLE4, _T("./data/sound/battlecastle/ranambi4.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AMBIENT_BATTLE5, _T("./data/sound/battlecastle/ranambi5.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_GUARD_STONE_DIS, _T("./data/sound/battlecastle/oguardstonedis.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_CATAPULT_ATTACK, _T("./data/sound/battlecastle/oswfire.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_CATAPULT_HIT, _T("./data/sound/battlecastle/oswhitg.wav"), 4, true)
		SOUND_LOAD(SOUND_BC_WALL_HIT, _T("./data/sound/battlecastle/oswhit.wav"), 4, true)
		SOUND_LOAD(SOUND_BC_GATE_OPEN, _T("./data/sound/battlecastle/ocdoormove.wav"), 1, true)

		SOUND_LOAD(SOUND_BC_HUNTINGGROUND_AMBIENT, _T("./data/sound/w31/aw31.wav"), 1, true)
		SOUND_LOAD(SOUND_BC_AXEWARRIOR_MOVE1, _T("./data/sound/w31/mawidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_AXEWARRIOR_MOVE2, _T("./data/sound/w31/mawidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_AXEWARRIOR_ATTACK1, _T("./data/sound/w31/mawattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_AXEWARRIOR_ATTACK2, _T("./data/sound/w31/mawattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_AXEWARRIOR_DIE, _T("./data/sound/w31/mawdeath.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_LIZARDWARRIOR_MOVE1, _T("./data/sound/w31/mlwidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_LIZARDWARRIOR_MOVE2, _T("./data/sound/w31/mlwidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_LIZARDWARRIOR_ATTACK1, _T("./data/sound/w31/mlwattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_LIZARDWARRIOR_ATTACK2, _T("./data/sound/w31/mlwattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_LIZARDWARRIOR_DIE, _T("./data/sound/w31/mlwdeath.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_MOVE1, _T("./data/sound/w31/mpgidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_MOVE2, _T("./data/sound/w31/mpgidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_ATTACK1, _T("./data/sound/w31/mpgattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_ATTACK2, _T("./data/sound/w31/mpgattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_ATTACK3, _T("./data/sound/w31/mpgeff1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_POISONGOLEM_DIE, _T("./data/sound/w31/mpgdeath.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_QUEENBEE_MOVE1, _T("./data/sound/w31/mqbidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_QUEENBEE_MOVE2, _T("./data/sound/w31/mqbidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_QUEENBEE_ATTACK1, _T("./data/sound/w31/mqbattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_QUEENBEE_ATTACK2, _T("./data/sound/w31/mqbattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_QUEENBEE_DIE, _T("./data/sound/w31/mqbdeath.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_FIREGOLEM_MOVE1, _T("./data/sound/w31/mfgidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_FIREGOLEM_MOVE2, _T("./data/sound/w31/mfgidle2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_FIREGOLEM_ATTACK1, _T("./data/sound/w31/mfgattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_FIREGOLEM_ATTACK2, _T("./data/sound/w31/mfgattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_FIREGOLEM_DIE, _T("./data/sound/w31/mfgdeath.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_EROHIM_ENTER, _T("./data/sound/w31/meloidle1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_EROHIM_ATTACK1, _T("./data/sound/w31/meloattack1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_EROHIM_ATTACK2, _T("./data/sound/w31/meloattack2.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_EROHIM_ATTACK3, _T("./data/sound/w31/meloeff1.wav"), 2, true)
		SOUND_LOAD(SOUND_BC_EROHIM_DIE, _T("./data/sound/w31/melodeath.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_AMBIENT, _T("./data/sound/w35/crywolf_ambi.wav"), 1, true)
		SOUND_LOAD(SOUND_CRY1ST_WWOLF_MOVE1, _T("./data/sound/w35/ww_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_WWOLF_MOVE2, _T("./data/sound/w35/ww_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_WWOLF_ATTACK1, _T("./data/sound/w35/ww_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_WWOLF_ATTACK2, _T("./data/sound/w35/ww_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_WWOLF_DIE, _T("./data/sound/w35/ww_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_SCOUT1_MOVE1, _T("./data/sound/w35/ww_s1_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT1_MOVE2, _T("./data/sound/w35/ww_s1_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT1_ATTACK1, _T("./data/sound/w35/ww_s1_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT1_ATTACK2, _T("./data/sound/w35/ww_s1_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT1_DIE, _T("./data/sound/w35/ww_s1_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_SCOUT2_MOVE1, _T("./data/sound/w35/ww_s2_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT2_MOVE2, _T("./data/sound/w35/ww_s2_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT2_ATTACK1, _T("./data/sound/w35/ww_s2_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT2_ATTACK2, _T("./data/sound/w35/ww_s2_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT2_DIE, _T("./data/sound/w35/ww_s2_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_SCOUT3_MOVE1, _T("./data/sound/w35/ww_s3_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT3_MOVE2, _T("./data/sound/w35/ww_s3_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT3_ATTACK1, _T("./data/sound/w35/ww_s3_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT3_ATTACK2, _T("./data/sound/w35/ww_s3_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SCOUT3_DIE, _T("./data/sound/w35/ww_s3_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_SORAM_MOVE1, _T("./data/sound/w35/soram_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SORAM_MOVE2, _T("./data/sound/w35/soram_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SORAM_ATTACK1, _T("./data/sound/w35/soram_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SORAM_ATTACK2, _T("./data/sound/w35/soram_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_SORAM_DIE, _T("./data/sound/w35/soram_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_BALRAM_MOVE1, _T("./data/sound/w35/balram_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALRAM_MOVE2, _T("./data/sound/w35/balram_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALRAM_ATTACK1, _T("./data/sound/w35/balram_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALRAM_ATTACK2, _T("./data/sound/w35/balram_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALRAM_DIE, _T("./data/sound/w35/balram_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_BALGAS_MOVE1, _T("./data/sound/w35/balga_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_MOVE2, _T("./data/sound/w35/balga_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_ATTACK1, _T("./data/sound/w35/balga_at1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_ATTACK2, _T("./data/sound/w35/balga_at2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_DIE, _T("./data/sound/w35/balga_death.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_SKILL1, _T("./data/sound/w35/balga_skill1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_BALGAS_SKILL2, _T("./data/sound/w35/balga_skill2.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_DARKELF_MOVE1, _T("./data/sound/w35/darkelf_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_MOVE2, _T("./data/sound/w35/darkelf_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_ATTACK1, _T("./data/sound/w35/darkelf_at1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_ATTACK2, _T("./data/sound/w35/darkelf_at2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_DIE, _T("./data/sound/w35/darkelf_death.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_SKILL1, _T("./data/sound/w35/darkelf_skill1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DARKELF_SKILL2, _T("./data/sound/w35/darkelf_skill2.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_DEATHSPIRIT_MOVE1, _T("./data/sound/w35/dths_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DEATHSPIRIT_MOVE2, _T("./data/sound/w35/dths_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DEATHSPIRIT_ATTACK1, _T("./data/sound/w35/dths_at1.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DEATHSPIRIT_ATTACK2, _T("./data/sound/w35/dths_at2.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_DEATHSPIRIT_DIE, _T("./data/sound/w35/dths_deat.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_TANKER_ATTACK1, _T("./data/sound/w35/tanker_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_CRY1ST_TANKER_DIE, _T("./data/sound/w35/tanker_death.wav"), 2, true)

		SOUND_LOAD(SOUND_CRY1ST_SUMMON, _T("./data/sound/w35/spawn_single.wav"), 1, false)
		SOUND_LOAD(SOUND_CRY1ST_SUCCESS, _T("./data/sound/w35/cw_win.wav"), 1, false)
		SOUND_LOAD(SOUND_CRY1ST_FAILED, _T("./data/sound/w35/cw_lose.wav"), 1, false)

		SOUND_LOAD(SOUND_AIDA_AMBIENT, _T("./data/sound/w34/aida_ambi.wav"), 1, true)
		SOUND_LOAD(SOUND_AIDA_BLUEGOLEM_MOVE1, _T("./data/sound/w34/bg_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_BLUEGOLEM_MOVE2, _T("./data/sound/w34/bg_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_BLUEGOLEM_ATTACK1, _T("./data/sound/w34/bg_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_BLUEGOLEM_ATTACK2, _T("./data/sound/w34/bg_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_BLUEGOLEM_DIE, _T("./data/sound/w34/bg_death.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHRAIDER_MOVE1, _T("./data/sound/w34/dr_idle01.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHRAIDER_MOVE2, _T("./data/sound/w34/dr_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHRAIDER_ATTACK1, _T("./data/sound/w34/dr_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHRAIDER_ATTACK2, _T("./data/sound/w34/dr_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHRAIDER_DIE, _T("./data/sound/w34/dr_death.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHTREE_MOVE1, _T("./data/sound/w34/dt_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHTREE_MOVE2, _T("./data/sound/w34/dt_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHTREE_ATTACK1, _T("./data/sound/w34/dt_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHTREE_ATTACK2, _T("./data/sound/w34/dt_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_DEATHTREE_DIE, _T("./data/sound/w34/dt_death.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_FORESTORC_MOVE1, _T("./data/sound/w34/fo_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_FORESTORC_MOVE2, _T("./data/sound/w34/fo_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_FORESTORC_ATTACK1, _T("./data/sound/w34/fo_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_FORESTORC_ATTACK2, _T("./data/sound/w34/fo_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_FORESTORC_DIE, _T("./data/sound/w34/fo_death.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_MOVE1, _T("./data/sound/w34/hm_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_MOVE2, _T("./data/sound/w34/hm_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_ATTACK1, _T("./data/sound/w34/hm_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_ATTACK2, _T("./data/sound/w34/hm_firelay.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_ATTACK3, _T("./data/sound/w34/hm_bloodywind.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_HELL_DIE, _T("./data/sound/w34/hm_death.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_WITCHQUEEN_MOVE1, _T("./data/sound/w34/wq_idle1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_WITCHQUEEN_MOVE2, _T("./data/sound/w34/wq_idle2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_WITCHQUEEN_ATTACK1, _T("./data/sound/w34/wq_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_WITCHQUEEN_ATTACK2, _T("./data/sound/w34/wq_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_AIDA_WITCHQUEEN_DIE, _T("./data/sound/w34/wq_death.wav"), 2, true)

		SOUND_LOAD(SOUND_KANTURU_1ST_BG_WATERFALL, _T("./data/sound/w37/kan_ruin_waterfall.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_1ST_BG_ELEC, _T("./data/sound/w37/kan_ruin_elec.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_1ST_BG_WHEEL, _T("./data/sound/w37/kan_ruin_wheel.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_1ST_BG_PLANT, _T("./data/sound/w37/kan_ruin_plant.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_1ST_BG_GLOBAL, _T("./data/sound/w37/kan_ruin_global.wav"), 1, false)

		SOUND_LOAD(SOUND_KANTURU_1ST_BER_MOVE1, _T("./data/sound/w37/ber_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BER_MOVE2, _T("./data/sound/w37/ber_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BER_ATTACK1, _T("./data/sound/w37/ber_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BER_ATTACK2, _T("./data/sound/w37/ber_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BER_DIE, _T("./data/sound/w37/ber_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GIGAN_MOVE1, _T("./data/sound/w37/gigan_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GIGAN_ATTACK1, _T("./data/sound/w37/gigan_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GIGAN_ATTACK2, _T("./data/sound/w37/gigan_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GIGAN_DIE, _T("./data/sound/w37/gigan_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GENO_MOVE1, _T("./data/sound/w37/geno_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GENO_MOVE2, _T("./data/sound/w37/geno_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GENO_ATTACK1, _T("./data/sound/w37/geno_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GENO_ATTACK2, _T("./data/sound/w37/geno_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_GENO_DIE, _T("./data/sound/w37/geno_death.wav"), 2, true)

		SOUND_LOAD(SOUND_KANTURU_1ST_KENTA_MOVE1, _T("./data/sound/w37/kenta_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_KENTA_MOVE2, _T("./data/sound/w37/kenta_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_KENTA_ATTACK1, _T("./data/sound/w37/kenta_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_KENTA_ATTACK2, _T("./data/sound/w37/kenta_skill-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_KENTA_DIE, _T("./data/sound/w37/kenta_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BLADE_MOVE1, _T("./data/sound/w37/blade_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BLADE_MOVE2, _T("./data/sound/w37/blade_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BLADE_ATTACK1, _T("./data/sound/w37/blade_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BLADE_ATTACK2, _T("./data/sound/w37/blade_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_BLADE_DIE, _T("./data/sound/w37/blade_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SATI_MOVE1, _T("./data/sound/w37/sati_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SATI_MOVE2, _T("./data/sound/w37/sati_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SATI_ATTACK1, _T("./data/sound/w37/sati_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SATI_ATTACK2, _T("./data/sound/w37/sati_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SATI_DIE, _T("./data/sound/w37/sati_death.wav"), 2, true)

		SOUND_LOAD(SOUND_KANTURU_1ST_SWOLF_MOVE1, _T("./data/sound/w37/swolf_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SWOLF_MOVE2, _T("./data/sound/w37/swolf_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SWOLF_ATTACK1, _T("./data/sound/w37/swolf_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SWOLF_ATTACK2, _T("./data/sound/w37/swolf_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_SWOLF_DIE, _T("./data/sound/w37/swolf_death.wav"), 2, true)

		SOUND_LOAD(SOUND_KANTURU_1ST_IR_MOVE1, _T("./data/sound/w37/ir_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_IR_MOVE2, _T("./data/sound/w37/ir_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_IR_ATTACK1, _T("./data/sound/w37/ir_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_IR_ATTACK2, _T("./data/sound/w37/ir_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_1ST_IR_DIE, _T("./data/sound/w37/ir_death.wav"), 2, true)

		SOUND_LOAD(SOUND_KANTURU_2ND_MAPSOUND_GEAR, _T("./data/sound/w38/kan_relic_gear.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_2ND_MAPSOUND_INCUBATOR, _T("./data/sound/w38/kan_relic_incubator.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_2ND_MAPSOUND_HOLE, _T("./data/sound/w38/kan_relic_hole.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_2ND_MAPSOUND_GLOBAL, _T("./data/sound/w38/kan_relic_global.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_2ND_PERSO_MOVE1, _T("./data/sound/w38/perso_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_PERSO_MOVE2, _T("./data/sound/w38/perso_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_PERSO_ATTACK1, _T("./data/sound/w38/perso_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_PERSO_ATTACK2, _T("./data/sound/w38/perso_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_PERSO_DIE, _T("./data/sound/w38/perso_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_TWIN_MOVE1, _T("./data/sound/w38/twin_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_TWIN_MOVE2, _T("./data/sound/w38/twin_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_TWIN_ATTACK1, _T("./data/sound/w38/twin_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_TWIN_ATTACK2, _T("./data/sound/w38/twin_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_TWIN_DIE, _T("./data/sound/w38/twin_death.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_DRED_MOVE1, _T("./data/sound/w38/dred_idle-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_DRED_MOVE2, _T("./data/sound/w38/dred_idle-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_DRED_ATTACK1, _T("./data/sound/w38/dred_attack-01.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_DRED_ATTACK2, _T("./data/sound/w38/dred_attack-02.wav"), 2, true)
		SOUND_LOAD(SOUND_KANTURU_2ND_DRED_DIE, _T("./data/sound/w38/dred_death.wav"), 2, true)

		SOUND_LOAD(SOUND_BMS_STUN, _T("./data/sound/battlecastle/sdstun.wav"), 4, true)
		//SOUND_LOAD(SOUND_BMS_STUN_REMOVAL, _T("./data/sound/battlecastle/sdstuncancel.wav"), 4, true)
		SOUND_LOAD(SOUND_BMS_MANA, _T("./data/sound/battlecastle/sdswellmana.wav"), 4, true)
		SOUND_LOAD(SOUND_BMS_INVISIBLE, _T("./data/sound/battlecastle/sdtrans.wav"), 4, true)
		//SOUND_LOAD(SOUND_BMS_VISIBLE, _T("./data/sound/battlecastle/sdstuncancel.wav"), 4, true)
		SOUND_LOAD(SOUND_BMS_MAGIC_REMOVAL, _T("./data/sound/battlecastle/sdmagiccancel.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_RUSH, _T("./data/sound/battlecastle/schaveyblow.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_JAVELIN, _T("./data/sound/battlecastle/scshockwave.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_DEEP_IMPACT, _T("./data/sound/battlecastle/scfirearrow.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_DEATH_CANON, _T("./data/sound/battlecastle/scmw.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_ONE_FLASH, _T("./data/sound/battlecastle/sccoldattack.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_SPACE_SPLIT, _T("./data/sound/battlecastle/scdarkattack.wav"), 4, true)
		SOUND_LOAD(SOUND_BCS_BRAND_OF_SKILL, _T("./data/sound/battlecastle/scdarkassist.wav"), 1, true)

		SOUND_LOAD(SOUND_FENRIR_RUN_1, _T("./data/sound/pw_run-01.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_RUN_2, _T("./data/sound/pw_run-02.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_RUN_3, _T("./data/sound/pw_run-03.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_WALK_1, _T("./data/sound/pw_step-01.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_WALK_2, _T("./data/sound/pw_step-02.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_DEATH, _T("./data/sound/pwdeath.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_IDLE_1, _T("./data/sound/pwidle1.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_IDLE_2, _T("./data/sound/pwidle2.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_DAMAGE_1, _T("./data/sound/pwpain1.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_DAMAGE_2, _T("./data/sound/pwpain2.wav"), 1, false)
		SOUND_LOAD(SOUND_FENRIR_SKILL, _T("./data/sound/pwskill.wav"), 1, false)

		SOUND_LOAD(SOUND_KANTURU_3RD_MAYA_INTRO, _T("./data/sound/w39/maya_intro.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAYA_END, _T("./data/sound/w39/maya_death.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAYA_STORM, _T("./data/sound/w39/maya_storm.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_INTRO, _T("./data/sound/w39/nightmare_intro.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAYAHAND_ATTACK1, _T("./data/sound/w39/maya_hand_attack-01.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAYAHAND_ATTACK2, _T("./data/sound/w39/maya_hand_attack-02.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAP_SOUND01, _T("./data/sound/w39/kan_boss_crystal.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAP_SOUND02, _T("./data/sound/w39/kan_boss_disfield.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAP_SOUND03, _T("./data/sound/w39/kan_boss_field.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAP_SOUND04, _T("./data/sound/w39/kan_boss_gear.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_MAP_SOUND05, _T("./data/sound/w39/kan_boss_incubator.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_AMBIENT, _T("./data/sound/w39/kan_boss_global.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_ATT1, _T("./data/sound/w39/nightmare_attack-01.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_ATT2, _T("./data/sound/w39/nightmare_skill-01.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_ATT3, _T("./data/sound/w39/nightmare_skill-02.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_ATT4, _T("./data/sound/w39/nightmare_skill-03.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_DIE, _T("./data/sound/w39/nightmare_death.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_IDLE1, _T("./data/sound/w39/nightmare_idle-01.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_IDLE2, _T("./data/sound/w39/nightmare_idle-02.wav"), 1, false)
		SOUND_LOAD(SOUND_KANTURU_3RD_NIGHTMARE_TELE, _T("./data/sound/w39/nightmare_tele.wav"), 1, false)
		SOUND_LOAD(SOUND_JEWEL02, _T("./data/sound/jewel_sound.wav"), 1, false)

		SOUND_LOAD(SOUND_XMAS_JUMP_SANTA, _T("./data/sound/xmasjumpsanta.wav"), 1, false)
		SOUND_LOAD(SOUND_XMAS_JUMP_DEER, _T("./data/sound/xmasjumpsasum.wav"), 1, false)
		SOUND_LOAD(SOUND_XMAS_JUMP_SNOWMAN, _T("./data/sound/xmasjumpsnowman.wav"), 1, false)
		SOUND_LOAD(SOUND_XMAS_TURN, _T("./data/sound/xmasturn.wav"), 1, false)

		SOUND_LOAD(SOUND_WORLDCUP_RND1, _T("./data/sound/handclap-01.wav"), 1, false)
		SOUND_LOAD(SOUND_WORLDCUP_RND2, _T("./data/sound/handclap-02.wav"), 1, false)
		SOUND_LOAD(SOUND_WORLDCUP_RND3, _T("./data/sound/handclap-03.wav"), 1, false)
		SOUND_LOAD(SOUND_WORLDCUP_RND4, _T("./data/sound/handclap-04.wav"), 1, false)
		SOUND_LOAD(SOUND_WORLDCUP_RND5, _T("./data/sound/handclap-05.wav"), 1, false)
		SOUND_LOAD(SOUND_WORLDCUP_RND6, _T("./data/sound/handclap-06.wav"), 1, false)

		SOUND_LOAD(SOUND_NEWYEARSDAY_DIE, _T("./data/sound/newyeardie.wav"), 1, false)
		SOUND_LOAD(SOUND_KUNDUN_ITEM_SOUND, _T("./data/sound/kundunitem.wav"), 1, false)
		SOUND_LOAD(SOUND_SHIELDCLASH, _T("./data/sound/shieldclash.wav"), 1, false)
		SOUND_LOAD(SOUND_INFINITYARROW, _T("./data/sound/infinityarrow.wav"), 1, false)
		SOUND_LOAD(SOUND_FIRE_SCREAM, _T("./data/sound/darklord_firescream.wav"), 1, false)

		SOUND_LOAD(SOUND_3RD_CHANGE_UP_BG_CAGE1, _T("./data/sound/w42/cage01.wav"), 1, false)
		SOUND_LOAD(SOUND_3RD_CHANGE_UP_BG_CAGE2, _T("./data/sound/w42/cage02.wav"), 1, false)
		SOUND_LOAD(SOUND_3RD_CHANGE_UP_BG_VOLCANO, _T("./data/sound/w42/volcano.wav"), 1, false)
		SOUND_LOAD(SOUND_3RD_CHANGE_UP_BG_FIREPILLAR, _T("./data/sound/w42/firepillar.wav"), 1, false)

		SOUND_LOAD(SOUND_CURSEDTEMPLE_GAMESYSTEM1, _T("./data/sound/w47/cursedtemple_start01.wav"), 1, false)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_GAMESYSTEM2, _T("./data/sound/w47/cursedtemple_statue01.wav"), 1, false)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_GAMESYSTEM3, _T("./data/sound/w47/cursedtemple_holy01.wav"), 1, false)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_GAMESYSTEM4, _T("./data/sound/w47/cursedtemple_score01.wav"), 1, false)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_GAMESYSTEM5, _T("./data/sound/w47/cursedtemple_end01.wav"), 1, false)

		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER1_IDLE, _T("./data/sound/w47/cursedtemple_idle01.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER_MOVE, _T("./data/sound/w47/cursedtemple_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER1_DAMAGE, _T("./data/sound/w47/cursedtemple_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER1_DEATH, _T("./data/sound/w47/cursedtemple_death01.wav"), 2, true)

		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER2_IDLE, _T("./data/sound/w47/cursedtemple_idle02.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER2_ATTACK, _T("./data/sound/w47/cursedtemple_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER2_DAMAGE, _T("./data/sound/w47/cursedtemple_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_CURSEDTEMPLE_MONSTER2_DEATH, _T("./data/sound/w47/cursedtemple_death02.wav"), 2, true)

		SOUND_LOAD(SOUND_MOONRABBIT_WALK, _T("./data/sound/se_ev_rabbit_walk.wav"), 2, true)
		SOUND_LOAD(SOUND_MOONRABBIT_DAMAGE, _T("./data/sound/se_ev_rabbit_damage.wav"), 2, true)
		SOUND_LOAD(SOUND_MOONRABBIT_DEAD, _T("./data/sound/se_ev_rabbit_death.wav"), 2, true)
		SOUND_LOAD(SOUND_MOONRABBIT_EXPLOSION, _T("./data/sound/se_ev_rabbit_explosion.wav"), 1, false)

		SOUND_LOAD(SOUND_ELBELAND_RABBITSTRANGE_ATTACK01, _T("./data/sound/w52/se_mon_rabbitstrange_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_RABBITSTRANGE_DEATH01, _T("./data/sound/w52/se_mon_rabbitstrange_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_RABBITUGLY_BREATH01, _T("./data/sound/w52/se_mon_rabbitugly_breath01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_RABBITUGLY_ATTACK01, _T("./data/sound/w52/se_mon_rabbitugly_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_RABBITUGLY_DEATH01, _T("./data/sound/w52/se_mon_rabbitugly_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_WOLFHUMAN_MOVE02, _T("./data/sound/w52/se_mon_wolfhuman_move02.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_WOLFHUMAN_ATTACK01, _T("./data/sound/w52/se_mon_wolfhuman_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_WOLFHUMAN_DEATH01, _T("./data/sound/w52/se_mon_wolfhuman_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BUTTERFLYPOLLUTION_MOVE01, _T("./data/sound/w52/se_mon_butterflypollution_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BUTTERFLYPOLLUTION_DEATH01, _T("./data/sound/w52/se_mon_butterflypollution_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_CURSERICH_MOVE01, _T("./data/sound/w52/se_mon_curserich_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_CURSERICH_ATTACK01, _T("./data/sound/w52/se_mon_curserich_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_CURSERICH_DEATH01, _T("./data/sound/w52/se_mon_curserich_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_TOTEMGOLEM_MOVE01, _T("./data/sound/w52/se_mon_totemgolem_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_TOTEMGOLEM_MOVE02, _T("./data/sound/w52/se_mon_totemgolem_move02.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_TOTEMGOLEM_ATTACK01, _T("./data/sound/w52/se_mon_totemgolem_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_TOTEMGOLEM_ATTACK02, _T("./data/sound/w52/se_mon_totemgolem_attack02.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_TOTEMGOLEM_DEATH01, _T("./data/sound/w52/se_mon_totemgolem_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BEASTWOO_MOVE01, _T("./data/sound/w52/se_mon_beastwoo_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BEASTWOO_ATTACK01, _T("./data/sound/w52/se_mon_beastwoo_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BEASTWOO_DEATH01, _T("./data/sound/w52/se_mon_beastwoo_death01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BEASTWOOLEADER_MOVE01, _T("./data/sound/w52/se_mon_beastwooleader_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_BEASTWOOLEADER_ATTACK01, _T("./data/sound/w52/se_mon_beastwooleader_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_ELBELAND_VILLAGEPROTECTION01, _T("./data/sound/w52/se_obj_villageprotection01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_WATERFALLSMALL01, _T("./data/sound/w52/se_obj_waterfallsmall01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_WATERWAY01, _T("./data/sound/w52/se_obj_waterway01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_ENTERDEVIAS01, _T("./data/sound/w52/se_obj_enterdevias01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_WATERSMALL01, _T("./data/sound/w52/se_obj_watersmall01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_RAVINE01, _T("./data/sound/w52/se_amb_ravine01.wav"), 1, false)
		SOUND_LOAD(SOUND_ELBELAND_ENTERATLANCE01, _T("./data/sound/w52/se_amb_enteratlance01.wav"), 1, false)

		SOUND_LOAD(SOUND_SWAMPOFQUIET_SAPI_UNUS_ATTACK01, _T("./data/sound/w57/sapi-attack.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SAPI_DEATH01, _T("./data/sound/w57/sapi-death.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SAPI_TRES_ATTACK01, _T("./data/sound/w57/sapi-attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SHADOW_PAWN_ATTACK01, _T("./data/sound/w57/shadowpawn-attack.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SHADOW_KNIGHT_ATTACK01, _T("./data/sound/w57/shadowknight-attack.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SHADOW_ROOK_ATTACK01, _T("./data/sound/w57/shadowrook-attack.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_SHADOW_DEATH01, _T("./data/sound/w57/shadow-death.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_THUNDER_NAIPIN_BREATH01, _T("./data/sound/w57/naipin-thunder.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_GHOST_NAIPIN_BREATH01, _T("./data/sound/w57/naipin-ghost.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_BLAZE_NAIPIN_BREATH01, _T("./data/sound/w57/naipin-blaze.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_NAIPIN_ATTACK01, _T("./data/sound/w57/naipin-attack.wav"), 2, true)
		SOUND_LOAD(SOUND_SWAMPOFQUIET_NAIPIN_DEATH01, _T("./data/sound/w57/naipin-death.wav"), 2, true)

		SOUND_LOAD(SOUND_SUMMON_SAHAMUTT, _T("./data/sound/se_ch_summoner_skill05_explosion01.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_EXPLOSION, _T("./data/sound/se_ch_summoner_skill05_explosion03.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_NEIL, _T("./data/sound/se_ch_summoner_skill06_requiem01.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_REQUIEM, _T("./data/sound/se_ch_summoner_skill06_requiem02.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMOM_RARGLE, _T("./data/sound/rargle.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_SKILL_LIGHTORB, _T("./data/sound/se_ch_summoner_skill01_lightningof.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_SKILL_SLEEP, _T("./data/sound/se_ch_summoner_skill03_sleep.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_SKILL_BLIND, _T("./data/sound/se_ch_summoner_skill04_blind.wav"), 1, false)
		SOUND_LOAD(SOUND_SUMMON_SKILL_THORNS, _T("./data/sound/se_ch_summoner_skill02_ssonze.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_CHAIN_LIGHTNING, _T("./data/sound/se_ch_summoner_skill08_chainlightning.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_DRAIN_LIFE, _T("./data/sound/se_ch_summoner_skill07_lifedrain.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_WEAKNESS, _T("./data/sound/se_ch_summoner_weakness.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_ENERVATION, _T("./data/sound/se_ch_summoner_innovation.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_BERSERKER, _T("./data/sound/berserker.wav"), 1, false)

		SOUND_LOAD(SOUND_CHERRYBLOSSOM_EFFECT0, _T("./data/sound/cherryblossom/eve_cherryblossoms01.wav"), 1, false)
		SOUND_LOAD(SOUND_CHERRYBLOSSOM_EFFECT1, _T("./data/sound/cherryblossom/eve_cherryblossoms02.wav"), 1, false)

		SOUND_LOAD(SOUND_RAKLION_ICEWALKER_ATTACK, _T("./data/sound/w58w59/icewalker_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_ICEWALKER_MOVE, _T("./data/sound/w58w59/icewalker_move.wav"), 2, true)

		SOUND_LOAD(SOUND_RAKLION_ICEGIANT_MOVE, _T("./data/sound/w58w59/icegiant_move.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_ICEGIANT_DEATH, _T("./data/sound/w58w59/icegiant_death.wav"), 2, true)

		SOUND_LOAD(SOUND_RAKLION_COOLERTIN_ATTACK, _T("./data/sound/w58w59/coolertin_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_COOLERTIN_MOVE, _T("./data/sound/w58w59/coolertin_move.wav"), 2, true)

		SOUND_LOAD(SOUND_RAKLION_IRON_KNIGHT_MOVE, _T("./data/sound/w58w59/ironknight_move.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_IRON_KNIGHT_ATTACK, _T("./data/sound/w58w59/ironknight_attack.wav"), 2, true)

		SOUND_LOAD(SOUND_RAKLION_GIANT_MAMUD_MOVE, _T("./data/sound/w58w59/giantmammoth_move.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_GIANT_MAMUD_ATTACK, _T("./data/sound/w58w59/giantmammoth_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_RAKLION_GIANT_MAMUD_DEATH, _T("./data/sound/w58w59/giantmammoth_death.wav"), 2, true)

		SOUND_LOAD(SOUND_RAKLION_SERUFAN_ATTACK1, _T("./data/sound/w58w59/selupan_attack1.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_ATTACK2, _T("./data/sound/w58w59/selupan_attack2.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_CURE, _T("./data/sound/w58w59/selupan_cure.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_RAGE, _T("./data/sound/w58w59/selupan_rage.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_WORD1, _T("./data/sound/w58w59/selupan_word1.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_WORD2, _T("./data/sound/w58w59/selupan_word2.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_WORD3, _T("./data/sound/w58w59/selupan_word3.wav"), 1, false)
		SOUND_LOAD(SOUND_RAKLION_SERUFAN_WORD4, _T("./data/sound/w58w59/selupan_word4.wav"), 1, false)

		SOUND_LOAD(SOUND_SKILL_BLOWOFDESTRUCTION, _T("./data/sound/blow_of_destruction.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_FLAME_STRIKE, _T("./data/sound/flame_strike.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_GIGANTIC_STORM, _T("./data/sound/gigantic_storm.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_LIGHTNING_SHOCK, _T("./data/sound/lightning_shock.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_SWELL_OF_MAGICPOWER, _T("./data/sound/swellofmagicpower.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_MULTI_SHOT, _T("./data/sound/multi_shot.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_RECOVER, _T("./data/sound/recover.wav"), 1, false)
		SOUND_LOAD(SOUND_SKILL_CAOTIC, _T("./data/sound/caotic.wav"), 1, false)

		SOUND_LOAD(SOUND_XMAS_FIRECRACKER, _T("./data/sound/xmas/christmas_fireworks01.wav"), 1, false)
		SOUND_LOAD(SOUND_XMAS_SANTA_IDLE_1, _T("./data/sound/xmas/darksanta_idle01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_IDLE_2, _T("./data/sound/xmas/darksanta_idle02.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_WALK_1, _T("./data/sound/xmas/darksanta_walk01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_WALK_2, _T("./data/sound/xmas/darksanta_walk02.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_ATTACK_1, _T("./data/sound/xmas/darksanta_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_DAMAGE_1, _T("./data/sound/xmas/darksanta_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_DAMAGE_2, _T("./data/sound/xmas/darksanta_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SANTA_DEATH_1, _T("./data/sound/xmas/darksanta_death01.wav"), 2, true)

		SOUND_LOAD(SOUND_XMAS_SNOWMAN_WALK_1, _T("./data/sound/xmas/snowman_walk01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SNOWMAN_ATTACK_1, _T("./data/sound/xmas/snowman_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SNOWMAN_ATTACK_2, _T("./data/sound/xmas/snowman_attack02.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SNOWMAN_DAMAGE_1, _T("./data/sound/xmas/snowman_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_XMAS_SNOWMAN_DEATH_1, _T("./data/sound/xmas/snowman_death01.wav"), 2, true)

		SOUND_LOAD(SOUND_DUEL_NPC_IDLE_1, _T("./data/sound/w64/gatekeepertitus.wav"), 2, true)

		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_ATTACK, _T("./data/sound/w64/zombiewarrior_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_DAMAGE01, _T("./data/sound/w64/zombiewarrior_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_DAMAGE02, _T("./data/sound/w64/zombiewarrior_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_MOVE01, _T("./data/sound/w64/zombiewarrior_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_MOVE02, _T("./data/sound/w64/zombiewarrior_move02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ZOMBIEWARRIOR_DEATH, _T("./data/sound/w64/zombiewarrior_death.wav"), 2, true)

		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_ATTACK, _T("./data/sound/w64/raisedgladiator_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_DAMAGE01, _T("./data/sound/w64/raisedgladiator_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_DAMAGE02, _T("./data/sound/w64/raisedgladiator_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_DEATH, _T("./data/sound/w64/raisedgladiator_death.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_MOVE01, _T("./data/sound/w64/raisedgladiator_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_RAISEDGLADIATOR_MOVE02, _T("./data/sound/w64/raisedgladiator_move02.wav"), 2, true)

		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_ATTACK, _T("./data/sound/w64/ashesbutcher_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_DAMAGE01, _T("./data/sound/w64/ashesbutcher_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_DAMAGE02, _T("./data/sound/w64/ashesbutcher_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_DEATH, _T("./data/sound/w64/ashesbutcher_death.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_MOVE01, _T("./data/sound/w64/ashesbutcher_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_ASHESBUTCHER_MOVE02, _T("./data/sound/w64/ashesbutcher_move02.wav"), 2, true)

		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_ATTACK, _T("./data/sound/w64/bloodassassin_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_DAMAGE01, _T("./data/sound/w64/bloodassassin_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_DAMAGE02, _T("./data/sound/w64/bloodassassin_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_DEDTH, _T("./data/sound/w64/bloodassassin_death.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_MOVE01, _T("./data/sound/w64/bloodassassin_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BLOODASSASSIN_MOVE02, _T("./data/sound/w64/bloodassassin_move02.wav"), 2, true)

		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_ATTACK01, _T("./data/sound/w64/burninglavagolem_attack01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_ATTACK02, _T("./data/sound/w64/burninglavagolem_attack02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_DAMAGE01, _T("./data/sound/w64/burninglavagolem_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_DAMAGE02, _T("./data/sound/w64/burninglavagolem_damage02.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_DEATH, _T("./data/sound/w64/burninglavagolem_death.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_MOVE01, _T("./data/sound/w64/burninglavagolem_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_PKFIELD_BURNINGLAVAGOLEM_MOVE02, _T("./data/sound/w64/burninglavagolem_move02.wav"), 2, true)

		SOUND_LOAD(SOUND_UMBRELLA_MONSTER_WALK1, _T("./data/sound/umbmon_walk01.wav"), 2, true)
		SOUND_LOAD(SOUND_UMBRELLA_MONSTER_WALK2, _T("./data/sound/umbmon_walk02.wav"), 2, true)
		SOUND_LOAD(SOUND_UMBRELLA_MONSTER_DAMAGE, _T("./data/sound/umbmon_damage01.wav"), 2, true)
		SOUND_LOAD(SOUND_UMBRELLA_MONSTER_DEAD, _T("./data/sound/umbmon_dead.wav"), 2, true)

		SOUND_LOAD(SOUND_DOPPELGANGER_SLIME_ATTACK, _T("./data/sound/doppelganger/doppelganger_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_SLIME_DEATH, _T("./data/sound/doppelganger/doppelganger_death.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_BUGBEAR_ATTACK, _T("./data/sound/doppelganger/butcher_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_BUGBEAR_DEATH, _T("./data/sound/doppelganger/butcher_death.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_RED_BUGBEAR_ATTACK, _T("./data/sound/doppelganger/angerbutcher_attack.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_RED_BUGBEAR_DEATH, _T("./data/sound/doppelganger/angerbutcher_death.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_LUGARD_BREATH, _T("./data/sound/doppelganger/lugard.wav"), 2, true)
		SOUND_LOAD(SOUND_DOPPELGANGER_JEWELBOX_OPEN, _T("./data/sound/doppelganger/treasurebox_open.wav"), 1, false)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_WEATHER_RAIN, _T("./data/sound/w69w70w71w72/imperialguardianfort_out1.wav"), 1, false)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_WEATHER_FOG, _T("./data/sound/w69w70w71w72/imperialguardianfort_out2.wav"), 1, false)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_WEATHER_STORM, _T("./data/sound/w69w70w71w72/imperialguardianfort_out3.wav"), 1, false)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_INDOOR_SOUND, _T("./data/sound/w69w70w71w72/imperialguardianfort_in.wav"), 1, false)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_MOVE, _T("./data/sound/w69w70w71w72/gaionkalein_move.wav"), 1, false)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_RAGE, _T("./data/sound/w69w70w71w72/gaionkalein_rage.wav"), 1, false)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_DEATH, _T("./data/sound/w69w70w71w72/grandwizard_death.wav"), 1, false)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01, _T("./data/sound/w69w70w71w72/jelint_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK03, _T("./data/sound/w69w70w71w72/jelint_attack3.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01, _T("./data/sound/w69w70w71w72/jelint_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02, _T("./data/sound/w69w70w71w72/jelint_move02.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_RAGE, _T("./data/sound/w69w70w71w72/jelint_rage.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_DEATH, _T("./data/sound/w69w70w71w72/jelint_death.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_ATTACK02, _T("./data/sound/w69w70w71w72/raymond_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_RAGE, _T("./data/sound/w69w70w71w72/raymond_rage.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_ERCANNE_MONSTER_ATTACK03, _T("./data/sound/w69w70w71w72/ercanne_attack3.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_1CORP_DEASULER_MONSTER_ATTACK02, _T("./data/sound/w69w70w71w72/1deasuler_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_1CORP_DEASULER_MONSTER_ATTACK03, _T("./data/sound/w69w70w71w72/1deasuler_attack3.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_ATTACK01, _T("./data/sound/w69w70w71w72/2vermont_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_ATTACK02, _T("./data/sound/w69w70w71w72/2vermont_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_DEATH, _T("./data/sound/w69w70w71w72/2vermont_death.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_3CORP_CATO_ATTACK02, _T("./data/sound/w69w70w71w72/3cato_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_3CORP_CATO_MOVE, _T("./data/sound/w69w70w71w72/3cato_move.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_4CORP_GALLIA_ATTACK02, _T("./data/sound/w69w70w71w72/4gallia_attack2.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_QUATERMASTER_ATTACK02, _T("./data/sound/w69w70w71w72/quatermaster_attack2.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01, _T("./data/sound/w69w70w71w72/combatmaster_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK02, _T("./data/sound/w69w70w71w72/combatmaster_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK03, _T("./data/sound/w69w70w71w72/combatmaster_attack3.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_GRANDWIZARD_DEATH, _T("./data/sound/w69w70w71w72/grandwizard_death.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_ASSASSINMASTER_DEATH, _T("./data/sound/w69w70w71w72/assassinmaster_death.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_ATTACK01, _T("./data/sound/w69w70w71w72/cavalryleader_attack1.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_ATTACK02, _T("./data/sound/w69w70w71w72/cavalryleader_attack2.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_MOVE01, _T("./data/sound/w69w70w71w72/cavalryleader_move01.wav"), 2, true)
		SOUND_LOAD(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_MOVE02, _T("./data/sound/w69w70w71w72/cavalryleader_move02.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_DEFENDER_ATTACK02, _T("./data/sound/w69w70w71w72/grandwizard_death.wav"), 2, true)

		SOUND_LOAD(SOUND_EMPIREGUARDIAN_PRIEST_STOP, _T("./data/sound/w69w70w71w72/priest_stay.wav"), 2, true)
	};
	const mu_uint32 LoadCount = mu_countof(Load);
};

namespace MUSICS
{
	const mu_text *Data[MUSICS_MAX] =
	{
		_T("./data/music/pub.ogg"),
		_T("./data/music/mutheme.ogg"),
		_T("./data/music/church.ogg"),
		_T("./data/music/devias.ogg"),
		_T("./data/music/noria.ogg"),
		_T("./data/music/dungeon.ogg"),
		_T("./data/music/atlans.ogg"),
		_T("./data/music/icarus.ogg"),
		_T("./data/music/tarkan.ogg"),
		_T("./data/music/lost_tower_a.ogg"),
		_T("./data/music/lost_tower_b.ogg"),
		_T("./data/music/kalima.ogg"),
		_T("./data/music/castle.ogg"),
		_T("./data/music/charge.ogg"),
		_T("./data/music/lastend.ogg"),
		_T("./data/music/huntingground.ogg"),
		_T("./data/music/aida.ogg"),
		_T("./data/music/crywolf1st.ogg"),
		_T("./data/music/crywolf_ready-02.ogg"),
		_T("./data/music/crywolf_before-01.ogg"),
		_T("./data/music/crywolf_back-03.ogg"),
		_T("./data/music/main_theme.ogg"),
		_T("./data/music/kanturu_1st.ogg"),
		_T("./data/music/kanturu_2nd.ogg"),
		_T("./data/music/kanturumayabattle.ogg"),
		_T("./data/music/kanturunightmarebattle.ogg"),
		_T("./data/music/kanturutower.ogg"),
		_T("./data/music/balgasbarrack.ogg"),
		_T("./data/music/balgasrefuge.ogg"),
		_T("./data/music/cursedtemplewait.ogg"),
		_T("./data/music/cursedtempleplay.ogg"),
		_T("./data/music/elbeland.ogg"),
		_T("./data/music/login_theme.ogg"),
		_T("./data/music/swampofcalmness.ogg"),
		_T("./data/music/raklion.ogg"),
		_T("./data/music/raklion_hatchery.ogg"),
		_T("./data/music/santa_village.ogg"),
		_T("./data/music/duelarena.ogg"),
		_T("./data/music/pk_field.ogg"),
		_T("./data/music/imperialguardianfort.ogg"),
		_T("./data/music/idoppelganger.ogg"),
		_T("./data/music/idoppelganger.ogg"),
	};
};