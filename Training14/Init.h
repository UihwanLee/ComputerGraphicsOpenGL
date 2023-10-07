#pragma once
#include "OpenGL.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "filebuf.h"

bool InitProgram(unsigned int& ShaderProgram);

bool Check(unsigned int ShaderProgram);

void CreateCube(GLuint& EBO, GLuint& VBO);
void CreateBoard(GLuint& EBO, GLuint& VBO);