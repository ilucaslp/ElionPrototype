#ifndef __MU_AUDIO_ENUM_H__
#define __MU_AUDIO_ENUM_H__

#pragma once

enum class EAudioBackendType : mu_int32
{
	eNone		= -1,
	eWindows	= 1,	// XAudio2
	eLinux		= 2,	// ???
	eAndroid	= 3,	// Oboe (OpenSLES or AAudio)
	eMacOS		= 4,	// ???
	eiOS		= 5,	// ???
	eMax,
};

#endif