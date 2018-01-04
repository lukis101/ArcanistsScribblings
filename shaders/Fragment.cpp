
#version 100

#ifdef GL_ES
precision mediump float;
#endif

vec4 color0 = vec4( 255.0/255.0, 204.0/255.0, 153.0/255.0, 1.0 ); // Skin
vec4 color1 = vec4( 000.0/255.0, 102.0/255.0, 204.0/255.0, 1.0 ); // Primary
vec4 color2 = vec4( 000.0/255.0, 153.0/255.0, 254.0/255.0, 1.0 ); // Secondary
vec4 color3 = vec4( 000.0/255.0, 102.0/255.0, 153.0/255.0, 1.0 ); // Hair

// Uniforms
uniform int U_Mode;
uniform sampler2D U_TexID;
//uniform sampler2D U_MaskId;
uniform vec2 U_TexSize;
//uniform vec2 U_Coords1;
//uniform vec2 U_Coords2;
uniform vec4 U_Color1; // Skin      || Grad-1 || Solid
uniform vec4 U_Color2; // Primary   || Grad-2
uniform vec4 U_Color3; // Secondary
uniform vec4 U_Color4; // Hair

// Vars from vertex shader
varying vec4 Vert_Pos;
varying vec2 Vert_TexPos;
varying vec4 Vert_Color;

#define Blendf(base, blend, scale) (scale * blend + (1.0 - scale) * base)
#define Screenf(base, blend)       (1.0 - ((1.0 - base) * (1.0 - blend)))
#define Overlayf(base, blend)      (base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))

vec4 Overlay( vec4 Target, vec4 Blend )
{
    return vec4( Overlayf(Target.r,Blend.r), Overlayf(Target.g,Blend.g), Overlayf(Target.b,Blend.b), Target.a );
}
float Invert( float Target, float Amount )
{
	return Target*(1.0-Amount);
}


void main()
{
	vec4 pixel;
	float eps = 0.01;

	if ( U_Mode == 0 ) /// Normal render
	{
		gl_FragColor = texture2D( U_TexID, Vert_TexPos );
		return;
	}
	if ( U_Mode == 1 ) /// Recoloring
	{
	    pixel = texture2D( U_TexID, Vert_TexPos );
        /// Greystyle
        if( all( bvec3( abs(pixel.r-pixel.g)<eps, abs(pixel.r-pixel.b)<eps, pixel.a>0.0 ) ) )
        {
            gl_FragColor = Overlay( pixel, U_Color1 );
            return;
		}
        /// Primary - Red
        if( all( bvec4( pixel.r>0.0, pixel.g==0.0, pixel.b==0.0, pixel.a>0.0 ) ) )
        {
            gl_FragColor = vec4( U_Color2.r*pixel.r, U_Color2.g*pixel.r, U_Color2.b*pixel.r, pixel.a );
            return;
        }
        /// Secondary - Green
        if( all( bvec4( pixel.r==0.0, pixel.g>0.0, pixel.b==0.0, pixel.a>0.0 ) ) )
        {
            gl_FragColor = vec4( U_Color3.r*pixel.g, U_Color3.g*pixel.g, U_Color3.b*pixel.g, pixel.a );
            return;
        }
        /// Blue
        if( all( bvec4( pixel.r==0.0, pixel.g==0.0, pixel.b>0.0, pixel.a>0.0 ) ) )
        {
            gl_FragColor = vec4( U_Color4.r*pixel.b, U_Color4.g*pixel.b, U_Color4.b*pixel.b, pixel.a );
            return;
        }
		gl_FragColor = pixel;
		return;
	}
	if ( U_Mode == 2 ) /// Horizontal Blur
	{
        float blurSize = (1.0/U_TexSize.x)*0.8;
        vec4 sum = vec4(0.0);
        vec2 Texpos = Vert_TexPos;

        sum += texture2D(U_TexID, vec2(Texpos.x - 4.0*blurSize, Texpos.y)) * 0.06;
        sum += texture2D(U_TexID, vec2(Texpos.x - 3.0*blurSize, Texpos.y)) * 0.09;
        sum += texture2D(U_TexID, vec2(Texpos.x - 2.0*blurSize, Texpos.y)) * 0.12;
        sum += texture2D(U_TexID, vec2(Texpos.x - blurSize, Texpos.y)) * 0.15;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y)) * 0.16;
        sum += texture2D(U_TexID, vec2(Texpos.x + blurSize, Texpos.y)) * 0.15;
        sum += texture2D(U_TexID, vec2(Texpos.x + 2.0*blurSize, Texpos.y)) * 0.12;
        sum += texture2D(U_TexID, vec2(Texpos.x + 3.0*blurSize, Texpos.y)) * 0.09;
        sum += texture2D(U_TexID, vec2(Texpos.x + 4.0*blurSize, Texpos.y)) * 0.06;

        gl_FragColor = sum;
		return;
	}
	if ( U_Mode == 3 ) /// Vertical Blur
	{
        float blurSize = (1.0/U_TexSize.y)*1.4;
        vec2 Texpos = Vert_TexPos;
        vec4 sum = vec4(0.0);

        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y - 4.0*blurSize)) * 0.06;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y - 3.0*blurSize)) * 0.09;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y - 2.0*blurSize)) * 0.12;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y - blurSize)) * 0.15;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y)) * 0.16;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y + blurSize)) * 0.15;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y + 2.0*blurSize)) * 0.12;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y + 3.0*blurSize)) * 0.09;
        sum += texture2D(U_TexID, vec2(Texpos.x, Texpos.y + 4.0*blurSize)) * 0.06;

        gl_FragColor = sum;
		return;
	}
	if ( U_Mode == 4 ) /// Solid color
	{
        gl_FragColor = U_Color1;
		return;
	}
	if ( U_Mode == 5 ) /// Simple gradient
	{
        gl_FragColor = Vert_Color;
		return;
	}
	if ( U_Mode == 7 ) /// Radial gradient
	{
	    vec2 center = vec2( 0.5, 0.5 );
        gl_FragColor = vec4( 0.0, 0.0, 0.0, 0.0);
        vec4 col1 = U_Color1;
        vec4 col2 = U_Color2;

//        // White borders
//        col1 = vec4( 255/255.0, 255/255.0, 255/255.0, -1.0 );
//        col2 = vec4( 255/255.0, 255/255.0, 255/255.0, 1.0 );
//        // Red smooth fill
//        col1 = vec4( 255/255.0, 200/255.0, 200/255.0, 0.8 );
//        col2 = vec4( 200/255.0, 70/255.0, 70/255.0, 0.8 );
//        // Green smooth fill
//        col1 = vec4( 200/255.0, 255/255.0, 200/255.0, 0.85 );
//        col2 = vec4( 70/255.0, 200/255.0, 70/255.0, 0.85 );

        float x1 = Vert_TexPos.x;
        float y1 = Vert_TexPos.y;
        float distance = distance( vec2( x1, y1 ), center );
	    if ( distance < 0.5 )
	    {
            float element = distance*2.0;
            gl_FragColor = vec4( Blendf(col1.r,col2.r,element), Blendf(col1.g,col2.g,element), Blendf(col1.b,col2.b,element), Blendf(col1.a,col2.a,element) );
	    }
		return;
	}

    /// Unknown rendering mode
    gl_FragColor = texture2D( U_TexID, Vert_TexPos );
}
