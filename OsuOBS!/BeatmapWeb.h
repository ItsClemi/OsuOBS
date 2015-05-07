#pragma once
#include "CallbackHelper.h"


struct sBeatmapQuery
{
	sBeatmapQuery( std::wstring& szDifficulty, size_t nBeatmapId, size_t nBeatmapSet ) 
		: m_szDifficulty( szDifficulty ), m_nBeatmapId( nBeatmapId ), m_nBeatmapSetId( nBeatmapSet )
	{
	}

	std::wstring	m_szDifficulty;
	
	size_t			m_nBeatmapId;
	size_t			m_nBeatmapSetId;

	float			m_fResult = 0.0f;
};

struct sBeatmapInfo;
class CBeatmapWeb
{
public:
	typedef std::function< void( sBeatmapQuery* ) > BmWebQueryFunc;

public:
	CBeatmapWeb( );
	~CBeatmapWeb( );

public:
	void StartBeatmapWebThread( );
	void StopBeatmapWebThread( );

private:
	std::wstring BmSetIdToBmId( ComPtr<IXMLHTTPRequest>& pReq, wchar_t* szStr, sBeatmapQuery* pQuery );
	std::wstring GetBeatmapFromSetId( BSTR szData, sBeatmapQuery* pQuery );



public:
	size_t RegWebInfoCallback( BmWebQueryFunc f )
	{
		m_bUseBmInfo = true;

		std::lock_guard< std::mutex > l( m_cs );

		size_t nId = ( size_t )rand( );
		{
			m_vecCallback.push_back( make_pair( nId, f ) );
		}
		return nId;
	}

	void UnregWebInfoCallback( size_t nId )
	{
		std::lock_guard< std::mutex > l( m_cs );
		RemoveCallback( m_vecCallback, nId );
	}

	void QueryBeatmapDifficulty( sBeatmapQuery* pQuery )
	{
		m_queueQuery.push( pQuery );
	}

public:
	HANDLE GetIoCp( )	{ return m_hKillBmWeb; }

public:
	std::atomic< bool >	m_bUseBmInfo = false;


private:
	HANDLE			m_hKillBmWeb = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadWebThread;
	std::mutex		m_cs;


	std::vector< std::pair< size_t, BmWebQueryFunc > >		m_vecCallback;
	Concurrency::concurrent_queue< sBeatmapQuery* >			m_queueQuery;
};

