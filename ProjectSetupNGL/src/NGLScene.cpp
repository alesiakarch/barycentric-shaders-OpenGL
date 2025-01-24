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
  setTitle("NPRRendering NGL");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  //glDeleteTextures(1, &m_textureID); // change names
  //glDeleteFramebuffers(1, &m_fboID);
}

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
      std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    // Unbind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return fbo;
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
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  m_view = ngl::lookAt({0, 20.0f, 20.0f}, {0, 0, 0}, {0, 1.0f, 0}); // AK sets the look at (eye pos, look at point pos, up axis)
  m_lightPos = ngl::Vec3(1.0f, 10.0f, 3.0f); // AK sets the light position
  // making a shader program for the final render to screen
  ngl::ShaderLib::loadShader("DiffuseShader", "../shaders/MeshVertexShader.glsl", "../shaders/DiffuseFragmentShader.glsl");
  ngl::ShaderLib::use("DiffuseShader");

  ngl::ShaderLib::loadShader("ShadowShader", "../shaders/MeshVertexShader.glsl", "../shaders/ShadowFragmentShader.glsl");
  ngl::ShaderLib::use("ShadowShader");
  
  ngl::ShaderLib::loadShader("SpecularShader", "../shaders/MeshVertexShader.glsl", "../shaders/SpecularFragmentShader.glsl");
  ngl::ShaderLib::use("SpecularShader");

  // load texture for the mesh
  ngl::Texture testtexture("../textures/testTexture.png");
  m_textureName = testtexture.setTextureGL();

  // load control images
  ngl::Texture controltexture1("../textures/control_image1.png");
  m_controltexture1 = controltexture1.setTextureGL();
  ngl::Texture controltexture2("../textures/control_image2.png");
  m_controltexture2 = controltexture2.setTextureGL();
  ngl::Texture controltexture3("../textures/control_image3.png");
  m_controltexture3 = controltexture3.setTextureGL();
  ngl::Texture controltexture4("../textures/control_image4.png");
  m_controltexture4 = controltexture4.setTextureGL();
  ngl::Texture controltexture5("../textures/control_image5.png");
  m_controltexture5 = controltexture5.setTextureGL();
  ngl::Texture controltexture6("../textures/control_image6.png");
  m_controltexture6 = controltexture6.setTextureGL();
  ngl::Texture controltexture7("../textures/control_image7.png");
  m_controltexture7 = controltexture7.setTextureGL();
  ngl::Texture controltexture8("../textures/control_image8.png");
  m_controltexture8 = controltexture8.setTextureGL();
  ngl::Texture controltexture9("../textures/control_image9.png");
  m_controltexture9 = controltexture9.setTextureGL();
  
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);

  ngl::ShaderLib::loadShader("TestShader", "../shaders/MeshVertexShader.glsl", "../shaders/HandFragmentShader.glsl");
  ngl::ShaderLib::use("TestShader");

  ngl::ShaderLib::loadShader("QuadShader", "../shaders/QuadVertexShader.glsl", "../shaders/FinalQuadFragmentShader.glsl");
  ngl::ShaderLib::use("QuadShader");


  FBO fboDiffuse = createFBO(m_win.width, m_win.height);
  FBO fboShadow = createFBO(m_win.width, m_win.height);
  FBO fboSpecular = createFBO(m_win.width, m_win.height);
  m_fboID = fboDiffuse.fboId;
  m_fbotexture = fboDiffuse.fboTexture; 
  m_fboID2 = fboShadow.fboId;
  m_fbotexture2 = fboShadow.fboTexture;
  m_fboID3 = fboSpecular.fboId;
  m_fbotexture3 = fboSpecular.fboTexture;

  // make screen quad
  m_quad = makeQuad();

  // have multiple shaders (diffuse, specular, etc) applied to the hand, then snapshoted to the fbo

  ngl::VAOPrimitives::createTrianglePlane("world_grid", 10, 10, 1, 1, ngl::Vec3::up()); // AK creates a grid

  m_mesh.reset(new BaryObj("../models/Hand.obj"));
  std::cout<<"Loaded Mesh\n";

  // now we need to create this as a VAO so we can draw it
  m_mesh->createBaryVAO();
  std::cout<<"Created VAO\n";

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

  // draws the grid
  // ngl::ShaderLib::use(ngl::nglColourShader);
  // ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  // ngl::VAOPrimitives::draw("world_grid"); 
  // draw light sphere

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::Transformation tx;
  tx.setPosition(m_lightPos);
  tx.setScale(1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * tx.getMatrix()* mouseRotation);
  ngl::VAOPrimitives::draw("cube");

  ngl::ShaderLib::use("DiffuseShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation);
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_controltexture5);
  ngl::ShaderLib::setUniform("diffuseTexture1", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_controltexture4);
  ngl::ShaderLib::setUniform("diffuseTexture2", 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_controltexture6);
  ngl::ShaderLib::setUniform("diffuseTexture3", 2);
  m_mesh->draw();

  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID2);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);

  mouseRotation.m_m[3][0] = m_modelPos.m_x; 
  mouseRotation.m_m[3][1] = m_modelPos.m_y; 
  mouseRotation.m_m[3][2] = m_modelPos.m_z; 

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::Transformation tm;
  tm.setPosition(m_lightPos);
  tm.setScale(0.1f,0.1f,0.1f);
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * tx.getMatrix()* mouseRotation);
  ngl::VAOPrimitives::draw("cube");
  
  ngl::ShaderLib::use("ShadowShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation);
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  std::cout << m_lightPos.m_x << ", " << m_lightPos.m_y << ", " << m_lightPos.m_z << std::endl;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_controltexture1);
  ngl::ShaderLib::setUniform("ShadowTexture1", 0);
  m_mesh->draw();

  // specular fbo
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID3);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0, m_win.width, m_win.height);
  
  ngl::ShaderLib::use("SpecularShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("modelMat", mouseRotation);
  ngl::ShaderLib::setUniform("lightPos", m_lightPos);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_controltexture9);
  ngl::ShaderLib::setUniform("specularTexture1", 0);
  m_mesh->draw();


  // default framebuffer draw quad
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_win.width, m_win.height);

  ngl::ShaderLib::use("QuadShader");
  glBindVertexArray(m_quad);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture);
  ngl::ShaderLib::setUniform("Diffusetex", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture2);
  ngl::ShaderLib::setUniform("Shadowtex", 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_fbotexture3);
  ngl::ShaderLib::setUniform("Speculartex", 2);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  glBindVertexArray(0);
  
  
}

//----------------------------------------------------------------------------------------------------------------------

GLuint NGLScene::makeQuad()
{
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
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
