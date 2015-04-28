#include "stdafx.h"
#include "BeatmapInfo.h"
#include "Core.h"

using namespace std;
using namespace tr2;
using namespace sys;
using namespace Concurrency;


CBeatmapInfo::CBeatmapInfo( )
{
}

CBeatmapInfo::~CBeatmapInfo( )
{
}


//=> TODO: Rewrite it a bit :D
task< sBeatmapInfo* > CBeatmapInfo::GetInfo( std::shared_ptr< std::wstring > szTitle )
{
	return create_task( [ this, szTitle ]( ){
		if( szTitle == nullptr )
		{
			//=>weird vc bug?
			return static_cast< sBeatmapInfo* >( nullptr );
		}

		std::wstring szFullName = *szTitle;


		sBeatmapInfo* pInfo = this->GetStoredInfo( szFullName );
		if( pInfo == nullptr )
		{
			//=> ToDo: If the difficulty name of a mape is like [  [easy ]] we can't find it!
			auto nDiff = szTitle->rfind( L" [" );
			wstring szDiff = wstring( szTitle->begin( ) + nDiff, szTitle->end( ) );

			szTitle->erase( szTitle->begin( ) + nDiff, szTitle->end( ) );

			//=> Remove all '.' in the string because you can't put a '.' into a folder name! 
			auto it = std::remove_if( szTitle->begin( ), szTitle->end( ), [ ]( char c ){return ( c == '.' ); } );
			szTitle->erase( it, szTitle->end( ) );


			//=> Add into map
			pInfo = new sBeatmapInfo;
			{
				pInfo->m_szFullName = szFullName;
				pInfo->m_szDifficulty = szDiff;


			}
			m_mapBmInfo.insert( make_pair( szFullName, pInfo ) );


			auto szPath = CCore::GetInstance()->GetProcessInfo()->GetProcessPath( ) / wpath( L"\\Songs\\" );
			for( auto i = wdirectory_iterator( szPath ); i != wdirectory_iterator( ); i++ )
			{
				const auto& p = i->path( );

				if( is_directory( p ) )
				{
					auto f = p.filename( );
					if( isdigit( f[ 0 ] ) )
					{
						auto n = f.find_first_of( ' ' );
						std::wstring szBmSetId = std::wstring( f.begin( ), f.begin( ) + n );
						std::wstring szBeatmap = std::wstring( f, n + 1 );


						pInfo->m_nBeatmapSetId = _wtoi( szBmSetId.c_str( ) );

						auto fn = szTitle->find( szBeatmap );
						if( fn != wstring::npos )
						{
							for( auto it = wdirectory_iterator( p ); it != wdirectory_iterator( ); it++ )
							{
								const auto& _fbm = it->path( );

								if( _fbm.extension( ) == L".osu" )
								{
									if( _fbm.filename( ).find( szDiff ) != wstring::npos )
									{
										ReadBeatmap( pInfo, _fbm );
										return pInfo;
									}
								}
							}
						}
					}
				}
			}
		}



		return pInfo;
	} );
}

async sBeatmapInfo* CBeatmapInfo::ReadBeatmap( sBeatmapInfo* pInfo, const std::tr2::sys::wpath& szFile )
{
	fstream f( szFile );
	if( f.is_open( ) )
	{
		std::istreambuf_iterator<char> eos;
		std::string s( std::istreambuf_iterator<char>( f ), eos );

		auto findNum = [ &s ]( char* szTagName )->string
		{
			auto l = strlen( szTagName );

			auto nPosBegin = s.find( szTagName );
			auto nPosEnd = s.find( '\n', nPosBegin + l );

			if( nPosBegin != string::npos && nPosEnd != string::npos )
			{
				return string( s.begin( ) + nPosBegin + l, s.begin( ) + nPosEnd );
			}

			return "0";
		};

		pInfo->m_nBeatmapId = atoi( findNum( "BeatmapID:" ).c_str( ) );
		
		int nBmSetId = atoi( findNum( "BeatmapSetID:" ).c_str( ) );

		if( pInfo->m_nBeatmapSetId != nBmSetId  && nBmSetId != 0 )
			pInfo->m_nBeatmapSetId = nBmSetId;
			
		pInfo->m_fDrain = ( float )atof( findNum( "HPDrainRate:" ).c_str( ) );
		pInfo->m_fCircleSize = ( float )atof( findNum( "CircleSize:" ).c_str( ) );
		pInfo->m_fOverallDifficulty = ( float )atof( findNum( "OverallDifficulty:" ).c_str( ) );
		pInfo->m_fApproachRate = ( float )atof( findNum( "ApproachRate:" ).c_str( ) );
		pInfo->m_fSliderMul = ( float )atof( findNum( "SliderMultiplier:" ).c_str( ) );
	}


	return pInfo;
}