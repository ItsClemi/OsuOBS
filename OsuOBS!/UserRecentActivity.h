#pragma once
#include "TextSource.h"

class CUserRecentActivity : public TextOutputSource
{
public:
	CUserRecentActivity( XElement* pData );
	virtual ~CUserRecentActivity( );

public:
	virtual void Tick( float fSeconds );
};

ImageSource* STDCALL _SourceUserRecentInfo( XElement* pData );
bool STDCALL _SourceConfigUserRecentInfo( XElement* pElement, bool bCreating );



