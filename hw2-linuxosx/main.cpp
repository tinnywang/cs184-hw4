/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include "UCB/grader.h"

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
void display(void) ;  // prototype for display function.  

Grader grader;
bool allowGrader = false;
int oldx;
int oldy;
int angle_change;

// Uses the Projection matrices (technically deprecated) to set perspective 
// We could also do this in a more modern fashion with glm.  
void reshape(int width, int height){
  w = width;
  h = height;
  mat4 mv ; // just like for lookat

  glMatrixMode(GL_PROJECTION);
  float aspect = w / (float) h, zNear = 0.1, zFar = 1000.0 ;
        // I am changing the projection stuff to be consistent with lookat
  if (useGlu) mv = glm::perspective(fovy,aspect,zNear,zFar) ; 
  else {
    mv = Transform::perspective(fovy,aspect,zNear,zFar) ; 
    mv = glm::transpose(mv) ; // accounting for row major 
  }
  glLoadMatrixf(&mv[0][0]) ; 

  glViewport(0, 0, w, h);
}

void saveScreenshot(string fname) {
  int pix = w * h;
  BYTE pixels[3*pix];	
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}


void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n" 
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n" 
    << "press 'i' to run image grader test cases\n"
    << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"       
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press ESC to quit.\n" ;      
}

void drag(int x, int y) {
  if (angle_change) {
    double diff_x = x - oldx;
    double diff_y = oldy - y;
    double x_ang = glm::degrees(glm::atan(diff_x * .01, 1.0));
    double y_ang = glm::degrees(glm::atan(diff_y*.01,1.0));
    Transform::up(y_ang, eye,  up, center);
    Transform::left(-x_ang, eye, up, center);
    oldx = x ;
    oldy = y ;
    glutPostRedisplay();
  }
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      oldx = x ;
      oldy = y ;
      angle_change = true;
    }
    if (state == GLUT_UP) {	
      angle_change = false;
    }
  }
}


void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case '+':
    amount++;
    std::cout << "amount set to " << amount << "\n" ;
    break;
    case '-':
    if (amount > 0) {
      amount--;
    }
    std::cout << "amount set to " << amount << "\n" ; 
    break;
    case 'g':
    useGlu = !useGlu;
    reshape(w,h) ; 
    std::cout << "Using glm::LookAt and glm::Perspective set to: " << (useGlu ? " true " : " false ") << "\n" ; 
    break;
    case 'h':
    printHelp();
    break;
    case 27:  // Escape to quit
    exit(0) ;
    break ;
    case 'r': // reset eye and up vectors, scale and translate. 
    eye = eyeinit ; 
    up = upinit ; 
    center = centerinit;
    fovy = fovyinit;
    sx = sy = 1.0 ; 
    reshape(w,h) ; 
    break ;   
    case 'v': 
    transop = view ;
    std::cout << "Operation is set to View\n" ; 
    break ; 
    case 's':
    transop = scale ; 
    std::cout << "Operation is set to Scale\n" ; 
    break ; 
    case 'z':
      if (fovy < 179) {
        fovy++;
      }
      reshape(w,h) ; 
      std::cout << fovy << "\n";
    break;
    case 'x':
      if (fovy > 1) {
        fovy--;
      }
      reshape(w,h) ; 
      std::cout << fovy << "\n";
      
    break;
  }
  glutPostRedisplay();
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
  switch(key) {
    case 100: //left
    if (transop == view) Transform::side(-amount * .1, eye,  up, center);
    else if (transop == scale) sx -= amount * 0.01 ; 
    break;
    case 101: //up
    if (transop == view) Transform::forward(amount * .1,  eye,  center);
    else if (transop == scale) sy += amount * 0.01 ; 
    break;
    case 102: //right
    if (transop == view) Transform::side(amount* .1, eye,  up, center);
    else if (transop == scale) sx += amount * 0.01 ; 
    break;
    case 103: //down
    if (transop == view) Transform::forward(-amount* .1,  eye,  center);
    else if (transop == scale) sy -= amount * 0.01 ; 
    break;
  }
  glutPostRedisplay();
}

void init() {
      // Initialize shaders
  vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
  shaderprogram = initprogram(vertexshader, fragmentshader) ; 
  enablelighting = glGetUniformLocation(shaderprogram,"enablelighting") ;
  lightpos = glGetUniformLocation(shaderprogram,"lightposn") ;       
  lightcol = glGetUniformLocation(shaderprogram,"lightcolor") ;       
  numusedcol = glGetUniformLocation(shaderprogram,"numused") ;       
  ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;       
  diffusecol = glGetUniformLocation(shaderprogram,"diffuse") ;       
  specularcol = glGetUniformLocation(shaderprogram,"specular") ;       
  emissioncol = glGetUniformLocation(shaderprogram,"emission") ;       
  shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;       
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cerr << "Usage: transforms scenefile [grader input (optional)]\n"; 
    exit(-1); 
  }

  FreeImage_Initialise();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("HW2: Scene Viewer");
  init();
  readfile(argv[1]) ; 
  glutDisplayFunc(display);
  glutSpecialFunc(specialKey);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);
  glutReshapeWindow(w, h);
  glutMotionFunc(drag);	


  if (argc > 2) {
    allowGrader = true;
    stringstream tcid;
    tcid << argv[1] << "." << argv[2];
    grader.init(tcid.str());
    grader.loadCommands(argv[2]);
    grader.bindDisplayFunc(display);
    grader.bindSpecialFunc(specialKey);
    grader.bindKeyboardFunc(keyboard);
    grader.bindScreenshotFunc(saveScreenshot);
  }

  printHelp();
  glutMainLoop();
  FreeImage_DeInitialise();
  return 0;
}
