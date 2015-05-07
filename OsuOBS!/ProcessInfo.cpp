#include "stdafx.h"
#include "ProcessInfo.h"
#include "Core.h"

using namespace std;


CProcessInfo::CProcessInfo( )
{
}

CProcessInfo::~CProcessInfo( )
{
	CloseHandle( m_hKillProcessInfo );
}

void CProcessInfo::StartProcessInfo( )
{
	m_threadInfo = thread( [ this ]( ){
		for( ;; )
		{
			if( WaitForSingleObject( m_hKillProcessInfo, 100 ) == WAIT_OBJECT_0 )
				break;

			if( m_bProcessAlive )
			{
				CheckBmChanged( );
			}
			else
			{
				TryFindProcess( );
			}
		}
	} );
}

void CProcessInfo::StopProcessInfo( )
{
	SetEvent( m_hKillProcessInfo );
	m_threadInfo.join( );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


async void CProcessInfo::CheckBmChanged( )
{
	lock_guard< mutex > l( m_cs );

	//=> Used to detect the change of the title
	static int s_nOldTitleLen = 0;


	wchar_t szBuffer[ 256 ] = { 0 };
	int n = GetWindowTextW( m_hWnd, szBuffer, ARRAYSIZE( szBuffer ) - 1 );
	if( n > 0 && n != s_nOldTitleLen )
	{
		shared_ptr< wstring > szTitle = nullptr;
		if( n > 4 )
		{
			szTitle = make_shared< wstring >( &szBuffer[ 8 ] );
		}

		CCore::GetInstance( )->GetBmInfo( )->GetInfo( szTitle ).then( [ this ]( sBeatmapInfo* pInfo )
		{ 
			lock_guard< mutex > l( m_cs );

			for( auto f : m_vecCallbacks )
				f.second( pInfo );
		} );

		s_nOldTitleLen = n;
	}
}

async void CProcessInfo::TryFindProcess( )
{
	lock_guard< mutex > l( m_cs );

	m_hProcess = GetProcessByName( m_szProcessName.c_str( ), m_dwPid );
	if( m_hProcess != nullptr )
	{
		wchar_t szBuffer[ MAX_PATH ];
		DWORD dwLen = ARRAYSIZE( szBuffer ) - 1;
		
		if( QueryFullProcessImageNameW( m_hProcess, 0, szBuffer, &dwLen ) )
		{
			m_szProcessPath = wstring( szBuffer );
		
			//=> Prepare path(remove "osu!.exe") 
			size_t n = m_szProcessPath.rfind( '\\' );
			m_szProcessPath.erase( m_szProcessPath.begin() + n, m_szProcessPath.end( ) );

			//=> Get Hwnd of osu!
			m_hWnd = GetWindowById( m_dwPid );
			if( ::IsWindow(m_hWnd) )
			{
				GetWindowTextW( m_hWnd, szBuffer, ARRAYSIZE( szBuffer ) - 1 );
				if( wcscmp( szBuffer, L"osu! (loading)" ) != 0 )
				{
					m_bProcessAlive = true;
				}
			}
		}
		else
		{
			Log( L"Error | Failed to query process name! check access rights! (%d)", GetLastError( ) );
		}
	}
}
