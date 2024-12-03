#ifndef __STDAFX_H__
#define __STDAFX_H__

#pragma once

#define USE_DIRECTX

#define ENABLE_CRASHRPT

#if defined(INTERNAL_SERVER)
//#define USE_VLD
//#define USE_CRTLEAK
#endif

#ifdef USE_CRTLEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>  
#include <crtdbg.h>  
#endif

#ifdef USE_SDL2
#pragma comment(lib, "SDL2.lib")
#endif

#ifdef _M_X64
#ifdef ENABLE_CRASHRPT
#pragma comment(lib, "CrashRpt1403_x64.lib")
#endif
#ifdef _DEBUG
#pragma comment(lib, "CryptBlowfishd_x64.lib")
#pragma comment(lib, "CRC32d_x64.lib")
#pragma comment(lib, "ElionMathd_x64.lib")
#pragma comment(lib, "CommonLib_Debug_x64.lib")
#pragma comment(lib, "zlibd_x64.lib")
#pragma comment(lib, "portaudiod_x64.lib")
#pragma comment(lib, "liboggd_x64.lib")
#pragma comment(lib, "libvorbisd_x64.lib")
#pragma comment(lib, "libvorbisfiled_x64.lib")
#else
#pragma comment(lib, "CryptBlowfish_x64.lib")
#pragma comment(lib, "CRC32_x64.lib")
#pragma comment(lib, "ElionMath_x64.lib")
#pragma comment(lib, "CommonLib_Release_x64.lib")
#pragma comment(lib, "zlib_x64.lib")
#pragma comment(lib, "portaudio_x64.lib")
#pragma comment(lib, "libogg_x64.lib")
#pragma comment(lib, "libvorbis_x64.lib")
#pragma comment(lib, "libvorbisfile_x64.lib")
#endif
#else
#ifdef ENABLE_CRASHRPT
#pragma comment(lib, "CrashRpt1403_x86.lib")
#endif
#ifdef _DEBUG
#pragma comment(lib, "CryptBlowfishd_x86.lib")
#pragma comment(lib, "CRC32d_x86.lib")
#pragma comment(lib, "ElionMathd_x86.lib")
#pragma comment(lib, "CommonLib_Debug_x86.lib")
#pragma comment(lib, "zlibd_x86.lib")
#pragma comment(lib, "portaudiod_x86.lib")
#pragma comment(lib, "liboggd_x86.lib")
#pragma comment(lib, "libvorbisd_x86.lib")
#pragma comment(lib, "libvorbisfiled_x86.lib")
#else
#pragma comment(lib, "CryptBlowfish_x86.lib")
#pragma comment(lib, "CRC32_x86.lib")
#pragma comment(lib, "ElionMath_x86.lib")
#pragma comment(lib, "CommonLib_Release_x86.lib")
#pragma comment(lib, "zlib_x86.lib")
#pragma comment(lib, "portaudio_x86.lib")
#pragma comment(lib, "libogg_x86.lib")
#pragma comment(lib, "libvorbis_x86.lib")
#pragma comment(lib, "libvorbisfile_x86.lib")
#endif
#endif

#if !defined(USE_VLD) && !defined(USE_CRTLEAK)
#define __TBB_NO_IMPLICIT_LINKAGE 1
#define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#ifdef _DEBUG
#pragma comment(lib, "tbb_debug.lib")
#pragma comment(lib, "tbbmalloc_debug.lib")
#pragma comment(lib, "tbbmalloc_proxy_debug.lib")
#else
#pragma comment(lib, "tbb.lib")
#pragma comment(lib, "tbbmalloc.lib")
#pragma comment(lib, "tbbmalloc_proxy.lib")
#endif
#endif

// Math Library
#include <EMathPCH.h>

#ifdef USE_SDL2
#include <SDL.h>
#endif

#ifndef USE_SDL2
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOATOM
#define NODRAWTEXT
//#define NOGDI
#define NOKERNEL
#define NOMEMMGR
#define NOMINMAX
#define NOSERVICE
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOHELP
#define NOPROFILER
#define NOMCX

