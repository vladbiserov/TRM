/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : anim.h
  * PURPOSE     : Ray marching project.
  *               Animation module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 05.01.2023
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __animation_h_
#define __animation_h_

#include <initializer_list>
#include "../def.h"
#include "../utils/stock.h"
#include "win/win.h"
#include "input/timer.h"
#include "input/input.h"
#include "render/render.h"
#include "../utils/directory_watcher.h"
#include "../utils/parser/parser.h"

/* NSF name space */
namespace trm
{
  class scene;
  class animation;
  /* Unit class */
  class unit
  {
  public:
    /* Virtual destructor */
    virtual ~unit( VOID )
    {
    } /* End of destructor */

    /* Virtual unit render function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    virtual VOID Render( animation *Ani )
    {
    } /* End of 'Render' function */

    /* Virtual unit response function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    virtual VOID Response( animation *Ani )
    {
    } /* End of 'Response' function */
  }; /* End of 'unit' class */

  /* Scene class */
  class scene : public unit
  {
  protected:
    stock<unit *> Units;
  public:
    /* Constructor */
    scene( VOID ) : Units()
    {
    } /* End of constructor */

    /* Destructor */
    ~scene( VOID ) override
    {
      Units.Walk([]( unit *Uni ){  delete Uni; });
    } /* End of destructor */

    /* Virtual unit render function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Render( animation *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Render(Ani);  });
    } /* End of 'Render' function */

    /* Virtual unit response function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Response( animation *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Response(Ani); });
    } /* End of 'Response' function */

    /* Add unit to scene operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (scene &) Self-reference.
     */
    scene & operator<<( const std::string &Name );
  }; /* End of 'scene' class */

  /* Animation class */
  class animation : public win, public render, public timer, public input
  {
  public:
    std::map<std::string, texture::tex_data> Textures;
  private:
    scene *Scene;
    struct UBO_ANIM
    {
      vec4 DummyTime;
    };
    buffer *UboAnim;
    directory_watcher DW;
    parser Parser;

    /* Hiden constructor */
    animation( VOID ) : render(win::hWnd, win::W, win::H), input(win::MouseWheel, win::hWnd), Scene(new scene),
      Parser("bin\\scenes\\a.scene", "bin\\shaders\\RT\\myfrag.glsl", "bin\\shaders\\RT\\frag.glsl", "bin\\error.log", Textures)
    {
      UBO_ANIM *UC = nullptr;
      UboAnim = CreateBuffer(3, UC);

      /* Update menu */
      win::CurSceneName = "a.scene";
      win::UpdateMenuSceneName();
    } /* End of constructor */

    static animation Instance; //Instance of animation interface
  public:
    std::map<std::string, unit * (*)( VOID )> UnitNames;

    template<class UnitType>
    class unit_register
    {
    public:
      static unit * Create( VOID )
      {
        return new UnitType(&Instance);
      }
      unit_register( const std::string &Name )
      {
        Instance.UnitNames[Name] = Create;
      }
    };

    /* Destructor */
    ~animation( VOID )
    {
      delete Scene;
    } /* End of destructor */

    /* Get single instance of animation interface function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (animation&) instance of animation interface.
     */
    static animation& Get( VOID )
    {
      return Instance;
    } /* End of 'Get' function */

    /* Get pointer to single instance of animation interface function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (animation*) pointer to instance of animation interface.
     */
    static animation* GetPtr( VOID )
    {
      return &Instance;
    } /* End of 'GetPtr' function */

    /* Add unit to animation stock operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (animation &) Self-reference.
     */
    animation & operator<<( const std::string &Name )
    {
      *Scene << Name;
      return *this;
    } /* End of 'operator<<' function */

  private:

    VOID UpdateTextures( VOID );

    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID );

    /* Initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID ) override;

    /* Deinitialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID ) override;

    /* Resize window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Resize( VOID ) override;

    /* Update timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Timer( VOID ) override;

    /* Activation window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Activate( VOID ) override;

    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Idle( VOID ) override;

    /* Erase background function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Erase( HDC hDC ) override;

    /* Paint window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Paint( HDC hDC ) override;
  }; /* End of 'animation' class */
} /* end of 'trm' name space */

#endif /* __animation_h_ */

/* END OF 'animation.h' FILE */