// RenameFiles.cpp : Defines the entry point for the application.
//

#pragma comment(lib,"bufferoverflowU.lib")

#include "stdafx.h"
#include "resource.h"
#include <windowsx.h>
#include "WINBASE.H"
#include <WinUser.h>
#include <commdlg.h> //dialog
#include <commctrl.h>
#define MAX_LOADSTRING 100
#include <SHLOBJ.H> //for setdir
#include <SHELLAPI.H>
#include <mmsystem.h>

#include <gdiplus.h>
#pragma  comment (lib, "gdiplus.lib")
//#define ULONG_PTR DWORD
using namespace Gdiplus;

int d;
extern int ver=122;
extern char subver[8]="";
int AutoFix=0;
int AutoFixTime=1; //hours
HWND hWnd=NULL;

char ExcelFileName[]="Names.xlsx";
char INDDFileName[]="source.indd";
char ProgramPatch[MAX_PATH];
char FilePatch[MAX_PATH];
WCHAR temp_path[1024];
WCHAR source_path[1024];
WCHAR target_path[1024];
int item_count=0;
extern DWORD n_bytes4;

extern HWND hCompareWnd;
extern int File_count,File_count2;
extern int Cur_source_pic,Cur_target_pic;
extern int scroll_x2,scroll_y2;

HFONT hUIFont;
int RenameSameFile=0;

WCHAR sU[128];
static WCHAR FindFileDirectory_[MAX_PATH]=L"\\";
static HWND hEditSourceDirecrory,	hBrowseFolderButton,hLoadXLSX;
HWND hProgressBarWnd,hOpenCompareWindowButton ,hRenameSameFile, hUndoRenameButton, hWaitingWnd, hSelectExtension,hRenameButton, hTranslatedSuffix, hSkipEmptyFiles, hSkipEnglishStrings, hListBoxBU,hOkButton,hFindFilesButton_, hFinfInSubDirectoriesCheckButton,hAddButton,hOpenDBButton, hDeleteButton_, hCancelButton ,hEditDescription,hListBoxSourceLang,hListBoxTargetLang,hEditProjectName,hEditDatabaseName,	hListSourceFiles,hEditPMName,hEditEditSubject,hEditMCN;
int Open_Excel_XLSX_file(char * source_file);
int Open_INDD_file(char * source_file);

void CreateCompareWindow( );

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int table_item_count=TABLE_ITEM;
WCHAR FileTable[TABLE_ITEM][2][64];


DWORD dwGlobPrevTime;
DWORD dwGlobCurTime;

RECT rt;
char UserName[64];
int  send(char*);
DWORD dwIdSend ;
HANDLE hThreadSend;
unsigned long __stdcall SendThread(void *Param)
{
	send(UserName);	
	ExitThread(0);
   return( 0 ); 
}
void On_paint();

DWORD dwIdAutoFixFolder ;
HANDLE hAutoFixFolder;
unsigned long __stdcall Sender2(void *Param);

int Current_hint2=0;
int max_hint2=6;
char hints2[6][200]={
"Rename Files: Please press 'Load Names List' to load prepared Excel file or press 'Create Names List' to create new one.",
"Rename Files: Please press 'Browse' button to select folder that contains localized pictures.",
"Rename Files: Please press 'Find' button to search localized pictures in the folder.",
"Rename Files: Please press 'Rename' button to rename localized pictures according to Excel file.",
"Rename Files: Please press 'Reverse' button to rename rename the files back, if necessary.",
"Rename Files"
};


unsigned long __stdcall AutoFixFolder(void *Param)
{
	Sleep(1000*60*1);//wait for creating Window
	
	while(hWnd>0)
	{
		//Sleep(1000*60*2);
		 Sleep(1000*60*60*AutoFixTime);
		 SendMessageW((HWND)hWnd,WM_COMMAND, FIND_FOLDERS,0);
		 Sleep(1000*60*10);//10 min
		 SendMessageW((HWND)hWnd,WM_COMMAND, RENAME_FILES,0);

	
	}
	ExitThread(0);
   return( 0 ); 
}

void Show_error()
{
LPVOID lpMsgBuf;
FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    0,
    (LPTSTR) &lpMsgBuf,
    0,
    NULL
);
MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONERROR);
LocalFree(lpMsgBuf);

};

int Process_cancelled=0;
LRESULT CALLBACK hWaitingWndProc(HWND hWaitingWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
 
 switch( uMsg )// InvalidateRect( hWaitingWnd, NULL, TRUE );
 {
 		case WM_CREATE:

			GetClientRect(hWaitingWnd, &rt);

			break;


	/*	case WM_COMMAND :
			switch( LOWORD( wParam ) )
             { 
				
			//	case CANCEL:
				//	Process_cancelled=1;
				//	break;

			//	default:
					return DefWindowProc(hWaitingWnd, uMsg, wParam, lParam);
			 }
             break;*/
			
      default :
            return( DefWindowProc( hWaitingWnd, uMsg, wParam, lParam ) );
   }
   return( 0L );
}

