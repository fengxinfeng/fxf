// capturepic.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "capturepic.h"
#include <Vfw.h>

#define MAX_LOADSTRING 100

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
void CreateBMPFile(HWND hwnd, LPCWSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

WCHAR szAppName[] = L"WebCam";
 
HWND camhwnd;
HDC hdc;
HDC hdcMem;
PAINTSTRUCT ps; 
HBITMAP hbm;
RECT rc;
HWND hWnd;
HINSTANCE g_hInstance;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CAPTUREPIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAPTUREPIC));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAPTUREPIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CAPTUREPIC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   //HWND hWnd = CreateWindowW(szAppName, szAppName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 600, 0, 0, hInstance, 0);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hButtStartCam;	 
	HWND hButtStopCam;
	HWND hButtGrabFrame;

    switch (message)
    {
	case WM_CTLCOLORSTATIC:
		 
			SetBkMode(hdc, TRANSPARENT);
	 
			return (LRESULT)CreateSolidBrush(0xFFFFFF);
	case WM_CREATE:
 
		{
 
				hButtStartCam = CreateWindowEx(0, L"BUTTON", L"Start Camera", WS_CHILD | WS_VISIBLE,
					0, 0, 100, 20, hWnd, (HMENU)1, hInst, 0);
				hButtStopCam = CreateWindowEx(0, L"BUTTON", L"Stop Camera", WS_CHILD | WS_VISIBLE,				 
					0, 25, 100, 20, hWnd, (HMENU)2, hInst, 0);
				hButtGrabFrame = CreateWindowEx(0, L"BUTTON", L"Grab Frame", WS_CHILD | WS_VISIBLE,				 
					0, 50, 100, 20, hWnd, (HMENU)3, hInst, 0);
		 		 
				camhwnd = capCreateCaptureWindow(L"camera window", WS_CHILD, 0, 100, 300, 300, hWnd, 0);			 
				//SendMessage(camhwnd, WM_CAP_DRIVER_CONNECT, 0, 0);		 
				SendMessage(camhwnd, WM_CAP_DLG_VIDEOSOURCE, 0, 0);	  
				break;
 
		}


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
			case 1:
				 
				{
					 
						ShowWindow(camhwnd, SW_SHOW);
				 
						SendMessage(camhwnd, WM_CAP_DRIVER_CONNECT, 0, 0);
				 
						SendMessage(camhwnd, WM_CAP_SET_SCALE, true, 0);
					 
						SendMessage(camhwnd, WM_CAP_SET_PREVIEWRATE, 66, 0);
					 
						SendMessage(camhwnd, WM_CAP_SET_PREVIEW, true, 0);
					 
						break;
					 
				}

			case 2:
				 
				{
					 
						ShowWindow(camhwnd, SW_HIDE);
					 
						SendMessage(camhwnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
					 
						break;
				 
				}
			case 3:
				 
				{
					 
						//Grab a Frame
						 
						SendMessage(camhwnd, WM_CAP_GRAB_FRAME, 0, 0);
					 
						//Copy the frame we have just grabbed to the clipboard
						 
						SendMessage(camhwnd, WM_CAP_EDIT_COPY, 0, 0);
					 

						 
						//Copy the clipboard image data to a HBITMAP object called hbm
						 
						hdc = BeginPaint(camhwnd, &ps);
					 
						hdcMem = CreateCompatibleDC(hdc);
					 
						if (hdcMem != NULL)
							 
						{
							 
								if (OpenClipboard(camhwnd))
									 
								{
									 
										hbm = (HBITMAP)GetClipboardData(CF_BITMAP);
									 
										SelectObject(hdcMem, hbm);
									 
										GetClientRect(camhwnd, &rc);
									 
										CloseClipboard();
									 
								}
							 
						}
					 
						//Save hbm to a .bmp file called Frame.bmp
						 
						PBITMAPINFO pbi = CreateBitmapInfoStruct(hWnd, hbm);
					 
						TCHAR szText[] = { 0 };
						LPCWSTR filename= L"TestWindow.bmp";
						CreateBMPFile(hWnd, filename, pbi, hbm, hdcMem);
						 
						 
						SendMessage(camhwnd, WM_CAP_DRIVER_CONNECT, 0, 0);
					 
						SendMessage(camhwnd, WM_CAP_SET_SCALE, true, 0);
					 
						SendMessage(camhwnd, WM_CAP_SET_PREVIEWRATE, 66, 0);
					 
						SendMessage(camhwnd, WM_CAP_SET_PREVIEW, true, 0);
					 
						break;
					 
				}
				 
 


            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		SendMessage(camhwnd, WM_CAP_DRIVER_DISCONNECT, 0, 0);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CreateBMPFile(HWND hwnd, LPCWSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
 
{
	 
		HANDLE hf;                  // file handle	 
		BITMAPFILEHEADER hdr;       // bitmap file-header	 
		PBITMAPINFOHEADER pbih;     // bitmap info-header	 
		LPBYTE lpBits;              // memory pointer	 
		DWORD dwTotal;              // total count of bytes	 
		DWORD cb;                   // incremental count of bytes	 
		BYTE *hp;                   // byte pointer 
		DWORD dwTmp;
	 
		pbih = (PBITMAPINFOHEADER)pbi;	 
		lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	 
		if (!lpBits)			 
		{		 
				MessageBox(hwnd, L"GlobalAlloc", L"Error", MB_OK);			 
		}
	 
		// Retrieve the color table (RGBQUAD array) and the bits
		 
		// (array of palette indices) from the DIB.
		 
		if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))		 
		{			 
				MessageBox(hwnd, L"GetDIBits", L"Error", MB_OK);			 
		}
	 
		// Create the .BMP file.
		 
		hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	 
		if (hf == INVALID_HANDLE_VALUE)
			 
		{			 
				MessageBox(hwnd, L"CreateFile", L"Error", MB_OK);			 
		}
	 
		hdr.bfType = 0x4d42;  // File type designator "BM" 0x42 = "B" 0x4d = "M"
	 
		// Compute the size of the entire file.
		 
		hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	 
		hdr.bfReserved1 = 0;
	 
		hdr.bfReserved2 = 0;
	 
		// Compute the offset to the array of color indices.
		 
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);
	 
		// Copy the BITMAPFILEHEADER into the .BMP file.
		 
		if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL))
			 
		{
			 
				MessageBox(hwnd, L"WriteFileHeader", L"Error", MB_OK);
			 
		}
	 
		// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
		 
		if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, NULL))			 
		{			 
				MessageBox(hwnd, L"WriteInfoHeader", L"Error", MB_OK);			 
		}
	 
		// Copy the array of color indices into the .BMP file.
		 
		dwTotal = cb = pbih->biSizeImage;
	 
		hp = lpBits;
	 
		if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))			 
		{			 
				MessageBox(hwnd, L"WriteFile", L"Error", MB_OK);			 
		}
	 
		// Close the .BMP file.
		 
		if (!CloseHandle(hf))
		{			 
				MessageBox(hwnd, L"CloseHandle", L"Error", MB_OK);			 
		}
 
		// Free memory.
		 
		GlobalFree((HGLOBAL)lpBits);
	 
}
 

 
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
 
