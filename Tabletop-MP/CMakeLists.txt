# CMakeList.txt : CMake project for Tabletop-MP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Tabletop-CL "Client.cpp" 
							"Client.h" 
							"Tabletop-MP.cpp" 
							"Tabletop-MP.h")
add_executable (Tabletop-SR "Server.cpp" 
							"Server.h" 
							"Tabletop-MP.cpp"
							"Tabletop-MP.h")

# TODO: Add tests and install targets if needed.
