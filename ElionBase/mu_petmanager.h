#ifndef __MU_PETMANAGER_H__
#define __MU_PETMANAGER_H__

namespace giPetManager
{
	void InitPetManager();
	void CreatePetDarkSpirit(CHARACTER*c);
	void CreatePetDarkSpirit_Now(CHARACTER* c);

	void MovePet(const mu_uint32 ThreadIndex, CHARACTER*c);
	void RenderPet(const mu_uint32 ThreadIndex, CHARACTER* c);
	void DeletePet(CHARACTER* c);

	void InitItemBackup();
	void SetPetInfo(const mu_uint32 ThreadIndex, mu_uint8 InvType, mu_uint8 InvPos, void* pPetinfo);
	mu_int64 GetPetItemValue(ITEM* pItem);
	mu_boolean RenderPetItemInfo(mu_int32 sx, mu_int32 sy, ITEM* pItem, mu_boolean useAlternativePos, mu_int32 iInvenType);

	mu_boolean SendPetCommand(CHARACTER* c, mu_int32 Index);
	void SetPetCommand(CHARACTER* c, mu_int32 Key, mu_uint8 Cmd);
	void SetAttack(CHARACTER* c, mu_int32 Key, mu_int32 attackType);
	mu_boolean RenderPetCmdInfo(mu_int32 sx, mu_int32 sy, mu_int32 Type, mu_int32 iRenderPoint);
};

#endif