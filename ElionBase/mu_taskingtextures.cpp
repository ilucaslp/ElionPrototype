#include "stdafx.h"
#include "mu_tasking.h"

#define TEMPLATE_INIT_HELPER(funcname, ...) \
template void MUTasking::funcname <false, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eMetal>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eMetal>(__VA_ARGS__);

TEMPLATE_INIT_HELPER(RenderPreTextures, void);
TEMPLATE_INIT_HELPER(RenderPostTextures, void);

#undef TEMPLATE_INIT_HELPER

void MUTasking::PreConfigureTextures(const mu_uint32 ThreadIndex)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, g_TexturesCulledCount, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < g_TexturesCulledCount; i += _ThreadsCount)
#endif
	{
		ETextureRenderProxy &proxy = TEXTURES::Proxy[g_TexturesCulledList[i]];

		mu_uint32 modesCount = 0;

		for (mu_uint32 n = 0; n < BLEND::TXB_MAX; ++n)
		{
			modesCount += proxy.ExchangeInstancesModeCount(n, modesCount);
		}

		const mu_uint32 modesBase = _EffectInstanceCount.fetch_add(modesCount);

		proxy.RegisterInstancesBase(modesBase);
	}
}

void MUTasking::PostConfigureTextures(const mu_uint32 ThreadIndex, const mu_uint32 effectsCount)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, effectsCount, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < effectsCount; i += _ThreadsCount)
#endif
	{
		EEffectCulledData &data = g_EffectsCulledData[i];

		switch (static_cast<EEffectProxyEnum>(data.Type))
		{
		case EEffectProxyEnum::eLeaves:
			{
				PARTICLE *o = &Leaves[data.Index];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[o->Type];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eLeaves, 0>(data.Index);

				if (GET_WORLD == WD_2DEVIAS ||
					IsIceCity() == true ||
					IsSantaTown() == true)
				{
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, o->Scale, o->Scale, o->Light);
				}
				else if (InChaosCastle() == true)
				{
					RenderPlane3D(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, o->TurningForce[0], o->TurningForce[1], EVector3(1.0f, 1.0f, 1.0f), o->Angle);
				}
				else if (o->Type == BITMAP_RAIN)
				{
					RenderPlane3D(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, 1.0f, 20.0f, EVector3(1.0f, 1.0f, 1.0f), o->Angle);
				}
				else if (o->Type == BITMAP_FIRE_SNUFF)
				{
					RenderPlane3D(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, o->Scale*2.0f, o->Scale*4.0f, EVector3(1.0f, 1.0f, 1.0f), o->Angle);
				}
				else
				{
					RenderPlane3D(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, 3.0f, 3.0f, EVector3(1.0f, 1.0f, 1.0f), o->Angle);
				}
			}
			break;

		case EEffectProxyEnum::eJoints:
			{
				JOINT *o = &g_Joints[data.Index];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[o->TexType];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eJoints, 0>(data.Index);

				EVector3 Light = o->Light;

				if (o->Type == MODEL_SPEARSKILL)
				{
					mu_float fAlpha;
					switch (o->SubType)
					{
					case 0:
					case 1:
					case 2:
					case 4:
					case 9:
					case 10:
						fAlpha = (mu_float)EMath::Min(o->LifeTime, 20) * 0.05f;
						Light *= fAlpha;
						break;
					}
				}
				else if (o->Type == BITMAP_SMOKE && o->SubType == 0)
				{
					mu_float fAlpha = (mu_float)EMath::Min(o->LifeTime, 20) * 0.1f;
					Light *= fAlpha;
				}

				for (mu_int32 j = o->BeginTails, m = 0; m < o->NumTails; ++m, j = (j + 1) % o->MaxTails)
				{
					if (o->Type == BITMAP_SMOKE &&
						o->SubType == 0 &&
						m < 1)
					{
						continue;
					}
					else if (o->Type == BITMAP_JOINT_HEALING &&
						(o->SubType == 9 || o->SubType == 10) &&
						m == o->NumTails - 1)
					{
						continue;
					}

					mu_float Light1, Light2;
					if (o->Features.Get(JOINT::eJFS_TILEMAPPING))
					{
						Light1 = (o->NumTails - (m)) / 16.0f;
						Light2 = (o->NumTails - (m + 1)) / 16.0f;
					}
					else if (o->ReverseUV == 3)
					{
						Light1 = 1.0f - (m) / (mu_float)(o->MaxTails - 1);
						Light2 = 1.0f - (m + 1) / (mu_float)(o->MaxTails - 1);
					}
					else
					{
						Light1 = (o->NumTails - (m)) / (mu_float)(o->MaxTails - 1);
						Light2 = (o->NumTails - (m + 1)) / (mu_float)(o->MaxTails - 1);
					}

					mu_float Scroll = (mu_float)(IWorldTime % 1000)*0.001f;
					if (o->Type == BITMAP_JOINT_THUNDER ||
						o->Type == BITMAP_JOINT_THUNDER + 1)
					{
						Light1 *= 2.0f;
						Light2 *= 2.0f;
						Light1 -= Scroll;
						Light2 -= Scroll;
					}

					if (o->Type == BITMAP_FLARE_FORCE &&
						(o->SubType <= 4 ||
						(o->SubType >= 11 && o->SubType <= 13))
						)
					{
						Light1 = (o->NumTails - (m)) / (mu_float)((o->MaxTails - 1) / 2);
						Light2 = (o->NumTails - (m + 1)) / (mu_float)((o->MaxTails - 1) / 2);
						Light1 -= Scroll;
						Light2 -= Scroll;
					}

					if (o->Features.Get(JOINT::eJFS_TILEMAPPING))
					{
						Scroll *= 2.0f;
						Light1 *= 2.0f;
						Light2 *= 2.0f;
						Light1 -= Scroll;
						Light2 -= Scroll;
					}

					mu_int32 q = (j + 1) % o->MaxTails;

					if (o->Type == BITMAP_JOINT_FORCE &&
						o->SubType == 0)
					{
						mu_float Luminosity = ((mu_float)((o->MaxTails - m) / (mu_float)(o->MaxTails)) * 2);
						Luminosity *= o->Light[0];

						RenderTail(GetEffectInstanceBuffer(instanceIndex++), o->TexType, o->Tails[j][0], o->Tails[q][0], o->Tails[q][1], o->Tails[j][1], EVector2(Light1, Light2), false, EVector4(Luminosity, Luminosity, Luminosity, 1.0f));
					}
					else
					{
						if (o->Type == MODEL_SPEARSKILL &&
							(o->SubType == 0 ||
								o->SubType == 4 ||
								o->SubType == 9))
						{
							if (o->Target != nullptr)
							{
								mu_float fJointHeight;

								if (o->SubType == 9)
									fJointHeight = (o->Tails[j][0][2] - (o->Target->Position[2] + 180))*0.01f;
								else
									fJointHeight = (o->Tails[j][0][2] - (o->Target->Position[2] + 50))*0.01f;

								if (fJointHeight > 0)
								{
									Vector(o->Light[0] - fJointHeight, o->Light[1] - fJointHeight, o->Light[2] - fJointHeight, Light);
								}
								else
								{
									VectorCopy(o->Light, Light);
								}
							}
						}
						else if (o->Type == BITMAP_JOINT_HEALING &&
							(o->SubType == 9 || o->SubType == 10))
						{
							if (o->Target != nullptr)
							{
								mu_float  fJointHeight = (m)*0.01f;
								VectorScale(o->Light, 0.9978f, o->Light);
								Vector(o->Light[0] - fJointHeight, o->Light[1] - fJointHeight, o->Light[2] - fJointHeight, Light);
							}
						}
						else if (o->Type == BITMAP_JOINT_THUNDER + 1 && o->SubType == 0)
						{
							mu_int32 tail = (mu_int32)(o->Light[2]);
							if (tail == m)
							{
								mu_float l = o->Light[2] - m;
								Light = EVector3(l, l, l);
							}
							else if (tail < m)
							{
								Light = EVector3(0.0f, 0.0f, 0.0f);
							}
							else
							{
								Light = EVector3(0.7f, 0.7f, 0.7f);
							}
						}
						else if (o->Type == BITMAP_FLARE_FORCE && (o->SubType <= 4
							|| (o->SubType >= 11 && o->SubType <= 13))
							)
						{
							mu_float Luminosity = ((mu_float)((o->NumTails - 1 - m) / (mu_float)(o->MaxTails)) * 2);

							Light = o->Light * Luminosity;
						}
						else if (o->Type == BITMAP_JOINT_FORCE && o->SubType == 1)
						{
							mu_float Luminosity = (1.0f - (o->NumTails - m) / (mu_float)(o->NumTails))*2.0f;

							Light = o->Light * Luminosity;
						}

						mu_boolean UVType = false; // 0.0f 1.0f
						mu_float L1 = Light1;
						mu_float L2 = Light2;
						if (o->ReverseUV == 1)
						{
							UVType = true;
						}
						else if (o->ReverseUV == 2)
						{
							L1 = 1.0f - L1;
							L2 = 1.0f - L2;
						}

						if (o->Features.Get(JOINT::eJFS_FACEONE) == true)
						{
							RenderTail(GetEffectInstanceBuffer(instanceIndex++), o->TexType, o->Tails[j][2], o->Tails[q][2], o->Tails[q][3], o->Tails[j][3], EVector2(L1, L2), !UVType, EVector4(Light, 1.0f));
						}

						if (o->Features.Get(JOINT::eJFS_FACETWO) == true)
						{
							if (o->Type == BITMAP_JOINT_THUNDER || o->Type == BITMAP_JOINT_THUNDER + 1)
							{
								L1 += Scroll * 2.0f;
								L2 += Scroll * 2.0f;
							}

							RenderTail(GetEffectInstanceBuffer(instanceIndex++), o->TexType, o->Tails[j][0], o->Tails[q][0], o->Tails[q][1], o->Tails[j][1], EVector2(L1, L2), UVType, EVector4(Light, 1.0f));
						}
					}
				}
			}
			break;

		case EEffectProxyEnum::eParticles:
			{
				PARTICLE *o = &g_Particles[data.Index];
				MUTexture &texture = TEXTURES::Data[o->TexType];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[o->TexType];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eParticles, 0>(data.Index);

				mu_float Width = texture.GetWidth() *o->Scale;
				mu_float Height = texture.GetHeight()*o->Scale;

				mu_int32 Frame;
				switch (o->Type)
				{
				case BITMAP_WATERFALL_1:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;

				case BITMAP_BUBBLE:
					Frame = o->Frame % 9;
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, 0.0f, Frame % 3 * 0.25f + 0.005f, Frame / 3 * 0.25f + 0.005f, 0.25f - 0.01f, 0.25f - 0.01f);
					break;
				case BITMAP_SPOT_WATER:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height*0.125, o->Light, o->Angle[0], 0.0f, o->Frame % 8 * 0.125f, 1.0f, 0.125f);
					break;

				case BITMAP_SPARK + 2:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, 0.0f, o->Frame % 2 * 0.5f, o->Frame / 2 * 0.5f, 0.5f, 0.5f);
					break;

				case BITMAP_EXPLOTION_MONO:
				case BITMAP_EXPLOTION:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, 0.0f, o->Frame % 4 * 0.25f + 0.005f, o->Frame / 4 * 0.25f + 0.005f, 0.25f - 0.01f, 0.25f - 0.01f);
					break;
				case BITMAP_EXPLOTION + 1:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width*0.25f, Height, o->Light, o->Angle[0], o->Frame % 4 * 0.25f, 0.0f, 0.25f, 1.0f);
					break;
				case BITMAP_SUMMON_SAHAMUTT_EXPLOSION:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, 0.0f, o->Frame % 4 * 0.25f + 0.005f, o->Frame / 4 * 0.25f + 0.005f, 0.25f - 0.01f, 0.25f - 0.01f);
					break;

					//////////////////////////////////////////////////////////////////////////

				case BITMAP_CLUD64:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_TORCH_FIRE:
					{
						EVector3 vPos;
						VectorCopy(o->Position, vPos);
						for (mu_int32 i = 0; i < 3; ++i)
						{
							RenderSprite(GetEffectInstanceBuffer(instanceIndex++), o->TexType, vPos, Width, Height, o->Light, o->Rotation);
							vPos[2] -= 10.0f;
						}
					}
					break;
				case BITMAP_GHOST_CLOUD1:
				case BITMAP_GHOST_CLOUD2:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;

				case BITMAP_LIGHT + 3:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;

				case BITMAP_TWINTAIL_WATER:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_SMOKE:
					if (o->SubType == 6)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light);
					}
					else
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_SMOKE + 1:
				case BITMAP_SMOKE + 4:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				case BITMAP_ADV_SMOKE + 1:
					if (o->SubType == 2)
					{
						RenderSpriteADD(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_SMOKE + 3:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				case BITMAP_LIGHTNING:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width*0.25f, Height, o->Light, o->Angle[0], o->Frame % 4 * 0.25f, 0.0f, 0.25f, 1.0f);
					break;
				case BITMAP_BLOOD + 1:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, 0.0f, o->Frame % 2 * 0.5f, o->Frame / 2 * 0.5f, 0.5f, 0.5f);
					break;
				case BITMAP_CHROME_ENERGY2:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width*0.25f, Height, o->Light, o->Rotation, o->Frame % 4 * 0.25f, 0.0f, 0.25f, 1.0f);
					break;
				case BITMAP_FIRE_CURSEDLICH:
				case BITMAP_FIRE_HIK2_MONO:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				case BITMAP_LEAF_TOTEMGOLEM:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				case BITMAP_FIRE:
				case BITMAP_FIRE + 2:
				case BITMAP_FIRE + 3:
					if (o->SubType == 17 ||
						o->SubType == 5 ||
						o->SubType == 7 ||
						o->SubType == 8 ||
						o->SubType == 11 ||
						o->SubType == 12 ||
						o->SubType == 13)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width*0.25f, Height, o->Light, o->Rotation, o->Frame % 4 * 0.25f, 0.0f, 0.25f, 1.0f);
					}
					else if (o->SubType == 14 ||
						o->SubType == 15)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width*0.25f, Height, o->Light, o->Angle[0], o->Frame % 4 * 0.25f, 0.0f, 0.25f, 1.0f);
					}
					break;
				case BITMAP_FIRECRACKER:
					{
						mu_int32 iCount = o->Frame;
						EVector3 Position;
						EVector3 Light;
						mu_int32 iTemp = o->LifeTime / 4 + o->SubType;
						mu_int32 iColor = iTemp / 10;
						mu_int32 iColorChange = iTemp % 10;
						for (mu_int32 j = iCount; j >= 0; --j)
						{
							for (mu_int32 k = 0; k < 3; ++k)
							{
								Position[k] = o->Position[k] - (mu_float)j * o->Velocity[k] * 0.1f;
								Light[k] = (mu_float)(EMath::Min(iCount - j, 10)) *
									(o->Light[(k + iColor) % 3] * (10 - iColorChange) +
										o->Light[(k + iColor + 1) % 3] * iColorChange) *
										((mu_float)EMath::Min(o->LifeTime, 10) * 0.001f);
							}
							RenderSprite(GetEffectInstanceBuffer(instanceIndex++), o->TexType, Position, Width, Height, Light, o->Rotation);
						}
					}
					break;
				case BITMAP_FLARE:
					if (o->SubType == 11)
					{
						MUTexture* pBitmap = &TEXTURES::Data[o->TexType];
						Width = pBitmap->GetWidth() * 0.5f * o->Scale;
						Height = pBitmap->GetHeight() * 0.4f;

						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else if (o->SubType != 4)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_FLARE_BLUE:
					if (o->SubType == 0)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else if (o->SubType == 1)
					{
						Width = texture.GetWidth() * 0.2f * o->Scale;
						Height = texture.GetHeight() * 0.3f;
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_FLARE + 1:
					if (o->SubType == 0)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;

				case BITMAP_LIGHT + 2:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;

				case BITMAP_MAGIC + 1:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;

				case BITMAP_CLOUD:
					switch (o->SubType)
					{
					case 10:
					case 12:
					case 7:
					case 14:
					case 16:
					case 0:
					case 8:
					case 3:
					case 18:
						if ((data.Index % 2) == 0)
						{
							o->Rotation = (FWorldTime*0.02f*o->TurningForce[0]) + o->StartPosition[1];
						}
						else
						{
							o->Rotation = (FWorldTime*(-0.02f)*o->TurningForce[0]) + o->StartPosition[1];
						}
						break;
					}
					if (o->SubType == 8
						|| o->SubType == 9
						|| o->SubType == 20
						|| o->SubType == 21)
					{
						EVector3 Light;
						Light[0] = o->Light[0] * o->Alpha;
						Light[1] = o->Light[1] * o->Alpha;
						Light[2] = o->Light[2] * o->Alpha;
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, Light, o->Rotation);
					}
					else if (o->SubType == 17)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else if (o->SubType == 18)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else if (o->SubType == 19)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;

				case BITMAP_SPARK:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;

				case BITMAP_FLAME:
					if (o->SubType == 11)
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					else
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_CURSEDTEMPLE_EFFECT_MASKER:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				case BITMAP_SHINY + 6:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
						ETextureRenderProxy &extraProxy = TEXTURES::Proxy[BITMAP_LIGHT];
						RenderSprite(GetEffectInstanceBuffer(extraProxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eParticles, 1>(data.Index)), BITMAP_LIGHT, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_SMOKELINE2:
					{
						RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					}
					break;
				case BITMAP_TRUE_FIRE:
				default:
					RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->TexType, o->Position, Width, Height, o->Light, o->Rotation);
					break;
				}
			}
			break;

		case EEffectProxyEnum::eBlurs:
			{
				MUBlur *b = &g_Blur[data.Index];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[b->Texture];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(b->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eBlurs, 0>(data.Index);

				for (mu_uint32 j = 0, l = b->BeginTails; j < b->Number - 1; ++j, l = (l + 1) % MAX_BLUR_TAILS)
				{
					mu_uint32 l2 = (l + 1) % MAX_BLUR_TAILS;

					RenderBlur(GetEffectInstanceBuffer(instanceIndex++), b->Texture, b, l, l2, j);
				}
			}
			break;

		case EEffectProxyEnum::eObjectBlurs:
			{
				MUObjectBlur *b = &g_ObjectBlur[data.Index];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[b->Texture];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(b->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eObjectBlurs, 0>(data.Index);

				for (mu_uint32 j = 0, l = b->BeginTails; j < b->Number - 1; ++j, l = (l + 1) % MAX_OBJECTBLUR_TAILS)
				{
					static constexpr mu_float Data = 300.0f;

					mu_uint32 l2 = (l + 1) % MAX_OBJECTBLUR_TAILS;

					if (b->SubType == 113 || b->SubType == 114)
					{
						if (EMath::Abs(b->p1[l][0] - b->p1[l2][0]) > Data ||
							EMath::Abs(b->p1[l][1] - b->p1[l2][1]) > Data ||
							EMath::Abs(b->p1[l][2] - b->p1[l2][2]) > Data ||
							EMath::Abs(b->p1[l][0] - b->p2[l2][0]) > Data ||
							EMath::Abs(b->p1[l][1] - b->p2[l2][1]) > Data ||
							EMath::Abs(b->p2[l][2] - b->p2[l2][2]) > Data)
						{
							continue;
						}
					}

					RenderObjectBlur(GetEffectInstanceBuffer(instanceIndex++), b->Texture, b, l, l2, j);
				}
			}
			break;

		case EEffectProxyEnum::ePointers:
			{
				MUPointer *o = &g_Pointers[data.Index];
				ETextureRenderProxy &proxy = TEXTURES::Proxy[o->Type];
				mu_uint32 instanceIndex = proxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::ePointers, 0>(data.Index);
				RenderTileSprite(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Rotation);
			}
			break;
		}
	}

	const mu_uint32 spritesCount = g_SpritesRenderCount;
	for (mu_uint32 i = ThreadIndex; i < spritesCount; i += _ThreadsCount)
	{
		MUSprite *o = &g_Sprites[i];

		const mu_uint32 RenderType = GetSpriteRenderType(o->Type, o->SubType);
		MUTexture &texture = TEXTURES::Data[o->Type];
		ETextureRenderProxy &proxy = TEXTURES::Proxy[o->Type];
		mu_uint32 instanceIndex = proxy.GetInstancesModeBase(RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eSprites, 0>(i);

		mu_float Width = texture.GetWidth() *o->Scale;
		mu_float Height = texture.GetHeight()*o->Scale;

		if (o->Type == BITMAP_FORMATION_MARK)
		{
			mu_float u = 0.0f, v = 0.0f, uw = 0.33f, vw = 0.33f;

			switch (o->SubType)
			{
			case 0:
				u = 0.0f; v = 0.0f;
				break;

			case 1:
				u = 0.33f; v = 0.0f;
				break;

			case 2:
				u = 0.66f; v = 0.0f;
				break;

			case 3:
				u = 0.0f; v = 0.33f;
				break;

			case 4:
				u = 0.33f; v = 0.33f;
				break;

			case 5:
				u = 0.66f; v = 0.33f;
				break;

			case 6:
				u = 0.0f; v = 0.66f;
				break;

			case 7:
				u = 0.33f; v = 0.66f;
				break;
			}

			RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, 64, 64, o->Light, o->Rotation, u, v, uw, vw);
		}
		else
		{
			RenderSprite(GetEffectInstanceBuffer(instanceIndex), o->Type, o->Position, Width, Height, o->Light, o->Rotation);
		}
	}

	const mu_uint32 tileSpritesCount = g_TileSpritesRenderCount;
	for (mu_uint32 i = ThreadIndex; i < tileSpritesCount; i += _ThreadsCount)
	{
		const MUTileSprite *o = &g_TileSprites[i];
		ETextureRenderProxy &proxy = TEXTURES::Proxy[o->Type];
		mu_uint32 instanceIndex = proxy.GetInstancesModeBase(o->RenderType) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eTileSprites, 0>(i);
		RenderTileSprite(GetEffectInstanceBuffer(instanceIndex), o->Type, o->PositionX, o->PositionY, o->SizeX, o->SizeY, o->Light, o->Rotation, o->Height);
	}

	const mu_uint32 guildSpritesCount = g_GuildSpritesRenderCount;
	for (mu_uint32 i = ThreadIndex; i < guildSpritesCount; i += _ThreadsCount)
	{
		MUGuildSprite *o = &g_GuildSprites[i];
		ETextureRenderProxy &proxy = TEXTURES::Proxy[o->Type];
		mu_uint32 instanceIndex = proxy.GetInstancesModeBase(BLEND::TXB_ALPHATEST_BEGIN) + MURenderProxyEffects::GetInstanceIndex<EEffectProxyEnum::eGuildSprites, 0>(i);
		RenderGuildSprite(GetEffectInstanceBuffer(instanceIndex), o);
	}
}

