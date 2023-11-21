/*+@@file@@----------------------------------------------------------------*//*!
 \file		ToastDemoUI.c
 \par Description 
            GUI demo for ToastLib
 \par Project: 
            ToastLib
 \date		Created  on Mon Nov 20 18:48:00 2023
 \date		Modified on Mon Nov 20 18:48:00 2023
 \author	frankie
\*//*-@@file@@----------------------------------------------------------------*/
#define __STDC_WANT_LIB_EXT1__ 1
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <guiddef.h>
#include <combaseapi.h>
#include <tchar.h>
#include <stdio.h>
#include <wchar.h>
#include "include/ToastLib.h"
#include "resources.h"
#ifdef __POCC__
#ifdef _WIN64
#pragma comment(lib, "WRlibs/x64/runtimeobject.lib")
#pragma comment(lib, "bin/x64/ToastLib.lib")
#else
#pragma comment(lib, "WRlibs/x32/runtimeobject.lib")
#pragma comment(lib, "bin/x32/ToastLib.lib")
#endif
#else
#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "ToastLib.lib")
#endif
#pragma comment(lib, "ComCtl32.lib")

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/
static INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/
/*
 * AUMID (Application User Model ID) for current app.
 */
#define APP_AUMID L"ToastActivatorPlainC-UI"
/*
 * To distinguish between user invocation and Toast notify invocation we add a parameter to command line.
 */
#define TOAST_ACTIVATED_LAUNCH_ARG "-ToastActivated"

/*
 * The GUID "{00660CDA-75E9-4E21-9DCC-E5DA3B980ECB}" that we associate with our factory that produces our
 * INotificationActivationCallback interface.
 */
static const GUID GUID_Impl_INotificationActivationCallback = {0x660cda,0x75e9,0x4e21,{0x9d,0xcc,0xe5,0xda,0x3b,0x98,0xe,0xcb}};

/*
 * The XML that describes the notification that will be shown.
 * Of course, this can be built at runtime, and more can be done with it, but for this basic example, this will suffice.
 */
const wchar_t wszText[] =
		L"<toast scenario=\"reminder\" "
		L"activationType=\"foreground\" launch=\"action=mainContent\" duration=\"short\">\r\n"
		L"	<visual>\r\n"
		L"		<binding template=\"ToastGeneric\">\r\n"
		L"			<text><![CDATA[This is a demo notification for PellesC]]></text>\r\n"
		L"			<text><![CDATA[It contains 2 lines of text]]></text>\r\n"
		L"			<text placement=\"attribution\"><![CDATA[Created by frankie.\r\nBased on Valentin-Gabriel Radu demo]]></text>\r\n"
		L"		</binding>\r\n"
		L"	</visual>\r\n"
		L"  <actions>\r\n"
		L"	  <input id=\"tbReply\" type=\"text\" placeHolderContent=\"Send a message to the app\"/>\r\n"
		L"	  <action content=\"Send\" activationType=\"foreground\" arguments=\"action=reply\"/>\r\n"
		L"	  <action content=\"Visit forum\" activationType=\"protocol\" arguments=\"https://forum.pellesc.de\"/>\r\n"
		L"	  <action content=\"Close app\" activationType=\"foreground\" arguments=\"action=closeApp\"/>\r\n"
		L"  </actions>\r\n"
		L"	<audio src=\"ms-winsoundevent:Notification.Default\" loop=\"false\" silent=\"false\"/>\r\n"
		L"</toast>\r\n";

static HANDLE ghInstance;
static H_CALLBACK h_Callback = NULL;
static H_TOAST hToast        = NULL;
static HFONT hfArial         = NULL;

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		SetLvItem
 \date		Created  on Mon Nov 20 21:29:18 2023
 \date		Modified on Mon Nov 20 21:29:18 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
