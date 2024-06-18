cmake_minimum_required(VERSION 3.0.0)
project(Bomberman VERSION 0.1.0)

set(FINC Includes)
set(FSRC Sources)
set(FEXT Extern)
set(FOA ${FEXT}/openApp)
set(FIRR ${FEXT}/Irrlicht)
set(FSFML ${FEXT}/SFML)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Extern/Irrlicht")

set(Irrlicht_FIND_REQUIRED true)

include(FindIrrlicht)

add_subdirectory(${FOA})

set(HEADERS
    ${FINC}/AMapViewer.hpp
    ${FINC}/GameApp.hpp
    ${FINC}/GameCore.hpp
    ${FINC}/GameObject.hpp
    ${FINC}/GameBonus.hpp
    ${FINC}/GameDeflagration.hpp
    ${FINC}/GamePlayer.hpp
    ${FINC}/GameBomb.hpp
    ${FINC}/GameTile.hpp
    ${FINC}/GameScreen.hpp
    ${FINC}/GameSound.hpp
    ${FINC}/GameRenderer.hpp
    ${FINC}/GameAI.hpp
    ${FINC}/Astar.hpp
)

set(SOURCES
    ${FSRC}/Main.cpp
    ${FSRC}/GameCore.cpp
    ${FSRC}/GameCoreUpdate.cpp
    ${FSRC}/GameRenderer.cpp
    ${FSRC}/GameRendererUpdate.cpp
    ${FSRC}/GameScreen.cpp
    ${FSRC}/MapGenerator.cpp
    ${FSRC}/MapGeneratorChunk.cpp
    ${FSRC}/GameAI.cpp
    ${FSRC}/Astar.cpp
)

include_directories(
	${FINC}
	${FOA}/Includes
	${Irrlicht_INCLUDE_DIRS}
	${FSFML}
)

if (MSVC)
	link_directories(${FOA} ${FSFML})
elseif (UNIX)
	link_directories(${FOA})
    set(CMAKE_CXX_FLAGS "-Wall -fPIC")
endif ()

find_package(Threads)

link_libraries(openApp ${Irrlicht_LIBRARIES} sfml-audio Threads::Threads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(bomberman ${HEADERS} ${SOURCES})
