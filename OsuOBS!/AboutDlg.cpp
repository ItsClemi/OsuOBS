#include "StdAfx.h"
#include "AboutDlg.h"
#include "resource.h"

using namespace Gdiplus;

//=> TODO: Wrap class
void OnPaint( HWND hWnd, HDC dc )
{
	RECT rc;
	GetClientRect( hWnd, &rc );

	Gdiplus::Graphics g( dc );
	g.SetSmoothingMode( Gdiplus::SmoothingMode::SmoothingModeHighQuality );
	g.Clear( Color::Gray );


	Bitmap* bmp = Bitmap::FromResource( g_hInstance, MAKEINTRESOURCE( IDB_BITMAP1 ) );
	g.DrawImage( bmp, 0, 0, rc.right, rc.bottom );


	Gdiplus::Font* fnt = new Gdiplus::Font( L"Arial", 14.0f );
	Brush* brText = new SolidBrush( Color::White );

	std::wstring szVer = L"Version: 01";
	g.DrawString( szVer.c_str(), (int)szVer.length( ), fnt, PointF( 9, 130 ), brText );

	Gdiplus::Font* fnt2 = new Gdiplus::Font( L"Arial", 9.0f );

	std::wstring szUsing = L"Using jsoncpp, parts of OBS";
	g.DrawString( szUsing.c_str( ), ( int )szUsing.length( ), fnt2, PointF( 9, 155 ), brText );

	std::wstring szArt = L"Artwork by Duplex";
	g.DrawString( szArt.c_str( ), ( int )szArt.length( ), fnt2, PointF( 9, 180 ), brText );


	delete( fnt2 );
	delete( fnt );
	delete( bmp );
	

	g.ReleaseHDC( dc );
}


INT_PTR CALLBACK AboutDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch( uMsg )
	{
		case WM_CLOSE:
			EndDialog( hWnd, 0 );
			return TRUE;


		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );
			OnPaint( hWnd, hDC );
			EndPaint( hWnd, &ps );
			return FALSE;
	}

	return FALSE;
}
