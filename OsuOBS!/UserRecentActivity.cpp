#include "stdafx.h"
#include "UserRecentActivity.h"

#include "UserInfo.h"
#include "Core.h"

#include "Dialog2.h"

#include "Config.h"

using namespace std;



INT_PTR CALLBACK ConfigActivityProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );


CUserRecentActivity::CUserRecentActivity( XElement* pData )
	: TextOutputSource( pData ), 
	m_pData( pData )
{
	this->SetString( L"text", L"[RecentActivty]\n" );
	m_nRowCount = pData->GetInt( L"rowCount", 5 );

	m_nCallbackId = CCore::GetInstance( )->GetUserInfo( )->RegisterCallbackActivity( [ this ]( shared_ptr< vector< wstring > > vec )
	{

		lock_guard< mutex > l( m_cs );

		m_vecActivity.clear( );

		for( size_t i = 0; i < vec->size( ); i++ )
		{
			m_vecActivity.push_back( vec->at( i ) );
		}

		if( m_nRowCount > vec->size( ) )
			m_nRowCount = vec->size( );

		m_bNewData = true;
	} );
}

CUserRecentActivity::~CUserRecentActivity( )
{
	CCore::GetInstance( )->GetUserInfo( )->UnregisterCallbackActivity( m_nCallbackId );
}

void CUserRecentActivity::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	if( m_cs.try_lock( ) )
	{
		if( m_bNewData )
		{
			wstring szOut;
			for( size_t i = 0; i < (m_nRowCount < m_vecActivity.size( ) ? m_nRowCount : m_vecActivity.size()); i++ )
			{
				szOut += m_vecActivity[ i ];
				szOut += '\n';
			}

			this->SetString( L"text", szOut.c_str( ) );

			m_bNewData = false;
		}

		m_cs.unlock( );
	}
}


void CUserRecentActivity::UpdateSettings( )
{
	TextOutputSource::UpdateSettings( );

	std::lock_guard < std::mutex > l( m_cs );
	m_nRowCount = m_pData->GetInt( TEXT( "rowCount" ), 5 );
	m_bNewData = true;
}

void CUserRecentActivity::SetInt( CTSTR szName, int nValue )
{
	TextOutputSource::SetInt( szName, nValue );

	if( scmpi( szName, TEXT( "rowCount" ) ) == 0 )
	{
		m_cs.lock( );
		{
			m_nRowCount = nValue;
			m_bNewData = true;
		}
		m_cs.unlock( );
	}

}

