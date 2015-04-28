#include "stdafx.h"
#include "BeatmapWeb.h"


using namespace std;


CBeatmapWeb::CBeatmapWeb( )
{
}


CBeatmapWeb::~CBeatmapWeb( )
{
}

void CBeatmapWeb::StartBeatmapWebThread( )
{
	m_threadWebThread = thread( [ this ]( ){
		//=> Startup web interface
		CoInitialize( nullptr );


		wchar_t szBuff[ 512 ];

		HRESULT hr;
		ComPtr<IXMLHTTPRequest> request;
		hr = ::CoCreateInstance( CLSID_XMLHTTP60, NULL, CLSCTX_ALL, __uuidof( IXMLHTTPRequest ), ( void** )&request );

		DWORD dwNumBytes = 0;
		ULONG_PTR ptrKey;
		LPOVERLAPPED	ov;
		while( true )
		{
			GetQueuedCompletionStatus( m_hQueryBm, &dwNumBytes, &ptrKey, &ov, INFINITE );
			if( dwNumBytes == -1 )
			{
				break;
			}
			


		}

		CoUninitialize( );
	} );
}

void CBeatmapWeb::StopBeatmapWebThread( )
{
	PostQueuedCompletionStatus( m_hQueryBm, -1, 0, nullptr );
	m_threadWebThread.join( );
}