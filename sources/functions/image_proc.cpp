
#include "SDL/SDL.h"
#include <GL/glew.h>

#include "lodepng.h"

#include "resources.h"
#include "functions/image_proc.h"

unsigned char component_blend( unsigned char base, unsigned char blend, float scale )
{
    return scale * blend + (1 - scale) * base;
}

bool LoadImage( std::string filename, unsigned GLTexID, unsigned TexID )
{
    if ( filename == "" ) return false;

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename.c_str());

    if ( !error )
    {
        glBindTexture( GL_TEXTURE_2D, GLTexID );

        unsigned Mode1 = GL_RGBA;
        unsigned Mode2 = GL_RGBA;
        unsigned filter = GL_NEAREST;

        // Different properties for background + gradient overlay
        if ( TexID>=MAT_BACKGROUND_GRASSLAND && TexID<=MAT_BACKGROUND_CRYSTALS )
        {
            filter = GL_LINEAR;

            std::string filename2 = filename;
            filename2.erase(filename2.end()-14,filename2.end());
            filename2 += "colors.png";

            std::vector<unsigned char> image2;
            unsigned char keyr = 255;
            unsigned char keyg = 0;
            unsigned char keyb = 255;
            unsigned char r1 = 255;
            unsigned char g1 = 255;
            unsigned char b1 = 255;
            unsigned char r2 = r1;
            unsigned char g2 = g1;
            unsigned char b2 = b1;
            unsigned width2, height2;
            unsigned error2 = lodepng::decode(image2, width2, height2, filename2.c_str());
            if ( !error2 )
            {
                r1 = image2[0];
                g1 = image2[1];
                b1 = image2[2];
                r2 = image2[4];
                g2 = image2[5];
                b2 = image2[6];
                for ( unsigned y=0; y<height; y++ )
                {
                    for ( unsigned x=0; x<width*4; x+=4 )
                    {
                        unsigned index = y*width*4+x;
                        if ( (image[index]==keyr) && (image[index+1]==keyg) && (image[index+2]==keyb) )
                        {
                            float scale = (float)y/height;
                            image[index]   = component_blend(r2,r1,scale);
                            image[index+1] = component_blend(g2,g1,scale);
                            image[index+2] = component_blend(b2,b1,scale);
                        }
                    }
                }
            }
            else
            {
                //std::cout<<"Image loading error: "<<filename<<std::endl;
                //std::cout<<lodepng_error_text(error2)<<std::endl;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, Mode1, width, height, 0, Mode2, GL_UNSIGNED_BYTE, &image[0]);

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

        glBindTexture( GL_TEXTURE_2D, 0 );

        return true;
    }
    else
    {
        std::cout<<"\nError loading image: "<<filename<<"\n";
        std::cout<<lodepng_error_text(error)<<"\n";
        return false;
    }

}
void apply_surface( SDL_Surface* source, SDL_Surface* destination, int x, int y )
{
    std::cout<<"apply_surface called!\n";
    //SDL_BlitSurface( source, NULL, destination, &offset );
}

