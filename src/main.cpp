
/*******************************************************************************************
*
*   MPD218 Interface
*
*   This program has been created using raylib 3.8 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   program authored by samieb (@samybencherif)
*
*   Copyright (c) 2021 samieb (@samybencherif)
*
*
*
*   ---------------------------------------
*
*   Use this program with Audacity and Virtual Audio Cable (https://vac.muzychenko.net/en/)  !
*
*
*
*
********************************************************************************************/

#include "raylib.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <signal.h>
#include "RtMidi.h"

// Platform-dependent sleep routines.
#if defined(WIN32)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

/* theme = OCEANIC */
//const Color BG = DARKBLUE;
//const Color FG = SKYBLUE;

/* theme = PEN&PAPER */
//const Color BG = WHITE;
//const Color FG = BLACK;

/* theme = BUMBLINGNIGHTS */
//const Color BG = BLACK;
//const Color FG = YELLOW;

/* theme = MIDNIGHT */
const Color BG = BLACK;
const Color FG = SKYBLUE;

/* theme = DEVILISH */
//const Color BG = BLACK;
//const Color FG = RED;



class Action {
  public: 
    bool defined;
    char* filename;
    Sound* sound;
    
  Action() {
    defined = false;
  }
};

std::vector<Action> pads;

void stopAllSounds()
{
  for (int i=0; i<16*3; i++)
  {
    if (pads[i].defined)
      StopSound(*pads[i].sound);
  }
}

/* ____ ____ ____ ____          ____ ____ ____ ____ 
  |    |    |    |    |        |    |    |    |    |
  | 12 | 13 | 14 | 15 |        | 1  | 2  | 3  | 4  |
  |____|____|____|____|        |____|____|____|____|
  |    |    |    |    |        |    |    |    |    |
  | 08 | 09 | 10 | 11 |        | Q  | W  | E  | R  |
  |____|____|____|____|  <-->  |____|____|____|____|
  |    |    |    |    |  <-->  |    |    |    |    |
  | 04 | 05 | 06 | 07 |        | A  | S  | D  | F  |
  |____|____|____|____|        |____|____|____|____|
  |    |    |    |    |        |    |    |    |    |
  | 00 | 01 | 02 | 03 |        | Z  | X  | C  | V  |
  |____|____|____|____|        |____|____|____|____|
*/
char keymap[17] = "ZXCVASDFQWER1234";

bool isKeymapDown() {
  for (int i=0; i<16; i++)
  {
    if (IsKeyDown(keymap[i])) return true;
  }
  return false;
}

bool isKeymapPressed() {
  for (int i=0; i<16; i++)
  {
    if (IsKeyPressed(keymap[i])) return true;
  }
  return false;
}

int getKeymapIndex() {
  for (int i=0; i<16; i++)
  {
    if (IsKeyDown(keymap[i])) return i;
  }
  return -1;
}


int main(void)
{
    // Initialization
    //------------------------------------------------------------------------
    const int screenWidth = 600;
    const int screenHeight = 470;
    
    // constant for signifying unset integer (index)
    const int None = -1;

    // initialize pads
    for (int i=0; i<16*3; i++)
      pads.push_back(*(new Action()));
    
    // High speed communication with midi device
    SetTargetFPS(60);
    
    InitWindow(screenWidth, screenHeight, "Music Pad Interface by Samie B");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    InitAudioDevice();

    SetExitKey(0);

    bool chooseASample = false;
    int activePad = None;
    
    // number of audio samples in user library "audio" folder
    int files_count;
    
    // this part hopefully loads all the samples into memory correctly
    char** files = GetDirectoryFiles("audio", &files_count);
    Sound* sounds = (Sound*) malloc((files_count-2) * sizeof(Sound));
    for (int i=2; i<files_count; i++)
    {
      // use string stream to prepend relative path "audio/" to filenames
      std::stringstream ss;
      ss << "audio/" << files[i];
      std::string filepath = ss.str();
      
      // convert C++ string to C char* for use with Raylib
      sounds[i-2] = LoadSound(filepath.c_str());
    }

    //----------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //------------------------------------------------------------------
        // TODO: Update variables / Implement example logic at this point
        
        //-------------------------------------------------------------------
        // Draw
        //-------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BG);

            // visual representation
            for (int x=0; x<4; x++)
              for (int y=0; y<4; y++)
              {
                if (isKeymapDown() && (getKeymapIndex())%16 == 4*y+x)
                  DrawRectangle(20+110*x,20+330-110*y,100,100,FG);
                
                DrawRectangleLines(20+110*x, 20+330-110*y, 100, 100, FG);
                
                // strictly bank A here
                if (pads[4*y+x].defined)
                  DrawText(pads[4*y+x].filename, 20+110*x+5, 20+330-110*y+5, 10, FG);
              }

            // button pressed
            if ( isKeymapPressed())
            {
              if (!pads[getKeymapIndex()].defined)
              {
                std::cout << "Pad not defined yet.\n";
                chooseASample = true;
                activePad = getKeymapIndex();
                
                // stop all sounds when undefined pad is tapped
                stopAllSounds();
              }
              else if (IsKeyDown(340)) // shift key means erase !
              {
                pads[getKeymapIndex()].defined = false;
              }
              else
              {
                std::cout << "Pad defined.\n";
                // if the audio file is defined, we will assume there
                // is a sound defined as well
                PlaySound(*pads[getKeymapIndex()].sound);
                
                // close CAS window if last hit pad is defined
                chooseASample = false;
              }
            }

            /*  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
            /*                                                                 */
            /*         "Choose A Sample" window                                */
            /*                                                                 */
            /*  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
            if (chooseASample)
            {
              int win_width = 400;
              int win_height = 400;
              int win_x = 30;
              int win_y = 30;
              
              DrawRectangle(win_x,win_y,win_width,win_height,BG);
              DrawRectangleLines(win_x,win_y,win_width,win_height,FG);
              DrawText("Choose a sample.", win_x+30, win_y+30, 20, FG);
              
              int font_size = 12;
              for (int i=0; i<files_count-2; i++)
              {
                DrawText(files[i+2], win_x+30, win_y + 60 + (font_size+3)*i, font_size, FG);
              }
              
              // index of audio sample being hovered over
              int hover_index = GetMouseY();
              hover_index -= win_y + 60; hover_index /= (font_size+3);
              if (hover_index < 0) hover_index = 0;
              if (hover_index >= files_count-2) hover_index = files_count-3;
              
              // show indicator at mouse position
              DrawRectangleLines(win_x, win_y + 60 + (font_size+3)*hover_index, win_width, font_size, FG);
            
              if (IsMouseButtonPressed(0))
              {
                // save the sound and its path into the last hit pad
                pads[activePad].defined = true;
                pads[activePad].filename = files[hover_index+2];
                pads[activePad].sound = &sounds[hover_index];
                
                // Now that a sample is selected, we do not need the selection window
                activePad = None;
                chooseASample = false;
              }
            }

        EndDrawing();
        //-------------------------------------------------------------------
    }

    // Deinitialization
    //-----------------------------------------------------------------------

    free(files);

    // TODO: Unload all loaded resources at this point
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    
    // Unload Sounds
    for (int i=0; i<files_count-2; i++)
      UnloadSound(sounds[i]);
    
  //--------------------------------------------------------------------------
  return 0;
}