void CreateProcessWindow(char* str )
{


#define WINDOWS_WIDTH 210
#define WINDOWS_HIGHT 100
DestroyWindow(hWaitingWnd);
GetWindowRect(hWnd, &rt);
WNDCLASSEX wc2;

						  wc2.style         = CS_PARENTDC ;//|CS_HREDRAW | CS_VREDRAW | |CS_HREDRAW | CS_VREDRAW; 
						  wc2.lpfnWndProc   = (WNDPROC)hWaitingWndProc;       
						  wc2.cbClsExtra    = sizeof( WORD );                           
						  wc2.cbWndExtra    = 0;                           
						  wc2.hInstance     = hInst;                   
						  wc2.hIcon         = LoadIcon(hInst, 0); 
						  wc2.hCursor       = LoadCursor(NULL, IDC_ARROW); 
						  wc2.hbrBackground = (HBRUSH)(16);  
						  wc2.lpszMenuName  = 0;//(LPCSTR)IDC_TRANSLATOR2;//lpszAppName;                   
						  wc2.lpszClassName = "New Class8";  
						  wc2.cbSize        = sizeof( WNDCLASSEX );
						  wc2.hIconSm       = LoadIcon(wc2.hInstance, (LPCTSTR)IDI_SMALL);;//LoadImage( hInst, lpszAppName,
								
						RegisterClassEx( &wc2 );
					 
						  //if(hNewWnd)//WS_EX_TOPMOST
								//hWaitingWnd = CreateWindowEx(WS_EX_TOPMOST,"New Class8",  str,
								hWaitingWnd = CreateWindowEx(0,"New Class8",  str,
											WS_POPUP | WS_VISIBLE | WS_CAPTION,
												   rt.left+(rt.right-rt.left)/2-WINDOWS_WIDTH/2,
													rt.top+(rt.bottom-rt.top)/2-WINDOWS_HIGHT/2,	
												 WINDOWS_WIDTH,WINDOWS_HIGHT,	  0,  NULL,	  hInst, NULL );
						  /*else
								hWaitingWnd = CreateWindow("New Class8",  str,
												  WS_POPUP | WS_VISIBLE | WS_CAPTION|WS_EX_TOPMOST,
												    (GetSystemMetrics(SM_CXSCREEN)-WINDOWS_WIDTH)/2,
													 (GetSystemMetrics(SM_CYSCREEN)-WINDOWS_HIGHT)/2,	
												 WINDOWS_WIDTH,WINDOWS_HIGHT,	  0,  NULL,	  hInst, NULL );
						  */
						UpdateWindow(hWaitingWnd);
}

DWORD User_name_len=sizeof(UserName);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	//MSG msg;
	HACCEL hAccelTable;
	MSG msg, * pMsg;
	pMsg=&msg;

 	 ULONG_PTR gdiplusToken;
 	GdiplusStartupInput gdiplusStartupInput;
 	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	GetUserNameA( UserName,&User_name_len);
 
	hThreadSend=CreateThread( NULL, 0,SendThread, NULL, 0, &dwIdSend );

	HANDLE 	hThreadSend=CreateThread( NULL, 0,Sender2, NULL, 0, &dwIdSend );

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RENAMEFILES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_RENAMEFILES);

	// Main message loop:
	while (TRUE)///GetMessage(&msg, NULL, 0, 0)) 
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (WM_QUIT==pMsg->message)
				break;

		 	/*if (WM_PAINT==pMsg->message)
			{	if(hCompareWnd>0)
				On_paint();
			
			} */
				
			if(WM_KEYDOWN == pMsg->message  )	
			if((HWND)pMsg->hwnd ==( HWND)hCompareWnd)
			{
				if( 0x57  == pMsg->wParam) //W up
					{ 
						Cur_source_pic--;
						if(Cur_source_pic<0)
						  Cur_source_pic=0;

						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.right=scroll_x2;
						 //InvalidateRect(hCompareWnd, &rt, TRUE );*/
						if(hCompareWnd>0)
							On_paint();
					}
		 
					if( 0x53  == pMsg->wParam) //S down
					{ 
						Cur_source_pic++;
						if(Cur_source_pic>=File_count)
						  Cur_source_pic=File_count-1;
						
						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.right=scroll_x2;
						// InvalidateRect(hCompareWnd, &rt, TRUE );*/
						if(hCompareWnd>0)
							On_paint();
					}
					if( 0x41  == pMsg->wParam) //W up
					{ 
						Cur_source_pic-=6;
						if(Cur_source_pic<0)
						  Cur_source_pic=0;

						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.right=scroll_x2;
						//InvalidateRect(hCompareWnd, &rt, TRUE );*/
					}
		 
					if( 0x44  == pMsg->wParam) //S down
					{ 
						Cur_source_pic+=6;
						if(Cur_source_pic>=File_count)
						  Cur_source_pic=File_count-1;
						
						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.right=scroll_x2;
						//InvalidateRect(hCompareWnd, &rt, TRUE );*/
					}
					if( 0x26  == pMsg->wParam) //up
					{ 
						Cur_target_pic-=1;
						if(Cur_target_pic<0)
						  Cur_target_pic=0;

						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.left=scroll_x2;
					//	InvalidateRect(hCompareWnd, &rt, TRUE );*/
							if(hCompareWnd>0)
							On_paint();
					}
		 
					if( 0x28  == pMsg->wParam) //down
					{ 
						Cur_target_pic+=1;
						if(Cur_target_pic>=File_count2)
						  Cur_target_pic=File_count2-1;
						
						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.left=scroll_x2;
						//InvalidateRect(hCompareWnd, 0, TRUE );//InvalidateRect(hCompareWnd, &rt, TRUE );*/
						if(hCompareWnd>0)
							On_paint();
					}
					if( 0x25  == pMsg->wParam) //left
					{ 
						Cur_target_pic-=6;
						if(Cur_target_pic<0)
						  Cur_target_pic=0;

						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.left=scroll_x2;
						//InvalidateRect(hCompareWnd, 0, TRUE );//InvalidateRect(hCompareWnd, &rt, TRUE );*/
					}
		 
					if( 0x27  == pMsg->wParam) //righ
					{ 
						Cur_target_pic+=6;
						if(Cur_target_pic>=File_count2)
						  Cur_target_pic=File_count2-1;
						
						/*GetClientRect(hCompareWnd, &rt);
						rt.top=scroll_y2;//rt.bottom=0;rt.left=0;
						rt.left=scroll_x2;
						//InvalidateRect(hCompareWnd, &rt, TRUE ); */
					}
					if( VK_SPACE == pMsg->wParam) //space
						SendMessage((HWND)hCompareWnd, WM_COMMAND,  (WPARAM)LINK_PIC,  (LPARAM)0); 
				 
					if( VK_DELETE == pMsg->wParam) //del
							SendMessage((HWND)hCompareWnd, WM_COMMAND,  (WPARAM)REMOVE_PIC,  (LPARAM)0);
			}

 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			dwGlobCurTime=timeGetTime();
		 
			if((dwGlobCurTime-dwGlobPrevTime)>=60)
			{			
				dwGlobPrevTime=dwGlobCurTime;
			 
				if(hCompareWnd>0)
				On_paint();
			}
		}
			

	}
 	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_RENAMEFILES);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= (LPCSTR)IDC_RENAMEFILES;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