// Windows Header
#include "resource.h"
#include <windows.h>
#include <shellapi.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <objbase.h>
#include <mmreg.h>
#include <audioclient.h>
#else
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOATOM
#define NODRAWTEXT
//#define NOGDI
#define NOKERNEL
#define NOMEMMGR
#define NOMINMAX
#define NOSERVICE
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOHELP
#define NOPROFILER
#define NOMCX

// Windows Header
#include "resource.h"
#include <windows.h>
#include <shellapi.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <objbase.h>
#include <mmreg.h>
#include <audioclient.h>
#endif

// STL
#include <string>
#include <array>
#include <bitset>
#include <tchar.h>
#include <atomic>
#include <map>
#include <list>
#include <algorithm>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <deque>
#include <limits>
#include <memory>
#include <bitset>

#ifdef ENABLE_CRASHRPT
#include <CrashRpt.h>
#endif

#if !defined(USE_VLD) && !defined(USE_CRTLEAK)
// TBB
#include <tbb\tbbmalloc_proxy.h>
#include <tbb\tbb.h>
#endif

// Freetype 2
#include <ft2build.h>

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>

// XAudio2
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <x3daudio.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "xaudio2.lib")
#else
// DirectX 11
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\d3d11.h>
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\d3dcompiler.h>

// XAudio2
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\xaudio2.h>
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\xaudio2fx.h>
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\xapofx.h>
#include <C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\x3daudio.h>

#ifdef _M_X64
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\d3d11.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\d3dcompiler.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\x3daudio.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\xapofx.lib")
#else
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3d11.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dcompiler.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\x3daudio.lib")
//#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\xapofx.lib")
#endif
#endif

#pragma comment(lib, "DXGI.lib")

// PortAudio and Vorbis
#include <..\zlib\zlib.h>
#include <portaudio.h>
#include <ogg\ogg.h>
#include <vorbis\codec.h>
#include <vorbis\vorbisfile.h>

// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include FT_LCD_FILTER_H

// HarfBuzz
#include <hb.h>
#include <hb-ft.h>

// Link library dependencies
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "usp10.lib")
#pragma comment(lib, "rpcrt4.lib")

#ifdef _DEBUG
	#ifdef _M_X64
		#pragma comment(lib, "freetyped_x64.lib")
		#pragma comment(lib, "harfbuzzd_x64.lib")
	#else
		#pragma comment(lib, "freetyped_x86.lib")
		#pragma comment(lib, "harfbuzzd_x86.lib")
	#endif
#else
	#ifdef _M_X64
		#pragma comment(lib, "freetype_x64.lib")
		#pragma comment(lib, "harfbuzz_x64.lib")
	#else
		#pragma comment(lib, "freetype_x86.lib")
		#pragma comment(lib, "harfbuzz_x86.lib")
	#endif
#endif

#include <thread>
#include <future>
#include <regex>

// Hashing
#include <mu_crc32.h>

// Network
#ifdef _DEBUG
	#ifdef _M_X64
		#pragma comment(lib, "ElionNetworkud_x64.lib")
	#else
		#pragma comment(lib, "ElionNetworkud_x86.lib")
	#endif
#else
	#ifdef _M_X64
		#pragma comment(lib, "ElionNetworku_x64.lib")
	#else
		#pragma comment(lib, "ElionNetworku_x86.lib")
	#endif
#endif

// Standard types
#include "mu_standard_types.h"
#include "mu_functions_helper.h"
#include "mu_misc.h"
#include "mu_math_helper.h"
#include "mu_random.h"
#include "mu_fpstimer.h"
#include "mu_timecheck.h"
#include "mu_timer.h"
#include "mu_console.h"
#include "mu_resolutioninfo.h"
#include "mu_sha512.h"

