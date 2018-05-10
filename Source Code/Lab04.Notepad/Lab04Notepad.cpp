// Lab04Notepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab04Notepad.h"
#include "windowsX.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk);
void OnClose(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB04NOTEPAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB04NOTEPAD));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB04NOTEPAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB04NOTEPAD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
//Edit box
HWND ctlEdit = NULL;
#define HK_OPEN 1
#define HK_SAVE 2
#define HK_NEW 3

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"NotePad", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
      341, 78, 683, 612, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   RegisterHotKey(hWnd, HK_OPEN, MOD_CONTROL, 0x4F); // Ctrl + O
   RegisterHotKey(hWnd, HK_SAVE, MOD_CONTROL, 0x53); // Ctrl + S
   RegisterHotKey(hWnd, HK_NEW, MOD_CONTROL, 0x4E); // Ctrl + N
   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_HOTKEY, OnHotKey);
		HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

//Biến cho biết file đang chỉnh sửa có sẵn hay chưa tạo
LPWSTR fileOpenedName = NULL;

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(17, 6,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	ctlEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | SS_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL,
		6, 4, 660, 550, hWnd, NULL, hInst, NULL);
	SendMessage(ctlEdit, WM_SETFONT, WPARAM(hFont), TRUE);
	return true;
}

void OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify) {
	WCHAR *bufferContent = NULL;
	WCHAR *buffOpen = NULL;
	string buffLine;
	string buffPar;
	ofstream oFile;
	ifstream iFile;
	string buffCompare;
	int count;


	//Tham khảo OPENFILEDIALOG và SAVEFILEDIALOG từ stackoverflow
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = (LPCWSTR)L"txt";
	//

	int sizeContent = 0;

	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case ID_FILE_NEW:
		//Lấy nội dung editbox
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (fileOpenedName) {
			iFile.open(fileOpenedName);
			if (iFile.is_open()) {
				while (!iFile.eof()) {
					getline(iFile, buffLine);
					buffCompare = buffCompare + buffLine;
					if (!iFile.eof()) {
						buffCompare = buffCompare + '\n';
					}
				}
				iFile.close();
			}
		}
		if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
			SetWindowText(ctlEdit, L"");
			delete[] fileOpenedName;
			fileOpenedName = NULL;

		}
		else {
			int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi thoát?", L"NotePad", MB_YESNOCANCEL);
			if (IDYES == id) {
				if (!fileOpenedName) {
					//Chuyển hộp thoại Open thành hộp thoại Save
					ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (GetSaveFileName(&ofn)) {
						oFile.open(ofn.lpstrFile);
						oFile << buffPar;
					}
				}
				else {
					oFile.open(fileOpenedName);
					oFile << buffPar;
				}
				SetWindowText(ctlEdit, L"");
				delete[] fileOpenedName;
				fileOpenedName = NULL;
			}
			else if (IDNO == id) {
				SetWindowText(ctlEdit, L"");
				delete[] fileOpenedName;
				fileOpenedName = NULL;
			}
		}
		break;
	case IDM_EXIT:
		//Lấy nội dung editbox
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (fileOpenedName) {
			iFile.open(fileOpenedName);
			if (iFile.is_open()) {
				while (!iFile.eof()) {
					getline(iFile, buffLine);
					buffCompare = buffCompare + buffLine;
					if (!iFile.eof()) {
						buffCompare = buffCompare + '\n';
					}
				}
				iFile.close();
			}
		}
		if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
			delete[] fileOpenedName;
			fileOpenedName = NULL;
			UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
			UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
			UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
			DestroyWindow(hWnd);
		}
		else {
			int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi thoát?", L"NotePad", MB_YESNOCANCEL);
			if (IDYES == id) {
				if (!fileOpenedName) {
					//Chuyển hộp thoại Open thành hộp thoại Save
					ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (GetSaveFileName(&ofn)) {
						oFile.open(ofn.lpstrFile);
						oFile << buffPar;
					}
				}
				else {
					oFile.open(fileOpenedName);
					oFile << buffPar;
				}
				delete[] fileOpenedName;
				fileOpenedName = NULL;
				UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
				UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
				UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
				DestroyWindow(hWnd);
			}
			else if (IDNO == id) {
				delete[] fileOpenedName;
				fileOpenedName = NULL;
				UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
				UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
				UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
				DestroyWindow(hWnd);
			}
		}
		break;
	case ID_FILE_SAVE:
		//Chuyển hộp thoại Open thành hộp thoại Save
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (!fileOpenedName) {
			if (GetSaveFileName(&ofn)) {
				oFile.open(ofn.lpstrFile);
				oFile << buffPar;
				count = 0;
				while (ofn.lpstrFile[count] != '\0') {
					count++;
				}
				delete[] fileOpenedName;
				fileOpenedName = NULL;
				fileOpenedName = new TCHAR[count + 1];
				for (int j = 0; j < count; j++)
					::fileOpenedName[j] = ofn.lpstrFile[j];
				::fileOpenedName[count] = '\0';
			}
		}
		else { // Tệp đang chỉnh sửa được load từ file có sẵn trước đó => Lưu trực tiếp lên file
			oFile.open(fileOpenedName);
			oFile << buffPar;
		}
		oFile.close();
		break;
	case ID_FILE_OPEN:
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (fileOpenedName) {
			iFile.open(fileOpenedName);
			if (iFile.is_open()) {
				while (!iFile.eof()) {
					getline(iFile, buffLine);
					buffCompare = buffCompare + buffLine;
					if (!iFile.eof()) {
						buffCompare = buffCompare + '\n';
					}
				}
				iFile.close();
			}
		}
		if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
			if (GetOpenFileName(&ofn))
			{
				iFile.open(ofn.lpstrFile);
				if (iFile.is_open()) { //Nếu file mở thành công
					buffPar.clear();
					while (!iFile.eof()) {
						getline(iFile, buffLine);
						buffPar = buffPar + buffLine;
						if (!iFile.eof()) {
							buffPar = buffPar + '\r' + '\n';
						}
					}
					//Chuyển string sang WCHAR* để setwindowtext
					int n = buffPar.length() + 1;
					buffOpen = new WCHAR[n];
					for (int i = 0; i < n - 1; i++) {
						buffOpen[i] = buffPar[i];
					}
					buffOpen[n - 1] = '\0';
					buffPar.clear();
					SetWindowText(ctlEdit, buffOpen);
					iFile.close();

				}
			}
			else break;
		}
		else {
			int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi mở file khác?", L"NotePad", MB_YESNOCANCEL);
			if (IDYES == id) {
				if (!fileOpenedName) {
					//Chuyển hộp thoại Open thành hộp thoại Save
					ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (GetSaveFileName(&ofn)) {
						oFile.open(ofn.lpstrFile);
						oFile << buffPar;
					}
				}
				else {
					oFile.open(fileOpenedName);
					oFile << buffPar;
				}
				break;
			}
			else if (IDNO == id) {
				if (GetOpenFileName(&ofn))
				{
					iFile.open(ofn.lpstrFile);
					if (iFile.is_open()) { //Nếu file mở thành công
						buffPar.clear();
						while (!iFile.eof()) {
							getline(iFile, buffLine);
							buffPar = buffPar + buffLine;
							if (!iFile.eof()) {
								buffPar = buffPar + '\r' + '\n';
							}
						}
						//Chuyển string sang WCHAR* để setwindowtext
						int n = buffPar.length() + 1;
						buffOpen = new WCHAR[n];
						for (int i = 0; i < n - 1; i++) {
							buffOpen[i] = buffPar[i];
						}
						buffOpen[n - 1] = '\0';
						buffPar.clear();
						SetWindowText(ctlEdit, buffOpen);
						iFile.close();
					}
				}
			}
			else break;
		}
		//Lấy tên file open
		count = 0;
		while (ofn.lpstrFile[count] != '\0') {
			count++;
		}
		delete[] fileOpenedName;
		fileOpenedName = NULL;
		fileOpenedName = new TCHAR[count + 1];
		for (int j = 0; j < count; j++)
			::fileOpenedName[j] = ofn.lpstrFile[j];
		::fileOpenedName[count] = '\0';
		break;
	case ID_EDIT_CUT:
		SendMessage(ctlEdit, WM_CUT, 0, 0);
		break;

	case ID_EDIT_COPY:
		SendMessage(ctlEdit, WM_COPY, 0, 0);
		break;

	case ID_EDIT_PASTE:
		SendMessage(ctlEdit, WM_PASTE, 0, 0);
		break;
	}

	if (!bufferContent)
		delete[] bufferContent;
	if (!buffOpen)
		delete[] buffOpen;
}

