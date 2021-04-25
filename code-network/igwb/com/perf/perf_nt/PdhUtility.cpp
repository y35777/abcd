///////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WIN32

#include "StdAfx.h"
#include "PdhUtility.h"

//////////////////////////////////////////////////////////////////////////////////////

CPerfTitleDatabase::CPerfTitleDatabase()
{
	m_nLastIndex = 0;
	m_pszRawStrings = 0;
	m_TitleStrings = 0;

	GetNameStrings();
}

CPerfTitleDatabase::~CPerfTitleDatabase()
{
	delete []m_pszRawStrings;
	m_pszRawStrings = 0;
	delete []m_TitleStrings;
	m_TitleStrings = 0;

	m_nLastIndex = 0;
}

DWORD CPerfTitleDatabase::GetIndexFromTitleString(LPTSTR pszTitleString)
{
    if ( IsBadStringPtr(pszTitleString, 0xFFFFFFFF) )
        return 0;
    // Loop through all the non-null string array entries, doing a case-
    // insensitive comparison.  If found, return the correpsonding index
    for ( DWORD i = 1; i <= m_nLastIndex; i++ ) 
	{
        if ( m_TitleStrings[i] )
            if ( 0 == _tcsicmp( pszTitleString, m_TitleStrings[i] ) )
                return i; 
	} 

	return 0;
}

LPTSTR CPerfTitleDatabase::GetTitleStringFromIndex(DWORD index)
{
    if ( index > m_nLastIndex ) // Is index within range?
        return 0; 
	
    return m_TitleStrings[ index ];
}

void CPerfTitleDatabase::GetNameStrings()
{
    HKEY hKeyPerflib;      // handle to registry key
    DWORD dwBuffer = 0;        // bytes to allocate for buffers
    DWORD dwBufferSize = 0;    // size of dwBuffer
    LPTSTR lpCurrentString; // pointer for enumerating data strings
    DWORD dwCounter;       // current counter index
	
	// Get the number of Counter items.
    RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			      "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib",
				  0,
				  KEY_READ,
				  &hKeyPerflib);
    dwBufferSize = sizeof(dwBuffer);
    RegQueryValueEx( hKeyPerflib,
					 "Last Counter",
					 NULL,
					 NULL,
					 (LPBYTE) &m_nLastIndex,
					 &dwBufferSize );
    RegCloseKey( hKeyPerflib );
	
	// Allocate memory for the names array.
    //m_TitleStrings = (LPTSTR *)malloc( (dwBuffer+1) * sizeof(LPTSTR) );
	m_TitleStrings = new LPTSTR[(m_nLastIndex + 1)];

	dwBuffer = 0;
	RegQueryValueEx( HKEY_PERFORMANCE_DATA,
					 "Counter 009",
					 NULL,
					 NULL,
					 NULL,
					 &dwBuffer );
	// Allocate memory for the counter and object names.
    //dwBuffer = dwMaxValueLen + 1;
    //m_pszRawStrings = (char *) malloc( dwBuffer * sizeof(CHAR) );
	m_pszRawStrings = new CHAR[dwBuffer];
	
	// Read Counter value.
	RegQueryValueEx( HKEY_PERFORMANCE_DATA,
					 "Counter 009",
					 NULL,
					 NULL,
					 (unsigned char *)m_pszRawStrings,
					 &dwBuffer );
	memset( m_TitleStrings, 0, sizeof(LPTSTR) * (m_nLastIndex+1) );
	// Load names into an array, by index.
    for( lpCurrentString = m_pszRawStrings; *lpCurrentString;
         lpCurrentString += (lstrlen(lpCurrentString)+1) ) 
	{
        dwCounter = atol( lpCurrentString );
        lpCurrentString += (lstrlen(lpCurrentString)+1);
        m_TitleStrings[dwCounter] = (LPTSTR) lpCurrentString;
    }
}

/////////////////////////////////////////////////////////////////////////////////////

CPerfSnapshot::CPerfSnapshot( CPerfTitleDatabase * pCounterTitles )
{
    m_pPerfDataHeader = 0;
    m_pCounterTitles = pCounterTitles;
}

CPerfSnapshot::~CPerfSnapshot()
{
    DisposeSnapshot();
}

