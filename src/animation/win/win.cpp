/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : win.cpp
  * PURPOSE     : Ray marching project.
  *               Window module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 05.01.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include "win.h"

BOOL trm::win::IsActive = true;

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK trm::win::WinFunc(HWND hWnd, UINT Msg,
  WPARAM wParam, LPARAM lParam)
{
  win* Win;
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO* minmax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO*)lParam;
    // minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
    //   GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    minmax->ptMinTrackSize.x = 347;
    minmax->ptMinTrackSize.y = 347;
    return 0;
  case WM_CREATE:
    SetWindowLongPtr(hWnd, 0, (UINT_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
  default:
    Win = reinterpret_cast<win*>(GetWindowLongPtr(hWnd, 0));
    if (Win != nullptr)
      switch (Msg)
      {
      case WM_CREATE:
        Win->hWnd = hWnd;
        return Win->OnCreate((CREATESTRUCT*)lParam) ? 0 : -1;
      case WM_SIZE:
        Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)Win->OnEraseBackground((HDC)wParam);
      case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Win->OnPaint(hDC, &ps);
        EndPaint(hWnd, &ps);
        return 0;
      case WM_TIMER:
        Win->OnTimer((UINT)wParam);
        return 0;
      case WM_MOUSEWHEEL:
        Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
          (INT)(SHORT)HIWORD(lParam),
          (INT)(SHORT)HIWORD(wParam),
          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        Win->OnButtonDown(FALSE,
          (INT)(SHORT)LOWORD(lParam),
          (INT)(SHORT)HIWORD(lParam),
          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        Win->OnButtonUp((INT)(SHORT)LOWORD(lParam),
          (INT)(SHORT)HIWORD(lParam),
          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_CLOSE:
        Win->OnClose();
        return 0;
      case WM_DESTROY:
        Win->OnDestroy();
        PostQuitMessage(30);
        return 0;
      case WM_COMMAND:
        Win->OnCommand(wParam);
        return 0;
      }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'trm::win::WinFunc' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
BOOL CALLBACK trm::win::DlgProc(HWND hWnd, UINT Msg,
  WPARAM wParam, LPARAM lParam)
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_INITDIALOG:
  {
    /* Set dialog icon */
    HICON hIcon;
    hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL),
      MAKEINTRESOURCEW(IDI_ICONTRM),
      IMAGE_ICON,
      GetSystemMetrics(SM_CXSMICON),
      GetSystemMetrics(SM_CYSMICON),
      0);
    if (hIcon)
      SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    return TRUE;
  }
  case WM_CLOSE:
  {
    EndDialog(hWnd, NULL);
    IsActive = TRUE;
    return TRUE;
  }
  case WM_DRAWITEM:
  {
    LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
    
    if (pDIS->hwndItem == GetDlgItem(hWnd, IDC_DLG_PML30))
    {
      HBITMAP hBM = (HBITMAP)LoadImage(GetModuleHandleW(NULL),
        MAKEINTRESOURCE(IDB_PML30),
        IMAGE_BITMAP,
        0,
        0,
        LR_CREATEDIBSECTION);
      
      BITMAP bm;
      GetObject(hBM, sizeof(bm), &bm);
      
      BITMAPINFOHEADER bmh{};
      bmh.biSize = sizeof(BITMAPINFOHEADER);
      bmh.biBitCount = bm.bmBitsPixel;
      bmh.biWidth = bm.bmWidth;
      bmh.biHeight = bm.bmHeight;
      bmh.biPlanes = 1;
      
      StretchDIBits(pDIS->hDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        0, 0, bm.bmWidth, bm.bmHeight,
        bm.bmBits, (BITMAPINFO*)&bmh, DIB_RGB_COLORS, SRCCOPY);
    }
    else if (pDIS->hwndItem == GetDlgItem(hWnd, IDC_DLG_CGSG))
    {
      HBITMAP hBM = (HBITMAP)LoadImage(GetModuleHandleW(NULL),
        MAKEINTRESOURCE(IDB_CGSG),
        IMAGE_BITMAP,
        0,
        0,
        LR_CREATEDIBSECTION);
      
      BITMAP bm;
      GetObject(hBM, sizeof(bm), &bm);
      
      BITMAPINFOHEADER bmh{};
      bmh.biSize = sizeof(BITMAPINFOHEADER);
      bmh.biBitCount = bm.bmBitsPixel;
      bmh.biWidth = bm.bmWidth;
      bmh.biHeight = bm.bmHeight;
      bmh.biPlanes = 1;
      
      StretchDIBits(pDIS->hDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        0, 0, bm.bmWidth, bm.bmHeight,
        bm.bmBits, (BITMAPINFO*)&bmh, DIB_RGB_COLORS, SRCCOPY);
    }

    return TRUE;
  }
  case WM_PAINT:
  {
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return TRUE;
  }
  default:
    return FALSE;
  }
} /* End of 'trm::win::WinFunc' function */

/* Window class constructor.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 * - window name:
 *       const char* WndClassName;
 */
trm::win::win(HINSTANCE hInst) : hInstance(hInst)
{
  WNDCLASS wc;

  /* Create and register window class */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(win*);
  wc.hbrBackground = (HBRUSH)COLOR_MENU;
  wc.hCursor = LoadCursor(nullptr, IDC_HAND);
  wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONTRM));
  wc.hInstance = hInstance;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = CLASS_NAME;
  wc.lpfnWndProc = WinFunc;
  if (!RegisterClass(&wc))
    MessageBox(nullptr, "Error register window class",
      "ERROR", MB_OK | MB_ICONERROR);

  /* Create window */
  hWnd = CreateWindow(CLASS_NAME, WINDOW_NAME,
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr, nullptr, hInstance, reinterpret_cast<VOID*>(this));

  /* Create menu */
  Menu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
  SetMenu(hWnd, Menu);

  /* Create dialog window about */
  DlgAbout = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hWnd, DlgProc);
  ShowWindow(DlgAbout, SW_HIDE);
  UpdateWindow(DlgAbout);
}; /* End of constructor */

/* Update menu scene name item function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID trm::win::UpdateMenuSceneName( VOID )
{
  ModifyMenu(Menu, ID_CURRENTSCENENAME, MF_BYCOMMAND | MF_STRING | MF_GRAYED,
    ID_CURRENTSCENENAME, CurSceneName.c_str());
  DrawMenuBar(hWnd);
} /* End of 'trm::win::UpdateMenuSceneName' function */

/* Start message loop.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) Auit message.
 */
INT trm::win::Run(VOID)
{
  MSG msg;
  HACCEL Accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

  if (Accel == NULL)
  {
    DestroyWindow(hWnd);
    return -1;
  }

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      if (!TranslateAccelerator(hWnd, Accel, &msg))
        DispatchMessage(&msg);
    }
    else
      if (IsInit)
        Idle();

  return msg.wParam;
}; /* End of 'trm::win::Run' function */

/* END OF 'win.cpp' FILE */