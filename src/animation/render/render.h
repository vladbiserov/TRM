/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : render.h
  * PURPOSE     : Ray marching project.
  *               Render module.
  * PROGRAMMER  : Vladislav Biserov.
  *               Maxim Molostov.
  * LAST UPDATE : 17.11.2022
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */


#ifndef __render_h_
#define __render_h_

#include "../../def.h"
#include "resource/resource.h"
#include "resource/primitive.h"
#include "resource/shader.h"
#include "resource/material.h"
#include "resource/texture.h"
#include "resource/target.h"
#include "resource/jittering.h"

namespace trm
{
  struct UBO_CAMERA
  {
    matr_data View;
    matr_data Proj;
    matr_data VP;
    vec4 LocFrameW;
    vec4 DirProjDist;
    vec4 RightWp;
    vec4 UpHp;
    vec4 AtFrameH;
    vec4 DummySize;
  };

  // Render system type
  class render :
    public shader_manager, public texture_manager,
    public buffer_manager,
    public material_manager, public primitive_manager
  {
  private:
    const HWND &hWnd;           // window to dender descriptor
    const HDC hDC;              // window to dender device context handle
    const INT &FrameW, &FrameH; // window to render frame size

    HGLRC hGLRC;                // OpenGL rendering context
    buffer *UboCamera;

    target Trg, Trg1;           // target of 1st and 2nd passes
    jittering Jit;              // jittering pass var
  public:
    vec2 GetScreen( VOID )
    {
      return vec2(FrameW, FrameH);
    }
    // User camera
    camera Camera;
    /* Render system type constructor.
     * ARGUMENTS:
     *   - window handle ref:
     *       HWND &hWnd;
     *   - window size ref:
     *       INT &W, &H;
     */
    render( HWND &hWnd, INT &W, INT &H );

    /* Render system type destructor.
     * ARGUMENTS: None.
     */
    ~render( VOID );

    /* Rendring resizing function.
     * ARGUMENTS: None
     * RETURNS: None.
     */
    VOID Resize( VOID );

    /* Rendring copy frame function.
     * ARGUMENTS:
         - WINDOW HANDLE:
             HWND hWnd;
     * RETURNS: None.
     */
    VOID CopyFrame( VOID );

    /* Rendring start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Start( VOID );

    /* Rendring end function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */ 
    VOID End( VOID );

    /* Debug output function.
     * ARGUMENTS:
     *   - source APi or device:
     *      UINT Source;
     *   - error type:
     *      UINT Type;
     *   - error message id:
     *      UINT Id, 
     *   - message severity:
     *      UINT severity, 
     *   - message text length:
     *      INT Length, 
     *   - message text:
     *      CHAR *Message, 
     *   - user addon parameters pointer:
     *      VOID *UserParam;
     * RETURNS: None.
     */
    static VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                        INT Length, const CHAR *Message, const VOID *UserParam );

    /***
     * Primitive methods
     ***/

    /* Load primitive from '*.OBJ' file function.
     * ARGUMENTS:
     *   - '*.OBJ' file name:
     *       const std::string &FileName;
     *   - pointer to material:
     *       material *Mtl;
     * RETURNS:
     *   (primitive *) loaded primitive.
     */
    primitive * LoadPrim( const std::string &FileName, material *Mtl );

    /* Draw render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      primitive *Pr;
     *   - matrix:
     *      const matr &World;
     * RETURNS: None.
     */
    VOID Draw( primitive *Pr, const matr &World, INT InstanceCount = 0 );

    /* Get texture from target function
     * ARGUMENTS: None.
     * RETURNS:
     *   (texture*) texture from target
     */
    texture *GetTexture( VOID );

    /* Draw render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      primitive *Pr;
     * RETURNS: None.
     */
    VOID Draw( primitive *Pr, INT InstanceCount = 0 );

    /* Add material function.
     * ARGUMENTS:
     *   - name of material:
     *       const std::string &MtlName;
     *   - shader name:
     *       const std::string &ShName;
     *   - buffer structure:
     *       ill_coefs &Ptr;
     *   - textures name:
     *       const std::string &Tex;
     * RETURNS: 
     *   (material *) Pointer to material.
     */
    material * AddMaterial( const std::string &MtlName, const std::string &ShName );

    /* Create shader function.
     * ARGUMENTS:
     *   - shader name:
     *       const std::string ShdFileNamePrefix;
     * RETURNS: 
     *   (shader *) Pointer to shader.
     */
    shader * LoadShader( const std::string &ShdFileNamePrefix );

    /* Free primitive resource function.
     * ARGUMENTS:
     *   - pointer to resource:
     *      primitive *Pr;
     * RETURNS: None.
     */
    VOID Free( primitive *Pr );

    /* Create buffer function
     * ARGUMENTS: 
     *   - binding point:
     *       UINT BindingPoint;
     *   - data structure:
     *       const data_structure &Ptr;
     *   - number of vec4;
     * RETURNS:
     *   (buffer &) pointer to buffer
     */
    template<typename data_structure>
      buffer * AddBuffer( UINT BindingPoint, const data_structure *Ptr )
      {
        return CreateBuffer(BindingPoint, Ptr);
      }
  };
}

#endif /* __render_h_ */


/* END OF 'render.h' FILE */