void CPerfSnapshot::DisposeSnapshot()
{
    delete m_pPerfDataHeader;
    m_pPerfDataHeader = 0;
}

BOOL CPerfSnapshot::ConvertSnapshotItemName( PTSTR pszIn, PTSTR pszOut, DWORD nSize )
{
    if ( IsBadStringPtr( pszIn, 0xFFFFFFFF ) ) 
		return FALSE;
  
	PTSTR pszCopy = _tcsdup( pszIn );   // Need a copy because _tcstok will
                                        // modify the string it processes
    PTSTR pszCopy2 = pszCopy, pszToken;    PTSTR pszOut2 = pszOut;
    
	// For each token (' ' delimited) from the input string, see if it
    // corresponds to a counter title.  If so, emit the title index as
    // in ASCII form to the output string.  If the input token isn't a
    // counter title, pass it through unmodified to the output string.
    // This allows things like "Global" and "Costly" to go through unscathed.
    while ( pszToken = _tcstok(pszCopy2, TEXT(" ")) )   // _tcstok == strtok
    {
        DWORD objectID = m_pCounterTitles->GetIndexFromTitleString(pszToken);
        // Needs to check output string len!
        if ( objectID )
            pszOut2 += wsprintf( pszOut2, TEXT("%u "), objectID );
        else
            pszOut2 += wsprintf( pszOut2, TEXT("%s "), pszToken );        
        
		pszCopy2 = 0;   // Set to 0 for 2nd and later passes through the loop
    }   
	
	free( pszCopy );
    
	return TRUE;
}

BOOL CPerfSnapshot::TakeSnapshot( PTSTR pszSnapshotItems )
{
    DisposeSnapshot();  // Clear out any current snapshot
    // Convert the input string (e.g., "Process") into the form required
    // by the HKEY_PERFORMANCE_DATA key (e.g., "232")
    TCHAR szConvertedItemNames[ 256 ];
    if ( !ConvertSnapshotItemName( pszSnapshotItems, szConvertedItemNames,
                                   sizeof(szConvertedItemNames) ) )
        return FALSE;
    DWORD cbPerfInfo = 0, cbAllocSize = 0;
    LONG retValue;    
	m_pPerfDataHeader = 0;
	
    while ( 1 ) // Loop until we get the data, or we fail unexpectedly
    {
        retValue = RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                                    szConvertedItemNames, 0, 0,
                                    (PBYTE)m_pPerfDataHeader, &cbPerfInfo );
        if ( retValue == ERROR_SUCCESS )    // We apparently got the snapshot
        { 
			// Verify that the signature is a unicode "PERF"
            if ( memcmp( m_pPerfDataHeader->Signature, L"PERF", 8 ) )
                break;
            return TRUE;
        }
        else if ( retValue != ERROR_MORE_DATA ) // Anything other failure
            break;                              // code means something
                                                // bad happened, so bail out.
        // If we get here, our buffer wasn't big enough.  Delete it and
        // try again with a bigger buffer.        delete []m_pPerfDataHeader;
                // The new buffer size will be 4096 bytes bigger than the larger
        // of: 1) The previous allocation size, or 2) The size that the
        // RegQueryValueEx call said was necessary.
        if ( cbPerfInfo > cbAllocSize )
            cbAllocSize = cbPerfInfo + 4096;
        else
            cbAllocSize += 4096;
        // Allocate a new, larger buffer in preparation to try again.
        m_pPerfDataHeader = (PPERF_DATA_BLOCK) new BYTE[ cbAllocSize ];
        if ( !m_pPerfDataHeader )
            break;
        cbPerfInfo = cbAllocSize;
    }
    // If we get here, the RegQueryValueEx failed unexpectedly.
    delete []m_pPerfDataHeader;
    m_pPerfDataHeader = 0;
    return FALSE;
}

DWORD CPerfSnapshot::GetNumObjectTypes()
{
    return m_pPerfDataHeader ? m_pPerfDataHeader->NumObjectTypes: 0;
}