// Defines
#ifdef _M_X64
#define ELION_APPNAME _T("Elion_64bits")
#else
#define ELION_APPNAME _T("Elion_32bits")
#endif
//#define INTERNAL_SERVER
#ifdef INTERNAL_SERVER
#define ELION_VERSION _T("0.0.0.24") // VersionMajor.VersionMinor.Status.Revision (Status : 0 (Alpha), 1 (Beta), 2 (Release Candidate), 3 (Final Release)
#define ELION_SERIAL _T("nmdk@*34r489NF(*n4nifHJ($3nifunido$(88h9#H93d9nfnndno3o8d83(F)&")
#else
#define ELION_VERSION _T("0.1.1.28") // VersionMajor.VersionMinor.Status.Revision (Status : 0 (Alpha), 1 (Beta), 2 (Release Candidate), 3 (Final Release)
#define ELION_SERIAL _T("nmdk@*34r489NF(*n4nifHJ($3nifunido$(88h9#H93d9nfnndno3o8d83(F)&")
#endif
#define ELION_COMPILETIME _T(__DATE__)

#ifdef INTERNAL_SERVER
#define SERVER_ADDRESS _T("connect.elion.mu")
//#define SERVER_ADDRESS _T("127.0.0.1")
#else
#define SERVER_ADDRESS _T("connect.elion.mu")
#endif
#define SERVER_PORT 30000

#ifdef ENABLE_CRASHRPT
#define CRASHRPT_AUTOTHREAD CrThreadAutoInstallHelper __crashInit
#else
#define CRASHRPT_AUTOTHREAD
#endif

//#define _SERVER_USAGE_
// Common Library
#include <mu_criticalsection.h>
#include <mu_criticalsectionspinlock.h>
#include <mu_rwlock.h>
#include <mu_threadbarrier.h>
#include <mu_threadevent.h>
#include <mu_readscript.h>
#include <mu_bitset.h>

#include <NetworkStd.h>
#include <shared_protocol.h>
#include <shared_define.h>

// Defines and Enums
#include "mu_define.h"
#include "mu_enum.h"
#include "mu_inline.h"
#include "mu_variables.h"

#include "cltocs_protocol.h"
#include "cltogs_protocol.h"
#include "mu_networkmanager.h"
#include "cs_networkpacket.h"
#include "cs_networkobject.h"
#include "gs_networkpacket.h"
#include "gs_networkobject.h"

// Main Structs
#include "mu_globaltext.h"
#include "mu_itemattribute.h"
#include "mu_itemcategoriesdb.h"
#include "mu_skillattribute.h"
#include "mu_gateattribute.h"
#include "mu_setitemoption.h"
#include "mu_itemaddoption.h"
#include "mu_jewelharmonyinfo.h"
#include "mu_socketsystem.h"
#include "mu_monster.h"
#include "mu_classattribute.h"
#include "mu_characterattribute.h"
#include "mu_guild.h"
#include "mu_party.h"
#include "CComGem.h"
#include "mu_changeringmanager.h"
#include "mu_movemanager.h"
#include "mu_mixmanager.h"

// Interface
#include "mu_interface.h"
#include "mu_information.h"

// AI
#include "mu_ai.h"
#include "mu_path.h"

// Containers
#include "mu_list.h"
#include "mu_textureatlas.h"

// Font Manager
#include "mu_fontcache.h"
#include "mu_text.h"

// Window Manager
#include "mu_windowmanager.h"

// Shaders
#include "Shaders\sh_uidefault.h"
#include "Shaders\sh_normalfont.h"
#include "Shaders\sh_terrain.h"
#include "Shaders\sh_model.h"
#include "Shaders\sh_containers.h"
#include "Shaders\sh_effects.h"

// Video Manager
#include "mu_shader.h"
#include "mu_blendstate.h"
#include "mu_depthstencil.h"
#include "mu_sampler.h"
#include "mu_rasterizer.h"
#include "mu_videomanager.h"

// Audio Manager
#include "mu_audiomanager.h"
#include "mu_wavdata.h"
#include "mu_audiosource.h"

// Music Manager
#include "mu_musicmanager.h"