// 

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
 

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      	(GetSystemMetrics(SM_CXSCREEN)-PROJECT_WINDOW_WIDTH)/2,
		(GetSystemMetrics(SM_CYSCREEN)-PROJECT_WINDOW_HIGHT)/2,
		PROJECT_WINDOW_WIDTH, PROJECT_WINDOW_HIGHT,  
			NULL,NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//



int show_percent=0,show_percent2=-1;
 WCHAR file_temp[1024];


 
void RenameFiles()
 {

 if(item_count)
					 {
						 
						//!!!!!!!!!!!!!!
					/*	for (int h=0;h<table_item_count;h++)
							if(FileTable[h][0][0])
							{
								wcscpy(FileTable[h][1],FileTable[h][0]);
								FileTable[h][1][wcslen(FileTable[h][1])-4]=0;
								wcscat(FileTable[h][1],L"_renamed.jpg");
							}*/
						//!!!!!!!!!

						int error=0, g;

						 ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
						SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);
						for(int f=0;f<item_count; f++)//for(int f=item_count-1; f>=0;f--)
						{
							int changed=0;
							memset( source_path,0,1024*2);
							memset(target_path,0,1024*2);

							SendMessageW((hListSourceFiles), LB_GETTEXT,(WPARAM)(int)(f), (LPARAM)(LPCSTR)(source_path));

							int len=wcslen(source_path); 

							for( g=len; g>0; g--) //find slash and skip last slash
								if(source_path[g]==(WCHAR)'\\'&&g!=len-1)
									break;
								g++;

							for (int h=0;h<table_item_count;h++)
							if(FileTable[h][0][0])
							if(!wcscmp(&source_path[g],FileTable[h][0]))	
							{
								wcscpy(  target_path,source_path);
								target_path[g]=0;
								if(FileTable[h][1][0])
								{
									wcscat(target_path,FileTable[h][1]);

									//---rename file with same name----------------------------
									if(RenameSameFile)
									{
										HANDLE	hFile11=CreateFileW(target_path, GENERIC_READ,
										   FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
										 if (hFile11!=INVALID_HANDLE_VALUE)
										 {
											CloseHandle(hFile11);	
											wcscpy(  target_path,source_path);
											//target_path[g]=0;
											//wcscat(  target_path,FileTable[h][0]);
										
											int len=wcslen(target_path); 
											for( g=len; g>0; g--) //find slash and skip last slash
												if(target_path[g]==(WCHAR)'.')
												{
													target_path[g]=(WCHAR)'_';
													target_path[g+1]=0;
													wcscat(target_path,FileTable[h][1]);
													break;
												}
										 } 	
									}
									//----------------------------------------------


									if(!MoveFileW(source_path, target_path))
									{
										error++;
										if(error<=3)  
											Show_error();
									}
									else
										changed=1;
								}
							}

							if(changed)
								//for(int ff=f+1;ff<item_count; ff++)
								{
								//	memset(	temp_path,0,1024*2);
								//	SendMessageW((hListSourceFiles), LB_GETTEXT,(WPARAM)(int)(ff), (LPARAM)(LPCSTR)(temp_path));
									//if(!memcmp(  source_path, temp_path,len*2))
									{
									//	memcpy(  temp_path,target_path,len*2);
										SendMessageW((hListSourceFiles), LB_DELETESTRING,(WPARAM)(int)(f), 0);
										SendMessageW((hListSourceFiles), LB_INSERTSTRING,(WPARAM)(int)(f), (LPARAM)(LPCSTR)target_path);
									}
								}

					 
						//	wcscpy(source_path, target_path);
						/*	if(!MoveFileW(source_path, target_path))
							{
								error++;
								if(error<=3) 	if(AutoFix==0)Show_error();
							}*/
							
							show_percent=100*f/item_count;
							if(show_percent!=show_percent2)
							{
								SendMessage( hProgressBarWnd, PBM_SETPOS, (int)show_percent, 0);
								show_percent2=show_percent;
							}

						}

						//SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);

					/*	if(error)
						if(AutoFix==0)
						{
							MessageBoxW(hWnd, L"Some Folders have not been  renamed. \nPlease press \"Rename\" button again to fix the rest Folders or please rename them manually", L"Notification Message", MB_ICONWARNING);
							SendMessageW((HWND)hWnd,WM_COMMAND, FIND_FOLDERS,0);
						}*/


						ShowWindow(hProgressBarWnd, SW_HIDE);
					 }
 }


 void UndoRenameFiles()
 {
	if(item_count)
					 {
						int error=0, g;

						 ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
						SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);
						for(int f=0;f<item_count; f++)//for(int f=item_count-1; f>=0;f--)
						{
							int changed=0;
							memset( source_path,0,1024*2);
							memset(target_path,0,1024*2);

							SendMessageW((hListSourceFiles), LB_GETTEXT,(WPARAM)(int)(f), (LPARAM)(LPCSTR)(source_path));

							int len=wcslen(source_path); 

							for( g=len; g>0; g--) //find slash and skip last slash
								if(source_path[g]==(WCHAR)'\\'&&g!=len-1)
									break;
								g++;

							for (int h=0;h<table_item_count;h++)
							if(FileTable[h][0][1])
							if(!wcscmp(&source_path[g],FileTable[h][1]))	
							{
								wcscpy(  target_path,source_path);
								target_path[g]=0;
								if(FileTable[h][1][0])
								{
									wcscat(target_path,FileTable[h][0]);

									if(!MoveFileW(source_path, target_path))
									{
										error++;
										if(error<=3)  
											Show_error();
									}
									else
										changed=1;
								}
							}

							if(changed)
								//for(int ff=f+1;ff<item_count; ff++)
								{
									//	memcpy(  temp_path,target_path,len*2);
										SendMessageW((hListSourceFiles), LB_DELETESTRING,(WPARAM)(int)(f), 0);
										SendMessageW((hListSourceFiles), LB_INSERTSTRING,(WPARAM)(int)(f), (LPARAM)(LPCSTR)target_path);
								}

					 
			
							
							show_percent=100*f/item_count;
							if(show_percent!=show_percent2)
							{
								SendMessage( hProgressBarWnd, PBM_SETPOS, (int)show_percent, 0);
								show_percent2=show_percent;
							}

						}

				


						ShowWindow(hProgressBarWnd, SW_HIDE);
					 }
 }



