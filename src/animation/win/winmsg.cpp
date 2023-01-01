/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : winmsg.cpp
  * PURPOSE     : Ray marching project.
  *               Window module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#include "win.h"

static const INT InitTimer = 130;
static const INT RefreshTimer = 30;
/* WM_CREATE window message handle function.
  * ARGUMENTS:
  *   - structure with creation data:
  *       CREATESTRUCT *CS;
  * RETURNS:
  *   (BOOL) TRUE to continue creation window, FALSE to terminate.
  */
BOOL trm::win::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(hWnd,             // handle to main window 
            InitTimer,        // timer identifier 
            0,                // 0-second interval 
            (TIMERPROC)nullptr);
  return TRUE;
}; /* End of 'trm::win::OnCreate' function */
 
/* WM_DESTROY window message handle function.
  * ARGUMENTS: None.
  * RETURNS: None.
  */
VOID trm::win::OnDestroy( VOID )
{
  if (IsInit)
    {
      Close();
      KillTimer(hWnd, RefreshTimer);
    }
    else
      KillTimer(hWnd, InitTimer);
}; /* End of 'trm::win::OnDestroy' function */
 
/* WM_SIZE window message handle function.
  * ARGUMENTS:
  *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
  *       UINT State;
  *   - new width and height of client area:
  *       INT W, H;
  * RETURNS: None.
  */
VOID trm::win::OnSize( UINT State, INT W, INT H )
{
  this->W = W;
  this->H = H;
  if (IsInit)
    Resize();
}; /* End of 'trm::win::OnSize' function */
 
/* WM_ERASEBKGND window message handle function.
  * ARGUMENTS:
  *   - device context of client area:
  *       HDC hDC;
  * RETURNS:
  *   (BOOL) TRUE if background is erased, FALSE otherwise.
  */
BOOL trm::win::OnEraseBackground( HDC hDC )
{
  if (IsInit)
      Erase(hDC);
  return TRUE;
}; /* End of 'trm::win::OnEraseBackground' function */
 
/* WM_PAINT window message handle function.
  * ARGUMENTS:
  *   - window device context:
  *       HDC hDC;
  *   - paint message structure pointer:
  *       PAINTSTRUCT *PS;
  * RETURNS: None.
  */
VOID trm::win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
  if (IsInit)
      Paint(hDC);
}; /* End of 'trm::win::OnPaint' function */

/* WM_ACTIVATE window message handle function.
  * ARGUMENTS:
  *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
  *       UINT Reason;
  *   - handle of active window:
  *       HWND hWndActDeact;
  *   - minimized flag:
  *       BOOL IsMinimized;
  * RETURNS: None.
  */
VOID trm::win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
{
  IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
  if (IsInit)
      Activate();
}; /* End of 'trm::win::OnActivate' function */
 
/* WM_TIMER window message handle function.
  * ARGUMENTS:
  *   - specified the timer identifier.
  *       INT Id;
  * RETURNS: None.
  */
VOID trm::win::OnTimer( INT Id ) 
{
  if (!IsInit)
    {
      KillTimer(hWnd, InitTimer);
      SetTimer(hWnd,             // handle to main window 
                RefreshTimer,     // timer identifier 
                0,                // 0-second interval 
                (TIMERPROC)nullptr);
      IsInit = TRUE;

      // call callbacks
      Init();
      Resize();
      Timer();
    }
    else
      Timer();
}; /* End of 'trm::win::OnTimer' function */
 
/* WM_MOUSEWHEEL window message handle function.
  * ARGUMENTS:
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse wheel relative delta value:
  *       INT Z;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
VOID trm::win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  MouseWheel += Z;
}; /* End of 'trm::win::OnMouseWheel' function */
  
/* WM_MBUTTONDOWN window messages handle function.
  * ARGUMENTS:
  *   - mouse is doble clicked:
  *       BOOL IsDblClick;
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
  VOID trm::win::OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys )
  {
  }; /* End of 'trm::win::OnButtonDown' function */

/* WM_MBUTTONUP window messages handle function.
  * ARGUMENTS:
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
VOID trm::win::OnButtonUp( INT X, INT Y, UINT Keys )
{
}; /* End of 'trm::win::OnButtonUp' function */

/* END OF 'winmsg.cpp' FILE */