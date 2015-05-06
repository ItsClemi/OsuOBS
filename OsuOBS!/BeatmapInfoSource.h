#pragma once
#include "TextSource.h"


class CBeatmapInfoSource : public TextOutputSource
{
public:
	CBeatmapInfoSource( XElement* pData );
	virtual ~CBeatmapInfoSource( );

public:
	virtual void Tick( float fSeconds );

private:
	size_t			m_nCallbackId;

	std::mutex		m_cs;
	bool			m_bIsDirty;

	bool			m_bShowDrain = true;
	bool			m_bShowCircleSize = true;
	bool			m_bShowOverallDifficulty = true;
	bool			m_bShowApprochRate = true;
	bool			m_bShowSliderMul = true;

	std::wstring	m_szText;

};


//////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapInfo( XElement *data );
bool STDCALL _SourceConfigBeatmapInfo( XElement *element, bool bCreating );



