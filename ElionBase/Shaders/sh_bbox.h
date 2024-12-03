#ifndef __SHADER_CONTAINERS_H__
#define __SHADER_CONTAINERS_H__

#pragma once

MU_ALIGN(4, struct) SHContainersVertex
{
	EVector3 Pos;
	EFloat Color;
};

#endif