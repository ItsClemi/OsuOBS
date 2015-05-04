#include "stdafx.h"
#include "Config.h"
#include "resource.h"

#include "Core.h"
#include "UserInfo.h"
#include "AboutDlg.h"

//=> using jsoncpp ( https://github.com/open-source-parsers/jsoncpp/wiki )
#include <json/config.h>
#include <json/json.h>



using namespace std;



namespace{
	static const int		s_nConfigVer = 01;
	static const wchar_t*	s_szConfigPath = L".\\plugins\\OsuOBS!_config.json";
}




CConfig::CConfig( )
{
}

CConfig::~CConfig( )
{
}

CConfig* CConfig::GetInstance( )
{
	static CConfig s_config;
	return &s_config;
}

void CConfig::LoadConfig( )
{
	ifstream f( s_szConfigPath, std::ifstream::binary );
	if( f.is_open( ) == false )
	{
		Log( L"OsuOBS!| Info | Config not found. Defaulting values!" );
		return;
	}

	Json::Value root;
	f >> root;

	if( root[ "ConfigVer" ].asInt( ) == s_nConfigVer )
	{
		m_nBanchoId = root[ "BanchoId" ].asInt( );
		m_eGameMode = ( eGameMode )root[ "GameMode" ].asInt( );
	}
	else
	{
		Log( L"OsuOBS! | Error | Wrong config ver!" );
	}
}

void CConfig::SaveConfig( )
{
	Json::Value root;

	root[ "ConfigVer" ] = s_nConfigVer;

	root[ "BanchoId" ] = m_nBanchoId.load( );
	root[ "GameMode" ] = m_eGameMode.load( );


	ofstream myfile;
	myfile.open( s_szConfigPath, ios::binary | ios::trunc );
	{
		myfile << root;
	}
	myfile.close( );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



INT_PTR CALLBACK ConfigDlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
		{
			UINT nUserId = CConfig::GetInstance()->GetUserId();
			SetWindowText( GetDlgItem( hwnd, IDC_EDIT_BANCHO_NAME ), std::to_wstring( nUserId ).c_str( ) );

			eGameMode eMode = CConfig::GetInstance( )->GetGameMode( );

			HWND hWndMode = GetDlgItem( hwnd, IDC_SETMODE );
			SendMessage( hWndMode, CB_ADDSTRING, 0, reinterpret_cast< LPARAM >( L"Osu!" ) );
			SendMessage( hWndMode, CB_ADDSTRING, 0, reinterpret_cast< LPARAM >( L"Taiko" ) );
			SendMessage( hWndMode, CB_ADDSTRING, 0, reinterpret_cast< LPARAM >( L"Catch The Beat" ) );
			SendMessage( hWndMode, CB_ADDSTRING, 0, reinterpret_cast< LPARAM >( L"Osu!Mainia" ) );
			SendMessage( hWndMode, CB_SETCURSEL, static_cast< WPARAM >( eMode ), 0 );



			return TRUE;
		}

		case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
				case IDC_ABOUT:
				{
					HWND hDlg;
					hDlg = CreateDialogParam( g_hInstance, MAKEINTRESOURCE( IDD_ABOUT ), 0, AboutDlgProc, 0 );
					ShowWindow( hDlg, 10 );
					break;
				}
				case IDOK:
				{
					wchar_t szBuffer[ 256 ];
					GetWindowTextW( GetDlgItem( hwnd, IDC_EDIT_BANCHO_NAME ), szBuffer, ARRAYSIZE( szBuffer ) );
					CConfig::GetInstance()->SetUserId( _wtoi( szBuffer ) );

					LRESULT sel = SendMessage( GetDlgItem( hwnd, IDC_SETMODE ), CB_GETCURSEL, ( WPARAM )0, ( LPARAM )0 );
					CConfig::GetInstance( )->SetGameMode( (eGameMode)sel );


					CCore::GetInstance( )->GetUserInfo( )->SetForceUpdate( );
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
