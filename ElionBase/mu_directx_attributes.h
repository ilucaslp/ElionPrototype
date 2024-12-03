#ifndef __MU_DIRECTX_ATTRIBUTES_H__
#define __MU_DIRECTX_ATTRIBUTES_H__

#pragma once

namespace EDirectXAttributes
{
	void Retrieve(const EInputLayout layout,
		mu_uint32 &count,
		D3D11_INPUT_ELEMENT_DESC *&attributes);
};

#endif