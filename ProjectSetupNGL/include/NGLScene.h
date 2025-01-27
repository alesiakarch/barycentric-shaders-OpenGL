#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <BaryObj.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Vec4.h>
#include "WindowParams.h"
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;

    struct FBO {
        GLuint fboId;
        GLuint fboTexture;
        GLuint fboDepth;
    };
    FBO createFBO(int _width, int _height);

private:

    std::unique_ptr<BaryObj> m_mesh;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;
    /// @brief windows parameters for mouse control etc.
    WinParams m_win;
    /// position for our model
    ngl::Vec3 m_modelPos;
    ngl::Vec3 m_eyePos;
    ngl::Mat4 m_cam; // AK stores camera values
    ngl::Mat4 m_view; // AK sets camera look at
    // AK stores light position
    ngl::Vec3 m_lightPos={0.0f,1.0f,0.0f};
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent(QTimerEvent *_event) override;
    // create a framebuffer object with a texture
   
    void setupFBO();
    void renderToFBO();
    // create a quad for drawing the fbo
    GLuint m_quad;
    GLuint makeQuad(); 
    // name for the fbo texture
    GLuint m_fbotexture;
    GLuint m_fbotexture2;
    GLuint m_fbotexture3;
    GLuint m_fbotexture4;
    GLuint m_fbotexture5;
    // name for the fbo
    GLuint m_fboID; 
    GLuint m_fboID2;
    GLuint m_fboID3;
    GLuint m_fboID4;
    GLuint m_fboID5;
    // name for the loaded textures
    GLuint m_textureName;
    GLuint m_controltexture1;
    GLuint m_controltexture2;
    GLuint m_controltexture3;
    GLuint m_controltexture4;
    GLuint m_controltexture5;
    GLuint m_controltexture6;
    GLuint m_controltexture7;
    GLuint m_controltexture8;
    GLuint m_controltexture9;
    // the VAO for the quad
    



};



#endif
