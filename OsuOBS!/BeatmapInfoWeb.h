#pragma once
#include "TextSource.h"



class CBeatmapInfoWeb : public TextOutputSource
{
public:
	CBeatmapInfoWeb( XElement* pData );
	virtual ~CBeatmapInfoWeb( );

public:
	virtual void Tick( float fSeconds );

private:
	std::mutex	m_cs;

	float		m_fDifficulty = 0.0f;
	bool		m_bIsDirty = false;
};



ImageSource* STDCALL _SourceBeatmapWebInfo( XElement* pData );
bool STDCALL _SourceConfigBeatmapWebInfo( XElement* pElement, bool bCreating );

