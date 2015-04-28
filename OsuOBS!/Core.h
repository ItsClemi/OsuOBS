#pragma once

#include "BeatmapInfo.h"
#include "UserInfo.h"
#include "ProcessInfo.h"


class CCore
{
	CCore( );
	~CCore( );

public:
	static CCore* GetInstance( );

public:
	void StartOsuThread( );
	void StopOsuThread( );


	size_t RegisterBeatmapChangedINT( );



	//=> Better coding style ! simple forward
	void RegBmChangedEvent( CProcessInfo::BmCallback f ) { GetProcessInfo( )->RegBmChangedEvent( f ); }


	CUserInfo*		GetUserInfo( )		{ return &m_userInfo; }
	CBeatmapInfo*	GetBmInfo( )		{ return &m_bmInfo; }
	CProcessInfo*	GetProcessInfo( )	{ return &m_processInfo; }

private:
	async void CheckBeatmapChange( );

public:
	std::atomic< bool >		m_bFoundOsu = false;
	DWORD					m_dwOsuProcId = 0;
	HANDLE					m_hProcOsu = INVALID_HANDLE_VALUE;
	HWND					m_hWndOsu = nullptr;
	wchar_t					m_szOsuExePath[ MAX_PATH ];
	std::wstring			m_szOsuPath;
	std::mutex				m_csPath;


	struct sBmDisplay{
		void SetDirty( )
		{
			for( auto i : m_vecIsDirty )
				i = true;
		}
		
		sBeatmapInfo* pInfo = nullptr;

		std::mutex m_cs;
		std::vector< bool >	m_vecIsDirty;
	};
	int			m_nOldTitleLen = 0;
	sBmDisplay	m_beatMap;


	Shader*	m_pSolidVertexShader = nullptr;
	Shader*	m_pSolidPixelShader = nullptr;

private:
	HANDLE			m_hKillOsuThread = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadOsu;


	CProcessInfo	m_processInfo;
	CUserInfo		m_userInfo;
	CBeatmapInfo	m_bmInfo;


};