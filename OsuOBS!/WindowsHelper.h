#pragma once


inline HANDLE GetProcessByName( const wchar_t* szName, DWORD &pId )
{
	pId = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( snapshot == INVALID_HANDLE_VALUE )
	{
		Log( L"ERROR | " __FUNCTIONW__ L" | Couldn't create process snapshot!" );
		return nullptr;
	}

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );
	if( !Process32First( snapshot, &entry ) )
	{
		Log( L"ERROR | " __FUNCTIONW__ L" | Couldn't enumerate first process!" );
	}
	else
	{
		while( Process32Next( snapshot, &entry ) )
		{
			if( wcscmp( entry.szExeFile, szName ) == 0 )
			{
				pId = entry.th32ProcessID;
				break;
			}
		}
	}
	CloseHandle( snapshot );

	if( pId != 0 )
	{
		Log( L"INFO  | " __FUNCTIONW__ L" | Found process", szName );

		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pId );
		if( hProcess == nullptr )
		{
			Log( L"ERROR | " __FUNCTIONW__ L" | Couldn't open process (%d)", szName, GetLastError( ) );
			return nullptr;
		}

		return hProcess;
	}

	return nullptr;
}

inline HWND GetWindowById( DWORD pId )
{
	HWND hwndCurrent = GetWindow( GetDesktopWindow( ), GW_CHILD );
	if( hwndCurrent == nullptr )
	{
		Log( L"ERROR | " __FUNCTIONW__ L" | Couldn't get desktop window handle!" );
		return nullptr;
	}

	DWORD pIdCurrent = 0;
	do
	{
		GetWindowThreadProcessId( hwndCurrent, &pIdCurrent );

		if( pIdCurrent == pId && IsWindowVisible( hwndCurrent ) && GetWindowTextLength( hwndCurrent ) != 0 )
		{
			Log( L"INFO  | " __FUNCTIONW__ L" | Found window with pId", pId );
			return hwndCurrent;
		}
	}
	while( hwndCurrent = GetNextWindow( hwndCurrent, GW_HWNDNEXT ) );
	return nullptr;
}


