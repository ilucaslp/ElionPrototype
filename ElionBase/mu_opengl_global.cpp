#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGL
{
	mu_uint32 CurrentCommandBufferIndex = 0;

	GLenum TopologyList[ETopologyTypeMax] =
	{
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
	};
	GLenum RetrieveTopology(const ETopologyType Topology)
	{
		return TopologyList[ECastEnum(Topology)];
	}

	GLenum IndexTypeList[EIndexTypeMax] =
	{
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_INT,
	};
	GLenum RetrieveIndexType(const EIndexType IndexType)
	{
		return IndexTypeList[ECastEnum(IndexType)];
	}

	mu_size IndexTypeSizeList[EIndexTypeMax] =
	{
		sizeof(mu_uint16),
		sizeof(mu_uint32),
	};
	const mu_size RetrieveIndexTypeSize(const EIndexType IndexType)
	{
		return IndexTypeSizeList[ECastEnum(IndexType)];
	}
};
#endif