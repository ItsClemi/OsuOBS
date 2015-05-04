#include "stdafx.h"
#include "UserInfo.h"
#include "Config.h"


using namespace std;

CUserInfo::CUserInfo( )
{
}


CUserInfo::~CUserInfo( )
{
}

void CUserInfo::StartUserThread( )
{
	m_threadUser = std::thread( [ this ]( ){
		CoInitialize( NULL );
		
		wchar_t szBuff[ 512 ];
		ULONGLONG dwNextUpdate = 0;


		HRESULT hr;
		ComPtr<IXMLHTTPRequest> request;
		hr = ::CoCreateInstance( CLSID_XMLHTTP60, NULL, CLSCTX_ALL, __uuidof( IXMLHTTPRequest ), ( void** )&request );

		while( true )
		{
			DWORD dwRes = WaitForSingleObject( m_hKillUserThread, 100 );
			if( dwRes == WAIT_OBJECT_0 )
				break;

			if( dwNextUpdate > GetTickCount64( ) )
			{
				//=> Process every 30s we don't need real time update here. 
				//=> But the thread needs to check if the user has stopped streaming
				
				if( m_bForceUpdate == false )
				{
					continue;
				}
				else
				{
					m_bForceUpdate = false;
				}
			}
			else
			{
				dwNextUpdate = GetTickCount64( ) + 30000;
			}


			UINT nUserId = CConfig::GetInstance()->GetUserId();
			eGameMode eMode = CConfig::GetInstance()->GetGameMode();
			
			//=> User id not set. don't waste performance and traffic on checking..
			if( nUserId == 0 )
			{
				continue;
			}
	

			//=> Generate random number so IE doesn't cache the web result.
			swprintf_s( szBuff, L"https://osu.ppy.sh/pages/include/profile-general.php?u=%d&m=%d?%d", nUserId, eMode, rand( ) );


			hr = request->open( _bstr_t( L"GET" ), _bstr_t( szBuff ), _variant_t( VARIANT_FALSE ), _variant_t( ), _variant_t( ) );
			if( FAILED( hr ) )
			{
				Log( L"Error | " __FUNCTIONW__ L" | Failed to open IXMLHTTPRequest hr: [ %x ]", hr );
				continue;
			}

			hr = request->send( _variant_t( ) );


			long status;
			hr = request->get_status( &status );
			
		
			if( status == 200 )
			{
				BSTR data;
				hr = request->get_responseText( &data );
				{
					wstring szHtml( data );

					{ 
						//=> JSON string would be so comfy :(
						
						wstring szHtmlTbl = wstring( szHtml );
						wstring szUserHref = wstring( L"<b><a href='/u/" + to_wstring( nUserId ) + L"'>" );

						m_nActivities = 0;
						for( ;; )
						{
							auto it = szHtmlTbl.find( szUserHref );
							if( it != wstring::npos )
							{
								szHtmlTbl.erase( szHtmlTbl.begin( ), szHtmlTbl.begin() + it + szUserHref.length() );

								auto itEnd = szHtmlTbl.find( L"</div></td></tr>" );
								m_aRecentActivity[ m_nActivities++ ] = wstring( szHtmlTbl.begin( ), szHtmlTbl.begin( ) + itEnd );
							}
							else break;
						}


						//=> remove html tags from the strings	
						for( size_t i = 0; i < m_nActivities; i++ )
						{
							wstring* sz = &m_aRecentActivity[ i ];

							auto fParser = [ &]( ){
								auto itB = sz->find( '<' );

								if( itB != wstring::npos )
								{
									auto itE = sz->find( '>', itB );
									{
										sz->erase( sz->begin( ) + itB, sz->begin( ) + itE + 1);
									}
									return true;
								}

								return false;
							};
						
							while( fParser( ) );
						}
					}


					auto it = szHtml.find( L"Performance</a>:" );
					auto itEnd = szHtml.find( L"</b>", it );

					m_cs.lock( );
					{
						if( it != wstring::npos && itEnd != wstring::npos )
						{
							m_szPerformance = wstring( szHtml.begin( ) + it + 16, szHtml.begin( ) + itEnd );
							m_szPerformance.insert( 0, L"Performance: " );
						}
						else
						{
							m_szPerformance = L"No information recorded!";
						}

						m_bNewData = true;
					}
					m_cs.unlock( );
				
				}
				SysFreeString( data );
			}
			else
			{
				m_cs.lock( );
				{
					m_szPerformance = L"Failed to query user data. Status: " + to_wstring( status );
				}
				m_cs.unlock( );
			}
		}	


		request.Clear( );
		CoUninitialize( );
	} );
}

void CUserInfo::StopUserThread( )
{
	SetEvent( m_hKillUserThread );
	m_threadUser.join( );
}
