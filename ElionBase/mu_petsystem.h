#ifndef __MU_PETSYSTEM_H__
#define __MU_PETSYSTEM_H__

enum CSPetType
{
	CS_PETTYPE_BASE,
	CS_PETTYPE_DARKSPIRIT,
};

class CSPetSystem
{
public:
	CHARACTER*  m_PetOwner;
	CHARACTER*  m_PetTarget;
	CHARACTER   m_PetCharacter;
	PET_TYPE    m_PetType;
	ITEM*		m_pPetItem;
	mu_uint8    m_byCommand;

public:
	CSPetSystem();
	~CSPetSystem();

	PET_TYPE GetPetType() { return m_PetType; }
	void SetPetItem(ITEM* pPetInfo) { m_pPetItem = pPetInfo; }

	void CreatePetPointer(mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation);
	mu_boolean PlayAnimation(const mu_uint32 ThreadIndex, OBJECT* o);

	void MoveInventory();

	void SetAI(mu_int32 AI);
	void SetCommand(mu_int32 Key, mu_uint8 cmd);
	void SetAttack(mu_int32 Key, mu_int32 attackType);

	mu_int32 GetObjectType()
	{
		return m_PetCharacter.Object.Type;
	}
};

class CSPetDarkSpirit : public CSPetSystem
{
public:
	CSPetDarkSpirit(CHARACTER* c);
	~CSPetDarkSpirit();

	void MovePet(const mu_uint32 ThreadIndex);
	void RenderPetInventory();
	void RenderPet(const mu_uint32 ThreadIndex, mu_int32 PetState = 0);

	void Eff_LevelUp(const mu_uint32 ThreadIndex);
	void Eff_LevelDown(const mu_uint32 ThreadIndex);

	void AttackEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);
	void RenderCmdType();
};

#endif