int Find_Directory2 (WCHAR* file)
{

int result=0;
//item_count=0;
HANDLE hFile;
WIN32_FIND_DATAW findFileData;
WCHAR s[512];
WCHAR s2[512];
WCHAR Buffer2[20000];
int p=0;
/*
WCHAR value_dir[16];	
ZeroMemory(&value_dir,sizeof(value_dir));
int find_lang=SendMessageW((HWND)hListBoxSourceLang ,CB_GETCURSEL,0,0);
	
wcscpy(value_dir,AndroidLanguageFolder[find_lang]);
*/
ZeroMemory(&Buffer2,sizeof(Buffer2));
ZeroMemory(&findFileData,sizeof(findFileData));

wcscpy(s2,file); 
for(int i=wcslen(s2);i>0;i--)
		if(s2[i]==0x5c)
		{
			s2[i+1]=0;
			break;
		}
wcscpy(s,s2);

ZeroMemory(&findFileData,sizeof(findFileData));

int len,len2=0;
hFile=0;
hFile=FindFirstFileW(file,&findFileData);

	if(hFile!=INVALID_HANDLE_VALUE)
	{
				
		if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL )
		{
			len=wcslen(findFileData.cFileName);
			wcscpy(s,s2);
			wcscat(s,findFileData.cFileName);

			SendMessageW((HWND)hListSourceFiles , LB_ADDSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)s);
			item_count++;

			len=wcslen(findFileData.cFileName);
					 		memcpy(&Buffer2[p], findFileData.cFileName,len*2); p+=len;
					 		Buffer2[p]=0x0d; p++;
					 		Buffer2[p]=0x0a; p++;
		}
		
		/*if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY&& findFileData.cFileName[0]!='.')  
			{
				int len=wcslen(findFileData.cFileName);
				for(int g=0;g<len;g++)
					if(findFileData.cFileName[g]==(WCHAR)' '||findFileData.cFileName[g]>=255||findFileData.cFileName[g]<=(WCHAR)'+')
					{
							wcscpy(s,s2);	
							wcscat(s,findFileData.cFileName);
							wcscat(s,L"\\");
							SendMessageW((HWND)hListSourceFiles , LB_ADDSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)s);
							item_count++;
							break;
					}
					
					g=0;
			
					{
						wcscpy(s,s2);	
						wcscat(s,findFileData.cFileName);
						wcscat(s,L"\\");
						wcscat(s,L"*.*");
						Find_Directory2(s);
					}
			}*/
	

		while(FindNextFileW(hFile,&findFileData))
		{	
			if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE ||findFileData.dwFileAttributes&FILE_ATTRIBUTE_NORMAL )
			{
				len=wcslen(findFileData.cFileName);
				wcscpy(s,s2);
				wcscat(s,findFileData.cFileName);

				SendMessageW((HWND)hListSourceFiles , LB_ADDSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)s);
				item_count++;

				len=wcslen(findFileData.cFileName);
					 		memcpy(&Buffer2[p], findFileData.cFileName,len*2); p+=len;
					 		Buffer2[p]=0x0d; p++;
					 		Buffer2[p]=0x0a; p++;
			}
		
		/*if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY&& findFileData.cFileName[0]!='.')  
			{
					
				int len=wcslen(findFileData.cFileName);
				for(int g=0;g<len;g++)
					if(findFileData.cFileName[g]==(WCHAR)' '||findFileData.cFileName[g]>=255||findFileData.cFileName[g]<=(WCHAR)'+'||
						findFileData.cFileName[g]==(WCHAR)'!'||findFileData.cFileName[g]==(WCHAR)'.'||findFileData.cFileName[g]==(WCHAR)'^')
					{
							wcscpy(s,s2);	
							wcscat(s,findFileData.cFileName);
							wcscat(s,L"\\");
							SendMessageW((HWND)hListSourceFiles , LB_ADDSTRING, (WPARAM)-1,(LPARAM)(LPCSTR)s);
						//	wcscat(s,L"*.*");
							//Find_Files2(s);
							item_count++;	//show_percent+=(float)0.01;
							break;
					}
					
					g=0;
				
					{

						wcscpy(s,s2);	
						wcscat(s,findFileData.cFileName);
						wcscat(s,L"\\");
						wcscat(s,L"*.*");
						Find_Directory2(s);
					}
				 
				
				show_percent++;//=(float)0.05;
				if(show_percent>100)show_percent=1;

				if(show_percent!=show_percent2)
				{
					SendMessage( hProgressBarWnd, PBM_SETPOS, (int)show_percent, 0);
					//InvalidateRect( hProgressBarWnd, NULL, TRUE );
					show_percent2=show_percent;
				}
				//if(show_percent%10==0)
				//	InvalidateRect( hProgressBarWnd, NULL, TRUE );
			}*/
		}
	
	FindClose(hFile);

 
	}




						char File[512];
						memset(File ,0,sizeof(File ));
						strcpy(File ,ProgramPatch);
						strcat(File ,"source_name.txt");
					 
						HANDLE hFile2=CreateFile(File , GENERIC_WRITE,
									FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,CREATE_ALWAYS, 0, NULL);
							if (hFile2==INVALID_HANDLE_VALUE)
							{ 
								MessageBox(hWnd ,"Cannot write file. Please check the filename or path","Error", MB_ICONERROR);
								return 0;
							}				
							else
							{
								SetFilePointer(hFile2, 0, NULL, FILE_BEGIN);			
								 if(!WriteFile(hFile2, (char*) Buffer2, wcslen( Buffer2)*2  , &n_bytes4, NULL))
								 { 
									 MessageBox(hWnd ,"Cannot write file. Please check the filename or path","Error", MB_ICONERROR);
									return 0;
								}

								CloseHandle(hFile2);								
							}

