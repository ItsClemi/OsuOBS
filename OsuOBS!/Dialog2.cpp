#include "stdafx.h"
#include "Dialog2.h"



CDialog2::CDialog2( UINT nResourceId ) : m_nResourceId( nResourceId )
{
}

CDialog2::~CDialog2( )
{
}

INT_PTR CDialog2::Run( )
{
	return OBSDialogBox( g_hInstance, GetTemplateName( ), API->GetMainWindow( ), [ ]( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )->INT_PTR{

		static bool bInitializedDialog = false;

		if( uMsg == WM_INITDIALOG )
		{
			CDialog2* pDialog = reinterpret_cast< CDialog2* >( lParam );
			SetWindowLongPtr( hWnd, DWLP_USER, ( LONG_PTR )pDialog );

			pDialog->SetHwnd( hWnd );
			pDialog->OnInitDialog( );

			
			bInitializedDialog = true;
		
			return TRUE;
		}
		else if( uMsg == WM_DESTROY )
		{
			CDialog2* pDialog = reinterpret_cast< CDialog2* >( GetWindowLongPtr( hWnd, DWLP_USER ) );
			if( pDialog != nullptr )
			{
				pDialog->OnDestroyDialog( );
			}

			bInitializedDialog = false;
		}
		else
		{
			CDialog2* pDialog = reinterpret_cast< CDialog2* >( GetWindowLongPtr( hWnd, DWLP_USER ) );
			if( pDialog != nullptr )
			{
				pDialog->OnMsg( uMsg, wParam, lParam );
			}
		}

		return 0;
	}, ( LPARAM )this );
}