// Input Manager
#include "mu_inputmanager.h"

// Camera
#include "mu_camera.h"

// Texture
#include "mu_texture.h"
#include "mu_textureindex.h"

// Files Header Data
#include "mu_fileheader.h"

// Culling sytems
#include "mu_cpuculling.h"

// Model
#include "mu_model.h"

// Terrain
#include "mu_terrain.h"
#include "mu_waterterrain.h"

// Dialog
#include "mu_dialogmanager.h"

// Quest
#include "mu_questmanager.h"

// Events
#include "mu_event.h"

// Maps
#include "mu_mapprocess.h"
#include "CSChaosCastle.h"
#include "GMHellas.h"
#include "GMBattleCastle.h"
#include "GMHuntingGround.h"
#include "GMAida.h"
#include "GMCrywolf1st.h"
#include "GM_Kanturu_1st.h"
#include "GM_Kanturu_2nd.h"
#include "GM_Kanturu_3rd.h"
#include "GM3rdChangeUp.h"
#include "w_CursedTemple.h"
#include "GMNewTown.h"
#include "GMSwampOfQuiet.h"
#include "GM_Raklion.h"
#include "GMSantaTown.h"
#include "GM_PK_Field.h"
#include "GMDuelArena.h"
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#include "GMEmpireGuardian1.h"
#include "GMEmpireGuardian2.h"
#include "GMEmpireGuardian3.h"
#include "GMEmpireGuardian4.h"
#include "GMUnitedMarketPlace.h"

// Render Proxy
#include "mu_renderproxy.h"
#include "mu_renderslot.h"

// Resource Manager
#include "mu_loadingdata.h"
#include "mu_resourcemanager.h"
#include "mu_loadingmanager.h"
#include "mu_shaderloader.h"

// Objects (Environment, Characters, Effects, etc...)
#include "mu_objectbase.h"
#include "mu_characters.h"
#include "mu_item.h"
#include "mu_charactermachine.h" 
#include "mu_environmentobject.h"
#include "mu_operates.h"
#include "mu_blurs.h"
#include "mu_effects.h"
#include "mu_sprites.h"
#include "mu_tilesprites.h"
#include "mu_guildsprites.h"
#include "mu_particles.h"
#include "mu_leaves.h"
#include "mu_pointers.h"
#include "mu_points.h"
#include "mu_joints.h"
#include "mu_skillprocessor.h"
#include "mu_petsystem.h"
#include "mu_petmanager.h"
#include "mu_parts.h"
#include "SummonSystem.h"
#include "mu_duelmanager.h"
#include "mu_petprocess.h"

// Physics
#include "mu_physicsmanager.h"

// Thread Information
#include "mu_threadinfo.h"

// Graphic User-Interface (GUI)
// Base
#include "mu_uibase.h"

// Containers
#include "mu_uicontainer.h"

// Widgets
#include "mu_uisprite.h"
#include "mu_uispriteuv.h"
#include "mu_uispritecolor.h"
#include "mu_uibutton.h"
#include "mu_uioverbutton.h"
#include "mu_uitogglebutton.h"
#include "mu_uiradiobutton.h"
#include "mu_uislidevertical.h"
#include "mu_uislidesection.h"
#include "mu_uieditbox.h"
#include "mu_uimultilineeditbox.h"
#include "mu_uiitemmanager.h"
#include "mu_uipickeditem.h"
#include "mu_uiinventorycontrol.h"
#include "mu_uicharacterinfoballoon.h"
#include "mu_uiscrollbar.h"
#include "mu_uisliderbar.h"

// Helpers
#include "mu_uihelpers_general.h"
#include "mu_uihelpers_msgbox.h"
#include "mu_uihelpers_genericlayout.h"

#include "mu_uiservermessage.h"

