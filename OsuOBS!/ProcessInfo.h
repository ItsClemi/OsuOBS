#pragma once


struct sBeatmapInfo;
class CProcessInfo
{
public:
	typedef std::function< void( sBeatmapInfo* ) > BmCallback;

public:
	CProcessInfo( );
	~CProcessInfo( );

public:
	void StartProcessInfo( );
	void StopProcessInfo( );

	void RegBmChangedEvent( BmCallback f );

public:
	async std::tr2::sys::wpath GetProcessPath( )
	{
		//std::lock_guard< std::mutex > l( m_cs );

		return std::tr2::sys::wpath( m_szProcessPath );
	}


private:
	async void CheckBmChanged( );
	async void TryFindProcess( );

private:
	std::mutex				m_cs;
	std::wstring			m_szProcessName = L"osu!.exe";
	std::wstring			m_szProcessPath;


	//=> Process info
	HANDLE					m_hProcess = nullptr;
	DWORD					m_dwPid;
	HWND					m_hWnd;
	std::atomic< bool >		m_bProcessAlive = false;

	HANDLE					m_hKillProcessInfo = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread				m_threadInfo;


	std::vector< BmCallback >	m_vecCallbacks;
};

