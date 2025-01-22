#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include <ngl/Util.h>
#include <iostream>

////// This code is Jon's boilerplate with of my alterations (properly reference later)

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("NPRRendering NGL");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  //glDeleteTextures(1, &m_textureID); // change names
  //glDeleteFramebuffers(1, &m_fboID);
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_cam = ngl::perspective(45.0f, float(_w)/float(_h), 0.01f, 200.0f); // defines camera and aspect ratio
}


void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  m_view = ngl::lookAt({0, 20.0f, 20.0f}, {0, 0, 0}, {0, 1.0f, 0}); // AK sets the look at (eye pos, look at point pos, up axis)

  // making a shader program for the final render to screen
  ngl::ShaderLib::createShaderProgram("DiffuseShader");
  ngl::ShaderLib::attachShader("DiffuseVertex", ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader("DiffuseFragment", ngl::ShaderType::FRAGMENT);
  ngl::ShaderLib::loadShaderSource("DiffuseVertex", "../shaders/MeshVertexShader.glsl");
  ngl::ShaderLib::loadShaderSource("DiffuseFragment", "../shaders/DiffuseFragmentShader.glsl"); 
  ngl::ShaderLib::compileShader("DiffuseVertex");
  ngl::ShaderLib::compileShader("DiffuseFragment"); 
  ngl::ShaderLib::attachShaderToProgram("DiffuseShader", "DiffuseVertex");
  ngl::ShaderLib::attachShaderToProgram("DiffuseShader", "DiffuseFragment");
  ngl::ShaderLib::linkProgramObject("DiffuseShader");
  ngl::ShaderLib::use("DiffuseShader"); // use the shader program
  // load texture for the mesh
  ngl::Texture texture("../textures/testTexture.png");
  m_textureName = texture.setTextureGL();

  
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 0.5f, 0.8f, 0.9f, 1.0f);

  ngl::ShaderLib::loadShader("TestShader", "../shaders/MeshVertexShader.glsl", "../shaders/HandFragmentShader.glsl");
  ngl::ShaderLib::use("TestShader");

  ngl::ShaderLib::loadShader("QuadShader", "../shaders/QuadVertexShader.glsl", "../shaders/FinalQuadFragmentShader.glsl");
  ngl::ShaderLib::use("QuadShader");
  // Create and bind the FBO
  glGenFramebuffers(1, &m_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

  // Create the texture to render to
  glGenTextures(1, &m_fbotexture);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_win.width, m_win.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbotexture, 0);

  // Create the render buffer for depth and stencil
  GLuint rboID;
  glGenRenderbuffers(1, &rboID);
  glBindRenderbuffer(GL_RENDERBUFFER, rboID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_win.width, m_win.height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cerr << "[ERROR] Framebuffer is not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

   // create a quad for drawing the fbo
  GLfloat quadData[] = {
    // Positions    // Texture Coords
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
  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

  // Texture coordinate attribute (location = 1) swap GLuint to texture name
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glBindVertexArray(0);
  
  m_quad = quadVAO;
  //m_textureName = texture.setTextureGL();
  // switch fbos around in the paintGL 
  // project the fbo texture onto the hand as the final shader
  // calculate how the hand needs to look before the final shader (applied to a quad)
  // have multiple shaders (diffuse, specular, etc) applied to the hand, then snapshoted to the fbo
  // then multiplied into the final shader
  ngl::VAOPrimitives::createTrianglePlane("world_grid", 10, 10, 1, 1, ngl::Vec3::up()); // AK creates a grid
  ngl::VAOPrimitives::loadObj("hand", "../models/Hand_openglTest.obj"); // AK loads the bunny model
  
  startTimer(10);  
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  update();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::paintGL()
{
  // clear the screen and depth buffer m_fboID
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);

  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace); // adds maya style controls
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  auto mouseRotation = rotX * rotY;
  mouseRotation.m_m[3][0] = m_modelPos.m_x; 
  mouseRotation.m_m[3][1] = m_modelPos.m_y; 
  mouseRotation.m_m[3][2] = m_modelPos.m_z; 

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::VAOPrimitives::draw("world_grid"); 

  ngl::ShaderLib::use("DiffuseShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureName);
  ngl::ShaderLib::setUniform("Handtex", 0);
  ngl::VAOPrimitives::draw("hand"); 

  // default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_win.width, m_win.height);

  ngl::ShaderLib::use("QuadShader");
  //ngl::VAOPrimitives::draw("m_quad");
  glBindVertexArray(m_quad);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture);
  ngl::ShaderLib::setUniform("Quadtex", 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  glBindVertexArray(0);
  
  
}

//----------------------------------------------------------------------------------------------------------------------

// makequad()

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
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
