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


		while( true )
		{
			if( WaitForSingleObject( m_hKillBmWeb, 1000 ) == WAIT_OBJECT_0 )
				break;

			sBeatmapQuery* pQuery;
			if( m_queueQuery.try_pop( pQuery ) )
			{
				swprintf_s( szBuff, L"https://osu.ppy.sh/s/%d", pQuery->m_nBeatmapSetId );


				//=> Post web request!
				hr = request->open( _bstr_t( L"GET" ), _bstr_t( szBuff ), _variant_t( VARIANT_FALSE ), _variant_t( ), _variant_t( ) );
				if( SUCCEEDED( hr ) )
				{
					hr = request->send( _variant_t( ) );

					long status;
					hr = request->get_status( &status );
					if( status == 200 )
					{
						BSTR szData;
						if( SUCCEEDED( hr = request->get_responseText( &szData ) ) )
						{
							wstring szBmQuery = GetBeatmapFromSetId( szData, pQuery );
							if( szBmQuery != L"" )
							{
								SysFreeString( szData );


								m_cs.lock( );
								{
									for( auto i : m_vecCallback )
										i( pQuery );
								}
								m_cs.unlock( );
							}	
						}
					}
				}
				else
				{
					Log( L"Error | " __FUNCTIONW__ L" | Failed to open IXMLHTTPRequest hr: [ %x ]", hr );
					SAFE_DELETE( pQuery );

					continue;
				}
			}
		}

		request.Clear( );
		CoUninitialize( );
	} );
}

std::wstring CBeatmapWeb::GetBeatmapFromSetId( BSTR szData, sBeatmapQuery* pQuery )
{
	wstring szHtml = szData;

	while( true )
	{
		auto itBegin = szHtml.find( L"<li><a class='beatmapTab " );
		auto itEnd = szHtml.find( L"</span></a></li>" );
		if( itBegin != wstring::npos )
		{
			szHtml.erase( szHtml.begin( ), szHtml.begin( ) + itBegin + ( itBegin == 0 ? itEnd + 16 : 0 ) );

			auto itEndLi = szHtml.find( L"</span></a></li>" );
			if( itEndLi != wstring::npos )
			{
				wstring szLine = wstring( szHtml.begin( ), szHtml.begin( ) + itEndLi );
				if( szLine.find( pQuery->m_szDifficulty ) != wstring::npos )
				{
					auto itHref = szLine.find( L"href='/b/" );
					auto itHrefEnd = szLine.find( L"'><div", itHref );

					if( itHref != wstring::npos && itHrefEnd != wstring::npos )
					{
						return wstring( szLine.begin( ) + itHref + 6, szLine.begin( ) + itHrefEnd );
					}
				}
			}

		}
		else break;
	}

	return L"";
}

void CBeatmapWeb::StopBeatmapWebThread( )
{
	SetEvent( m_hKillBmWeb );
	m_threadWebThread.join( );
}

