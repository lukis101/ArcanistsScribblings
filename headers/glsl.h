#ifndef GLSL_H
#define GLSL_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

extern unsigned sh_program;

extern unsigned sh_projmat;
extern unsigned sh_transmat;

extern unsigned sh_mode;
extern unsigned sh_texture;
extern unsigned sh_mask;
extern unsigned sh_texsize;
extern unsigned sh_coords1;
extern unsigned sh_coords2;
extern unsigned sh_color1;
extern unsigned sh_color2;
extern unsigned sh_color3;
extern unsigned sh_color4;

extern unsigned sh_v_pos;
extern unsigned sh_v_uv;
extern unsigned sh_v_color;

extern glm::mat4 ProjMat;

std::string ReadFromFile( std::string filename );
unsigned CreateShader( int type, std::string filename );


#endif