void OnHotKey(HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk) {
	WCHAR *bufferContent = NULL;
	WCHAR *buffOpen = NULL;
	string buffLine;
	string buffPar;
	ofstream oFile;
	ifstream iFile;
	string buffCompare;

	//Tham khảo OPENFILEDIALOG và SAVEFILEDIALOG từ stackoverflow
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = (LPCWSTR)L"txt";
	//

	int sizeContent = 0;
	int count;

	switch (idHotKey) {
	case HK_OPEN:
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (fileOpenedName) {
			iFile.open(fileOpenedName);
			if (iFile.is_open()) {
				while (!iFile.eof()) {
					getline(iFile, buffLine);
					buffCompare = buffCompare + buffLine;
					if (!iFile.eof()) {
						buffCompare = buffCompare + '\n';
					}
				}
				iFile.close();
			}
		}
		if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
			if (GetOpenFileName(&ofn))
			{
				iFile.open(ofn.lpstrFile);
				if (iFile.is_open()) { //Nếu file mở thành công
					buffPar.clear();
					while (!iFile.eof()) {
						getline(iFile, buffLine);
						buffPar = buffPar + buffLine;
						if (!iFile.eof()) {
							buffPar = buffPar + '\r' + '\n';
						}
					}
					//Chuyển string sang WCHAR* để setwindowtext
					int n = buffPar.length() + 1;
					buffOpen = new WCHAR[n];
					for (int i = 0; i < n - 1; i++) {
						buffOpen[i] = buffPar[i];
					}
					buffOpen[n - 1] = '\0';
					buffPar.clear();
					SetWindowText(ctlEdit, buffOpen);
					iFile.close();

				}
			}
			else break;
		}
		else {
			int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi mở file khác?", L"NotePad", MB_YESNOCANCEL);
			if (IDYES == id) {
				if (!fileOpenedName) {
					//Chuyển hộp thoại Open thành hộp thoại Save
					ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (GetSaveFileName(&ofn)) {
						oFile.open(ofn.lpstrFile);
						oFile << buffPar;
					}
				}
				else {
					oFile.open(fileOpenedName);
					oFile << buffPar;
				}
				break;
			}
			else if (IDNO == id) {
				if (GetOpenFileName(&ofn))
				{
					iFile.open(ofn.lpstrFile);
					if (iFile.is_open()) { //Nếu file mở thành công
						buffPar.clear();
						while (!iFile.eof()) {
							getline(iFile, buffLine);
							buffPar = buffPar + buffLine;
							if (!iFile.eof()) {
								buffPar = buffPar + '\r' + '\n';
							}
						}
						//Chuyển string sang WCHAR* để setwindowtext
						int n = buffPar.length() + 1;
						buffOpen = new WCHAR[n];
						for (int i = 0; i < n - 1; i++) {
							buffOpen[i] = buffPar[i];
						}
						buffOpen[n - 1] = '\0';
						buffPar.clear();
						SetWindowText(ctlEdit, buffOpen);
						iFile.close();
					}
				}
			}
			else break;
		}
		//Lấy tên file open
		count = 0;
		while (ofn.lpstrFile[count] != '\0') {
			count++;
		}
		delete[] fileOpenedName;
		fileOpenedName = NULL;
		fileOpenedName = new TCHAR[count + 1];
		for (int j = 0; j < count; j++)
			::fileOpenedName[j] = ofn.lpstrFile[j];
		::fileOpenedName[count] = '\0';
		break;
	case HK_SAVE:
		//Chuyển hộp thoại Open thành hộp thoại Save
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (!fileOpenedName) {
			if (GetSaveFileName(&ofn)) {
				oFile.open(ofn.lpstrFile);
				oFile << buffPar;
				//Lấy tên file open
				count = 0;
				while (ofn.lpstrFile[count] != '\0') {
					count++;
				}
				delete[] fileOpenedName;
				fileOpenedName = NULL;
				fileOpenedName = new TCHAR[count + 1];
				for (int j = 0; j < count; j++)
					::fileOpenedName[j] = ofn.lpstrFile[j];
				::fileOpenedName[count] = '\0';
			}
		}
		else { // Tệp đang chỉnh sửa được load từ file có sẵn trước đó => Lưu trực tiếp lên file
			oFile.open(fileOpenedName);
			oFile << buffPar;
		}
		oFile.close();
		break;
	case HK_NEW:
		//Lấy nội dung editbox
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		sizeContent = GetWindowTextLength(ctlEdit);
		bufferContent = new WCHAR[sizeContent + 1];
		GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
		buffPar.clear();

		for (int i = 0; i < sizeContent; i++) {
			if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
				buffPar.push_back(bufferContent[i]);
		}
		if (fileOpenedName) {
			iFile.open(fileOpenedName);
			if (iFile.is_open()) {
				while (!iFile.eof()) {
					getline(iFile, buffLine);
					buffCompare = buffCompare + buffLine;
					if (!iFile.eof()) {
						buffCompare = buffCompare + '\n';
					}
				}
				iFile.close();
			}
		}
		if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
			SetWindowText(ctlEdit, L"");
			delete[] fileOpenedName;
			fileOpenedName = NULL;
		}
		else {
			int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi thoát?", L"NotePad", MB_YESNOCANCEL);
			if (IDYES == id) {
				if (!fileOpenedName) {
					//Chuyển hộp thoại Open thành hộp thoại Save
					ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (GetSaveFileName(&ofn)) {
						oFile.open(ofn.lpstrFile);
						oFile << buffPar;
					}
				}
				else {
					oFile.open(fileOpenedName);
					oFile << buffPar;
				}
				SetWindowText(ctlEdit, L"");
				delete[] fileOpenedName;
				fileOpenedName = NULL;
			}
			else if (IDNO == id) {
				SetWindowText(ctlEdit, L"");
				delete[] fileOpenedName;
				fileOpenedName = NULL;
			}
		}
		break;
	}
	if (!bufferContent)
		delete[] bufferContent;
	if (!buffOpen)
		delete[] buffOpen;
}