// Game UI
#include "mu_uimainframe.h"
#include "mu_uiitemhotkey.h"
#include "mu_uiskillframe.h"
#include "mu_uicharacterinfo.h"
#include "mu_uiquestwindow.h"
#include "mu_uinpcquestwindow.h"
#include "mu_uimyinventory.h"
#include "mu_uimixinventory.h"
#include "mu_uistorageinventory.h"
#include "mu_uishopinventory.h"
#include "mu_uitradeinventory.h"
#include "mu_uipartywindow.h"
#include "mu_uipartylist.h"
#include "mu_uiguildwindow.h"
#include "mu_uiguildcreate.h"
#include "mu_uichatwindow.h"
#include "mu_uichatinput.h"
#include "mu_uimapname.h"
#include "mu_uisimplehpbar.h"
#include "mu_uisimplename.h"
#include "mu_uisimplechat.h"
#include "mu_uisimplenotice.h"
#include "mu_uibuffstatus.h"
#include "mu_uiheroposition.h"
#include "mu_uimovewindow.h"
#include "mu_uibloodcastletimer.h"
#include "mu_uibloodcastleenter.h"
#include "mu_uicommandwindow.h"
#include "mu_uihelpersetup.h"

// Login UI
#include "mu_uilogo.h"
#include "mu_uiserverlist.h"
#include "mu_uiloginwindow.h"
#include "mu_uiloginframe.h"

// Character UI
#include "mu_uicharacterframe.h"
#include "mu_uicreatecharacter.h"

// Options UI
#include "mu_uiloginmenu.h"
#include "mu_uicharactermenu.h"
#include "mu_uigamemenu.h"
#include "mu_uioptionwindow.h"

// Messages Box
#include "mu_uiokbox.h"
#include "mu_uifailedconnect.h"
#include "mu_uiserverfull.h"
#include "mu_uibannedfromserver.h"
#include "mu_uiblockedfromserver.h"
#include "mu_uiwaitingbox.h"
#include "mu_uiconnectbox.h"
#include "mu_uidisconnectedbox.h"
#include "mu_uideletecharacterwarning.h"
#include "mu_uiexitgamebox.h"
#include "mu_uichangeserverbox.h"
//#include "mu_uichangecharacterbox.h"
#include "mu_uiguildleavebox.h"
#include "mu_uiguildrequestbox.h"
#include "mu_uiguildcancelstatusbox.h"
#include "mu_uiguildgivestatusbox.h"
#include "mu_uipartyrequestbox.h"
#include "mu_uitraderequestbox.h"
#include "mu_uitrademoneybox.h"
#include "mu_uitradeacceptbox.h"
#include "mu_uistoragedepositbox.h"
#include "mu_uistoragewithdrawbox.h"
#include "mu_uistoragelockbox.h"
#include "mu_uihighvaluewarningbox.h"
#include "mu_uigamblebuybox.h"
#include "mu_uipcpointbuybox.h"
#include "mu_uichaosmixmenu.h"
#include "mu_uimixcheck.h"
#include "mu_uibloodcastlerank.h"
#include "mu_uihelperskillsetting.h"
#include "mu_uihelperpotionsetting.h"
#include "mu_uihelperelfpotionsetting.h"
#include "mu_uihelperpartysetting.h"
#include "mu_uihelperwizardpartysetting.h"
#include "mu_uihelperskillselect.h"
#include "mu_uihelperitemadd.h"
#include "mu_uihelperstart.h"

// Roots
#include "mu_uigameroot.h"
#include "mu_uiloginroot.h"
#include "mu_uicharacterroot.h"
#include "mu_uioptionroot.h"
#include "mu_uimessageroot.h"

// Events
#include "mu_chaoscastle.h"

// Direction
#include "mu_direction.h"

// Scenes
#include "mu_scenebase.h"
#include "mu_loadingscene.h"
#include "mu_loginscene.h"
#include "mu_characterscene.h"
#include "mu_waitingscene.h"
#include "mu_gamescene.h"
#include "mu_scenes.h"

// Multithreading
#include "mu_tasking.h"

// Root
#include "mu_root.h"

// Inline
#include "mu_skillattribute_inline.h"

#endif