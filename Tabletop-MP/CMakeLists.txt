# CMakeList.txt : CMake project for Tabletop-MP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS graphics audio REQUIRED)

# Add source to this project's executable.
add_executable (Tabletop-CL "Client.cpp" 
							"Client.h" 
							"Tabletop-MP.cpp" 
							"Tabletop-MP.h"
							"Serialization.hpp"
							"enet.h")
add_executable (Tabletop-SR "Server.cpp" 
							"Server.h" 
							"Tabletop-MP.cpp"
							"Tabletop-MP.h"
							"Serialization.hpp"
							"enet.h")

# TODO: Add tests and install targets if needed.
target_link_libraries(Tabletop-CL sfml-graphics sfml-audio sfml-system)
target_link_libraries(Tabletop-SR sfml-graphics sfml-audio sfml-system)