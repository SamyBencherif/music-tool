
# An interactive program for live beat-making with the MPD218
main:
	g++ -Wall -D__WINDOWS_MM__ -o build/main deps/rtmidi-4.0.0/RtMidi.cpp src/main.cpp -s -O2 -Ideps/raylib-3.7.0_win64_mingw-w64\include -Ideps/rtmidi-4.0.0 -Ldeps/raylib-3.7.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17 -mwindows
	build/main.exe

# the full program, compiled to show a debug terminal
debug:
	g++ -D__WINDOWS_MM__ -o build/main deps/rtmidi-4.0.0/RtMidi.cpp src/main.cpp -s -O2 -Ideps/raylib-3.7.0_win64_mingw-w64\include -Ideps/rtmidi-4.0.0 -Ldeps/raylib-3.7.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17
	build/main.exe

# this program discovers and lists the files in the 
# user's sample library (audio folder)
audiolist:
	g++ -Wall -D__WINDOWS_MM__ -o build/audiolist deps/rtmidi-4.0.0/RtMidi.cpp src/audiolist.cpp -s -O2 -Ideps/raylib-3.7.0_win64_mingw-w64\include -Ideps/rtmidi-4.0.0 -Ldeps/raylib-3.7.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17
	build/audiolist.exe

# this program polls midi input device #0 for messages
# make sure device is connected before running program
midi:
	g++ -Wall -D__WINDOWS_MM__ -o build/midi deps/rtmidi-4.0.0/RtMidi.cpp src/midi.cpp -Ideps/rtmidi-4.0.0 -lwinmm
	build/midi.exe
	
# this program simply opens a window
window:
	g++ -Wall -o build/window src/window.cpp -s -O2 -Ideps/raylib-3.7.0_win64_mingw-w64\include -Ldeps/raylib-3.7.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17 -mwindows
	build/window.exe
	