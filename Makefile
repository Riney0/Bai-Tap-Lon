all:
	g++  -I ../src/Include -L ../src/lib -o main main.cpp AudioManager.cpp Background.cpp BossBattle.cpp Challenge.cpp Game.cpp Item.cpp Player.cpp TappingSystem.cpp UI.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

run:
	.\main

clean:
