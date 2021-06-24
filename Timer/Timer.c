// Timer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
//
//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
//
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define START_TAG_FILE_NAME        "timer.tag"
#define TIME_FORMAT                "%02u:%02u:%02u"

struct dostime_t dtime;

void ShowCopyright(void)
{
    fprintf(stderr, "Timer Utility V0.91\n");
    fprintf(stderr, "Copyright (c) 2008 Insyde Software Corporation\n\n");
}

void ShowUsage(void)
{
    printf("This tool support timestamps to batch operation\n\n"
        "Usage:  Timer [/t] [/s] [/p]\n"
        "\t/t : Show current time\n"
        "\t/s : Make a start tag\n"
        "\t/p : Make a stop operation\n"
        "\n");
}

void ShowTime(void)
{
    //
    // Get current time
    //
    _dos_gettime(&dtime);
    printf(TIME_FORMAT, dtime.hour, dtime.minute, dtime.second);
}

void StartTimerOperation(char* sStartTagFileName)
{
    FILE* StartTagFile;

    if ((StartTagFile = fopen(sStartTagFileName, "wt")) == NULL)
    {
        fprintf(stderr, "Can not open Accumultor file %s\n\n", sStartTagFileName);
        exit(-2);
    }
    _dos_gettime(&dtime);
    fprintf(StartTagFile, TIME_FORMAT, dtime.hour, dtime.minute, dtime.second);

    fclose(StartTagFile);
}

struct dostime_t* pDiffDOSTime(struct dostime_t* pStartTime,
    struct dostime_t* pStopTime
)
{
    struct dostime_t IntervalTime;
    int mcarry, hcarry;

    if (pStopTime->second < pStartTime->second)
    {
        pStopTime->second += 60;
        mcarry = 1;
    }
    else
    {
        mcarry = 0;
    }
    IntervalTime.second = pStopTime->second - pStartTime->second;

    if ((pStopTime->minute - mcarry) < pStartTime->minute)
    {
        pStopTime->minute += 60;
        hcarry = 1;
    }
    else
    {
        hcarry = 0;
    }
    IntervalTime.minute = pStopTime->minute - pStartTime->minute - mcarry;

    if ((pStopTime->hour - hcarry) < pStartTime->hour)
    {
        pStopTime->hour += 24;
        //		dcarry = 1;
    }
    else
    {
        //		dcarry = 0;
    }
    IntervalTime.hour = pStopTime->hour - pStartTime->hour - hcarry;

    return &IntervalTime;
}

void StopTimerOperation(char* sStartTagFileName)
{
    FILE* StartTagFile;
    struct dostime_t dtime_s;
    struct dostime_t* pdtime_i;

    if ((StartTagFile = fopen(sStartTagFileName, "rt")) == NULL)
    {
        printf("Can not open Accumultor file %s\n\n", sStartTagFileName);
        exit(-2);
    }

    fscanf(StartTagFile, TIME_FORMAT, &dtime_s.hour, &dtime_s.minute, &dtime_s.second);
    _dos_gettime(&dtime);
    printf("Start-");
    printf(TIME_FORMAT, dtime_s.hour, dtime_s.minute, dtime_s.second);
    printf(",\tStop-");
    printf(TIME_FORMAT, dtime.hour, dtime.minute, dtime.second);
    printf(",\tDuration-");
    pdtime_i = pDiffDOSTime(&dtime_s, &dtime);
    printf(TIME_FORMAT, pdtime_i->hour, pdtime_i->minute, pdtime_i->second);
    printf("\n");

    fclose(StartTagFile);
}

int main(int argc, char* argv[])
{
    int i;
    char option_char;

    ShowCopyright();
    //
    // processing parameters
    //
    if (argc > 1)
    {
        for (i = 1; i < argc; i++)
        {
            if ((*argv[i] == '/') || (*argv[i] == '-'))
            {
                option_char = (char)(*(argv[i] + 1));
                switch (option_char)
                {
                case 'T':
                case 't':
                    ShowTime();
                    break;
                case 'S':
                case 's':
                    StartTimerOperation(START_TAG_FILE_NAME);
                    break;
                case 'P':
                case 'p':
                    StopTimerOperation(START_TAG_FILE_NAME);
                    break;
                case 'H':
                case 'h':
                case '?':
                    ShowUsage();
                    break;
                default:
                    break;
                }
            }
        }
    }
    else
    {
        ShowUsage();
    }

    return 0;

}

