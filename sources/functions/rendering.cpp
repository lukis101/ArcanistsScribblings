
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "types.h"
#include "globals.h"
#include "resources.h"
#include "glsl.h"
#include "functions/rendering.h"

void RenderTexture( unsigned tex_id, Vector_f pos, unsigned* buffer, float scale, short rendermode, bool flipped )
{
    glEnableVertexAttribArray( sh_v_uv );

    int tex_w = 0;
    int tex_h = 0;

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_w );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_h );

    glUniform1i(sh_mode, rendermode);
    glUniform1ui(sh_texture, tex_id);
    glUniform2f(sh_texsize, tex_w, tex_h);

    float x1 = pos.x;
    float x2 = pos.x+tex_w*scale;
    float y1 = pos.y;
    float y2 = pos.y+tex_h*scale;
    float u1 = 0.f;
    float u2 = 1.f;
    float v1 = 0.f;
    float v2 = 1.f;
    if (flipped)
    {
        float temp = v1;
        v1 = v2;
        v2 = temp;
    }

    float data[] =
    {
        // Vertex Pos
        x1, y1, 1.f,
        x2, y1, 1.f,
        x2, y2, 1.f,
        x1, y2, 1.f,
        // Texture UV
        u1, v1,
        u2, v1,
        u2, v2,
        u1, v2,
    };

    unsigned tempbuff = 0;
    unsigned* buffptr;
    if ( buffer == NULL ) buffptr = &tempbuff;
    else buffptr = buffer;

    if ( *buffptr == 0 )
    {
        glGenBuffers( 1, buffptr );
        glBindBuffer( GL_ARRAY_BUFFER, *buffptr);
        glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW );
    }
    else
    {
        glBindBuffer( GL_ARRAY_BUFFER, *buffptr);
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(data), &data );
    }
    glVertexAttribPointer( sh_v_pos, 3, GL_FLOAT, 0, 0, 0 );
    glVertexAttribPointer( sh_v_uv, 2, GL_FLOAT, 0, 0, (GLvoid*)(sizeof(float)*12) );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDeleteBuffers( 1, &tempbuff );

    glDisableVertexAttribArray( sh_v_uv );
}

void RenderMaterial( unsigned matid, short frameid, Vector_f offset, float scale, float rotation, Vector_f uvoffset )
{
    glEnableVertexAttribArray( sh_v_uv );

    CMaterial material = Materials->at(matid);
    int texid = material.GetTexID();
    unsigned* buffer = material.GetBuffer();
    int filter = material.GetFilter();
    bool animated = material.IsAnimated();
    bool mirrored = material.IsMirrored();
    //Color_f color = material.GetColor();
    Frame frame;
    if (frameid == 0) frame = material.GetFrame(material.GetBaseFrame());
    else frame = material.GetFrame(frameid);

    int rendermode = 0;
    if ( texid>=MAT_BACKGROUND_GRASSLAND && texid<=MAT_BACKGROUND_CRYSTALS ) rendermode=2;

    glBindTexture( GL_TEXTURE_2D, texid );

    glUniform1i(sh_mode, rendermode);
    glUniform1ui(sh_texture, texid);
    glUniform2f(sh_texsize, frame.image.w, frame.image.h);

    float x1 = offset.x+frame.image.x;
    float x2 = offset.x+frame.image.x+frame.image.w*scale;
    float y1 = offset.y+frame.image.y;
    float y2 = offset.y+frame.image.y+frame.image.h*scale;

    float u1 = frame.uv.x;
    float u2 = frame.uv.x+frame.uv.w;
    float v1 = frame.uv.y;
    float v2 = frame.uv.y+frame.uv.h;
    if (mirrored)
    {
        float temp = u1;
        u1 = u2;
        u2 = temp;
    }
    float data[] =
    {
        // Vertex Pos
        x1, y1, 1.f,
        x2, y1, 1.f,
        x2, y2, 1.f,
        x1, y2, 1.f,
        // Texture UV
        u1, v1,
        u2, v1,
        u2, v2,
        u1, v2,
    };

    if ( buffer != NULL )
    {
        if ( *buffer == 0 )
        {
            unsigned bufftype = GL_STATIC_DRAW;
            if (animated) bufftype = GL_DYNAMIC_DRAW;
            glGenBuffers( 1, buffer );
            glBindBuffer( GL_ARRAY_BUFFER, *buffer);
            glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, bufftype );
        }
        else
        {
            glBindBuffer( GL_ARRAY_BUFFER, *buffer);
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(data), &data );
        }
        glVertexAttribPointer( sh_v_pos, 3, GL_FLOAT, 0, 0, 0 );
        glVertexAttribPointer( sh_v_uv, 2, GL_FLOAT, 0, 0, (GLvoid*)(sizeof(float)*12) );

        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    }
    glDisableVertexAttribArray( sh_v_uv );
}