return result;
}

HANDLE hThread_Finding=0;
DWORD dwIdThread_Finding;
unsigned long __stdcall Finding_process(void *Param)
{
 show_percent=0,show_percent2=-1;
	ShowWindow(hProgressBarWnd, SW_SHOWNORMAL);
	SendMessage( hProgressBarWnd, PBM_SETPOS,0,0);
	
	Find_Directory2 (file_temp);

	ShowWindow(hProgressBarWnd, SW_HIDE);
	ExitThread(0);
   return( 0 );  
}

 

 








int SelectFolder(WCHAR* dir)
{



//char SelFolder[MAX_PATH];	
LPITEMIDLIST lpIID;
BROWSEINFOW bi;
WCHAR szName[MAX_PATH];

ZeroMemory(&bi,sizeof(BROWSEINFO));
WCHAR Dir[MAX_PATH];

LPITEMIDLIST pidlRoot = NULL;
LPSHELLFOLDER pDesktopFolder = NULL;
SHGetDesktopFolder(&pDesktopFolder);
	if ( pDesktopFolder != NULL )
	{
	//char szPath[MAX_PATH];
	OLECHAR olePath[MAX_PATH]; wcscpy(olePath,dir);
	ULONG chEaten;
	ULONG dwAttributes;
	HRESULT hr;

	//GetCurrentDirectory(MAX_PATH, szName);
	//GetWindowText(hDirectory,CurrentDirectory,MAX_PATH);
	//if(CurrentDirectory[3]==' ')CurrentDirectory[3]=0;
	//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "e:\\project", -1, olePath, MAX_PATH);

	// Convert the path to an ITEMIDLIST.
	hr = pDesktopFolder->ParseDisplayName(NULL,
	NULL,
	olePath,
	&chEaten,
	&pidlRoot,
	&dwAttributes);

	pDesktopFolder->Release();

	}

	bi.pidlRoot = pidlRoot;

	bi.hwndOwner = hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI|  
		BIF_BROWSEINCLUDEURLS |BIF_NONEWFOLDERBUTTON|BIF_SHAREABLE   ;
	bi.lpszTitle = L"Select a Folder";
	bi.pszDisplayName = Dir;

	lpIID = SHBrowseForFolderW(&bi);
	if(lpIID)
	{
		SHGetPathFromIDListW (lpIID, szName );

		int len=wcslen(szName);
		if(szName[len-1]=='\\') 
								szName[len-1]=0;

///------checl for spec symbols------------------------
	/*	int error_=0;
		for(int g=0;g<len;g++)
				if(   szName[g]==(WCHAR)' '||szName[g]==(WCHAR)'&'||szName[g]==(WCHAR)'('||szName[g]==(WCHAR)')'
					||szName[g]==(WCHAR)'*'||szName[g]==(WCHAR)'^'||szName[g]==(WCHAR)'%'||szName[g]==(WCHAR)'$'
					||szName[g]==(WCHAR)'@'||szName[g]==(WCHAR)'#'||szName[g]==(WCHAR)'`'||szName[g]==(WCHAR)'+'
					||szName[g]==(WCHAR)'!'||szName[g]==(WCHAR)'.'
					||szName[g]==(WCHAR)0x27)//||szName[g]==(WCHAR)0x5e)
					error_=1;
		if(error_)
			MessageBoxW(hWnd, L"The selected Folder contains special symbols.", L"Warning", MB_ICONSTOP);*/
//----------------------------------



		wcscpy(FindFileDirectory_, szName);
	//	wcscpy(Pr.FileDirectory, szName);
	//	strcpy(CurrentDirectory , szName);
		SetWindowTextW(hEditSourceDirecrory,szName);
		SendMessageW( (HWND)hListSourceFiles,LB_RESETCONTENT,0,0);
		return 1;
	}
	return 0;
/*// Browsing for directory.
#define BIF_RETURNONLYFSDIRS   0x0001  // For finding a folder to start document searching
#define BIF_DONTGOBELOWDOMAIN  0x0002  // For starting the Find Computer
#define BIF_STATUSTEXT         0x0004   // Top of the dialog has 2 lines of text for BROWSEINFO.lpszTitle and one line if
                                        // this flag is set.  Passing the message BFFM_SETSTATUSTEXTA to the hwnd can set the
                                        // rest of the text.  This is not used with BIF_USENEWUI and BROWSEINFO.lpszTitle gets
                                        // all three lines of text.
#define BIF_RETURNFSANCESTORS  0x0008
#define BIF_EDITBOX            0x0010   // Add an editbox to the dialog
#define BIF_VALIDATE           0x0020   // insist on valid result (or CANCEL)

#define BIF_NEWDIALOGSTYLE     0x0040   // Use the new dialog layout with the ability to resize
                                        // Caller needs to call OleInitialize() before using this API

#define BIF_USENEWUI           (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)

#define BIF_BROWSEINCLUDEURLS  0x0080   // Allow URLs to be displayed or entered. (Requires BIF_USENEWUI)
#define BIF_UAHINT             0x0100   // Add a UA hint to the dialog, in place of the edit box. May not be combined with BIF_EDITBOX
#define BIF_NONEWFOLDERBUTTON  0x0200   // Do not add the "New Folder" button to the dialog.  Only applicable with BIF_NEWDIALOGSTYLE.
#define BIF_NOTRANSLATETARGETS 0x0400   // don't traverse target as shortcut

#define BIF_BROWSEFORCOMPUTER  0x1000  // Browsing for Computers.
#define BIF_BROWSEFORPRINTER   0x2000  // Browsing for Printers
#define BIF_BROWSEINCLUDEFILES 0x4000  // Browsing for Everything
#define BIF_SHAREABLE          0x8000  // sharable resources displayed (remote shares, requires BIF_USENEWUI)
	*/
}



 


	OPENFILENAME ofn;
