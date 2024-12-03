#include "stdafx.h"
#include "mu_renderslot.h"

MURenderSlot g_PreRenderSlot[MAX_RENDERSLOT];
MURenderSlot g_PostRenderSlot[MAX_RENDERSLOT];
MUClothRenderSlot g_ClothRenderSlot[MAX_CLOTH_RENDERSLOT];
MUGUIRenderSlot g_GUIRenderSlot[MAX_GUIRENDERSLOT];
mu_uint32 g_PreRenderSlotUsed[MAX_RENDERSLOT]; // Fast Arrange
mu_uint32 g_ClothRenderSlotUsed[MAX_CLOTH_RENDERSLOT]; // Fast Arrange
mu_uint32 g_GUIRenderSlotUsed[MAX_GUIRENDERSLOT]; // Fast Arrange
mu_atomic_uint32_t g_PreRenderSlotCount(0);
mu_atomic_uint32_t g_PostRenderSlotCount(0);
mu_atomic_uint32_t g_ClothRenderSlotCount(0);
mu_uint32 g_GUIRenderSlotCount = 0;