void RenderColor( Rect_f rect, unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
    float data[] =
    {
        rect.x,        rect.y,        1.f,
        rect.x+rect.w, rect.y,        1.f,
        rect.x+rect.w, rect.y+rect.h, 1.f,
        rect.x,        rect.y+rect.h, 1.f,
    };
    glUniform1i(sh_mode, 4);
    glUniform4f( sh_color1, r/255.f, g/255.f, b/255.f, a/255.f );

    unsigned tempbuff;
    glGenBuffers( 1, &tempbuff );
    glBindBuffer( GL_ARRAY_BUFFER, tempbuff );
    glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW );
    glVertexAttribPointer( sh_v_pos, 3, GL_FLOAT, 0, 0, 0 );

    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDeleteBuffers( 1, &tempbuff );
}

void RenderGradient( Rect_f rect, Color_c ColA, Color_c ColB, unsigned type )
{
    unsigned tempbuff;
    glGenBuffers( 1, &tempbuff );
    glBindBuffer( GL_ARRAY_BUFFER, tempbuff );

    if ( type != GRADIENT_RADIAL ) // Simple gradients
    {
        glEnableVertexAttribArray( sh_v_color );

        Color_c A, B, C, D;
        typedef unsigned char uchar;
        switch (type)
        {
            case GRADIENT_VERTICAL:
                A=ColB; B=ColB; C=ColA; D=ColA; break;
            case GRADIENT_HORIZONTAL:
                A=ColA; B=ColB; C=ColB; D=ColA; break;
            case GRADIENT_DIAGONAL_UP:
                A={ uchar((ColA.r+ColB.r)/2), uchar((ColA.g+ColB.g)/2), uchar((ColA.b+ColB.b)/2), uchar((ColA.a+ColB.a)/2) };
                B=ColB; C=A; D=ColA; break;
            case GRADIENT_DIAGONAL_DOWN:
                B={ uchar((ColA.r+ColB.r)/2), uchar((ColA.g+ColB.g)/2), uchar((ColA.b+ColB.b)/2), uchar((ColA.a+ColB.a)/2) };
                A=ColA; C=ColB; D=B; break;
        }
        float data[] =
        {
            rect.x,        rect.y,        1.f,
            rect.x+rect.w, rect.y,        1.f,
            rect.x+rect.w, rect.y+rect.h, 1.f,
            rect.x,        rect.y+rect.h, 1.f,
            A.r/255.f, A.g/255.f, A.b/255.f, A.a/255.f,
            B.r/255.f, B.g/255.f, B.b/255.f, B.a/255.f,
            C.r/255.f, C.g/255.f, C.b/255.f, C.a/255.f,
            D.r/255.f, D.g/255.f, D.b/255.f, D.a/255.f,
        };
        glUniform1i( sh_mode, 5 );
        glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW );
        glVertexAttribPointer( sh_v_color, 4, GL_FLOAT, 0, 0, (GLvoid*)(sizeof(float)*12) );
    }
    else // Radial gradient
    {
        glEnableVertexAttribArray( sh_v_uv );

        float data[] =
        {
            rect.x,        rect.y,        1.f,
            rect.x+rect.w, rect.y,        1.f,
            rect.x+rect.w, rect.y+rect.h, 1.f,
            rect.x,        rect.y+rect.h, 1.f,
            0.f, 0.f,
            1.f, 0.f,
            1.f, 1.f,
            0.f, 1.f,
        };
        glUniform1i( sh_mode, 7 );
        glUniform2f( sh_coords1, 0.5f, 0.5f );
        glUniform4f( sh_color1, float(ColA.r)/255.f, float(ColA.g)/255.f, float(ColA.b)/255.f, float(ColA.a)/255.f );
        glUniform4f( sh_color2, float(ColB.r)/255.f, float(ColB.g)/255.f, float(ColB.b)/255.f, float(ColB.a)/255.f );
        glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW );
        glVertexAttribPointer( sh_v_uv, 2, GL_FLOAT, 0, 0, (GLvoid*)(sizeof(float)*12) );
    }
    glVertexAttribPointer( sh_v_pos, 3, GL_FLOAT, 0, 0, 0 );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    glDeleteBuffers( 1, &tempbuff );

    glDisableVertexAttribArray( sh_v_color );
    glDisableVertexAttribArray( sh_v_uv );
}

