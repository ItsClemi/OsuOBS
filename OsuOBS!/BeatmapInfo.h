#pragma once

struct sBeatmapInfo
{

	std::wstring m_szFullName;

	size_t		m_nBeatmapId = 0;
	size_t		m_nBeatmapSetId = 0;

	float		m_fDrain = 0.0f;
	float		m_fCircleSize = 0.0f;
	float		m_fOverallDifficulty = 0.0f;
	float		m_fApproachRate = 0.0f;
	float		m_fSliderMul = 0.0f;
};



class CBeatmapInfo
{
public:
	CBeatmapInfo( );
	~CBeatmapInfo( );

public:
	Concurrency::task< sBeatmapInfo* > GetInfo( std::shared_ptr< std::wstring > szTitle );

	void SetBeatmapFolder( std::wstring& szPath )	{ m_szBmPath = szPath.c_str(); }
	
private:
	async sBeatmapInfo* ReadBeatmap( sBeatmapInfo* pInfo, const std::tr2::sys::wpath& szFile );

private:
	sBeatmapInfo* GetStoredInfo( std::wstring& szBm )
	{
		auto it = m_mapBmInfo.find( szBm );
		return it != m_mapBmInfo.end( ) ? it->second : nullptr;
	}

public:
	//=> If a user don't want to show the beatmap stats theres no need to read dat shit :D
	std::atomic< size_t >	m_nDisplayName = 0;

private:
	std::tr2::sys::wpath	m_szBmPath;

	Concurrency::concurrent_unordered_map< std::wstring, sBeatmapInfo* >	m_mapBmInfo;
};

