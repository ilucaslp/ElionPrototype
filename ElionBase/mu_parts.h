#ifndef __MU_PARTS_H__
#define __MU_PARTS_H__

#pragma once

class CSIPartsMDL
{
protected:
	OBJECT m_pObj;
	mu_int32 m_iBoneNumber;
	EVector3 m_vOffset;

public:
	CSIPartsMDL() : m_iBoneNumber(-1) {};
	virtual ~CSIPartsMDL() {}

	virtual void IRender(const mu_uint32 ThreadIndex, CHARACTER* c) = 0;
	inline OBJECT* GetObject() { return &m_pObj; }
};

class CSParts : public CSIPartsMDL
{
public:
	CSParts(mu_int32 Type, mu_int32 BoneNumber, mu_boolean bBillBoard = false, mu_float x = 0.0f, mu_float y = 0.0f, mu_float z = 0.0f, mu_float ax = 0.0f, mu_float ay = 0.0f, mu_float az = 0.0f);
	virtual ~CSParts() override {}
	virtual void IRender(const mu_uint32 ThreadIndex, CHARACTER* c) override;
};

class CSAnimationParts : public CSIPartsMDL
{
public:
	CSAnimationParts(mu_int32 Type, mu_int32 BoneNumber, mu_boolean bBillBoard = false, mu_float x = 0.0f, mu_float y = 0.0f, mu_float z = 0.0f, mu_float ax = 0.0f, mu_float ay = 0.0f, mu_float az = 0.0f);
	virtual ~CSAnimationParts() override {}
	void Animation(const mu_uint32 ThreadIndex, CHARACTER* c);
	virtual void IRender(const mu_uint32 ThreadIndex, CHARACTER* c) override;
};

class CSParts2D : public CSIPartsMDL
{
public:
	CSParts2D(mu_int32 Type, mu_int32 SubType, mu_int32 BoneNumber, mu_float x = 0.0f, mu_float y = 0.0f, mu_float z = 0.0f);
	virtual ~CSParts2D() override {}
	virtual void IRender(const mu_uint32 ThreadIndex, CHARACTER* c) override;
};

void CreatePartsFactory(CHARACTER* c);
void RenderParts(const mu_uint32 ThreadIndex, CHARACTER* c);
void DeleteParts(CHARACTER* c);

#endif