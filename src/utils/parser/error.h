/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : error.h
  * PURPOSE     : Ray marching project.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 28.12.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __error_h_
#define __error_h_

#include <format>
#include <fstream>
#include <string>

/* Error logger class */
class error_log
{
private:
  int ErrorCnt = 0;  // Error count
  std::string
    ErrFileName,     // Error file name
    ErrBuf;          // Error buffer

  /* Clear all error buffers function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Clear(void)
  {
    ErrBuf.clear();
    ErrorCnt = 0;
  } /* End of 'Clear' function */

public:
  /* Class constructor */
  error_log(const std::string& FileName) : ErrFileName(FileName)
  {
  } /* End of 'error_log' function */

  /* Log error with params function.
   * ARGUMENTS:
   *   - error:
   *       const std::string &Err;
   * RETURNS: None.
   */
  void LogParams(const std::string& Err)
  {
    Log(std::format("'{}' has incorrect parameters.", Err));
  } /* End of 'LogParams' function */

  /* Log default error function.
   * ARGUMENTS:
   *   - error:
   *       const std::string &Err;
   * RETURNS: None.
   */
  void Log(const std::string& Err)
  {
    ErrBuf += std::format("{}: {}\n", ErrorCnt++, Err);
  } /* End of 'Log' function */

  /* Print error buffer to file function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  void Print(void)
  {
    std::ofstream File(ErrFileName);

    if (File.is_open())
      File << ErrBuf;

    Clear();

    File.close();
  } /* End of 'Print' function */
}; /* End of 'error_log' class */

#endif /* __error_h_ */

/* END OF 'error.h' FILE */