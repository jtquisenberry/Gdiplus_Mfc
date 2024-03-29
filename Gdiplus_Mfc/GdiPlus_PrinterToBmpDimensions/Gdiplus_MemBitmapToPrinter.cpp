// Printer_DC_to_Bitmap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "afxwin.h"
#include "atlimage.h"
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include <conio.h>
using namespace Gdiplus;

int main()
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize DOCINFO structure
	DOCINFO docInfo;
	ZeroMemory(&docInfo, sizeof(docInfo));
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = L"GdiplusPrint";

	// Get printer HDC, CDC
	LPCTSTR printerName = L"Bullzip PDF Printer";
	HDC hPrinterDC = CreateDC(L"WINSPOOL", printerName, NULL, NULL);

	// Setup graphics objects
	Gdiplus::Graphics* graphics;
	Pen* pen = new Pen(Color(255, 0, 255, 0));

	// Start printing
	StartDoc(hPrinterDC, &docInfo);
	StartPage(hPrinterDC);

	// Get printer CDC*, memory CDC
	CDC * PrinterDC = CDC::FromHandle(hPrinterDC);
	CDC memDC;
	memDC.CreateCompatibleDC(PrinterDC);

	// Create a new bitmap and size it based on the printer device capabilities.
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(PrinterDC, PrinterDC->GetDeviceCaps(HORZRES), PrinterDC->GetDeviceCaps(VERTRES));
	//bitmap.CreateCompatibleBitmap(PrinterDC, 1500, 1500);

	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);

	/* Draw without Gdiplus::Graphics*/
	// Draw a rectangle almost the size of the bitmap
	memDC.Rectangle(100, 100, PrinterDC->GetDeviceCaps(HORZRES) - 100, PrinterDC->GetDeviceCaps(VERTRES) - 100);
	// Draw a smaller rectangle
	memDC.Rectangle(0, 0, 500, 500);

	// Draw with Gdiplus::Graphics
	// Draw green rectangles
	graphics = new Gdiplus::Graphics(memDC);
	graphics->DrawRectangle(pen, 50, 50, 200, 300);
	graphics->DrawRectangle(pen, 150, 150, 301, 400);
	graphics->DrawRectangle(pen, 150, 150, 401, 500);
	graphics->DrawRectangle(pen, 50, 50, 501, 600);

	// Copy the contents of the memDC to PrinterDC
	PrinterDC->BitBlt(0, 0, PrinterDC->GetDeviceCaps(HORZRES), PrinterDC->GetDeviceCaps(VERTRES), &memDC, 0, 0, SRCCOPY);


	CImage Image;
	Image.Attach(bitmap);
	Image.Save(L"D:/file.png", Gdiplus::ImageFormatPNG);

	
	// End printing
	EndPage(hPrinterDC);
	EndDoc(hPrinterDC);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return 0;

}

