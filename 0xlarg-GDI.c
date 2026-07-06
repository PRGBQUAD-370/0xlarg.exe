#include <windows.h>
#include <math.h>
#include <stdio.h>
DWORD WINAPI payload1(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
        StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI payload2(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, -80, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, w - 80, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
		ReleaseDC(0, hdc);
	}
}
DWORD WINAPI payload3(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        SetStretchBltMode(hdc, 4);
        StretchBlt(hdc, 1, 1, sw + 2, sh + 2, hdc, 0, 0, sw, sh, SRCCOPY);
        StretchBlt(hdc, -1, -1, sw - 2, sh - 2, hdc, 0, 0, sw, sh, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI payload4(LPVOID lpParam) {
	int ticks = GetTickCount(), w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {

		HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc); HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0; BYTE bt = 0;
		if ((GetTickCount() - ticks) > 60000) bt = rand() & 0xffffff;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0) v = rand() % 50;
			((BYTE*)(data + i))[v ? 252 : 252] += ((BYTE*)(data + i))[i % 3] ^ bt;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hbm); DeleteObject(hdcMem);
		DeleteObject(hdc);
	}
}
DWORD WINAPI FinalPayload(LPVOID lpParam) {
    HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            rgbtriple[i].rgbtRed = (rgbtriple[i].rgbtRed * 2) % (RGB(255, 0, 0));
            rgbtriple[i].rgbtGreen = (rgbtriple[i].rgbtGreen * 2) % (RGB(0, 255, 0));
            rgbtriple[i].rgbtBlue = (rgbtriple[i].rgbtBlue * 2) % (RGB(0, 0, 255));
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
    }
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    if (MessageBoxW(NULL, L"0xlarg-GDI.exe, This is a GDI\r\nRun?", L"0xlarg.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"Are you sure? Final chance of stoping it", L"0xlarg-GDI.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else

        Sleep(1000);
        HANDLE T1 = CreateThread(NULL, 0, payload1, NULL, 0, NULL);
        Sleep(30000);
        TerminateThread(T1, 0);
        CloseHandle(T1);
        Sleep(100);
        HANDLE T2 = CreateThread(NULL, 0, payload2, NULL, 0, NULL);
        Sleep(30000);
        TerminateThread(T2, 0);
        CloseHandle(T2);
        Sleep(100);
        HANDLE T3 = CreateThread(NULL, 0, payload3, NULL, 0, NULL);
        Sleep(30000);
        TerminateThread(T3, 0);
        CloseHandle(T3);
        Sleep(100);
        HANDLE T4 = CreateThread(NULL, 0, payload4, NULL, 0, NULL);
        Sleep(30000);
        TerminateThread(T4, 0);
        CloseHandle(T4);
        HANDLE T5 = CreateThread(NULL, 0, FinalPayload, NULL, 0, NULL);
        Sleep(30000);
    }
}