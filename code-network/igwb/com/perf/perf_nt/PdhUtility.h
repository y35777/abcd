///////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WIN32

#ifndef _PDHUTILITY_H
#define _PDHUTILITY_H

#include <winperf.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <tchar.h>

#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

class CPerfTitleDatabase
{
private:
	DWORD    m_nLastIndex;
	LPTSTR*		 m_TitleStrings;   
	LPTSTR       m_pszRawStrings;
public:
    CPerfTitleDatabase();
    ~CPerfTitleDatabase();
    DWORD GetLastTitleIndex(){	return m_nLastIndex; }
    LPTSTR       GetTitleStringFromIndex( DWORD index );
    DWORD		 GetIndexFromTitleString( LPTSTR pszTitleString );

private:
	void GetNameStrings();
};

class CPerfSnapshot
{
private:        
    PPERF_DATA_BLOCK    m_pPerfDataHeader;      // Points to snapshot data
    CPerfTitleDatabase  * m_pCounterTitles;     // The title conversion object
    
	// Private function to convert the ASCII strings passedto TakeSnapshot()
    // into a suitable form for the RegQueryValue call
    BOOL ConvertSnapshotItemName( PTSTR pszIn, PTSTR pszOut, DWORD nSize );

public:
    CPerfSnapshot( CPerfTitleDatabase * pCounterTitles );    
    ~CPerfSnapshot();
    BOOL TakeSnapshot( PTSTR pszSnapshotItems );
    void DisposeSnapshot();
    DWORD GetNumObjectTypes();    // # of objects the snapshot includes
    BOOL GetSystemName( PTSTR pszSystemName, DWORD nSize );    
    PVOID GetPostHeaderPointer(); // Pointer to data following header
};

class CPerfObject;

class CPerfObjectList
{
public:        
    CPerfObjectList(CPerfSnapshot * const pPerfSnapshot,
                    CPerfTitleDatabase * const pPerfTitleDatabase );
    ~CPerfObjectList(){ };

    // Functions that return CPerfObject pointers.  Caller is responsible
    // for deleting the CPerfObject * when done with it.
    CPerfObject * GetFirstPerfObject();
    CPerfObject * GetNextPerfObject();    
    CPerfObject * GetPerfObject( PTSTR const pszObjListName );

protected:
    CPerfSnapshot * m_pPerfSnapshot;
    CPerfTitleDatabase * m_pPerfCounterTitles;
    DWORD m_currentObjectListIndex;
    PPERF_OBJECT_TYPE m_pCurrObjectType;    // current first/next object ptr
};

typedef CPerfObjectList * PCPerfObjectList;

class CPerfObjectInstance;

class CPerfObject
{
public:        
    CPerfObject(    PPERF_OBJECT_TYPE const pObjectList,
                    CPerfTitleDatabase * const pPerfTitleDatabase );
    ~CPerfObject(){ };
    
    // Functions that return CPerfObjectInstance pointers.  Caller is
    // responsible for deleting the CPerfObjectInstance * when done with it.
    CPerfObjectInstance * GetFirstObjectInstance();
    CPerfObjectInstance * GetNextObjectInstance();    
    DWORD GetObjectInstanceCount(){return m_pObjectList->NumInstances;}
    BOOL GetObjectTypeName( PTSTR pszObjTypeName, DWORD nSize );

protected:
    PPERF_OBJECT_TYPE m_pObjectList;
    DWORD m_currentObjectInstance;
    PPERF_INSTANCE_DEFINITION m_pCurrentObjectInstanceDefinition;        
    CPerfTitleDatabase * m_pPerfCounterTitles;
};

typedef CPerfObject * PCPerfObject;

class CPerfCounter;

class CPerfObjectInstance
{
public:        
    CPerfObjectInstance(
            PPERF_INSTANCE_DEFINITION const pPerfInstDef,
            PPERF_COUNTER_DEFINITION const pPerfCntrDef, DWORD nCounters,
            CPerfTitleDatabase * const pPerfTitleDatabase, BOOL fDummy );

    ~CPerfObjectInstance(){ };
    
    BOOL GetObjectInstanceName( PTSTR pszObjInstName, DWORD nSize );    
    
	// Functions that return CPerfCounter pointers.  Caller is
    // responsible for deleting the CPerfCounter * when done with it.
    CPerfCounter * GetFirstCounter();
    CPerfCounter * GetNextCounter();
    CPerfCounter * GetCounterByName( PTSTR const pszName );

protected:
    PPERF_INSTANCE_DEFINITION m_pPerfInstDef;
    DWORD m_nCounters;
    DWORD m_currentCounter;    
    PPERF_COUNTER_DEFINITION m_pPerfCntrDef;
    CPerfTitleDatabase * m_pPerfCounterTitles;
    CPerfCounter * MakeCounter( PPERF_COUNTER_DEFINITION const pCounter );
    CPerfCounter * GetCounterByIndex( DWORD index );
    CPerfTitleDatabase *m_pCounterTitleDatabase;
    BOOL m_fDummy;  // FALSE normally, TRUE when an object with no instances
};

typedef CPerfObjectInstance * PCPerfObjectInstance;

class CPerfCounter
{
public:
    CPerfCounter(   PTSTR const pszName, DWORD type,
                    PBYTE const pData, DWORD cbData );
    ~CPerfCounter();
    PTSTR GetName() { return m_pszName; }
    DWORD GetType() { return m_type; }    
    BOOL GetData( PBYTE pBuffer, DWORD cbBuffer, DWORD *pType );    
    BOOL Format( PTSTR pszBuffer, DWORD nSize, BOOL fHex = FALSE );
   
protected:
	PTSTR m_pszName;
    DWORD m_type;
    PBYTE m_pData;
    DWORD m_cbData;
};

typedef CPerfCounter * PCPerfCounter;

#endif

#endif //_PLATFORM_WIN32