ImageSource* STDCALL _SourceUserRecentInfo( XElement* pData )
{
	return new CUserRecentActivity( pData );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK ConfigureActivityProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static bool bInitializedDialog = false;

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			ConfigActivitySourceInfo* configInfo = ( ConfigActivitySourceInfo* )lParam;
			SetWindowLongPtr( hWnd, DWLP_USER, ( LONG_PTR )configInfo );

			XElement* pData = configInfo->m_pElement;

			SendMessage( GetDlgItem( hWnd, IDC_SPIN_ROW_NUM ), UDM_SETRANGE32, 1, 20 );
			SendMessage( GetDlgItem( hWnd, IDC_SPIN_ROW_NUM ), UDM_SETPOS32, 0, pData->GetInt( L"rowCount", 5 ) );

			bInitializedDialog = true;
			return TRUE;
		}
		case WM_DESTROY:
			bInitializedDialog = false;
			break;

		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
				case IDC_SETUP_FONT:
				{
					ConfigActivitySourceInfo* pConfig = reinterpret_cast< ConfigActivitySourceInfo* >( GetWindowLongPtr( hWnd, DWLP_USER ) );

					ConfigureTextSource( pConfig->m_pElement->GetParent(), pConfig->m_bCreating, true );

					break;
				}

				case IDC_ROW_NUM:
				{
					if( HIWORD( wParam ) == EN_CHANGE && bInitializedDialog )
					{
						int val = ( int )SendMessage( GetWindow( ( HWND )lParam, GW_HWNDNEXT ), UDM_GETPOS32, 0, 0 );

						ConfigActivitySourceInfo* configInfo = ( ConfigActivitySourceInfo* )GetWindowLongPtr( hWnd, DWLP_USER );
						if( !configInfo ) break;

						ImageSource *source = API->GetSceneImageSource( configInfo->m_szName );
						if( source )
						{
							if( LOWORD( wParam ) == IDC_ROW_NUM )
							{
								source->SetInt( TEXT( "rowCount" ), val );
							}
						}
					}

					break;
				}

				case IDOK:
				{
					ConfigActivitySourceInfo *configInfo = ( ConfigActivitySourceInfo* )GetWindowLongPtr( hWnd, DWLP_USER );
					if( !configInfo ) break;
					XElement* data = configInfo->m_pElement;


					UINT nRowCount = ( UINT )SendMessage( GetDlgItem( hWnd, IDC_SPIN_ROW_NUM ), UDM_GETPOS32, 0, 0 );
					data->SetInt( L"rowCount", nRowCount );

				}
				case IDCANCEL:
					if( LOWORD( wParam ) == IDCANCEL )
						DoCancelStuff( hWnd );

					EndDialog( hWnd, LOWORD( wParam ) );
			}
			break;

		case WM_CLOSE:
			DoCancelStuff( hWnd );
			EndDialog( hWnd, IDCANCEL );
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool STDCALL _SourceConfigUserRecentInfo( XElement* pElement, bool bCreating )
{
	if( !pElement )
	{
		AppWarning( __FUNCTIONW__ L" NULL element" );
		return false;
	}

	XElement *data = pElement->GetElement( TEXT( "data" ) );
	if( !data )
		data = pElement->CreateElement( TEXT( "data" ) );

	ConfigActivitySourceInfo configInfo;
	configInfo.m_szName = pElement->GetName( );
	configInfo.m_pElement = data;
	configInfo.m_bCreating = bCreating;

	if( OBSDialogBox( g_hInstance, MAKEINTRESOURCE( IDD_CONFIG_ACTIVITY ), API->GetMainWindow( ), ConfigureActivityProc, ( LPARAM )&configInfo ) == IDOK )
	{
		return true;
	}

	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CConfig2 : public CDialog2
{
public:
	CConfig2( XElement* pElement )
		: CDialog2( IDD_CONFIG_ACTIVITY ),
		m_pElement( pElement )
	{
		m_pData = pElement->GetElement( L"data" );
	}

	virtual void OnInitDialog( )
	{
		m_hWndSpinner = GetDlgItem( GetHwnd(), IDC_SPIN_ROW_NUM );

		SendMessage( m_hWndSpinner, UDM_SETRANGE32, 1, 20 );
		SendMessage( m_hWndSpinner, UDM_SETPOS32, 0, m_pData->GetInt( L"rowCount", 5 ) );
	}

	virtual void OnDestroyDialog( ){ }

	virtual void OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
// 		switch( uMsg )
// 		{
// 			case WM_COMMAND:
// 			{
// 				switch( LOWORD( wParam ) )
// 				{
// 					case IDC_SETUP_FONT:
// 					{
// 						ConfigureTextSource( m_pElement, false, true );
// 						break;
// 					}
// 
// 					case IDC_ROW_NUM:
// 					{
// 						if( HIWORD( wParam ) == EN_CHANGE )
// 						{
// 							int val = ( int )SendMessage( GetWindow( ( HWND )lParam, GW_HWNDNEXT ), UDM_GETPOS32, 0, 0 );
// 
// // 							ImageSource *source = API->GetSceneImageSource( configInfo->m_szName );
// // 							if( source )
// // 							{
// // 								switch( LOWORD( wParam ) )
// // 								{
// // 									case IDC_ROW_NUM:      source->SetInt( TEXT( "rowCount" ), val ); break;
// // 
// // 								}
// // 							}
// 						}
// 
// 
// 						break;
// 					}
// 
// 					case IDOK:
// 					{
// 						XElement* data = configInfo->m_pElement;
// 
// 
// 						UINT nRowCount = ( UINT )SendMessage( GetDlgItem( hWnd, IDC_SPIN_ROW_NUM ), UDM_GETPOS32, 0, 0 );
// 						data->SetInt( L"rowCount", nRowCount );
// 
// 
// 					}
// 					case IDCANCEL:
// 						if( LOWORD( wParam ) == IDCANCEL )
// 							DoCancelStuff( GetHwnd() );
// 
// 						EndDialog( GetHwnd( ), LOWORD( wParam ) );
// 
// 					case WM_CLOSE:
// 						DoCancelStuff( GetHwnd( ) );
// 						EndDialog( GetHwnd( ), IDCANCEL );
// 				}
// 			}
// 		}
	}

private:
	XElement*	m_pElement;
	XElement*	m_pData;

	HWND		m_hWndSpinner;

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool STDCALL _SourceConfigUserRecentInfo2( XElement* pElement, bool bCreating )
{
	if( !pElement )
	{
		AppWarning( __FUNCTIONW__ L" NULL element" );
		return false;
	}

	XElement *data = pElement->GetElement( TEXT( "data" ) );
	if( !data ) 
		data = pElement->CreateElement( TEXT( "data" ) );


	CConfig2 pDialog( pElement );
	
	if( pDialog.Run( ) == IDOK )
	{

	}
	
}