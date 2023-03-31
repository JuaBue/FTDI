#include <stdio.h>

#include <stdarg.h>
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>

#include <string.h>
#include <math.h>

#include "ftd2xx.h"

int main()
{
    int i,n;
    unsigned char read_buf = 0;
    DWORD numDevs;
    FT_STATUS ftStatus;
    FT_HANDLE handle;
    DWORD wBytes;
    DWORD rBytes;
    FT_STATUS res;
    LONG comport;

    /* Configuración FTDI */
    if(FT_Open(0, &handle) != FT_OK) {
        puts("Can't open device"); 
        return 1;
    }

    ftStatus = FT_SetDataCharacteristics(handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_EVEN);
    if (ftStatus == FT_OK) {
        printf("FT_SetDataCharacteristics OK\n");
    }
    else {
        printf("FT_SetDataCharacteristics Failed\n");
    }

    ftStatus = FT_SetBaudRate(handle, 1000000);
    if (ftStatus == FT_OK) {
        printf("FT_SetBaudRate OK\n");
    }
    else {
        printf("FT_SetBaudRate Failed\n");
    }

    while(1)
    {
        res = FT_Read(handle, &read_buf, 1, &rBytes);
        if (res != FT_OK)
        {
            printf("read failed %d\n", res);
            FT_Close(handle);
            return 1;
        }
        else
        {
            printf("0x%02X\n", read_buf);       
        }
    }    

    FT_Close(handle);
    return 0;
}