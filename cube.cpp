#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <chrono>
#include <thread>
#include <math.h>
#include <cstdio>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE; // hide cursor
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
#endif

void hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

float A, B, C;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float cubeWidth = 15;
int width = 136;
int height = 30;
float zBuffer[136 * 30];
char buffer[136 * 30];
int distanceFromCam = 100;
int backgroundASCIIcode = ' ';
float K1 = 40;
float horizontalOffset;

float incrementSpeed = 1;

float calculateX(int i, int j, int k) 
{
    return j * sin(A) * sin(B) * cos(C) - 
           k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + 
           k * sin(A) * sin(C) + 
           i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) 
{
    return j * cos(A) * cos(C) + 
           k * sin(A) * cos(C) - 
           j * sin(A) * sin(B) * sin(C) +
           k * cos(A) * sin(B) * sin(C) - 
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) 
{
    return k * cos(A) * cos(B) - 
           j * sin(A) * cos(B) + 
           i * sin(B);
}

void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch) 
{
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    // projection equations & line index for selecting pixel (or char)
    // width and height help center
    xp = 45 + (width / 2) + horizontalOffset + (K1 * ooz * x * 2);
    yp = (height / 2) + (K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) 
    {
        if (ooz > zBuffer[idx]) 
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    system("cls");
    hideCursor();

    while (true) 
    {
        std::cout << "\033[H" << std::flush;

        // sizeof(float) is because we need to multiply width and height by the bytesize of float
        memset(buffer, backgroundASCIIcode, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        cubeWidth = 20;
        horizontalOffset = -2 * cubeWidth;

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                calculateSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateSurface(cubeWidth, cubeY, cubeX, '$');
                calculateSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        for (int k = 0; k < width * height; k++) 
        {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.15;
        B += 0.15;
        C += 0.03;

        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
    return 0;
}