BOOL CPerfSnapshot::GetSystemName( PTSTR pszSystemName, DWORD nSize )
{
    if ( !m_pPerfDataHeader )   // If no snapshot data, bail out.
        return FALSE;    // Make sure the input buffer size is big enough
    if ( nSize < m_pPerfDataHeader->SystemNameLength )
        return FALSE;
    // Make a unicode string point to the system name string
    // that's stored in the PERF_DATA_BLOCK
    LPWSTR lpwszName = MakePtr( LPWSTR, m_pPerfDataHeader,
                                m_pPerfDataHeader->SystemNameOffset );
#ifdef UNICODE  // Copy the PERF_DATA_BLOCK string to the input buffer
    lstrcpy( pszSystemName, lpwszName );
#else
    wcstombs( pszSystemName, lpwszName, nSize );
#endif     
	
    return TRUE;
}

PVOID CPerfSnapshot::GetPostHeaderPointer()
{
    // Returns a header to the first byte following the PERF_DATA_BLOCK
    // (including the variable length system name string at the end)
    return m_pPerfDataHeader ?
            MakePtr(PVOID, m_pPerfDataHeader,m_pPerfDataHeader->HeaderLength)
            : 0;
}

///////////////////////////////////////////////////////////////////////////////////////

CPerfObjectList::CPerfObjectList( CPerfSnapshot * const pPerfSnapshot,
								  CPerfTitleDatabase * const pPerfTitleDatabase )
{
    m_pPerfSnapshot = pPerfSnapshot;
    m_pPerfCounterTitles = pPerfTitleDatabase;
}

CPerfObject *CPerfObjectList::GetFirstPerfObject()
{
    m_currentObjectListIndex = 0;
    if ( m_currentObjectListIndex >= m_pPerfSnapshot->GetNumObjectTypes() )
        return 0; 
	
	m_pCurrObjectType = 
        (PPERF_OBJECT_TYPE)m_pPerfSnapshot->GetPostHeaderPointer();
    return new CPerfObject( m_pCurrObjectType, m_pPerfCounterTitles );
}

CPerfObject *CPerfObjectList::GetNextPerfObject()
{
    // Are we at the last object in the list?  Return NULL if so.
    if ( ++m_currentObjectListIndex >= m_pPerfSnapshot->GetNumObjectTypes() )
        return 0;    // Advance to the next PERF_OBJECT_TYPE structure

    m_pCurrObjectType = MakePtr(PPERF_OBJECT_TYPE,
                                m_pCurrObjectType,
                                m_pCurrObjectType->TotalByteLength );
                                
    return new CPerfObject( m_pCurrObjectType, m_pPerfCounterTitles );
}

