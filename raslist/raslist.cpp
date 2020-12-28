// raslist.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>
#include <Ras.h>
#include <Raserror.h>

#pragma comment (lib,"rasapi32.lib")

#include <string>
#include <vector>

using namespace std;


// 拨号前查找PPPoE设备
//
static DWORD EnumDialDevices (const char * requiredDeviceType, vector<string> & deviceNames)
{
    DWORD i         = 0;
    DWORD dwRet     = ERROR_SUCCESS;
    DWORD dwcb      = 0;
    DWORD dwDevices = 0;

    LPRASDEVINFOA lpRasDevInfo = NULL;

    // allocate buffer for one device
    dwcb = sizeof(RASDEVINFOA);
    lpRasDevInfo = (LPRASDEVINFOA) HeapAlloc(GetProcessHeap(), 0, (UINT) dwcb);
    lpRasDevInfo->dwSize = dwcb;

    // Call RasEnumDevices with lpRasDevInfo = NULL.
    // dwCb is returned with the required buffer size and 
    // A return code of ERROR_BUFFER_TOO_SMALL
    dwRet = RasEnumDevicesA(lpRasDevInfo, &dwcb, &dwDevices);

    if (dwRet == ERROR_BUFFER_TOO_SMALL && dwDevices != 0) {
        HeapFree(GetProcessHeap(), 0, lpRasDevInfo);

        // found more then one device(s)
        lpRasDevInfo = (LPRASDEVINFOA) HeapAlloc(GetProcessHeap(), 0, (UINT) dwcb);

        for (i = 0; i < dwDevices; i++) {
            lpRasDevInfo[i].dwSize = sizeof(RASDEVINFOA);
        }

        dwRet = RasEnumDevicesA(lpRasDevInfo, &dwcb, &dwDevices);
    }

    if (dwRet == 0) {
        for(i = 0; i < dwDevices; i++) {
            printf("[ras dev#%d] %s:%s\n", i+1, lpRasDevInfo[i].szDeviceType, lpRasDevInfo[i].szDeviceName);

            deviceNames.push_back(lpRasDevInfo[i].szDeviceName);
        }
    }

    HeapFree(GetProcessHeap(), 0, lpRasDevInfo);

    // There was either a problem with RAS or there are no RAS devices to enumerate
    return dwRet;
}


int main()
{
    vector<string> deviceNames;

    int ret = EnumDialDevices(0, deviceNames);

    system("pause");
    return 0;
}

 