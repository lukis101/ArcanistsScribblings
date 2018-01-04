
#version 100

uniform mat4 U_Projection;
//uniform mat4 U_Transform;

// Vars from program
attribute vec3 VertPos;
attribute vec4 VertColor;
attribute vec2 TexturePos;

// Vars for fragment shader
varying vec4 Vert_Pos;
varying vec2 Vert_TexPos;
varying vec4 Vert_Color;

void main()
{
    Vert_Color = VertColor;
    Vert_TexPos = TexturePos;
    //Vert_Pos = U_Transform * vec4( VertPos.x/800.0, VertPos.y/600.0, VertPos.z, 1.0 );
	gl_Position = U_Projection * vec4( VertPos.x, VertPos.y, VertPos.z, 1.0 );
}
