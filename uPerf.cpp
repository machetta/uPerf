// uPerf.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <pdh.h>
#include <pdhmsg.h>
#include "stdafx.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x86\\pdh.lib")

bool run = true;
int interval = 1000;
int line_number = 0;
int counter_number = 0;
wchar_t counters[10][1024];
char line_buffer[1024];

CONST PWSTR COUNTER_OBJECT = L"PhysicalDisk";

HQUERY hQuery = NULL;
HLOG hLog = NULL;
PDH_STATUS pdhStatus;
DWORD dwLogType = PDH_LOG_TYPE_BINARY;
//DWORD dwLogType = PDH_LOG_TYPE_CSV;
HCOUNTER hCounter;
FILE *pConfigFile;

/*
wchar_t* counters[] = {
L"\\PhysicalDisk(0 C: D:)\\Disk Reads/sec",
L"\\PhysicalDisk(0 C: D:)\\Disk Writes/sec",
L"\\PhysicalDisk(0 C: D:)\\Disk Read Bytes/sec",
L"\\PhysicalDisk(0 C: D:)\\Disk Write Bytes/sec",
L"\\PhysicalDisk(0 C: D:)\\Avg. Disk Bytes/Read",
L"\\PhysicalDisk(0 C: D:)\\Avg. Disk sec/Write",
L"\\PhysicalDisk(0 C: D:)\\Avg. Disk sec/Read",
L"\\PhysicalDisk(0 C: D:)\\Avg. Disk Read Queue Length",
L"\\PhysicalDisk(0 C: D:)\\Avg. Disk Write Queue Length" };
*/

BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
	
	run = false;

	switch( fdwCtrlType ) 
    {
		// Handle the CTRL-C signal. 
		case CTRL_C_EVENT: 
		printf( "\nCtrl-C event detected, exiting\n" );
		Beep( 750, 300 );
		return( TRUE );

		default:
		return TRUE;
	}
}

void ListCounters(void)
{
    PDH_STATUS status = ERROR_SUCCESS;
    LPWSTR pwsCounterListBuffer = NULL;
    DWORD dwCounterListSize = 0;
    LPWSTR pwsInstanceListBuffer = NULL;
    DWORD dwInstanceListSize = 0;
    LPWSTR pTemp = NULL;

    // Determine the required buffer size for the data. 
    status = PdhEnumObjectItems(
        NULL,                   // real-time source
        NULL,                   // local machine
        COUNTER_OBJECT,         // object to enumerate
        pwsCounterListBuffer,   // pass NULL and 0
        &dwCounterListSize,     // to get required buffer size
        pwsInstanceListBuffer, 
        &dwInstanceListSize, 
        PERF_DETAIL_WIZARD,     // counter detail level
        0); 

    if (status == PDH_MORE_DATA) 
    {
        // Allocate the buffers and try the call again.
        pwsCounterListBuffer = (LPWSTR)malloc(dwCounterListSize * sizeof(WCHAR));
        pwsInstanceListBuffer = (LPWSTR)malloc(dwInstanceListSize * sizeof(WCHAR));

        if (NULL != pwsCounterListBuffer && NULL != pwsInstanceListBuffer) 
        {
            status = PdhEnumObjectItems(
                NULL,                   // real-time source
                NULL,                   // local machine
                COUNTER_OBJECT,         // object to enumerate
                pwsCounterListBuffer, 
                &dwCounterListSize,
                pwsInstanceListBuffer, 
                &dwInstanceListSize, 
                PERF_DETAIL_WIZARD,     // counter detail level
                0); 
     
            if (status == ERROR_SUCCESS) 
            {
                wprintf(L"\nCounters that the PhysicalDisk object defines:\n");
				wprintf(L"---------------------------------------------\n");

                // Walk the counters list. The list can contain one
                // or more null-terminated strings. The list is terminated
                // using two null-terminator characters.
                for (pTemp = pwsCounterListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1) 
                {
                     wprintf(L"%s\n", pTemp);
                }

                wprintf(L"\nInstances of the PhysicalDisk object:\n");
				wprintf(L"------------------------------------\n");

                // Walk the instance list. The list can contain one
                // or more null-terminated strings. The list is terminated
                // using two null-terminator characters.
                for (pTemp = pwsInstanceListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1) 
                {
                     wprintf(L"%s\n", pTemp);
                }
            }
            else 
            {
                wprintf(L"Second PdhEnumObjectItems failed with %0x%x.\n", status);
            }
        } 
        else 
        {
            wprintf(L"Unable to allocate buffers.\n");
            status = ERROR_OUTOFMEMORY;
        }
    } 
    else 
    {
        wprintf(L"\nPdhEnumObjectItems failed with 0x%x.\n", status);
    }

    if (pwsCounterListBuffer != NULL) 
        free (pwsCounterListBuffer);

    if (pwsInstanceListBuffer != NULL) 
        free (pwsInstanceListBuffer);
}

