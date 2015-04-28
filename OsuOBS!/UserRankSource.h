#pragma once
#include "TextSource.h"

class CUserRankSource : public TextOutputSource
{
public:
	CUserRankSource( XElement* pData );
	virtual ~CUserRankSource( );

public:
	virtual void Tick( float fSeconds );

};

ImageSource* STDCALL _SourceUserRank( XElement *data );
bool STDCALL _SourceConfigUserRank( XElement *element, bool bCreating );