template<mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderPreTextures()
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisablePreTextureRender == true)
		return;
#endif

	// Depth rendering objects
	const mu_uint32 texturesCount = g_PreTexturesCulledCount;
	if (texturesCount > 0)
	{
		EVideoPipeline::BindGraphicsEffectTerrainData<backendType>();
		EVideoPipeline::BindGraphicsEffectInstancesBuffer<backendType>();
		EVideoPipeline::BindGraphicsEffectSamplers<backendType>();
		
		ECombinedShader programShader;
		const ERasterizer rasterizer = ERasterizer::eNoCull;
		EBlendState blendState;
		EDepthStencil depthStencil;

		mu_uint32 lastTexture = (mu_uint32)-1;
		for (mu_uint32 index = 0; index < texturesCount; ++index)
		{
			CULLED_DATA &data = g_PreTexturesCulledList[index];
			MUTexture &texture = TEXTURES::Data[data.TexType];
			if (data.TexType >= BITMAP_BEGIN_RUNTIME &&
				texture.IsLoaded() == false)
			{
				if (lastTexture != data.TexType)
				{
					lastTexture = data.TexType;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[data.TexType]);
					}
				}
				continue;
			}

			if (lastTexture != data.TexType)
			{
				lastTexture = data.TexType;
			}

			if (data.RenderType >= BLEND::TXB_ALPHATEST_BEGIN &&
				data.RenderType <= BLEND::TXB_ALPHATEST_END)
			{
				blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				switch (data.RenderType)
				{
				case BLEND::TXB_ALPHATEST_NOMASK:
				case BLEND::TXB_ALPHATEST_NONDEPTH:
					{
						blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
						depthStencil = EDepthStencil::eNoDepthMask;
					}
					break;
				}
			}

			programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eEffectEZ : ECombinedShader::eEffect;

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);
			EVideoPipeline::BindGraphicsEffectTexture<backendType>(&texture);

			ETextureRenderProxy &proxy = TEXTURES::Proxy[data.TexType];
			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(0, 6, proxy.GetInstancesModeBase(data.RenderType), proxy.GetInstancesModeCount(data.RenderType), ETopologyType::eTriangleList, EInputLayout::eEffect);
		}

		EVideoPipeline::UnbindVAO();
	}
}

