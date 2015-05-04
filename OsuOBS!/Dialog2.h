#pragma once
class CDialog2
{
public:
	CDialog2( UINT nResourceId );
	~CDialog2( );

public:
	virtual void OnInitDialog( ) = 0;
	virtual void OnDestroyDialog( ) = 0;

	virtual void OnMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;



public:
	INT_PTR Run( );

public:
	LPCWSTR	GetTemplateName( )	{ return MAKEINTRESOURCE( m_nResourceId ); }

public:
	void SetHwnd( HWND hWnd )	{ m_hWnd = hWnd; }

public:
	HWND GetHwnd( )				{ return m_hWnd; }

private:
	HWND	m_hWnd = nullptr;

	UINT	m_nResourceId;

};

