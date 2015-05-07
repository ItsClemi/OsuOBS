#pragma once
#include "CallbackHelper.h"

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

public:
	size_t CProcessInfo::RegBmChangedEvent( BmCallback f )
	{
		std::lock_guard< std::mutex > l( m_cs );

		size_t nId = ( size_t )rand( );
		{
			m_vecCallbacks.push_back( make_pair( nId, f ) );
		}
		return nId;
	}

	void CProcessInfo::UnregBmChangedEvent( size_t nId )
	{
		std::lock_guard< std::mutex > l( m_cs );

		RemoveCallback( m_vecCallbacks, nId );
	}


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


	std::vector< std::pair< size_t, BmCallback > >	m_vecCallbacks;
};