CPerfObject *CPerfObjectList::GetPerfObject( PTSTR const pszObjListName )
{
    DWORD objListIdx
        = m_pPerfCounterTitles->GetIndexFromTitleString( pszObjListName );
    if ( 0 == objListIdx )
        return 0;
    
	// Point at first PERF_OBJECT_TYPE, and loop through the list, looking
    // for one that matches.
    PPERF_OBJECT_TYPE pCurrObjectType = 
            (PPERF_OBJECT_TYPE)m_pPerfSnapshot->GetPostHeaderPointer();
    
	for ( unsigned i=0; i < m_pPerfSnapshot->GetNumObjectTypes(); i++ )
    {
        // Is this the one that matches?
        if ( pCurrObjectType->ObjectNameTitleIndex == objListIdx )
            return new CPerfObject(pCurrObjectType, m_pPerfCounterTitles);
        
		// Nope... try the next object type
		pCurrObjectType = MakePtr(  PPERF_OBJECT_TYPE,
                                    pCurrObjectType,
                                    pCurrObjectType->TotalByteLength );    
	}
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////

CPerfObject::CPerfObject(   PPERF_OBJECT_TYPE const pObjectList,
                            CPerfTitleDatabase * const pPerfCounterTitles)
{
    m_pObjectList = pObjectList;
    m_pPerfCounterTitles = pPerfCounterTitles;
}

CPerfObjectInstance *CPerfObject::GetFirstObjectInstance()
{
    m_currentObjectInstance = 0;
    if ( m_currentObjectInstance >= GetObjectInstanceCount() )
        return 0;
    
	// Point at the first PERF_INSTANCE_DEFINITION
    m_pCurrentObjectInstanceDefinition = 
        MakePtr( PPERF_INSTANCE_DEFINITION, m_pObjectList,
                m_pObjectList->DefinitionLength );
    
	return new CPerfObjectInstance(
                m_pCurrentObjectInstanceDefinition,
                MakePtr(PPERF_COUNTER_DEFINITION,
                        m_pObjectList, m_pObjectList->HeaderLength),
                m_pObjectList->NumCounters,                
				m_pPerfCounterTitles,
                m_pObjectList->NumInstances ==
                    PERF_NO_INSTANCES ? TRUE : FALSE );
}

CPerfObjectInstance *CPerfObject::GetNextObjectInstance()
{
    if ( m_pObjectList->NumInstances == PERF_NO_INSTANCES )
        return 0;
    if ( ++m_currentObjectInstance >= GetObjectInstanceCount() )
        return 0;
    
	// Advance to the next PERF_INSTANCE_DEFINITION in the list.  However,
    // following the current PERF_INSTANCE_DEFINITION is the counter data,
    // which is also of variable length.  So, we gotta take that into
    // account when finding the next PERF_INSTANCE_DEFINITION        
    // First, get a pointer to the counter data size field
    PDWORD pCounterDataSize
        = MakePtr(PDWORD, m_pCurrentObjectInstanceDefinition,
                    m_pCurrentObjectInstanceDefinition->ByteLength);
    
	// Now we can point at the next PPERF_INSTANCE_DEFINITION
    m_pCurrentObjectInstanceDefinition = MakePtr(PPERF_INSTANCE_DEFINITION,
                m_pCurrentObjectInstanceDefinition,
                m_pCurrentObjectInstanceDefinition->ByteLength
                + *pCounterDataSize);            
    
	// Create a CPerfObjectInstance based around the PPERF_INSTANCE_DEFINITION
    return new CPerfObjectInstance(m_pCurrentObjectInstanceDefinition,
                                   MakePtr(PPERF_COUNTER_DEFINITION,
                                           m_pObjectList, 
                                           m_pObjectList->HeaderLength),
                                   m_pObjectList->NumCounters,
                                   m_pPerfCounterTitles,
                                   FALSE );
}

BOOL CPerfObject::GetObjectTypeName( PTSTR pszObjTypeName, DWORD nSize )
{
    PTSTR pszName = m_pPerfCounterTitles->GetTitleStringFromIndex(
                                    m_pObjectList->ObjectNameTitleIndex );
    
	if ( !pszName )
        return FALSE;
    
    lstrcpyn( pszObjTypeName, pszName, nSize );    
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

CPerfObjectInstance::CPerfObjectInstance(
        PPERF_INSTANCE_DEFINITION const pPerfInstDef,
        PPERF_COUNTER_DEFINITION const pPerfCntrDef,
        DWORD nCounters, CPerfTitleDatabase * const pPerfCounterTitles,
        BOOL fDummy)
{
    m_pPerfInstDef = pPerfInstDef;
    m_pPerfCntrDef = pPerfCntrDef;
    m_nCounters = nCounters;
    m_pPerfCounterTitles = pPerfCounterTitles; 
	m_fDummy = fDummy;
}

BOOL CPerfObjectInstance::GetObjectInstanceName(
    PTSTR pszObjInstName, DWORD nSize )
{
    if ( m_fDummy )
    {
        *pszObjInstName = 0;    // Return an empty string        
		return FALSE;
    }
	
	if ( nSize < (m_pPerfInstDef->NameLength / sizeof(TCHAR)) )
        return FALSE;
    PWSTR pszName = MakePtr(PWSTR, m_pPerfInstDef, m_pPerfInstDef->NameOffset);
        
#ifdef UNICODE
    lstrcpy( pszObjInstName, pszName );
#else
    wcstombs( pszObjInstName, pszName, nSize );
#endif        
    return TRUE;
}

CPerfCounter * CPerfObjectInstance::MakeCounter( PPERF_COUNTER_DEFINITION const pCounterDef )
{
    // Look up the name of this counter in the title database
    PTSTR pszName = m_pPerfCounterTitles->GetTitleStringFromIndex(
                                pCounterDef->CounterNameTitleIndex );        

    DWORD nInstanceDefSize = m_fDummy ? 0 : m_pPerfInstDef->ByteLength;
    // Create a new CPerfCounter.  The caller is responsible for deleting it.
    return new CPerfCounter(pszName,
                            pCounterDef->CounterType,
                            MakePtr( PBYTE, m_pPerfInstDef,
                                    nInstanceDefSize +
                                    pCounterDef->CounterOffset ),
                            pCounterDef->CounterSize );
}

CPerfCounter * CPerfObjectInstance::GetCounterByIndex( DWORD index )
{
    PPERF_COUNTER_DEFINITION pCurrentCounter;
	if ( index >= m_nCounters )
        return 0;
	
	pCurrentCounter = m_pPerfCntrDef;
    // Find the correct PERF_COUNTER_DEFINITION by looping
    for ( DWORD i = 0; i < index; i++ )  
	{
        pCurrentCounter = MakePtr( PPERF_COUNTER_DEFINITION,
                                    pCurrentCounter,
                                    pCurrentCounter->ByteLength );    
	}
    
	if ( pCurrentCounter->ByteLength == 0 ) 
		return 0;
    return MakeCounter( pCurrentCounter );
}

CPerfCounter * CPerfObjectInstance::GetFirstCounter()
{
    m_currentCounter = 0;
    return GetCounterByIndex( m_currentCounter );
}

CPerfCounter * CPerfObjectInstance::GetNextCounter()
{
    m_currentCounter++;
    return GetCounterByIndex( m_currentCounter );
}

CPerfCounter * CPerfObjectInstance::GetCounterByName( PTSTR const pszName )
{
    DWORD cntrIdx = m_pPerfCounterTitles->GetIndexFromTitleString(pszName);
    if ( cntrIdx == 0 )
        return 0;
    
    PPERF_COUNTER_DEFINITION pCurrentCounter = m_pPerfCntrDef;
    // Find the correct PERF_COUNTER_DEFINITION by looping and comparing
    for ( DWORD i = 0; i < m_nCounters; i++ )
    {
        if ( pCurrentCounter->CounterNameTitleIndex == cntrIdx )
            return MakeCounter( pCurrentCounter );        
        // Nope.  Not this one.  Advance to the next counter
        pCurrentCounter = MakePtr( PPERF_COUNTER_DEFINITION,
                                    pCurrentCounter,
                                    pCurrentCounter->ByteLength );
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

CPerfCounter::CPerfCounter( PTSTR const pszName, DWORD type,
                            PBYTE const pData, DWORD cbData )
{
    m_pszName = _tcsdup( pszName );
    m_type = type;
    m_cbData = cbData;
    m_pData = new BYTE[m_cbData];
    memcpy( m_pData, pData, m_cbData );
}

CPerfCounter::~CPerfCounter()
{
    free( m_pszName );
    delete []m_pData;
}

BOOL CPerfCounter::GetData( PBYTE pBuffer, DWORD cbBuffer, DWORD *pType )
{
    if ( cbBuffer < m_cbData )  // Make sure the buffer is big enough
        return FALSE;    
    memcpy( pBuffer, m_pData, m_cbData );   // copy the data
    if ( pType )            // If the user wants the type, give it to them
        *pType = m_type;
	return TRUE;
}

BOOL CPerfCounter::Format( PTSTR pszBuffer, DWORD nSize, BOOL fHex )
{
	PTSTR pszFmt = fHex ? TEXT("%Xh") : TEXT("%u");    
    switch ( m_cbData )
    {
        case 1: wsprintf(pszBuffer, pszFmt, *(PBYTE)m_pData);
                break;
        case 2: wsprintf(pszBuffer, pszFmt, *(PWORD)m_pData);
                break;
        case 4: wsprintf(pszBuffer, pszFmt, *(PDWORD)m_pData);
                break;
        case 8: // Danger!  Assumes little-endian (X86) byte ordering
                wsprintf( pszBuffer, TEXT("%X%X"), 
                        *(PDWORD)(m_pData+4), *(PDWORD)m_pData );
				break;
        default:
				wsprintf( pszBuffer, TEXT("<unhandled size %u>"), m_cbData );
    }
    
	return TRUE;
}

#endif //_PLATFORM_WIN32
