#pragma once

enum eGameMode : UINT {
	eGameModeOsu = 0,
	eGameModeTaiko = 1,
	eGameModeCTB = 2,
	eGameModeMania = 3
};

class CConfig
{

	CConfig( );
	~CConfig( );

public:
	static CConfig* GetInstance( );

public:
	void LoadConfig( );
	void SaveConfig( );

public:
	UINT		GetUserId( )			{ return m_nBanchoId; }
	eGameMode	GetGameMode( )			{ return m_eGameMode; }


public:
	void SetUserId( UINT nId )			{ m_nBanchoId = nId; }
	void SetGameMode( eGameMode e )		{ m_eGameMode = e; }


public:
	std::wstring GetProcessName( ){
		std::lock_guard< std::mutex > l( m_cs );
		return m_szProcessName;
	}


private:
	std::atomic< UINT >			m_nBanchoId = 0;
	std::atomic< eGameMode >	m_eGameMode = eGameMode::eGameModeOsu;

	std::mutex		m_cs;
	std::wstring	m_szProcessName;

};


INT_PTR CALLBACK ConfigDlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );