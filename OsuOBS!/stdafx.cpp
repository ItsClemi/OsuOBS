#include "StdAfx.h"

HINSTANCE	g_hInstance = nullptr;



namespace{
	ULONG_PTR m_gdiplusToken;
}

BOOL CALLBACK DllMain( _In_ HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		g_hInstance = hInstance;

		InitColorControl( g_hInstance );
		
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL );
	}
	else if( fdwReason == DLL_PROCESS_DETACH )
	{
		Gdiplus::GdiplusShutdown( m_gdiplusToken );
		
	}

	return TRUE;
}
