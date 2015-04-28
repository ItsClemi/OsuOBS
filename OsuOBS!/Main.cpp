#include "StdAfx.h"
#include "BeatmapInfoSource.h"
#include "BeatmapName.h"
#include "UserRankSource.h"
#include "BeatmapInfoWeb.h"

#include "Core.h"
#include "UserInfo.h"


INT_PTR CALLBACK ConfigProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );


OBSEXPORT CTSTR GetPluginName( )
{
	return L"OsuOBS!";
}

OBSEXPORT CTSTR GetPluginDescription( )
{
	return L"Add osu infos to OBS";
}

OBSEXPORT bool LoadPlugin( )
{
	Log( L"Info | OSUOBS by It'sClemi (C)2015 v0.1" );

	API->RegisterImageSourceClass( L"OSUOBS-BMPN", L"[OSUOBS]Beatmap name", ( OBSCREATEPROC )_SourceBeatmapName, ( OBSCONFIGPROC )_SourceConfigBeatmapName );
	API->RegisterImageSourceClass( L"OSUOBS-BMPI", L"[OSUOBS]Beatmap info", ( OBSCREATEPROC )_SourceBeatmapInfo, ( OBSCONFIGPROC )_SourceConfigBeatmapInfo );
	API->RegisterImageSourceClass( L"OSUOBS-BMSD", L"[OSUOBS]Beatmap Star Difficulty", ( OBSCREATEPROC )_SourceBeatmapWebInfo, ( OBSCONFIGPROC )_SourceConfigBeatmapWebInfo );

	API->RegisterImageSourceClass( L"OSUOBS-USRK", L"[OSUOBS]User rank", ( OBSCREATEPROC )_SourceUserRank, ( OBSCONFIGPROC )_SourceConfigUserRank );


	return true;
}

OBSEXPORT void UnloadPlugin( )
{
}

OBSEXPORT void OnStartStream( )
{
	CCore::GetInstance( )->StartOsuThread( );
}

OBSEXPORT void OnStopStream( )
{
	CCore::GetInstance( )->StopOsuThread( );
}

OBSEXPORT void ConfigPlugin( HWND /*hWndParent*/ )
{
	if( OBSDialogBox( g_hInstance, MAKEINTRESOURCE( IDD_CONFIG ), API->GetMainWindow( ), ConfigProc ) == IDOK )
	{
		
	}

}


INT_PTR CALLBACK ConfigProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
		{
			UINT nUserId = CCore::GetInstance( )->GetUserInfo( )->GetUserId( );
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT_BANCHO_NAME ), std::to_wstring( nUserId ).c_str( ) );

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
				case IDOK:
				{
					wchar_t szBuffer[ 256 ];
					GetWindowTextW( GetDlgItem( hwnd, IDC_EDIT_BANCHO_NAME ), szBuffer, ARRAYSIZE( szBuffer ) );

					CCore::GetInstance( )->GetUserInfo( )->SetUserId( _wtoi( szBuffer ) );
				}
				case IDCANCEL:
				{
					EndDialog( hwnd, LOWORD( wParam ) );
				}
			}

			break;
		}
	}

	
	return 0;
}
