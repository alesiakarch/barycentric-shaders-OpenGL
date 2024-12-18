#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <iostream>

////// This code is Jon's boilerplate with some of my alterations (properly reference later)

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Blank NGL");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
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
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  m_view = ngl::lookAt({0, 20.0f, 20.0f}, {0, 0, 0}, {0, 1.0f, 0}); // AK sets the look at (eye pos, look at point pos, up axis)
  ngl::VAOPrimitives::createTrianglePlane("world_grid", 10, 10, 1, 1, ngl::Vec3::up()); // AK creates a grid
  ngl::VAOPrimitives::loadObj("hand", "models/Hand_openglTest.obj"); // AK loads the bunny model
  ngl::ShaderLib::loadShader("BunnyShader", "shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
  startTimer(10); // AK
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  update();
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);

  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace); //AK adds maya style controls
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace); //AK
  auto mouseRotation = rotX * rotY; // AK
  mouseRotation.m_m[3][0] = m_modelPos.m_x; //AK
  mouseRotation.m_m[3][1] = m_modelPos.m_y; //AK
  mouseRotation.m_m[3][2] = m_modelPos.m_z; //AK

  ngl::ShaderLib::use(ngl::nglCheckerShader);
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::VAOPrimitives::draw("world_grid"); // AK

  ngl::ShaderLib::use("BunnyShader");
  ngl::ShaderLib::setUniform("MVP", m_cam * m_view * mouseRotation);
  ngl::VAOPrimitives::draw("hand"); // AK
  
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
