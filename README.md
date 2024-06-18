# indie_studio
Epitech Final Second Year Project, IndieStudio


![alt text](https://github.com/alexandre10044/indie_studio/blob/master/demo1.png)


![alt text](https://github.com/alexandre10044/indie_studio/blob/master/demo2.png)


Usage:

git submodule init

git submodule update

./build.sh

./bomberman

Subject:

The aim of theIndie Studiois to implement across-platform3D video game withreal-worldtools.Making an enjoyable game is just as important as writing clean code.A working game which is no fun to play is pointless!

You MUST use CMakeas a cross-platform build system:
  •Linux: it must generates a makefile to be able to build the project
  •Windows: it must generates a visual studio solution (will be tested with Visual Studio 2017).
  
  
You also must use the Irrlicht 1.8.4 library (http://irrlicht.sourceforge.net/) and use assets. We don’t want to see any fighting cubes! A CMakemodule for Irrlicht is provided, you MUST use it to generate your project.Sounds and musics must be handled using SFML-audio 2.4 library. Others SFML components are not al-lowed.You may use boost 1.66 to help you with some aspects of the project (or for bonuses).

The game you must implement is a Bomberman-like game.Bombermanis one of the most famous video games of all time.With over 70 franchises, ranging from the first version on MSX, ZX Spectrum and Sharp MZ-700 in 1983to the lastest versions on modern consoles, over 10 million units have been sold.

Our gameplay reference isNeo Bomberman, released on the Neo Geo and MVS systems in 1997.Here’s the required features :

  •Local multi-player with the possibility the play with a friend + AI’s controlled bots.
  
  •A main game menu (to start a new game or load an existing one).
  
  •Your game must feature 3D graphics but a 2D gameplay.
  
  •Your game must featureprocedurally generated maps.
  
  •Save/Load a game•Animation and sounds to give life to the game (especially during various interactions)
  
    •Think about bomb explosion, player walking, background music, . . .
    
  •Some Power-ups
  
    •Bomb Up
    
    •Speed Up
    
    •Fire Up
    
    •Wall Pass
    

The goal of this project is not only to code a video game, but it’s to code a full finished video game. Thismeans that the efforts you make to polish your game will count as much as the technical aspect.Do your best to avoid clunky animations or graphical glitches, manage your camera in a way that the “out-side” of the game is never visible, pay attention to game design, level design and sound design, add varia-tions in ennemies and environnements, etc