{
	 
		BITMAP bmp;
	 
		PBITMAPINFO pbmi;
	 
		WORD cClrBits;
	 
		// Retrieve the bitmap color format, width, and height.
		 
		if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
			 
		{
			 
				MessageBox(hwnd, L"GetObject", L"Error", MB_OK);
			 
		}
	 
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
		 

		 
		if (cClrBits != 24)
			 
		{
			 
				pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
			 
		}
	 
		// There is no RGBQUAD array for the 24-bit-per-pixel format.
		 
		else
			 
			pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
	 
 
		// Initialize the fields in the BITMAPINFO structure.
		 
		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	 
		pbmi->bmiHeader.biWidth = bmp.bmWidth;
	 
		pbmi->bmiHeader.biHeight = bmp.bmHeight;
	 
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	 
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	 
		if (cClrBits < 24)
			 
		{
			 
				pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
			 
		}
	 
		// If the bitmap is not compressed, set the BI_RGB flag.
		 
		pbmi->bmiHeader.biCompression = BI_RGB;
 
		// Compute the number of bytes in the array of color
		 
		// indices and store the result in biSizeImage.
		 
		// For Windows NT, the width must be DWORD aligned unless
		 
		// the bitmap is RLE compressed. This example shows this.
		 
		// For Windows 95/98/Me, the width must be WORD aligned unless the
		 
		// bitmap is RLE compressed.
		 
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * pbmi->bmiHeader.biHeight;
	 
		// Set biClrImportant to 0, indicating that all of the
		 
		// device colors are important.
		 
		pbmi->bmiHeader.biClrImportant = 0;
	 
		return pbmi; //return BITMAPINFO
	 
}

