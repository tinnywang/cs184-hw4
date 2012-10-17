/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Transform.h"

using namespace std ; 
#include "variables.h"
#include "readfile.h"
#include <vector>

void load_obj(const char * filename, vector<glm::vec3> &face_vertices, vector<glm::vec3> &face_normals) {
  string str, cmd;
  ifstream in;
  in.open(filename);
  if (!in.is_open()) {
    cerr << "Unable to open OBJ file " << filename << "\n";
    throw 2;
  }
  vector<glm::vec3> vertices, normals;
  getline(in, str);
  while (in) {
    if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
      stringstream ss(str);
      ss >> cmd;
      // vertex
      if (cmd.compare("v") == 0) {
	float x, y, z;
	ss >> x >> y >> z;
	glm::vec3 vertex = glm::vec3(x, y, z);
	vertices.push_back(vertex);
	//cout << x << " " << y << " " << z << "\n";
      // vertex normal
      } else if (cmd.compare("vn") == 0) {
	float x, y, z;
	ss >> x >> y >> z;
	glm::vec3 normal = glm::vec3(x, y, z);
	normals.push_back(normal);

      // face
      } else if (cmd.compare("f") == 0) {
	int vertexIndex[3], vertexTextureIndex[3], normalIndex[3];
	string index, temp[3];
	ss >> temp[0] >> temp[1] >> temp[2];
	int i;
	for (i = 0; i < 3; i++) {
	  stringstream temp_ss(temp[i]);
	  getline(temp_ss, index, '/');
	  glm::vec3 vertex = vertices[atoi(index.c_str()) - 1];
	  face_vertices.push_back(vertex);
	  getline(temp_ss, index, '/');
	  // skip texture
	  getline(temp_ss, index, '/');
	  glm::vec3 normal = normals[atoi(index.c_str()) - 1];
	  face_normals.push_back(normal);
	}
      }  else {
      }
    }
    getline(in, str);
  }
}

void draw_obj(vector<glm::vec3> &vertices, vector<glm::vec3> &normals) {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glNormalPointer(GL_FLOAT, 0, &normals[0]);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void draw_claptrap() {
  vector<glm::vec3> vertices, normals;
  load_obj("claptrap/Clap Trap.obj", vertices, normals);
  draw_obj(vertices, normals);
}

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  

void transformvec (const GLfloat input[4], GLfloat output[4]) {
  GLfloat modelview[16] ; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ; 
  
  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0 ; 
    for (int j = 0 ; j < 4 ; j++) { 
      output[i] += modelview[4*j+i] * input[j] ;
    }
  }
}

void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // I'm including the basic matrix setup for model view to 
        // give some sense of how this works.  

	glMatrixMode(GL_MODELVIEW);
	mat4 mv ; 

        if (useGlu) mv = glm::lookAt(eye,center,up) ; 
	else {
          mv = Transform::lookAt(eye,center,up) ;
          mv = glm::transpose(mv) ; // accounting for row major
        }
        glLoadMatrixf(&mv[0][0]) ; 

        // Set Light and Material properties for the teapot
        // Lights are transformed by current modelview matrix. 
        // The shader can't do this globally. 
        // So we need to do so manually.  
        if (numused) {
          glUniform1i(enablelighting,true) ;

          // YOUR CODE FOR HW 2 HERE.  
          // You need to pass the lights to the shader. 
          // Remember that lights are transformed by modelview first.
	  for (int i = 0; i < numused; i++) {
	    const GLfloat _light[] = {lightposn[4*i], lightposn[4*i+1], lightposn[4*i+2], lightposn[4*i+3]};
	    GLfloat light[4];
	    transformvec(_light, light);
	    GLfloat color[] = {lightcolor[4*i], lightcolor[4*i+1], lightcolor[4*i+2], lightcolor[4*i+3]};
	    glUniform4fv(lightpos+i, 1, light);
	    glUniform4fv(lightcol+i, 1, color);
	  }
	  glUniform1i(numusedcol, numused);
        }
        else glUniform1i(enablelighting,false) ; 
     
        // Transformations for objects, involving translation and scaling 
        mat4 sc(1.0) , tr(1.0), transf(1.0) ; 
        sc = Transform::scale(sx,sy,1.0) ; 
        tr = Transform::translate(tx,ty,0.0) ; 

        // YOUR CODE FOR HW 2 HERE.  
        // You need to use scale, translate and modelview to 
        // set up the net transformation matrix for the objects.  
        // Account for GLM issues etc.  
        glLoadMatrixf(&transf[0][0]) ; 

	transf = glm::transpose(mv) * transf;	
	transf = tr * transf;
	transf = sc * transf;
	for (int i = 0 ; i < numobjects ; i++) {
          object * obj = &(objects[i]) ; 

          {
          // YOUR CODE FOR HW 2 HERE. 
          // Set up the object transformations 
          // And pass in the appropriate material properties
	    mat4 transform = obj -> transform;
	    glLoadMatrixf(&glm::transpose(transform * transf)[0][0]);
	    glUniform4fv(ambientcol, 1, obj -> ambient);
            glUniform4fv(diffusecol, 1, obj -> diffuse);
            glUniform4fv(specularcol, 1, obj -> specular);
            glUniform4fv(emissioncol, 1, obj -> emission);
            glUniform1f(shininesscol, obj -> shininess);
          }

	  // Draw the outline of objects
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_FRONT);
	  glEnable(GL_LINE_SMOOTH);
	  glLineWidth(4.0);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  glColor3f(0, 0, 0);
	  glUniform1i(enablelighting, false);

          // Actually draw the object
          // We provide the actual glut drawing functions for you. 
	  if (obj -> type == claptrap) {
	    draw_claptrap();
	  } else if (obj -> type == cube) {
            glutSolidCube(obj->size) ; 
          }
          else if (obj -> type == sphere) {
            const int tessel = 20 ; 
            glutSolidSphere(obj->size, tessel, tessel) ; 
          }
          else if (obj -> type == teapot) {
	    // MUST do this because teapot is inverted.
	    glCullFace(GL_BACK);
            glutSolidTeapot(obj->size) ;
          }

	  // Undo wireframe rendering.
	  glDisable(GL_CULL_FACE);
	  glDisable(GL_LINE_SMOOTH);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  glUniform1i(enablelighting, true);
	  
	  // Redraw the object so that it can be cel shaded.
	  if (obj -> type == claptrap) {
	    draw_claptrap();
	  } else if (obj -> type == cube) {
            glutSolidCube(obj->size) ;
          }
          else if (obj -> type == sphere) {
            const int tessel = 20 ;
            glutSolidSphere(obj->size, tessel, tessel) ;
          }
          else if (obj -> type == teapot) {
            glutSolidTeapot(obj->size) ;
          }
	  
        }
    
        glutSwapBuffers();
}
