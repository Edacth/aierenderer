#pragma once
#include "render.h"

#include <string>

#include "tinyobjloader/tiny_obj_loader.h"

std::string fileLoadShader(std::string filepath);

std::vector<tinyobj::shape_t> fileLoadObj(std::string filepath, tinyobj::attrib_t &attrib);
geometry fileLoadObj(const char * filename);