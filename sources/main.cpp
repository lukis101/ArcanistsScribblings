
#include "main.h"

/// /////////////////////////////
/// TO-DELETE
const int MOVE_SPEED      = 100;
const int MAX_FALL_SPEED  = 80;
const int MAX_RISE_SPEED  = 80;
const int COLLISION_Y     = 5;

const float JUMP_VELOCITY = 10.f;
const int DOT_WIDTH       = 16;
const int DOT_HEIGHT      = 16;
/// //////////////////////////////

int exitcode = 0;
bool should_exit = false;
bool fps_cap = true; // true : false
bool colliding = false;
bool folowing = false;

    bool setup = true;
    bool should_paint = false;
    char reason=0;
    short frame = 0;
    float delta = 0.f;
    int fps = 0;

/// GENERAL FUNCTIONS
std::string to_string( int number )
{
    char numstr[21]; // enough to hold all numbers up to 64-bits
    sprintf(numstr, "%d", number);
    return numstr;
}
/// TO-DO
//std::string to_string( double number )
//{
//    char numstr[21];
//    sprintf(numstr, "%d", number);
//    return numstr;
//}

bool GLError()
{
    GLenum error = glGetError();
    if( error == GL_NO_ERROR )
        return false;
    else
    {
        std::cout<<"\nGLError #"<<error<<" :\n";
        std::cout<<gluErrorString(error)<<"\n";
        return true;
    }
}

bool initGL();
bool initGLSL();
bool init()
{
    /// Main SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;

    /// Screen surface
    screen = SDL_SetVideoMode( SCREEN_W, SCREEN_H, SCREEN_BPP, SCREEN_FLAGS );
    if( screen == NULL ) return false;

    /// SDL OpenGL
    if( !initGL() )
    {
        std::cout<<"Unable to initialise OpenGL!\n";
        return false;
    }
    if( !initGLSL() )
    {
        std::cout<<"Unable to initialise shader programs!\n";
        return false;
    }

    /// Input
    for (int i=0;i<KEY_AMOUNT;i++)
    {
        Keyboard[i].state = false;
    }
    Mouse.WheelUp = false;
    Mouse.WheelDown = false;
    Mouse.x = 0;
    Mouse.y = 0;

    return true;
}
bool precache()
{
    std::cout<<"Precaching...\n";
    PrecacheTextures();
    SetupMaterials();
    //PrecacheSounds();

    if ( GLError() ) return false;
    return true;
}
bool decache()
{
    DecacheTextures();
}
void clean_up()
{
    decache();
    SDL_Quit();
}
//////////////////////////////////////////////////////////////////////////////////////////////////

GLuint gVBO = 0;
GLuint gIBO = 0;
GLuint vao = 0;

GLuint gVertexBuffer = 0;
GLuint gIndexBuffer = 0;
GLuint gTextureBuffer = 1;

/// OpenGL
bool initGL()
{
    std::cout<<"Initialising OpenGL...\n";
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        std::cout<<"\nGLEW Error #"<<glewError<<" :\n";
        std::cout<<glewGetErrorString( glewError )<<"\n";
        return false;
    }
    if( !GLEW_VERSION_2_0 )
    {
        std::cout<<"GLEW: OpenGL 2.0 not supported!\n";
        return false;
    }
    if( !GLEW_VERSION_2_1 )
    {
        std::cout<<"GLEW: OpenGL 2.1 not supported!\n";
        return false;
    }
    else if( !GLEW_VERSION_3_0 )
    {
        std::cout<<"GLEW: OpenGL 3.0 not supported!\n";
        //return false;
    }
    else if( !GLEW_VERSION_3_3 )
    {
        std::cout<<"GLEW: OpenGL 3.3 not supported!\n";
        //return false;
    }
    else if( !GLEW_VERSION_4_0 )
    {
        std::cout<<"GLEW: OpenGL 4.0 not supported!\n";
        //return false;
    }
//    if( !GLEW_ARB_separate_shader_objects )
//    {
//        std::cout<<"GLEW: X extention not supported!\n";
//        //return false;
//    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    //glDepthFunc(GL_NEVER);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ZERO ); // inverse

    glClearColor( 0.f, 0.f, 0.f, 1.f );

    if ( GLError() ) return false;
    return true;
}
// Helper func, used in initGLSL()
void PrepareUniform( const GLuint program, GLuint* uvariable, const char* uname )
{
    *uvariable = glGetUniformLocation( program, uname );
	if ( *uvariable == -1)
        std::cout<<"Uniform \""<<uname<<"\" was not linked!\n";
    return;
}
bool initGLSL()
{
    std::cout<<"Initialising GLSL...\n";
    GLuint vertexId, fragmentId;

    vertexId = CreateShader( GL_VERTEX_SHADER, "shaders/Vertex.cpp" );
    fragmentId = CreateShader( GL_FRAGMENT_SHADER, "shaders/Fragment.cpp" );

    sh_program = glCreateProgram();
    glAttachShader( sh_program, vertexId );
    glAttachShader( sh_program, fragmentId );
    glLinkProgram( sh_program );
    glUseProgram( sh_program );

    PrepareUniform( sh_program, &sh_projmat, "U_Projection" );
    //PrepareUniform( sh_program, &sh_transmat, "U_Transform" );

    PrepareUniform( sh_program, &sh_mode,    "U_Mode" );
    PrepareUniform( sh_program, &sh_texture, "U_TexID" );
    //PrepareUniform( sh_program, &sh_mask,    "U_MaskId" );
    PrepareUniform( sh_program, &sh_texsize, "U_TexSize" );
    //PrepareUniform( sh_program, &sh_coords1, "U_Coords1" );
    //PrepareUniform( sh_program, &sh_coords2, "U_Coords2" );
    PrepareUniform( sh_program, &sh_color1,  "U_Color1" );
    PrepareUniform( sh_program, &sh_color2,  "U_Color2" );
    PrepareUniform( sh_program, &sh_color3,  "U_Color3" );
    PrepareUniform( sh_program, &sh_color4,  "U_Color4" );

    sh_v_pos   = glGetAttribLocation(sh_program,"VertPos");
    sh_v_color = glGetAttribLocation(sh_program,"VertColor");
    sh_v_uv    = glGetAttribLocation(sh_program,"TexturePos");

    if ( GLError() ) return false;

    return true;
}

