/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : win.h
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


#ifndef __win_h_ 
#define __win_h_

#include <windowsx.h>
#include <string>
#include "../../def.h"
#include "../../../res/resource.h"

#define CLASS_NAME "Ray Marching Class Window"
#define WINDOW_NAME "Tough Ray Marching"

/* NSF name space */
namespace trm
{
  /* Window handler class */
  class win
  {
  protected:
    HWND hWnd;           // window handle
    INT W = 0, H = 0;    // window size
    HINSTANCE hInstance; // application handle
    INT MouseWheel = 0;  //mouse wheel state/change
    std::string CurSceneName;   // Current file scene
    bool IsFileChanged = false; // Flag is changed file
    std::string WorkDirectory;  // Working directory
    static BOOL IsActive;

    /* Update menu scene name item function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateMenuSceneName( VOID );
 
  private:
    HMENU Menu;     // Menu
    HWND DlgAbout;  // Dialog about

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
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );

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
    static BOOL CALLBACK DlgProc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );
 
    BOOL IsFullScreen = FALSE;        //is window full screen flag
    RECT FullScreenSaveRect = RECT(); //full screen save rectangle
  public:
    BOOL IsInit = FALSE;   //initialize flag
 
    /* Constructor declaration */
    win( HINSTANCE hInst = GetModuleHandle(nullptr) );
 
    /* Run window function declaration */
    INT Run( VOID );
  private:
    /* virtual animation methods */

    /* Initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Init( VOID )
    {
    } /* End of 'Init' function */

    /* Deinitialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Close( VOID )
    {
    } /* End of 'Close' function */

    /* Resize window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Resize( VOID )
    {
    } /* End of 'Resize' function */

    /* Update timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Timer( VOID )
    {
    } /* End of 'Timer' function */

    /* Activation window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Activate( VOID )
    {
    } /* End of 'Activate' function */

    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Idle( VOID )
    {
    } /* End of 'Idle' function */

    /* Erase background function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Erase( HDC hDC )
    {
    } /* End of 'Erase' function */

    /* Paint window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Paint( HDC hDC )
    {
    } /* End of 'Paint' function */

    /* Message crackers functions declaration */

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    BOOL OnCreate( CREATESTRUCT *CS );

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT W, INT H );   
 
    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    VOID OnTimer( INT Id );

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
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );

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
    VOID OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys );

    /* WM_MBUTTONUP window messages handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    VOID OnButtonUp( INT X, INT Y, UINT Keys );
 
    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    BOOL OnEraseBackground( HDC hDC );  
 
    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS );

    /* WM_CLOSE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnClose( VOID );

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID );

    /* WM_COMMAND window message handle function.
     * ARGUMENTS:
     *   - message 'word' parameter:
     *       WPARAM wParam;
     * RETURNS: None.
     */
    VOID OnCommand( WPARAM wParam );

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
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );
  }; /* End of 'win' class */
} /* end of 'trm' name space */

#endif /* __win_h_ */

/* END OF 'win.h' FILE */