int _tmain(int argc, _TCHAR* argv[])
{

    if (argc < 2) 
    {
        wprintf(L"\nFormat: uPerf <log file name> [<interval in milisecs, 10 - 1000000, default: 1000>]\n\n");
		wprintf(L"Counters displayed below can be used to update the configuration file.\n");
		wprintf(L"Configuration file format: \\PhysicalDisk(<Instance>)\\<Counter>\n");
		wprintf(L"Example: \\PhysicalDisk(0 C: D:)\\Disk Reads/sec\n");
		ListCounters();
        goto cleanup;
    }

	if (argc == 3) interval = _ttoi(argv[2]);

	if (interval < 10 || interval > 1000000)
	{
		wprintf(L"Update interval should be between 10 and 3,600,000 ms");
		return 1;
	}

	if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) != TRUE ) 
	{
		wprintf(L"ERROR: Could not set control handler\n");
		return 1;
	}

    // Open a query object.
    pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);

    if (pdhStatus != ERROR_SUCCESS)
    {
        wprintf(L"PdhOpenQuery failed with 0x%x\n", pdhStatus);
        return 1;
    }

	// Open and read the config file
	if ( fopen_s(&pConfigFile, "uPerf.cfg", "rb") != 0)
	{
		wprintf(L"Failed to open config file\n");
		return 1;
	}

	while ( fgets (line_buffer, sizeof (line_buffer), pConfigFile) )
	{
		size_t pR = 0;
		errno_t err;

		line_buffer[strlen(line_buffer) - 2] = '\0';
		err = mbstowcs_s(&pR, counters[line_number], sizeof (counters[line_number]), line_buffer, sizeof (line_buffer));
		if ( err != 0)
		{
			wprintf(L"mbstowcs_s failed to convert ANSI string to WIDE: %d\n", err);
			return 1;
		}
//		wprintf(L"Counter #%d read: %ls\n", line_number, counters[line_number]);
		line_number++;
	}
	fclose (pConfigFile);

	// Open the log file for write access.
    pdhStatus = PdhOpenLog(argv[1], 
        PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS,
        &dwLogType,
        hQuery,
        0, 
        NULL,
        &hLog);

    if (pdhStatus != ERROR_SUCCESS)
    {
        wprintf(L"PdhOpenLog failed with 0x%x\n", pdhStatus);
        goto cleanup;
    }

	while ( counter_number < line_number)
	{
		// Add one counter that will provide the data.
		pdhStatus = PdhAddCounter(hQuery, (LPCWSTR)counters[counter_number], 0, &hCounter);

		if (pdhStatus != ERROR_SUCCESS)
		{
			wprintf(L"PdhAddCounter failed with 0x%x\n", pdhStatus);
			goto cleanup;
		}
		else
		{
			wprintf(L"Registered: %ls\n", counters[counter_number]);
		}

		counter_number++;
	}

    // Write performance data to the log file.
    while (run) 
    {
        wprintf(L".");

        pdhStatus = PdhUpdateLog (hLog, NULL);
        if (ERROR_SUCCESS != pdhStatus)
        {
            wprintf(L"PdhUpdateLog failed with 0x%x\n", pdhStatus);
            goto cleanup;
        }

        // Wait one second between samples for a counter update.
        Sleep(interval); 
    }

cleanup:

    // Close the log file.
    if (hLog)
        PdhCloseLog (hLog, 0);

    // Close the query object.
    if (hQuery)
        PdhCloseQuery (hQuery);

	return 0;
}

