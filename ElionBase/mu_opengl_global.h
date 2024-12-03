#ifndef __MU_OPENGL_GLOBAL_H__
#define __MU_OPENGL_GLOBAL_H__

#pragma once

namespace EOpenGL
{
	extern mu_uint32 CurrentCommandBufferIndex;

	GLenum RetrieveTopology(const ETopologyType Topology);
	GLenum RetrieveIndexType(const EIndexType IndexType);
	const mu_size RetrieveIndexTypeSize(const EIndexType IndexType);
};

#endif