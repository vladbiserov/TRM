/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : directory_watcher.h
  * PURPOSE     : Ray marching project.
  *               Directory watcher module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __directory_watcher_h_
#define __directory_watcher_h_

#include "../def.h"

/* Project namespace */
namespace trm
{
  /* Directory watcher class */
  class directory_watcher
  { 
  private:
    // Overlapped structure for changing hook
    OVERLAPPED Ovr {};
    // Traching directory handle
    HANDLE hDir {};
    // Read directory changes data buffer
    BYTE Buffer[27 * 47] {};
    // Store path
    std::string Path;

  public: 
    /* Start directory watching function.
     * ARGUMENTS:
     *   - directory path:
     *       const std::string &DirPath; 
     * RETURNS: None. 
     */ 
    VOID StartWatch( const std::string &DirPath ) 
    { 
      StopWatch();
      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      Path = DirPath; 
      // Open directory 
      hDir = 
        CreateFile(DirPath.c_str(), 
          FILE_LIST_DIRECTORY, 
          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
          nullptr, OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
          nullptr);
      // Start monitoring
      Ovr.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
      ReadDirectoryChangesW(hDir,
        Buffer, sizeof(Buffer),
        TRUE,
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
        nullptr,
        &Ovr,
        nullptr);
    } /* End of 'StartWatch' function */

    /* Stop directory watching function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID StopWatch( VOID )
    {
      if (Ovr.hEvent != nullptr)
        CloseHandle(Ovr.hEvent);
      if (hDir != nullptr)
        CloseHandle(hDir);

      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      hDir = nullptr;
      Path = "";
    } /* End of 'StopWatch' function */ 

    /* Check if directory is changed function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if directory contents is changed (and subfolders). 
     */ 
    BOOL IsChanged( VOID )
    {
      if (Ovr.hEvent == nullptr || hDir == nullptr)
        return FALSE;

      INT id = WaitForSingleObject(Ovr.hEvent, 0);
      if (id == WAIT_OBJECT_0)
      {
        // Restart monitoring
        ResetEvent(Ovr.hEvent);
        ReadDirectoryChangesW(hDir,
          Buffer, sizeof(Buffer),
          TRUE,
          FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
          nullptr,
          &Ovr,
          nullptr);
        return TRUE;
      }
      return FALSE;
    } /* End of 'IsChanged' function */
  }; /* End of 'directory_watcher' class */
}; /* End of 'vlrt' namespace */

#endif /* __directory_watcher_h_ */

/* END OF 'tea_directory_watcher.h' FILE */
