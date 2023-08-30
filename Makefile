CC=g++
BN=TripTrip#build name
CFLAGS=-O3 -Wall
INCLUDEPATH =-I ./src/include
UNAME_S := $(shell uname -s)
SRC = $(wildcard ./src/*.cpp)
all: game

game:
ifeq ($(OS),Windows_NT)
	-rd /s /q build
	@echo "Creating game directory..."
	mkdir build
	cd build && \
	mkdir $(BN) && \
	cd $(BN) && \
	mkdir assets && \
	mkdir data && \
	cd assets && mkdir levels

	@echo "Copying assets..."
	-copy .\src\sprites\export\* build\$(BN)\assets
	-copy .\src\levels\* build\$(BN)\assets\levels
	-copy .\src\fonts\* build\$(BN)\assets
	-copy .\src\sounds\* build\$(BN)\assets
	-copy .\src\dlls\* build\$(BN)
	@echo "Building binaries..."
	$(CC) -o "build/$(BN)/$(BN).exe" $(SRC) $(CFLAGS) $(INCLUDEPATH) -L src/lib/ -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
endif

ifeq ($(UNAME_S),Linux)
	rm -rf ./build/*
	@echo "You may need to install .so files from ./src/so to /usr/bin"
	mkdir -p build/$(BN)/assets/levels
	mkdir -p build/$(BN)/data
	cp ./src/sprites/export/* ./build/$(BN)/assets/
	cp ./src/levels/* ./build/$(BN)/assets/levels
	cp ./src/fonts/* ./build/$(BN)/assets/
	cp ./src\sounds/* ./build/$(BN)/assets/
	cp ./src/so/*.so* ./build/$(BN)
	$(CC) -o ./build/$(BN)/$(BN).bin $(SRC) $(CFLAGS) $(INCLUDEPATH) -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L./src/so
endif

ifeq ($(UNAME_S),Darwin)
	@echo "Buy normal pc"
endif

install_lib:
	sudo cp ./src/so/* /usr/lib/