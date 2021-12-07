
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

int main(void)
{
  int file_count;
  char** files = GetDirectoryFiles("audio", &file_count);
  
  for (int i=0; i<file_count; i++)
  {
    std::cout << i << ": " << files[i] << "\n";
  }
  
  free(files);
  
  return 0;
}
