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
	Gdiplus::Graphics* graphics2;
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

	// Print a rectangle almost the size of the page
	PrinterDC->Rectangle(100, 100, PrinterDC->GetDeviceCaps(HORZRES) - 100, PrinterDC->GetDeviceCaps(VERTRES) - 100);
	// Print a smaller rectangle
	PrinterDC->Rectangle(0, 0, 500, 500);
	
	/* Draw with Gdiplus::Graphics */
	// Draw green rectangles
	graphics = new Gdiplus::Graphics(memDC);
	graphics->DrawRectangle(pen, 50, 50, 200, 300);

	// Print a green rectangle
	graphics2 = new Gdiplus::Graphics(PrinterDC->GetSafeHdc());
	graphics2->DrawRectangle(pen, 50, 50, 200, 300);
	
	// Save to PNG
	CImage Image;
	Image.Attach(bitmap);
	Image.Save(L"D:/file.png", Gdiplus::ImageFormatPNG);
	
	// Finish printing document
	EndPage(hPrinterDC);
	EndDoc(hPrinterDC);

	// Bmp is now safe from further drawing.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return 0;
}