SYSTEMTIME sm;
char table_file[MAX_PATH];
char szTableFile [32]="Names.xlsx";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;

	switch (message) 
	{
	case WM_SIZE:
	
			GetClientRect(hWnd, &rt);
			SetWindowPos(hEditSourceDirecrory,NULL,10,				10, rt.right-200,24,   SWP_NOZORDER);  
			SetWindowPos(hBrowseFolderButton,NULL,rt.right-200+10+10 , 10, 80, 24,   SWP_NOZORDER); 
			SetWindowPos(hFindFilesButton_,  NULL, rt.right-100+10 , 10, 80, 24,    SWP_NOZORDER);  
			SetWindowPos(hProgressBarWnd,    NULL,10,				 55, rt.right-20, 5, SWP_NOZORDER);  
			SetWindowPos( hListSourceFiles,  NULL,10,				60, rt.right-20, rt.bottom-100, SWP_NOZORDER);  
			SetWindowPos(hRenameButton,      NULL,rt.right-90  , rt.bottom-40, 80, 24, SWP_NOZORDER); 
			SetWindowPos(hUndoRenameButton,  NULL,rt.right-190  , rt.bottom-40, 80, 24, SWP_NOZORDER); 
			SetWindowPos(hRenameSameFile,    NULL,10 ,			rt.bottom-40, 180, 24, SWP_NOZORDER); 
		
			SetWindowPos(hOpenCompareWindowButton, NULL,   	10, 35, 130, 24,   SWP_NOZORDER); 
			SetWindowPos(hLoadXLSX, NULL,   150, 35, 130, 24,   SWP_NOZORDER); 
			break;
	
	case WM_CREATE:	
		
	 
			//----------------------------get path------------------------------------------	
		GetModuleFileName(hInst,ProgramPatch,sizeof(ProgramPatch));
					for( d=MAX_PATH;d>0;d--)
						if(ProgramPatch[d]==0x5c)
						{ProgramPatch[d+1]=0;
						break;}
		//----------------------------get path------------------------------------------
				hUIFont=CreateFontW(16,0,0,0,FW_MEDIUM,0,0,0,
					0,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,
					ANTIALIASED_QUALITY,DEFAULT_PITCH,L"Arial Unicode MS");			
		memset(&sm,0,sizeof(SYSTEMTIME));
		GetLocalTime(&sm);
		if(sm.wYear>2014&&sm.wMonth>9 &&sm.wDay>5)
			{	
				MessageBoxW(hWnd, L"The debug version has expired. Please update the tool.", L"Error", MB_ICONSTOP);
				break;
			}
		
		
				hEditSourceDirecrory = CreateWindowEx( WS_EX_TOPMOST      ,
								"EDIT", "",
								WS_VISIBLE | WS_CHILD | WS_BORDER  | ES_AUTOHSCROLL |WS_TABSTOP ,//|WS_CAPTION |  ES_RIGHT   ES_RIGHT            0x0002L
											10, 10, PROJECT_WINDOW_WIDTH-200,24,
											hWnd, (HMENU)FILE_DIR, hInst, NULL );
								SendMessage(hEditSourceDirecrory , WM_SETFONT, (WPARAM)hUIFont, FALSE);


				hOpenCompareWindowButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
							  L"BUTTON",     L"Create Names List"  ,       // button text
								WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles         0x0002L
											10, 10+24, 80, 24,  
											hWnd, (HMENU)OPEN_COMPARE_WINDOW, hInst, NULL );
							SendMessage(hOpenCompareWindowButton , WM_SETFONT, (WPARAM)hUIFont, FALSE);

				hLoadXLSX  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
							  L"BUTTON",     L"Load Names List",     // button text
								WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles
									100, 10+24, 80, 24,      
									hWnd, (HMENU)LOAD_XLSX, hInst, NULL ); 
							SendMessage(hLoadXLSX, WM_SETFONT, (WPARAM)hUIFont, FALSE);
				
				hBrowseFolderButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
							  L"BUTTON",     L"Browse"  ,       // button text
								WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles
								 PROJECT_WINDOW_WIDTH-200+10 , 10, 80, 24,      
									hWnd, (HMENU)SET_PATH_FOR_FOLDERS, hInst, NULL ); 
						SendMessage(hBrowseFolderButton, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			

				hFindFilesButton_  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
					L"BUTTON",     L"Find",       // button text
						WS_VISIBLE | WS_CHILD |WS_TABSTOP ,  // styles
						 PROJECT_WINDOW_WIDTH-200+10+90, 10, 80, 24,     
						hWnd, (HMENU) FIND_FOLDERS, hInst, NULL );
					SendMessage(hFindFilesButton_, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			

				hProgressBarWnd=CreateWindow( PROGRESS_CLASS        ,
							  "progress bar",    
							WS_VISIBLE | WS_CHILD  ,  // styles
							  10,  55, PROJECT_WINDOW_WIDTH-40, 5,      
							  hWnd,   NULL,  (HINSTANCE) GetWindowLong(hWnd,0), NULL); 

				 SendMessage( hProgressBarWnd, PBM_SETRANGE, 0, MAKELPARAM(0,100));
				 SendMessage( hProgressBarWnd, PBM_SETSTEP, 1, 0);
				 SendMessage( hProgressBarWnd, PBM_SETPOS, 0, 0);
				 
				 ShowWindow(hProgressBarWnd, SW_HIDE);

				 
				 
				 hListSourceFiles  = CreateWindowExW(0       ,
							L"LISTBOX",     L"",       // button text
							WS_CHILD | WS_BORDER |WS_VISIBLE |LBS_NOTIFY | ES_AUTOHSCROLL |ES_AUTOVSCROLL |ES_RIGHT| WS_VSCROLL,//|LBS_STANDARD,   // styles
							10,  60, PROJECT_WINDOW_WIDTH-40, PROJECT_WINDOW_HIGHT-180,             // parent window
							hWnd, (HMENU)-1, hInst, NULL ); 
				 	SendMessage(hListSourceFiles, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			
				
				SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
				SendMessageW((HWND)hListSourceFiles, LB_SETCURSEL,0,0);


				hRenameButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
							  L"BUTTON",     L"Rename"  ,       // button text
								WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles
								 PROJECT_WINDOW_WIDTH-120  , PROJECT_WINDOW_HIGHT-100, 80, 24,      
									hWnd, (HMENU)RENAME_FILES, hInst, NULL ); 
					SendMessage(hRenameButton, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			

				hUndoRenameButton  = CreateWindowExW(WS_EX_CONTROLPARENT        ,
							  L"BUTTON",     L"Reverse"  ,       // button text
								WS_VISIBLE | WS_CHILD | WS_TABSTOP ,  // styles
								 PROJECT_WINDOW_WIDTH-120  , PROJECT_WINDOW_HIGHT-200, 80, 24,      
									hWnd, (HMENU)UNDO_RENAME_FILES, hInst, NULL ); 
					SendMessage(hUndoRenameButton, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			
					
				//SetWindowTextW( hEditSourceDirecrory, L"\\\\192.168.184.94\\tdd\\Members\\Sasha_Pakholkov_x50808\\backup");
			SetWindowTextW( hEditSourceDirecrory, L"\\");	
			//		SetWindowTextW( hEditSourceDirecrory, L"C:\\test\\RU_test");
				//	SetWindowTextW( hEditSourceDirecrory, L"\\");
				

			hRenameSameFile=CreateWindowExW(WS_EX_CONTROLPARENT,L"BUTTON", L"Rename same files" ,
					WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, 
						  10 , PROJECT_WINDOW_HIGHT-30, 180, 24,  
						hWnd, (HMENU) RENAME_SAME_FILES, hInst, NULL ); 
						SendMessage(hRenameSameFile, WM_SETFONT, (WPARAM)hUIFont, FALSE);
			

									
						CheckDlgButton(	hRenameSameFile,0,BST_UNCHECKED);
				//if(AutoFix)
				//	hAutoFixFolder=CreateThread( NULL, 0, AutoFixFolder, NULL, 0, &dwIdAutoFixFolder );
				
				 /*memset(FileTable,0,sizeof(FileTable));

				 strcpy(FilePatch,ProgramPatch);
				 strcat(FilePatch,ExcelFileName);
				 Open_Excel_XLSX_file(FilePatch);*/

				 
		
				/* strcpy(FilePatch,ProgramPatch);
				 strcat(FilePatch,INDDFileName);
				Open_INDD_file(FilePatch);*/
					
				InvalidateRect( hWnd, NULL, TRUE );
				//CreateCompareWindow( );
				break;

	
		case WM_COMMAND:

			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case LOAD_XLSX:
				memset(FileTable,0,sizeof(FileTable));

				 //strcpy(FilePatch,ProgramPatch);
				 //strcat(FilePatch,ExcelFileName);
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ZeroMemory(table_file, MAX_PATH);
				ofn.lStructSize = sizeof(OPENFILENAME); 
						ofn.hwndOwner = hWnd; 
						ofn.lpstrFile = table_file; 
				ofn.nMaxFile = MAX_PATH; 
				ofn.lpstrFilter =      &"Excel Spreadsheet Files( *.xlsx)\0*.XLSX\0"  \
														 	"All Files (*.*)\0*.*\0";
									
				ofn.lpstrTitle=&"Open File with filenames "; 
				ofn.lpstrInitialDir=ProgramPatch; 
				ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_ENABLESIZING | OFN_HIDEREADONLY;
								 					
				ofn.lpstrFileTitle    = szTableFile;//get filename witoput path
				ofn.nMaxFileTitle     = sizeof(szTableFile);
										
				if(!GetOpenFileName(&ofn)) 
							break;

				 Open_Excel_XLSX_file(table_file);

				 	Current_hint2++;
					if(Current_hint2>max_hint2-1) Current_hint2=1;
					SetWindowText(hWnd, hints2[Current_hint2]);
			break;

			case OPEN_COMPARE_WINDOW:
					CreateCompareWindow( );
					break;
			
			case IDM_ABOUT:

					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					Current_hint2=5;
					  SetWindowText(hWnd, hints2[Current_hint2]);
				   break;

				case IDM_HELP:
				   	char BrowserExec[255];
					SetCurrentDirectory(ProgramPatch);		

					SetCurrentDirectory(ProgramPatch);
					ShellExecute(NULL,"open","help.rtf",NULL,BrowserExec,SW_SHOWNORMAL);
					Current_hint2=5;
					  SetWindowText(hWnd, hints2[Current_hint2]);
					break;

				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;

				 case SET_PATH_FOR_FOLDERS:
					
						GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );

						if(SelectFolder(sU))
						{
							Current_hint2++;
							if(Current_hint2>max_hint2-1) Current_hint2=1;
							SetWindowText(hWnd, hints2[Current_hint2]);
						}
					break;

				 

				 case  FIND_FOLDERS:
						
				
						SendMessageW((HWND)hListSourceFiles, LB_RESETCONTENT,0,0);
						GetWindowTextW(hEditSourceDirecrory,sU, sizeof(sU)-1 );
						
						wcscpy(FindFileDirectory_,sU);
						wcscat(sU,L"\\");
						wcscat(sU,L"*.*");
						item_count=0;

						//Find_Directory2(sU);

						wcscpy(file_temp,sU);
						hThread_Finding = CreateThread( NULL, 0, Finding_process, NULL, 0, &dwIdThread_Finding );
					
						Current_hint2++;
						if(Current_hint2>max_hint2-1) Current_hint2=1;
						SetWindowText(hWnd, hints2[Current_hint2]);
						InvalidateRect(hWnd, 0, TRUE );
					
					break;


				 case RENAME_FILES:
					
					 RenameFiles();
					 Current_hint2++;
					if(Current_hint2>max_hint2-1) Current_hint2=5;
					SetWindowText(hWnd, hints2[Current_hint2]);
					 	break;

				 case UNDO_RENAME_FILES:

					  UndoRenameFiles();
					  Current_hint2=5;
					  SetWindowText(hWnd, hints2[Current_hint2]);
					 	break;

				case RENAME_SAME_FILES:
					if(IsDlgButtonChecked(hWnd,RENAME_SAME_FILES))
						RenameSameFile=1;
					else
						RenameSameFile=0;
					Current_hint2=5;
					  SetWindowText(hWnd, hints2[Current_hint2]);
					break;

				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			SetWindowText(hWnd, hints2[Current_hint2]);
			// TODO: Add any drawing code here...
//			RECT rt;
			//GetClientRect(hWnd, &rt);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	HDC hdc;
	PAINTSTRUCT ps;
//	HFONT hCurFont;
	char s[200];
	int y;

	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;

		case WM_PAINT:
			                      
			hdc = BeginPaint(hDlg, &ps);
			SelectObject(hdc,hUIFont);

			SetBkMode(hdc, 0);
			y=0;			
				
			wsprintf(s ,"%s",szTitle);
			if((ver%100)<10)
				wsprintf(s ,"%s  Version %d.0%d%s  Freeware",szTitle,ver/100,ver%100,subver);
			else
				wsprintf(s ,"%s  Version %d.%d%s  Freeware",szTitle,ver/100,ver%100,subver);
		
			TextOut(hdc, 80,20+y*20,s ,strlen(s  ));
			y++;

				//SelectObject(hdc,hCurFont);
				wsprintf(s,"Copyright © 2013 Sasha P (sasha_p@asus.com)");
				TextOut(hdc, 80,20+y*20,s ,strlen(s  ));	

		

			EndPaint(hDlg, &ps);

			break;
	}
    return FALSE;
}
