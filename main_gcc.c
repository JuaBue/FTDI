#include <stdio.h>
#include <conio.h>
#include <time.h>

#include <stdarg.h>
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>

#include <string.h>
#include <math.h>

#include "ftd2xx.h"

static FILE * s_fileHandler = NULL;

void fileOpen()
{
    char fileName[FILENAME_MAX] = {0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fileHandler = NULL;

    snprintf(fileName, sizeof(fileName),"%d%02d%02d%02d%02d%02d_raw.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    s_fileHandler = fopen(fileName, "w+");
    if (NULL == s_fileHandler)
    {
        printf("[ERROR] Opening file %s", fileName);
    }
    printf("Open file %s\n",fileName);
}

void fileClose()
{
    if (NULL != s_fileHandler)
    {
        fclose(s_fileHandler);
        s_fileHandler = NULL;
        printf("File closed");
    }
}

void fileAppendData(unsigned char * dataRead)
{
    int ret = 0;
    int bytesToWrite = 0;
    unsigned char writeBuffer[4] = {0};
    
    if (( NULL != s_fileHandler) && (NULL != dataRead))
    {
        bytesToWrite = snprintf(writeBuffer, sizeof(writeBuffer), "%02X,", *dataRead);
        ret = fwrite(writeBuffer, 1, bytesToWrite, s_fileHandler);
        if (ret != bytesToWrite)
        {
            printf("[ERROR] %d of %u written", ret, bytesToWrite);
        }
    }
}

int main()
{
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

    ftStatus = FT_SetTimeouts(handle,200,0);
    if (ftStatus == FT_OK) {
        printf("FT_SetTimeouts OK\n");
    }
    else {
        printf("FT_SetTimeouts Failed\n");
    }

    //Open the file
    fileOpen();
    printf("Press any key to stop reading\n");  
    while(!kbhit())
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
            fileAppendData(&read_buf);
            memset(&read_buf, 0, sizeof(unsigned char));
        }
    }    
    printf("Read stopped\n");  
    fileClose();
    FT_Close(handle);
    return 0;
}