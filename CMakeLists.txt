cmake_minimum_required(VERSION 3.10)
project(FusionTech5)  # Change project name if needed

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Dependancies/dll)

# Add dependencies
include_directories(Dependancies)
link_directories(Dependancies/lib)

# Collect all .cpp source files automatically
file(GLOB_RECURSE SOURCES 
    Dependancies/imgui/*.cpp
    Enviroments/*.cpp
    Enviroments/Interpreter/*.cpp
    Enviroments/Physics/Jolt/*.cpp
    Enviroments/Physics/Jolt/AABBTree/*.cpp
    Enviroments/Physics/Jolt/AABBTree/NodeCodec*.cpp
    Enviroments/Physics/Jolt/AABBTree/TriangleCodec*.cpp
    Enviroments/Physics/Jolt/Core/*.cpp
    Enviroments/Physics/Jolt/Geometry/*.cpp
    Enviroments/Physics/Jolt/Math/*.cpp
    Enviroments/Physics/Jolt/ObjectStream/*.cpp
    Enviroments/Physics/Jolt/Physics/*.cpp
    Enviroments/Physics/Jolt/Physics/Body/*.cpp
    Enviroments/Physics/Jolt/Physics/Character/*.cpp
    Enviroments/Physics/Jolt/Physics/Collision/*.cpp
    Enviroments/Physics/Jolt/Physics/Collision/BroadPhase/*.cpp
    Enviroments/Physics/Jolt/Physics/Collision/Shape/*.cpp
    Enviroments/Physics/Jolt/Physics/Constraints/*.cpp
    Enviroments/Physics/Jolt/Physics/Constraints/ConstraintPart/*.cpp
    Enviroments/Physics/Jolt/Physics/Ragdoll/*.cpp
    Enviroments/Physics/Jolt/Physics/Vehicle/*.cpp
    Enviroments/Physics/Jolt/Renderer/*.cpp
    Enviroments/Physics/Jolt/Skeleton/*.cpp
    Enviroments/Physics/Jolt/TriangleSplitter/*.cpp
    Enviroments/Physics/Utilites/*.cpp
    Enviroments/Physics/Actions/*.cpp
    Enviroments/Physics/*.cpp
    Enviroments/Rendering/Terrain/*.cpp
    Enviroments/World/*.cpp
    Enviroments/Common/*.cpp
    Enviroments/Common/Commands.cpp
    Enviroments/Common/Commands.h
    Enviroments/Rendering/*.cpp
    Enviroments/Rendering/Buffers/*.cpp
    Enviroments/Rendering/Effects/Particle2D/*.cpp
    Enviroments/Rendering/Interface/Core/*.cpp
    Enviroments/Rendering/Interface/Elements/*.cpp
    Enviroments/Humanoid/*.cpp
    Enviroments/Rendering/Scene/*.cpp
    Enviroments/Dynamic/*.cpp
    Enviroments/Audio/*.cpp
    Enviroments/Engine.cpp
    Main.cpp
    glad.c
)

# Define the executable
add_executable(Main ${SOURCES})

# Link necessary libraries
target_link_libraries(Main glfw3dll z winmm ws2_32 sndfile OpenAl32)

