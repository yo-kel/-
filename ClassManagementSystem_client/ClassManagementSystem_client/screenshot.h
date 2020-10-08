#pragma once
#ifndef SCREENSHOTH
#define SCREENSHOTH


#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>


PBITMAPINFO CreateBitmapInfoStruct(HBITMAP);
void CreateBMPFile(LPCWSTR pszFile, HBITMAP hBMP);
HBITMAP ScreenShot();
#endif // !SCREENSHOTH