void OnClose(HWND hWnd) { // Luồng sự kiện phụ: Kiểm soát khi thoát chương trình
	//Tham khảo OPENFILEDIALOG và SAVEFILEDIALOG từ stackoverflow
	OPENFILENAME ofn;
	ifstream iFile;
	string buffLine;
	string buffPar;
	string buffCompare;
	char szFileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = (LPCWSTR)L"txt";
	//
	WCHAR *bufferContent = NULL;
	ofstream oFile;
	int sizeContent = 0;

	//Lấy nội dung editbox
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	sizeContent = GetWindowTextLength(ctlEdit);
	bufferContent = new WCHAR[sizeContent + 1];
	GetWindowText(ctlEdit, bufferContent, sizeContent + 1);
	buffPar.clear();

	for (int i = 0; i < sizeContent; i++) {
		if ('\r' != bufferContent[i] || '\n' != bufferContent[i + 1])
			buffPar.push_back(bufferContent[i]);
	}
	if (fileOpenedName) {
		iFile.open(fileOpenedName);
		if (iFile.is_open()) {
			while (!iFile.eof()) {
				getline(iFile, buffLine);
				buffCompare = buffCompare + buffLine;
				if (!iFile.eof()) {
					buffCompare = buffCompare + '\n';
				}
			}
			iFile.close();
		}
	}
	if ((!fileOpenedName && sizeContent == 0) || (fileOpenedName != NULL && buffCompare.compare(buffPar) == 0)) {
		delete[] fileOpenedName;
		fileOpenedName = NULL;
		UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
		UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
		UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
		DestroyWindow(hWnd);
	}
	else {
		int id = MessageBox(hWnd, L"Bạn chưa lưu file đang chỉnh sửa, bạn có muốn lưu file trước khi thoát?", L"NotePad", MB_YESNOCANCEL);
		if (IDYES == id) {
			if (!fileOpenedName) {
				//Chuyển hộp thoại Open thành hộp thoại Save
				ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

				if (GetSaveFileName(&ofn)) {
					oFile.open(ofn.lpstrFile);
					oFile << buffPar;
				}
			}
			else {
				oFile.open(fileOpenedName);
				oFile << buffPar;
			}
			delete[] fileOpenedName;
			fileOpenedName = NULL;
			UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
			UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
			UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
			DestroyWindow(hWnd);
		}
		else if (IDNO == id) {
			delete[] fileOpenedName;
			fileOpenedName = NULL;
			UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
			UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
			UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
			DestroyWindow(hWnd);
		}
	}
}

void OnDestroy(HWND hWnd) {
	delete[] fileOpenedName;
	fileOpenedName = NULL;
	UnregisterHotKey(hWnd, HK_OPEN); // Bỏ đăng ký Ctrl + O
	UnregisterHotKey(hWnd, HK_SAVE); // Bỏ đăng ký Ctrl + S
	UnregisterHotKey(hWnd, HK_NEW); // Bỏ đăng ký Ctrl + N
	PostQuitMessage(0);
}