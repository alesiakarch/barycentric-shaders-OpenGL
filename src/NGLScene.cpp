#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include <ngl/Util.h>
#include <iostream>

////// This code is Jon's boilerplate with of my alterations (properly reference later)

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("NPR Charcoal Rendering");
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's, Shaders, FBOs and Textures\n";
  GLuint fbos[] = {m_fboID1, m_fboID2, m_fboID3, m_fboID4, m_fboID5};
  GLuint textures[] = {m_fbotexture1, m_fbotexture2, m_fbotexture3, m_fbotexture4, m_fbotexture5,
                       m_controltexture1, m_controltexture2, m_controltexture3, m_controltexture4,
                       m_controltexture5, m_controltexture6, m_controltexture7, m_controltexture8, m_controltexture9};  
  glDeleteFramebuffers(5, fbos);
  glDeleteTextures(14, textures);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_cam = ngl::perspective(45.0f, float(_w)/float(_h), 0.01f, 200.0f); // defines camera and aspect ratio
}

//----------------------------------------------------------------------------------------------------------------------

NGLScene::FBO NGLScene::createFBO(int width, int height) {
    FBO fbo;

    glGenFramebuffers(1, &fbo.fboId);    // Generate FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboId);

    // Create a colour texture
    glGenTextures(1, &fbo.fboTexture);
    glBindTexture(GL_TEXTURE_2D, fbo.fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.fboTexture, 0);

    // Create the render buffer for depth and stencil
    GLuint rboID;
    glGenRenderbuffers(1, &rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_win.width, m_win.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID);

    // Check if the FBO is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      ngl::NGLMessage::addError("Framebuffer is not complete!");
    }
    else
    {
      ngl::NGLMessage::addMessage("Created FBO with ID: " + std::to_string(fbo.fboId));
    }

    // Unbind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return fbo;
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  m_eyePos = ngl::Vec3(0, 20.0f, 20.0f); // sets the eye position
  m_view = ngl::lookAt(m_eyePos, {0, 0, 0}, {0, 1.0f, 0}); // sets the look at (eye pos, look at point pos, up axis)
 
  // loading shaders for each pass
  ngl::ShaderLib::loadShader("DiffuseShader", "../shaders/MeshVertexShader.glsl", "../shaders/DiffuseFragmentShader.glsl");
  ngl::ShaderLib::use("DiffuseShader");

  ngl::ShaderLib::loadShader("ShadowShader", "../shaders/MeshVertexShader.glsl", "../shaders/ShadowFragmentShader.glsl");
  ngl::ShaderLib::use("ShadowShader");
  
  ngl::ShaderLib::loadShader("SpecularShader", "../shaders/MeshVertexShader.glsl", "../shaders/SpecularFragmentShader.glsl");
  ngl::ShaderLib::use("SpecularShader");

  ngl::ShaderLib::loadShader("OutlineShader", "../shaders/MeshVertexShader.glsl", "../shaders/OutlineFragmentShader.glsl");
  ngl::ShaderLib::use("OutlineShader");

  ngl::ShaderLib::loadShader("UVShader", "../shaders/MeshVertexShader.glsl", "../shaders/UVFragmentShader.glsl");
  ngl::ShaderLib::use("UVShader");

  // load control images
  m_controltexture1 = loadTexture("../textures/control_image1.png");
  m_controltexture2 = loadTexture("../textures/control_image2.png");
  m_controltexture3 = loadTexture("../textures/control_image3.png");
  m_controltexture4 = loadTexture("../textures/control_image4.png");
  m_controltexture5 = loadTexture("../textures/control_image5.png");
  m_controltexture6 = loadTexture("../textures/control_image6.png");
  m_controltexture7 = loadTexture("../textures/control_image7.png");
  m_controltexture8 = loadTexture("../textures/control_image8.png");
  m_controltexture9 = loadTexture("../textures/control_image9.png");
  
  // load shader for a light cube
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);

  // load shader for the screen quad
  ngl::ShaderLib::loadShader("QuadShader", "../shaders/QuadVertexShader.glsl", "../shaders/FinalQuadFragmentShader.glsl");
  ngl::ShaderLib::use("QuadShader");

  // create a FBOs for each pass
  FBO fboDiffuse = createFBO(m_win.width, m_win.height);
  FBO fboShadow = createFBO(m_win.width, m_win.height);
  FBO fboSpecular = createFBO(m_win.width, m_win.height);
  FBO fboOutline = createFBO(m_win.width, m_win.height);
  FBO fboUV = createFBO(m_win.width, m_win.height);
  
  m_fboID1 = fboUV.fboId;
  m_fbotexture1 = fboUV.fboTexture;
  m_fboID2 = fboDiffuse.fboId;
  m_fbotexture2 = fboDiffuse.fboTexture; 
  m_fboID3 = fboShadow.fboId;
  m_fbotexture3 = fboShadow.fboTexture;
  m_fboID4 = fboSpecular.fboId;
  m_fbotexture4 = fboSpecular.fboTexture;
  m_fboID5 = fboOutline.fboId;
  m_fbotexture5 = fboOutline.fboTexture;
  

  // make screen quad
  m_quad = makeQuad();

  // load the mesh
  ngl::VAOPrimitives::loadObj("hand", "../models/Hand.obj");

  startTimer(10);  
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  update();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::paintGL()
{ 
  // add maya style controls
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace); 
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  auto mouseRotation = rotX * rotY;
  mouseRotation.m_m[3][0] = m_modelPos.m_x; 
  mouseRotation.m_m[3][1] = m_modelPos.m_y; 
  mouseRotation.m_m[3][2] = m_modelPos.m_z; 

  // connect the UV FBO
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
  
  ngl::ShaderLib::use("UVShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation); 
  ngl::VAOPrimitives::draw("hand");

  // connect the diffuse FBO
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID2);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
 
  // draw light
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::Transformation tx;
  tx.setPosition(m_lightPos);
  tx.setScale(1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * tx.getMatrix()* mouseRotation);
  ngl::VAOPrimitives::draw("cube");

  ngl::ShaderLib::use("DiffuseShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
    ngl::ShaderLib::setUniform("modelMat", mouseRotation); //modelMat for the vertex shader
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  ngl::VAOPrimitives::draw("hand"); 

  // connect the shadow FBO
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID3);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
  
  ngl::ShaderLib::use("ShadowShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation); // modelMat for the vertex shader
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  ngl::VAOPrimitives::draw("hand"); 

  // connect the specular FBO
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID4);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
  
  ngl::ShaderLib::use("SpecularShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation); // modelMat for the vertex shader
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  ngl::ShaderLib::setUniform("viewPos", m_eyePos);
  ngl::VAOPrimitives::draw("hand"); 

  // connect the outline FBO
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID5);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
  
  ngl::ShaderLib::use("OutlineShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation); // modelMat for the vertex shader
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  ngl::VAOPrimitives::draw("hand"); 

  // default framebuffer draw quad
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_win.width, m_win.height);

  ngl::ShaderLib::use("QuadShader");
  glBindVertexArray(m_quad);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture1);
  ngl::ShaderLib::setUniform("UVTexture", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture2);
  ngl::ShaderLib::setUniform("DiffuseWeight", 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture3);
  ngl::ShaderLib::setUniform("ShadowWeight", 2);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture4);
  ngl::ShaderLib::setUniform("SpecularWeight", 3);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture5);
  ngl::ShaderLib::setUniform("OutlineWeight", 4);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, m_controltexture6);
  ngl::ShaderLib::setUniform("diffuseTexture", 5);
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, m_controltexture3);
  ngl::ShaderLib::setUniform("shadowTexture", 6);
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, m_controltexture9);
  ngl::ShaderLib::setUniform("specularTexture", 7);
  glActiveTexture(GL_TEXTURE8);
  glBindTexture(GL_TEXTURE_2D, m_controltexture1);
  ngl::ShaderLib::setUniform("outlineTexture", 8);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  glBindVertexArray(0);
    
}