template<mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderPostTextures()
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisablePostTextureRender == true)
		return;
#endif

	// Depth rendering objects
	const mu_uint32 texturesCount = g_PostTexturesCulledCount;
	if (texturesCount > 0)
	{
		EVideoPipeline::BindGraphicsEffectTerrainData<backendType>();
		EVideoPipeline::BindGraphicsEffectInstancesBuffer<backendType>();
		EVideoPipeline::BindGraphicsEffectSamplers<backendType>();

		ECombinedShader programShader;
		const ERasterizer rasterizer = ERasterizer::eNoCull;
		EBlendState blendState;
		EDepthStencil depthStencil;

		mu_uint32 lastTexture = (mu_uint32)-1;
		for (mu_uint32 index = 0; index < texturesCount; ++index)
		{
			CULLED_DATA &data = g_PostTexturesCulledList[index];
			MUTexture &texture = TEXTURES::Data[data.TexType];
			if (data.TexType >= BITMAP_BEGIN_RUNTIME &&
				texture.IsLoaded() == false)
			{
				if(lastTexture != data.TexType)
				{
					lastTexture = data.TexType;
					if(texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[data.TexType]);
					}
				}
				continue;
			}
			
			if(lastTexture != data.TexType)
			{
				lastTexture = data.TexType;
			}

			switch (data.RenderType)
			{
			case BLEND::TXB_BLEND01:
				{
					blendState = EBlendState::eOne_One;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;

			case BLEND::TXB_BLENDMINUS:
				{
					blendState = EBlendState::eZero_InvColorSrc;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;

			case BLEND::TXB_BLEND02:
				{
					blendState = EBlendState::eInvColorSrc_One;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;

			case BLEND::TXB_BLEND03:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;

			case BLEND::TXB_BLEND04:
				{
					blendState = EBlendState::eOne_InvColorSrc;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;

			case BLEND::TXB_LIGHTMAP:
				{
					blendState = EBlendState::eZero_ColorSrc;
					depthStencil = EDepthStencil::eNoDepthMask;
				}
				break;
			}

			programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eEffectEZ : ECombinedShader::eEffect;

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);
			EVideoPipeline::BindGraphicsEffectTexture<backendType>(&texture);

			ETextureRenderProxy &proxy = TEXTURES::Proxy[data.TexType];
			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(0, 6, proxy.GetInstancesModeBase(data.RenderType), proxy.GetInstancesModeCount(data.RenderType), ETopologyType::eTriangleList, EInputLayout::eEffect);
		}

		EVideoPipeline::UnbindVAO();
	}
}