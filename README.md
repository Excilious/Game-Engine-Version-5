# Basic OpenGL rendered game engine (Game Engine Version 5)

An improvement to the last engine including fixing bugs to do with bsp textures and adding an IMGUI interface, replacing the older interface. Audio has now been implemented using OpenAL. Registers
are upgraded and other bug fixes are introduced to the program. Dialogs have been introduced alongside improvements to lightmap textures and brightness. Textures and meshes correctly load using both 
.png/.jpg and .imgbuf.

Collision system is now updated with the use of Quake 3's Trace functions, using bounding boxes for tracing. There are other functions such as TraceRay and TraceSphere that could help with collision detection and tracing. Collision could be improved in terms of bounding boxes, but maximum bounds vertices are approximated according to the players model.

Registers can be added/removed during runtime and edited during runtime. The console can now dump all logs into a file (normally called "ConsoleLog.txt")

![image](https://github.com/user-attachments/assets/19f8b935-c9aa-4bcd-b43e-797edf8e6d8a)
![image](https://github.com/user-attachments/assets/c0814c9a-acb9-4a04-bad3-036dc599f584)
