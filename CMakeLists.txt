cmake_minimum_required (VERSION 3.10)
project ("Projekt_2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(Projekt_2 
    "Projekt_2/src/main.cpp"
    "Projekt_2/include/Tablica.hpp"
    "Projekt_2/include/Double_List.hpp"
    "Projekt_2/include/Graph.hpp"
    "Projekt_2/include/MatrixGraph.hpp"
    "Projekt_2/include/ListGraph.hpp"
    "Projekt_2/include/Reader.hpp"
    "Projekt_2/include/Parameters.h"
)
if(UNIX)
    target_include_directories(Projekt_2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Projekt_2/include")
    target_link_directories(Projekt_2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Projekt_2/lib")
    target_link_libraries(Projekt_2 PRIVATE parameters)
    
    set_target_properties(Projekt_2 PROPERTIES
        BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/Projekt_2/lib"
        INSTALL_RPATH "$ORIGIN/lib"
    )
else()
endif()
