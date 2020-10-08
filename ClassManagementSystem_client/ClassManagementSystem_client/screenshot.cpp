#include "screenshot.h"


HBITMAP ScreenShot() {

    HWND hwnd;
    HDC hdc[2];
    HBITMAP hbitmap;
    RECT rect;

    hwnd = GetDesktopWindow();
    GetClientRect(hwnd, &rect);
    hdc[0] = GetWindowDC(hwnd);
    hbitmap = CreateCompatibleBitmap(hdc[0], rect.right, rect.bottom);
    hdc[1] = CreateCompatibleDC(hdc[0]);
    SelectObject(hdc[1], hbitmap);

    BitBlt(
        hdc[1],
        0,
        0,
        rect.right,
        rect.bottom,
        hdc[0],
        0,
        0,
        SRCCOPY
    );
    return hbitmap;
}

HBITMAP ReadFromFile(LPCWSTR path) {
    HBITMAP hBMP = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return hBMP;
}

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.  
    assert(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure  
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
    // data structures.)  

    if (cClrBits < 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER) +
            sizeof(RGBQUAD) * (1 << cClrBits));

    // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure.  

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    // The width must be DWORD aligned unless the bitmap is RLE 
    // compressed. 
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
        * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

void CreateBMPFile(LPCWSTR pszFile, HBITMAP hBMP)
{
    HANDLE hf;                 // file handle  
    BITMAPFILEHEADER hdr;       // bitmap file-header  
    PBITMAPINFOHEADER pbih;     // bitmap info-header  
    LPBYTE lpBits;              // memory pointer  
    DWORD dwTotal;              // total count of bytes  
    DWORD cb;                   // incremental count of bytes  
    BYTE* hp;                   // byte pointer  
    DWORD dwTmp;
    PBITMAPINFO pbi;
    HDC hDC;

    hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
    SelectObject(hDC, hBMP);

    pbi = CreateBitmapInfoStruct(hBMP);

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    assert(lpBits);

    // Retrieve the color table (RGBQUAD array) and the bits  
    // (array of palette indices) from the DIB.  
    assert(GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
        DIB_RGB_COLORS));

    std::cout << pszFile << std::endl;

    // Create the .BMP file.  
    hf = CreateFile(pszFile,
        GENERIC_READ | GENERIC_WRITE,
        (DWORD)0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    assert(hf != INVALID_HANDLE_VALUE);

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
    // Compute the size of the entire file.  
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.  
    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.  
    assert(WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
        (LPDWORD)&dwTmp, NULL));

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
    assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
        + pbih->biClrUsed * sizeof(RGBQUAD),
        (LPDWORD)&dwTmp, (NULL)));

    // Copy the array of color indices into the .BMP file.  
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    assert(WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL));

    // Close the .BMP file.  
    assert(CloseHandle(hf));

    // Free memory.  
    GlobalFree((HGLOBAL)lpBits);
}

double CompareHbitmap(HBITMAP HBitmapLeft, HBITMAP HBitmapRight){
    long long cnt=0;
    long long total=0;
    if (HBitmapLeft == HBitmapRight){
        return 1.0;
    }

    if (NULL == HBitmapLeft || NULL == HBitmapRight){
        return 0.0;
    }


    HDC hdc = GetDC(NULL);
    BITMAPINFO BitmapInfoLeft = { 0 };
    BITMAPINFO BitmapInfoRight = { 0 };

    BitmapInfoLeft.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfoRight.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    if (0 != GetDIBits(hdc, HBitmapLeft, 0, 0, NULL, &BitmapInfoLeft, DIB_RGB_COLORS) &&
        0 != GetDIBits(hdc, HBitmapRight, 0, 0, NULL, &BitmapInfoRight, DIB_RGB_COLORS))
    {
        // Compare the BITMAPINFOHEADERs of the two bitmaps

        if (0 == memcmp(&BitmapInfoLeft.bmiHeader, &BitmapInfoRight.bmiHeader,
            sizeof(BITMAPINFOHEADER)))
        {
            // The BITMAPINFOHEADERs are the same so now compare the actual bitmap bits

            BYTE* pLeftBits = new BYTE[BitmapInfoLeft.bmiHeader.biSizeImage];
            BYTE* pRightBits = new BYTE[BitmapInfoRight.bmiHeader.biSizeImage];
            BYTE* pByteLeft = NULL;
            BYTE* pByteRight = NULL;

            PBITMAPINFO pBitmapInfoLeft = &BitmapInfoLeft;
            PBITMAPINFO pBitmapInfoRight = &BitmapInfoRight;

            // calculate the size in BYTEs of the additional

            // memory needed for the bmiColor table

            int AdditionalMemory = 0;
            switch (BitmapInfoLeft.bmiHeader.biBitCount)
            {
            case 1:
                AdditionalMemory = 1 * sizeof(RGBQUAD);
                break;
            case 4:
                AdditionalMemory = 15 * sizeof(RGBQUAD);
                break;
            case 8:
                AdditionalMemory = 255 * sizeof(RGBQUAD);
                break;
            case 16:
            case 32:
                AdditionalMemory = 2 * sizeof(RGBQUAD);
            }

            if (AdditionalMemory)
            {
                // we have to allocate room for the bmiColor table that will be

                // attached to our BITMAPINFO variables

                pByteLeft = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
                if (pByteLeft)
                {
                    memset(pByteLeft, 0, sizeof(BITMAPINFO) + AdditionalMemory);
                    memcpy(pByteLeft, pBitmapInfoLeft, sizeof(BITMAPINFO));
                    pBitmapInfoLeft = (PBITMAPINFO)pByteLeft;
                }

                pByteRight = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
                if (pByteRight)
                {
                    memset(pByteRight, 0, sizeof(BITMAPINFO) + AdditionalMemory);
                    memcpy(pByteRight, pBitmapInfoRight, sizeof(BITMAPINFO));
                    pBitmapInfoRight = (PBITMAPINFO)pByteRight;
                }
            }

            if (pLeftBits && pRightBits && pBitmapInfoLeft && pBitmapInfoRight)
            {
                // zero out the bitmap bit buffers

                memset(pLeftBits, 0, BitmapInfoLeft.bmiHeader.biSizeImage);
                memset(pRightBits, 0, BitmapInfoRight.bmiHeader.biSizeImage);

                // fill the bit buffers with the actual bitmap bits

                if (0 != GetDIBits(hdc, HBitmapLeft, 0,
                    pBitmapInfoLeft->bmiHeader.biHeight, pLeftBits, pBitmapInfoLeft,
                    DIB_RGB_COLORS) && 0 != GetDIBits(hdc, HBitmapRight, 0,
                        pBitmapInfoRight->bmiHeader.biHeight, pRightBits, pBitmapInfoRight,
                        DIB_RGB_COLORS))
                {
                    // compare the actual bitmap bits of the two bitmaps
                    total = pBitmapInfoLeft->bmiHeader.biSizeImage;
                    for (int i = 0;i < pBitmapInfoLeft->bmiHeader.biSizeImage;i++) {
                        if (pLeftBits[i] == pRightBits[i])cnt++;
                    }
                   /* bool bSame = 0 == memcmp(pLeftBits, pRightBits,
                        pBitmapInfoLeft->bmiHeader.biSizeImage);*/
                }
            }

            // clean up

            delete[] pLeftBits;
            delete[] pRightBits;
            delete[] pByteLeft;
            delete[] pByteRight;
        }
    }

    ReleaseDC(NULL, hdc);

    return 1.0*cnt/total;
}