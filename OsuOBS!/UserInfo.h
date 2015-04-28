#pragma once

enum eGameMode : byte{
	eGameModeOsu = 0,
	eGameModeTaiko = 1,
	eGameModeCTB = 2,
	eGameModeMania = 3
};

class CUserInfo
{
public:
	CUserInfo( );
	~CUserInfo( );

public:
	void StartUserThread( );
	void StopUserThread( );

public:
	void SetUserId( UINT nId )
	{
		std::lock_guard< std::mutex > l( m_cs );
		m_nUserId = nId;
	}
	
	UINT GetUserId( )
	{
		std::lock_guard< std::mutex > l( m_cs );
		return m_nUserId;
	}

	void GetUserData( UINT* nUserId, eGameMode* eMode )
	{
		std::lock_guard< std::mutex > l( m_cs );

		*nUserId = m_nUserId;
		*eMode = m_eGameMode;
	}


public:
	std::mutex		m_cs;
	bool			m_bNewData = false;
	std::wstring	m_szPerformance;


private:
	HANDLE			m_hKillUserThread = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadUser;

	eGameMode		m_eGameMode = eGameMode::eGameModeOsu;
	UINT			m_nUserId = 0;


};

