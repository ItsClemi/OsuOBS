#pragma once
class CBeatmapWeb
{
public:
	CBeatmapWeb( );
	~CBeatmapWeb( );

public:
	void StartBeatmapWebThread( );
	void StopBeatmapWebThread( );


private:

	HANDLE			m_hQueryBm = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	std::thread		m_threadWebThread;



};

