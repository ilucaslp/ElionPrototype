#ifndef __MU_SHADER_LIST_H__
#define __MU_SHADER_LIST_H__

#pragma once

struct ECombinedShaderData
{
	ECombinedShaderData(const EVertexShader vertex,
		const EFragmentShader fragment,
		const EGeometryShader geometry, 
		const EInputLayout inputLayout,
		const EUniformLayout uniformLayout,
		const ETopologyType topologyType,
		const ERenderPass renderPass,
		const mu_boolean early) :
		Vertex(vertex),
		Fragment(fragment),
		Geometry(geometry),
		InputLayout(inputLayout),
		UniformLayout(uniformLayout),
		TopologyType(topologyType),
		RenderPass(renderPass),
		Early(early)
	{
	}

	const EVertexShader Vertex;
	const EFragmentShader Fragment;
	const EGeometryShader Geometry;
	const EInputLayout InputLayout;
	const EUniformLayout UniformLayout;
	const ETopologyType TopologyType;
	const ERenderPass RenderPass;
	const mu_boolean Early;
};

extern ECombinedShaderData CombinedShaderList[ECombinedShaderMax];

struct EShaderData
{
	EShaderData(const mu_text *path,
		const mu_boolean early,
		const EInputLayout inputLayout = EInputLayout::eUIDefault) :
		Path(path),
		Early(early),
		InputLayout(inputLayout)
	{
	}

	const mu_text *Path;
	const mu_boolean Early;
	const EInputLayout InputLayout;
};

extern EShaderData VertexShaderData[EVertexShaderMax];
extern EShaderData FragmentShaderData[EFragmentShaderMax];
extern EShaderData GeometryShaderData[EGeometryShaderMax];

#endif