void Handle_Input()
{
    while( SDL_PollEvent( &event ) )
    {
        if( event.type == SDL_QUIT )
        {
            should_exit = true;
            exitcode = 1;
            break;
        }
        else if( event.type == SDL_MOUSEBUTTONDOWN )
        {
            switch( event.button.button )
            {
                case SDL_BUTTON_LEFT:
                    Mouse.left.state = true;
                    Mouse.left.x = event.button.x;
                    Mouse.left.y = event.button.y;
                    break;
                case SDL_BUTTON_RIGHT:
                    Mouse.right.state  = true;
                    Mouse.right.x = event.button.x;
                    Mouse.right.y = event.button.y;
                    break;
                case SDL_BUTTON_MIDDLE:
                    Mouse.middle.state = true;
                    Mouse.middle.x = event.button.x;
                    Mouse.middle.y = event.button.y;
                    break;
                case SDL_BUTTON_WHEELUP:
                    Mouse.WheelUp = true;
                    break;
                case SDL_BUTTON_WHEELDOWN:
                    Mouse.WheelDown = true;
                    break;
            }
            break;
        }
        else if( event.type == SDL_MOUSEBUTTONUP )
        {
            switch( event.button.button )
            {
                case SDL_BUTTON_LEFT:
                    Mouse.left.state = false;
                    Mouse.left.x = event.button.x;
                    Mouse.left.y = event.button.y;
                    break;
                case SDL_BUTTON_RIGHT:
                    Mouse.right.state = false;
                    Mouse.right.x = event.button.x;
                    Mouse.right.y = event.button.y;
                    break;
                case SDL_BUTTON_MIDDLE:
                    Mouse.middle.state = false;
                    Mouse.middle.x = event.button.x;
                    Mouse.middle.y = event.button.y;
                    break;
            }
            break;
        }
        else if( event.type == SDL_KEYDOWN )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_BACKQUOTE: Keyboard[KEY_CONSOLE].state = true; break;
                case SDLK_KP_ENTER:  Keyboard[KEY_ENTER  ].state = true; break;
                case SDLK_ESCAPE: Keyboard[KEY_ESC  ].state = true; break;
                case SDLK_RETURN: Keyboard[KEY_ENTER].state = true; break;
                case SDLK_SPACE:  Keyboard[KEY_SPACE].state = true; break;
                case SDLK_LCTRL:  Keyboard[KEY_CTRL ].state = true; break;
                case SDLK_RCTRL:  Keyboard[KEY_CTRL ].state = true; break;
                case SDLK_LSHIFT: Keyboard[KEY_SHIFT].state = true; break;
                case SDLK_RSHIFT: Keyboard[KEY_SHIFT].state = true; break;
                //case SDLK_LALT: Keyboard[KEY_ALT].state = true; break;
                //case SDLK_RALT: Keyboard[KEY_ALT].state = true; break;
                case SDLK_w: Keyboard[KEY_W].state = true; break;
                case SDLK_s: Keyboard[KEY_S].state = true; break;
                case SDLK_a: Keyboard[KEY_A].state = true; break;
                case SDLK_d: Keyboard[KEY_D].state = true; break;
                case SDLK_UP   : Keyboard[KEY_ARROW_UP   ].state = true; break;
                case SDLK_DOWN : Keyboard[KEY_ARROW_DOWN ].state = true; break;
                case SDLK_LEFT : Keyboard[KEY_ARROW_LEFT ].state = true; break;
                case SDLK_RIGHT: Keyboard[KEY_ARROW_RIGHT].state = true; break;
            }
            break;
        }
        else if( event.type == SDL_KEYUP )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_BACKQUOTE: Keyboard[KEY_CONSOLE].state = false; break;
                case SDLK_KP_ENTER:  Keyboard[KEY_ENTER  ].state = false; break;
                case SDLK_ESCAPE: Keyboard[KEY_ESC  ].state = false; break;
                case SDLK_RETURN: Keyboard[KEY_ENTER].state = false; break;
                case SDLK_SPACE:  Keyboard[KEY_SPACE].state = false; break;
                case SDLK_LCTRL:  Keyboard[KEY_CTRL ].state = false; break;
                case SDLK_RCTRL:  Keyboard[KEY_CTRL ].state = false; break;
                case SDLK_LSHIFT: Keyboard[KEY_SHIFT].state = false; break;
                case SDLK_RSHIFT: Keyboard[KEY_SHIFT].state = false; break;
                //case SDLK_LALT: Keyboard[KEY_ALT].state = false; break;
                //case SDLK_RALT: Keyboard[KEY_ALT].state = false; break;
                case SDLK_w: Keyboard[KEY_W].state = false; break;
                case SDLK_s: Keyboard[KEY_S].state = false; break;
                case SDLK_a: Keyboard[KEY_A].state = false; break;
                case SDLK_d: Keyboard[KEY_D].state = false; break;
                case SDLK_UP   : Keyboard[KEY_ARROW_UP   ].state = false; break;
                case SDLK_DOWN : Keyboard[KEY_ARROW_DOWN ].state = false; break;
                case SDLK_LEFT : Keyboard[KEY_ARROW_LEFT ].state = false; break;
                case SDLK_RIGHT: Keyboard[KEY_ARROW_RIGHT].state = false; break;
            }
            break;
        }
    }
    // Current cursor position
    SDL_GetMouseState(&(Mouse.x),&(Mouse.y));
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////
void MainLoop ();