INT SetLvItem(HWND hListView, PCWCH wszText, INT iItem, INT iSubItem)
{
	LVITEM lvi;
	lvi.mask     = LVIF_TEXT;
	lvi.pszText  = (PWCHAR)wszText;
	lvi.iItem    = iItem;
	lvi.iSubItem = iSubItem;
	return (INT)SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvi);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		AddLvItem
 \date		Created  on Mon Nov 20 21:29:47 2023
 \date		Modified on Mon Nov 20 21:29:47 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
INT AddLvItem(HWND hListView, PCWCH wszText)
{
	LVITEM lvi;
	lvi.mask     = LVIF_TEXT;
	lvi.pszText  = (PWCHAR)wszText;
	lvi.iItem    = (INT)SendMessage(hListView, LVM_GETITEMCOUNT, 0, 0);
	lvi.iSubItem = 0;
	return (INT)SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fnNotificationCallback
 \date		Created  on Mon Nov 20 18:17:49 2023
 \date		Modified on Mon Nov 20 18:17:49 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static INT_PTR WINAPI fnNotificationCallback(INT_PTR args, LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
{
	/*
	 * The user callback function is called from the toast notify callback and receives the same data
	 * passed from framework to the COM callback with another parameter 'args' that holds data passed
	 * by user application in the 'fToastRegisterCallback()' function when registering app for callback.
	 */
	HWND hListView = (HWND)args;
	INT index = AddLvItem(hListView, appUserModelId);
	SetLvItem(hListView, appUserModelId, index, 0);
	SetLvItem(hListView, invokedArgs,    index, 1);
	WCHAR wszCount[16];
	SetLvItem(hListView, _itow(count, wszCount, 10), index, 2);
	if (count)
	{
		SetLvItem(hListView, data[0].Key,    index, 3);
		SetLvItem(hListView, data[0].Value,  index, 4);
	}

	if (!_wcsicmp(invokedArgs, L"action=closeApp"))
	{
		if (IDYES == MessageBox(GetParent(hListView), L"Do you want quit this app?", L"Close App", MB_YESNO|MB_ICONQUESTION))
			PostThreadMessage(GetWindowThreadProcessId(GetParent(hListView), NULL), WM_QUIT, 0, 0);
	}

	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		MakeFont
 \date		Created  on Mon Nov 20 20:44:00 2023
 \date		Modified on Mon Nov 20 20:44:00 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HFONT MakeFont(LPTSTR szFont, INT nPointSize, INT nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, DWORD dwCharSet) // by Napalm
{
	HDC hDC = GetDC(HWND_DESKTOP);
	nPointSize = -MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(HWND_DESKTOP, hDC);
	return CreateFont(nPointSize, 0, 0, 0, nWeight, bItalic, bUnderline, bStrikeOut, dwCharSet, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, FF_DONTCARE, szFont);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		SetColumns
 \date		Created  on Mon Nov 20 21:48:52 2023
 \date		Modified on Mon Nov 20 21:48:52 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
void SetLvColumns(HWND hWndListView)
{
	static PCWCH awszHeaders[] = {L"AUMID", L"invokedArgs", L"Count", L"data[0].Key", L"data[0].Value"};
	static INT   ColWidth[]    = {     180,            150,       55,            120,              400};
	LVCOLUMN lvColumn = {LVCF_FMT | LVCF_TEXT | LVCF_WIDTH, LVCFMT_LEFT, 0, NULL};
	for (int iColumn=0; iColumn<(sizeof(awszHeaders)/sizeof(awszHeaders[0])); iColumn++)
	{
		lvColumn.cx      = ColWidth[iColumn];
		lvColumn.pszText = (PWCHAR)awszHeaders[iColumn];
		ListView_InsertColumn(hWndListView, iColumn, &lvColumn);
	}
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		GetExePath
 \date		Created  on Mon Nov 20 19:08:04 2023
 \date		Modified on Mon Nov 20 19:08:04 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static BOOL GetExePath(WCHAR *wszExePath, DWORD size, WCHAR *wszArg)
{
	if (!GetModuleFileNameW(NULL, wszExePath + 1, size - (DWORD)wcslen(wszArg) - 2))
		return FALSE;
	wszExePath[0] = L'"';
	(void)wcscat_s(wszExePath, size, L"\" ");
	(void)wcscat_s(wszExePath, size, wszArg);
	return TRUE;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		wWinMain
 \date		Created  on Mon Nov 20 17:47:39 2023
 \date		Modified on Mon Nov 20 17:47:39 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, WCHAR *pszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASSEX wcx;

    ghInstance = hInstance;

    /* Initialize common controls. Also needed for MANIFEST's */
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);


    /* Get system dialog information */
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    /* Add our own stuff */
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.lpszClassName = L"ToastDemClass";
    if (!RegisterClassEx(&wcx))
        return 0;

	/*
	 * Initialize COM and Windows Runtime on this thread.
	 * Make sure that the threading models of the two match.
	 */
	if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		return 1;

	if (!SUCCEEDED(RoInitialize(RO_INIT_MULTITHREADED)))
		return 1;

    /* The user interface is a modal dialog box */
    return (int)DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		MainDlgProc
 \date		Created  on Mon Nov 20 17:46:26 2023
 \date		Modified on Mon Nov 20 17:46:26 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
		{
			wchar_t  wszExePath[MAX_PATH + 100] = {0};
			// Obtain full path of our app.
			if (!GetExePath(wszExePath, MAX_PATH + 100, _T(TOAST_ACTIVATED_LAUNCH_ARG)))
			{
				MessageBox(hwndDlg, L"GetExePath() failure.", L"Error", MB_OK|MB_ICONERROR);
				PostQuitMessage(1);
			}
			// Register app for COM execution
			if (!fToastRegisterApp(wszExePath, APP_AUMID, L"Toast Activator Plain C Example for PellesC", L"FF00FF00", &GUID_Impl_INotificationActivationCallback))
			{
				MessageBox(hwndDlg, L"fToastRegisterApp() failure.", L"Error", MB_OK|MB_ICONERROR);
				PostQuitMessage(1);
			}
			// Register our callback function for toast notifications
			if (S_OK != fToastRegisterCallback(&h_Callback, &GUID_Impl_INotificationActivationCallback, fnNotificationCallback, (INT_PTR)GetDlgItem(hwndDlg, LV_NOTIFY)))
			{
				MessageBox(hwndDlg, L"fToastRegisterCallback() failure.", L"Error", MB_OK|MB_ICONERROR);
				PostQuitMessage(1);
			}
			// Create a toast manager to generate toast motifications
			if (!SUCCEEDED(fToastCreateNotifyManager(&hToast, APP_AUMID, &GUID_Impl_INotificationActivationCallback)))
			{
				MessageBox(hwndDlg, L"fToastCreateNotifyManager() failure.", L"Error", MB_OK|MB_ICONERROR);
				PostQuitMessage(1);
			}

			hfArial = MakeFont(_TEXT("Arial"), 11, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET);
			SendMessage(GetDlgItem(hwndDlg, LV_NOTIFY), WM_SETFONT, (WPARAM)hfArial, FALSE);
			ListView_SetTextColor(GetDlgItem(hwndDlg, LV_NOTIFY), RGB(10, 10, 160));
			SetLvColumns(GetDlgItem(hwndDlg, LV_NOTIFY));
            return TRUE;
		}

        case WM_SIZE:
            return TRUE;

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
				case BT_SEND:
				{
					if (!fToastSendNotification(hToast, wszText))
					{
						MessageBox(hwndDlg, L"fToastSendNotification() failure.", L"Error", MB_OK|MB_ICONERROR);
						PostQuitMessage(3);
					}
					return TRUE;
				}
                case IDOK:
                    EndDialog(hwndDlg, TRUE);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
			/*
			 * De-initialize and exit.
			 */
			if (hfArial)
				DeleteObject(hfArial);
			if (hToast)
				fToastDeleteNotifyManager(hToast);
			if (h_Callback)
				fToastUnRegisterCallback(h_Callback);
			RoUninitialize();
			CoUninitialize();
            EndDialog(hwndDlg, 0);
            return TRUE;
    }

    return FALSE;
}
