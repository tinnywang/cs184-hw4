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
  int loop_count = 3;
	string index, temp[5];
	ss >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4];
  if (temp[4] != "") {
    loop_count = 5;
  } else if (temp[3] != "") {
    loop_count = 4;
  }
	int i;
  vector<glm::vec3> temp_face;
  vector<glm::vec3> temp_norm;
	for (i = 0; i < loop_count; i++) {
	  stringstream temp_ss(temp[i]);
	  getline(temp_ss, index, '/');
	  glm::vec3 vertex = vertices[atoi(index.c_str()) - 1];
	  temp_face.push_back(vertex);
	  getline(temp_ss, index, '/');
	  // skip texture
	  getline(temp_ss, index, '/');
	  glm::vec3 normal = normals[atoi(index.c_str()) - 1];
	  temp_norm.push_back(normal);
	}
	if (temp_face.size() == 4) {
    face_vertices.push_back(temp_face[0]);
    face_vertices.push_back(temp_face[1]);
    face_vertices.push_back(temp_face[2]);
    face_normals.push_back(temp_norm[0]);
    face_normals.push_back(temp_norm[1]);
    face_normals.push_back(temp_norm[2]);
    
    face_vertices.push_back(temp_face[0]);
    face_vertices.push_back(temp_face[2]);
    face_vertices.push_back(temp_face[3]);
    face_normals.push_back(temp_norm[0]);
    face_normals.push_back(temp_norm[2]);
    face_normals.push_back(temp_norm[3]);
	}
	if (temp_face.size() == 5) {
    face_vertices.push_back(temp_face[0]);
    face_vertices.push_back(temp_face[1]);
    face_vertices.push_back(temp_face[2]);
    face_normals.push_back(temp_norm[0]);
    face_normals.push_back(temp_norm[1]);
    face_normals.push_back(temp_norm[2]);
    
    face_vertices.push_back(temp_face[0]);
    face_vertices.push_back(temp_face[2]);
    face_vertices.push_back(temp_face[3]);
    face_normals.push_back(temp_norm[0]);
    face_normals.push_back(temp_norm[2]);
    face_normals.push_back(temp_norm[3]);
    
    face_vertices.push_back(temp_face[0]);
    face_vertices.push_back(temp_face[3]);
    face_vertices.push_back(temp_face[4]);
    face_normals.push_back(temp_norm[0]);
    face_normals.push_back(temp_norm[3]);
    face_normals.push_back(temp_norm[4]);
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

void draw_sword() {
  vector<glm::vec3> vertices, normals;
  load_obj("sword.obj", vertices, normals);
  draw_obj(vertices, normals);
}

void draw_arch() {
  vector<glm::vec3> vertices, normals;
  load_obj("arch.obj", vertices, normals);
  draw_obj(vertices, normals);
}

void draw_cube(double width, double length, double height, double y_start, bool inverse_norm) {
  vector<glm::vec3> vertices, normals;
  //left
  vertices.push_back(glm::vec3(-width/2,y_start,length/2));
  vertices.push_back(glm::vec3(-width/2,y_start,-length/2));
  vertices.push_back(glm::vec3(-width/2,y_start+height,length/2));
  
  vertices.push_back(glm::vec3(-width/2,y_start,-length/2));  
  vertices.push_back(glm::vec3(-width/2,y_start+height,-length/2));
  vertices.push_back(glm::vec3(-width/2,y_start+height,length/2));
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(1,0,0));
    normals.push_back(glm::vec3(1,0,0));
    normals.push_back(glm::vec3(1,0,0));
    normals.push_back(glm::vec3(1,0,0));
    normals.push_back(glm::vec3(1,0,0));
    normals.push_back(glm::vec3(1,0,0));
  } else {
    normals.push_back(-glm::vec3(1,0,0));
    normals.push_back(-glm::vec3(1,0,0));
    normals.push_back(-glm::vec3(1,0,0));
    normals.push_back(-glm::vec3(1,0,0));
    normals.push_back(-glm::vec3(1,0,0));
    normals.push_back(-glm::vec3(1,0,0));
  }
  //right
  vertices.push_back(glm::vec3(width/2,y_start,length/2));
  vertices.push_back(glm::vec3(width/2,y_start+height,length/2));
  vertices.push_back(glm::vec3(width/2,y_start,-length/2));
  
  vertices.push_back(glm::vec3(width/2,y_start,-length/2));  
  vertices.push_back(glm::vec3(width/2,y_start+height,length/2));
  vertices.push_back(glm::vec3(width/2,y_start+height,-length/2));
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(-1,0,0));
    normals.push_back(glm::vec3(-1,0,0));
    normals.push_back(glm::vec3(-1,0,0));
    normals.push_back(glm::vec3(-1,0,0));
    normals.push_back(glm::vec3(-1,0,0));
    normals.push_back(glm::vec3(-1,0,0));
  } else {
    normals.push_back(-glm::vec3(-1,0,0));
    normals.push_back(-glm::vec3(-1,0,0));
    normals.push_back(-glm::vec3(-1,0,0));
    normals.push_back(-glm::vec3(-1,0,0));
    normals.push_back(-glm::vec3(-1,0,0));
    normals.push_back(-glm::vec3(-1,0,0));
  }
  //top
  vertices.push_back(glm::vec3(-width/2,y_start+height,length/2));
  vertices.push_back(glm::vec3(-width/2,y_start+height,-length/2));
  vertices.push_back(glm::vec3(width/2,y_start+height,-length/2));
  
  vertices.push_back(glm::vec3(width/2,y_start+height,-length/2));  
  vertices.push_back(glm::vec3(width/2,y_start+height,length/2));
  vertices.push_back(glm::vec3(-width/2,y_start+height,length/2));
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
  } else {
    normals.push_back(-glm::vec3(0,-1,0));
    normals.push_back(-glm::vec3(0,-1,0));
    normals.push_back(-glm::vec3(0,-1,0));
    normals.push_back(-glm::vec3(0,-1,0));
    normals.push_back(-glm::vec3(0,-1,0));
    normals.push_back(-glm::vec3(0,-1,0));
  }
  
  //bottom
  vertices.push_back(glm::vec3(-width/2,y_start,length/2));
  vertices.push_back(glm::vec3(width/2,y_start,-length/2));
  vertices.push_back(glm::vec3(-width/2,y_start,-length/2));
  
  vertices.push_back(glm::vec3(width/2,y_start,-length/2));  
  vertices.push_back(glm::vec3(-width/2,y_start,length/2));  
  vertices.push_back(glm::vec3(width/2,y_start,length/2));
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
  } else {
    normals.push_back(-glm::vec3(0,1,0));
    normals.push_back(-glm::vec3(0,1,0));
    normals.push_back(-glm::vec3(0,1,0));
    normals.push_back(-glm::vec3(0,1,0));
    normals.push_back(-glm::vec3(0,1,0));
    normals.push_back(-glm::vec3(0,1,0));
  }
  
  //near
  vertices.push_back(glm::vec3(-width/2,y_start,length/2));
  vertices.push_back(glm::vec3(width/2,y_start+height,length/2));
  vertices.push_back(glm::vec3(width/2,y_start,length/2));
  
  vertices.push_back(glm::vec3(-width/2,y_start,length/2));  
  vertices.push_back(glm::vec3(-width/2,y_start+height,length/2));  
  vertices.push_back(glm::vec3(width/2,y_start+height,length/2));
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(0,0,-1));
    normals.push_back(glm::vec3(0,0,-1));
    normals.push_back(glm::vec3(0,0,-1));
    normals.push_back(glm::vec3(0,0,-1));
    normals.push_back(glm::vec3(0,0,-1));
    normals.push_back(glm::vec3(0,0,-1));
  } else {
    normals.push_back(-glm::vec3(0,0,-1));
    normals.push_back(-glm::vec3(0,0,-1));
    normals.push_back(-glm::vec3(0,0,-1));
    normals.push_back(-glm::vec3(0,0,-1));
    normals.push_back(-glm::vec3(0,0,-1));
    normals.push_back(-glm::vec3(0,0,-1));
  }
  //far
  vertices.push_back(glm::vec3(-width/2,y_start,-length/2));
  vertices.push_back(glm::vec3(width/2,y_start,-length/2));
  vertices.push_back(glm::vec3(width/2,y_start+height,-length/2));
  
  vertices.push_back(glm::vec3(-width/2,y_start,-length/2));  
  vertices.push_back(glm::vec3(width/2,y_start+height,-length/2));
  vertices.push_back(glm::vec3(-width/2,y_start+height,-length/2));  
  
  if (inverse_norm) {
    normals.push_back(glm::vec3(0,0,1));
    normals.push_back(glm::vec3(0,0,1));
    normals.push_back(glm::vec3(0,0,1));
    normals.push_back(glm::vec3(0,0,1));
    normals.push_back(glm::vec3(0,0,1));
    normals.push_back(glm::vec3(0,0,1));
  } else {
    normals.push_back(-glm::vec3(0,0,1));
    normals.push_back(-glm::vec3(0,0,1));
    normals.push_back(-glm::vec3(0,0,1));
    normals.push_back(-glm::vec3(0,0,1));
    normals.push_back(-glm::vec3(0,0,1));
    normals.push_back(-glm::vec3(0,0,1));
  }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glNormalPointer(GL_FLOAT, 0, &normals[0]);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void draw_cylinder(double top_radius, double bottom_radius, double height, double y_start) {
  vector<glm::vec3> vertices, normals;
  for (int i = 0; i < 360; ++i) {
    // main cylinder
    glm::vec3 point1(-bottom_radius,y_start,0);
    glm::vec3 point2(-top_radius,y_start+height,0);
    glm::vec3 v1 = point1 * Transform::rotate(i, glm::vec3(0,1,0));
    glm::vec3 v2 = point2 * Transform::rotate(i, glm::vec3(0,1,0));
    glm::vec3 v3 = point1 * Transform::rotate(i+1, glm::vec3(0,1,0));
    glm::vec3 v4 = point2 * Transform::rotate(i+1, glm::vec3(0,1,0));
    
    glm::vec3 norm1 = glm::vec3(-1,0,0) * Transform::rotate(i, glm::vec3(0,1,0));
    glm::vec3 norm2 = glm::vec3(-1,0,0) * Transform::rotate(i+1, glm::vec3(0,1,0));
    
    // curve part
    vertices.push_back(v1); 
    vertices.push_back(v3);  
    vertices.push_back(v4);  
    
    vertices.push_back(v1);  
    vertices.push_back(v4);  
    vertices.push_back(v2);  
    
    normals.push_back(norm1);
    normals.push_back(norm2);
    normals.push_back(norm2);

    normals.push_back(norm1);
    normals.push_back(norm2);
    normals.push_back(norm1);   
    
    // top face
    vertices.push_back(v2);
    vertices.push_back(v4);
    vertices.push_back(glm::vec3(0,y_start+height,0));
    
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
        
    // bottom face
    vertices.push_back(v1);
    vertices.push_back(glm::vec3(0,y_start,0));
    vertices.push_back(v3);
    
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
    normals.push_back(glm::vec3(0,-1,0));
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glNormalPointer(GL_FLOAT, 0, &normals[0]);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}


void draw_room(double width, double length, double height) {
  draw_cube(width, length, height, -height/2, true);
}

void draw_pillar() {
  float cyl_height = 5;
  float normalize_side = 1.4;
  float normalize_radius = 2.8;
  float normalize_height = cyl_height + 1;
  float normalized_offset = .5 - ((cyl_height/2+.2)/ normalize_height + .1 / normalize_height);
  draw_cube(1.4/normalize_side, 1.4/normalize_side, .1 / normalize_height, (cyl_height/2+.2)/ normalize_height + normalized_offset, false);
  draw_cube(1.2/normalize_side, 1.2/normalize_side, .1/ normalize_height, (cyl_height/2+.1)/ normalize_height + normalized_offset, false);
  draw_cylinder(1.2/normalize_radius, 1.2/normalize_radius, .1/ normalize_height, (cyl_height/2)/ normalize_height + normalized_offset);
  draw_cylinder(1/normalize_radius, 1/normalize_radius,  cyl_height/ normalize_height, -(cyl_height/2)/ normalize_height + normalized_offset);
  draw_cylinder(1.2/normalize_radius, 1.2/normalize_radius, .1/ normalize_height, (-cyl_height/2 - .1)/ normalize_height + normalized_offset);
  draw_cylinder(1.2/normalize_radius, 1.4/normalize_radius, .2/ normalize_height, (-cyl_height/2 - .3)/ normalize_height + normalized_offset);
  draw_cube(1.4/normalize_side, 1.4/normalize_side, .4/ normalize_height, (-cyl_height/2 - .7)/ normalize_height + normalized_offset, false);
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
	  /*
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_FRONT);
	  glEnable(GL_LINE_SMOOTH);
	  glLineWidth(4.0);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  glColor3f(0, 0, 0);
	  glUniform1i(enablelighting, false);
	  

          // Actually draw the object
          // We provide the actual glut drawing functions for you. 
	  if (obj -> type == pillar) {
	    draw_pillar();
	  } else if (obj -> type == room) {
      draw_room(obj->width, obj->length, obj->height);
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
	  */    
	  // Redraw the object so that it can be cel shaded.
	  if (obj -> type == pillar) {
	    draw_pillar();
	  } else if (obj -> type == room) {
        draw_room(obj->width, obj->length, obj->height);
  	} else if (obj -> type == sword) {
        draw_sword();
    } else if (obj -> type == arch) {
	draw_arch();
    }else if (obj -> type == cylinder) {
        draw_cylinder(obj->width/2, obj->length/2, obj->height, -obj->height/2);
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