/// ----MAIN---- ///
int main( int argc, char* args[] )
{
    char dummy;
    bool inited = true;
    if( init() == false )
    {
        std::cout<<"Initialising failed!";
        std::cin>>dummy;
        //SDL_Delay(2000);
        inited = false;
        exitcode = 424;
    }
    if( precache() == false )
    {
        std::cout<<"Precaching failed!";
        std::cin>>dummy;
        //SDL_Delay(2000);
        inited = false;
        exitcode = 404;
    }

    if (inited)
    {
        std::cout<<"Everything OK, running the game.\n";
        //Set the current state ID
        stateID = STATE_GAME; // STATE_TITLE;
        nextState = STATE_GAME;
        //Set the current game state object
        currentState = new Game(); // Title();

        GlobalTimer->Start();
        FPSTimer->Start();
        LogicTimer->Start();

        #ifdef __emscripten_h__
            emscripten_set_main_loop(MainLoop, 200, 1);
        #else
            MainLoop();
        #endif
    }

    /// Clean sh** up before quiting
    delete GlobalTimer;
    delete FPSTimer;
    delete LogicTimer;
    //delete currentState; // TO-DO: fix segfault
    clean_up();

    // 0   = X button
    // 1   = other legit exit
    // 404 = missing resources
    // 424 = initialising failed
    return exitcode;
}

/// ////////////// MAIN LOOP /////////////// ///
void MainLoop ()
{
    #ifdef __emscripten_h__
        if( !should_exit )
    #else
        while( !should_exit )
    #endif
    {

    /// -------- INPUTS -------- ///
        Handle_Input();
        if ( Keyboard[KEY_ESC].state )
        {
            should_exit = true;
            exitcode = 1;
        }
    /// -------- MAIN -------- ///

        Handle_States();
        currentState->logic();
        LogicTimer->Start();
        currentState->render();

    /// -------- Monitoring info -------- ///
        if( GlobalTimer->GetTicks()%4 == 0 )
        {
            std::stringstream caption;
            //caption <<"Arcanists Pre-Alpha   ";
            caption<<"Cursor("<<currentState->GetCursorPos().x<<"; "<<currentState->GetCursorPos().y<<")";
            caption<<"Mouse("<<Mouse.x<<"; "<<Mouse.y<<")";
            caption<<"   Camera("<<currentState->camera->pos.x<<"; "<<currentState->camera->pos.y<<"; "<<currentState->camera->pos.z<<")";
            caption<<" Focus("<<currentState->camera->GetFocus().x<<"; "<<currentState->camera->GetFocus().y<<")";
            caption<<"   "<<std::fixed<<std::setprecision(2)<< delta << " ms/frame";
            SDL_WM_SetCaption( caption.str().c_str(), NULL );
        }
/// ///////////////////////////////////////////// ///

    /// -------- FPS Stuff -------- ///
        FPSTimer->Pause();

        /// Calculations (1)
        frame++;
        if (frame >= 20)
        {
            delta = float(FPSTimer->GetTicks())/20.f;
            if (delta == 0.f) delta = 0.05f;
            fps = 1000.f/delta;
        }

        /// CAP
        #ifndef __emscripten_h__
            if (fps_cap)
            {
                float desireddelay = (1000.f/float(FPS_MAX));
                int delay = desireddelay-delta;
                if( delta < desireddelay )
                    SDL_Delay( delay );
                else std::cout<<"Too slow!\n";
            }
        #endif

        FPSTimer->Unpause();

        /// Calculations (2)
        if (frame >= 20)
        {
            frame = 0;
            FPSTimer->Start();
        }
    }
}
