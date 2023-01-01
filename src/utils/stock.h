/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : stock.h
  * PURPOSE     : Ray marching project.
  *               Stock module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __stock_h_
#define __stock_h_

#include <vector>
#include "../def.h"

/* NSF name space */
namespace trm
{
  /* Stock storage class */
  template<typename type>
  class stock : public std::vector<type>
  {
  public:
    /* Add data to stock function.
     * ARGUMENTS:
     *   - data to add:
     *       const type &NewData;
     * RETURNS: (stock &) Self-reference.
     */
    stock & Add( const type &NewData )
    {
      this->push_back(NewData);
      return *this;
    } /* End of 'Add' function */

    /* Add operator.
     * ARGUMENTS:
     *   - data to add:
     *       const type& NewData;
     * RETURNS: (stock &) Self-reference.
     */
    stock & operator<<( const type &NewData )
    {
      return Add(NewData);
    } /* End of 'operator<<' function */
 
    /* Call Walker function for each element of the stock.
     * ARGUMENTS:
     *   - walker function:
     *       const type2 &Walker;
     * RETURNS: None.
     */
    template<class WalkType>
    void Walk( WalkType Walk )
    {
      for (auto &x : *this)
        Walk(x);
    } /* End of 'Walk' function */
  }; /* End of 'stock' class */
} /* end of 'trm' name space */

#endif /* __stock_h_ */

/* END OF 'stock.h' FILE */