//----------------------------------------------------------------------------------------------------------------------

GLuint NGLScene::loadTexture(std::string_view _texpath)
{
  ngl::Texture texture(_texpath);
  GLuint texname;
  texname = texture.setTextureGL();

  // check or errors
  if (texname == 0)
  {
    ngl::NGLMessage::addError("Failed to load texture!");
  }
  else
  {
    ngl::NGLMessage::addMessage("Loaded Texture: " + std::string(_texpath));
  }

  return texname;
}

//----------------------------------------------------------------------------------------------------------------------

GLuint NGLScene::makeQuad()
{
  // create a quad for drawing the final fbo
  GLfloat quadData[] = {
    // Positions    // UVs
     -1.0f,  1.0f,  0.0f, 1.0f,
     -1.0f, -1.0f,  0.0f, 0.0f,
      1.0f, -1.0f,  1.0f, 0.0f,

     -1.0f,  1.0f,  0.0f, 1.0f,
      1.0f, -1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f, 1.0f
  };

  GLuint quadVBO;
  glGenBuffers(1, &quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), &quadData, GL_STATIC_DRAW);

  GLuint quadVAO;
  // positions vao
  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

  // UVs vao
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glBindVertexArray(0);
  
  // check for errors
  if (glGetError() != GL_NO_ERROR)
  {
    ngl::NGLMessage::addError("Failed to create a Screen Quad!");
  }
  else
  {
    ngl::NGLMessage::addMessage("Created Screen Quad");
  }

  return quadVAO;
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
      break;
  // WASD + QE keys to move the light source    
  case Qt::Key_W : 
      m_lightPos.m_y+=0.1f;
      break;
  case Qt::Key_S : 
      m_lightPos.m_y-=0.1f;
      break;
  case Qt::Key_A : 
      m_lightPos.m_x-=0.1f;
      break;
  case Qt::Key_D :
      m_lightPos.m_x+=0.1f;
      break;
  case Qt::Key_Q :
      m_lightPos.m_z+=0.1f;
      break;
  case Qt::Key_E :
      m_lightPos.m_z-=0.1f;
      break;

  
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
