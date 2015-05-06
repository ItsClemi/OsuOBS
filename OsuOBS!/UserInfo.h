#pragma once


class CUserInfo
{
	typedef std::function< void( std::shared_ptr< std::wstring > ) >				fCallbackPerformance;
	typedef std::function< void( std::shared_ptr< std::vector< std::wstring > > ) >	fCallbackActivity;

public:
	CUserInfo( );
	~CUserInfo( );

public:
	void StartUserThread( );
	void StopUserThread( );

public:
	size_t RegisterCallbackPerformance( fCallbackPerformance f ) {
		std::lock_guard< std::mutex > l( m_csP );
		m_vecPerformance.push_back( f );

		return m_vecPerformance.size( ) - 1;
	}

	size_t RegisterCallbackActivity( fCallbackActivity f ) {
		std::lock_guard< std::mutex > l( m_csA );
		m_vecActivity.push_back( f );

		return m_vecActivity.size( ) - 1;
	}

public:
	void UnregisterCallbackActivity( size_t nId )
	{
		std::lock_guard< std::mutex > l( m_csA );
		m_vecActivity.erase( m_vecActivity.begin( ) + nId );
	}

	void UnregisterCallbackPerformance( size_t nId )
	{
		std::lock_guard< std::mutex > l( m_csP );
		m_vecPerformance.erase( m_vecPerformance.begin( ) + nId );
	}



public:
	void SetForceUpdate( ) { m_bForceUpdate = true; }




public:
	std::mutex		m_cs;
	bool			m_bNewData = false;
	std::wstring	m_szPerformance;

	size_t							m_nActivities = 0;
	std::array< std::wstring, 20 >	m_aRecentActivity;

	std::atomic< bool >				m_bForceUpdate = false;

private:
	HANDLE			m_hKillUserThread = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadUser;


	std::mutex		m_csA;
	std::mutex		m_csP;

	std::vector< fCallbackPerformance >	m_vecPerformance;
	std::vector< fCallbackActivity >	m_vecActivity;
};

