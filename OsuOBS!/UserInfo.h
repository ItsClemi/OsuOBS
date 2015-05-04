#pragma once


class CUserInfo
{
public:
	CUserInfo( );
	~CUserInfo( );

public:
	void StartUserThread( );
	void StopUserThread( );

public:
	void SetForceUpdate( ) { m_bForceUpdate = true; }


public:
	std::mutex		m_cs;
	bool			m_bNewData = false;
	std::wstring	m_szPerformance;

	size_t							m_nActivities = 0;
	std::array< std::wstring, 4 >	m_aRecentActivity;

	std::atomic< bool >				m_bForceUpdate = false;

private:
	HANDLE			m_hKillUserThread = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadUser;
};

