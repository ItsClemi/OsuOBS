#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=> Defines

#define OBSEXPORT extern "C" __declspec(dllexport)

#ifndef SAFE_DELETE
#define SAFE_DELETE( x ) if( x != nullptr ){ delete( x ); x = nullptr; } 
#endif

#define DllhInstance (HINSTANCE)&__ImageBase;
#define async

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=> Include

#include <OBSApi.h>


//=> C Runtime
#include <TlHelp32.h>
#include <oleauto.h>
#include <comcat.h>
#include <stddef.h>
#include <olectl.h>


//=> Std
#include <string>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <filesystem>
#include <hash_map>
#include <thread>	
#include <array>
#include <iostream>


//=> PPL
#include <ppl.h>
#include <ppltasks.h>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>


//=> Gdi+
#include <gdiplus.h>

//=> WebAPI (COM)
#include <InitGuid.h>
#include <msxml6.h>
#include <comutil.h>
#include <comdef.h>

//=> Own stuff

#include "WindowsHelper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=> Libs

#pragma comment( lib, "OBSApi.lib" )
#pragma comment( lib, "gdiplus.lib" )
#pragma comment( lib, "msxml6.lib" )


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=> Global

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

extern HINSTANCE	g_hInstance;




