FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code\
	-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
	-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
	-Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo\
	-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits\
	-Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

TARGET = Language

all: language
	language.exe text_files\sourse_file.txt

language: main.o parcer.o simplifier.o
	g++ -o language main.o parcer.o simplifier.o

main.o: src\main.cpp
	g++ -c src\main.cpp $(FLAGS)

parcer.o: src\parcer.cpp
	g++ -c src\parcer.cpp $(FLAGS)

simplifier.o: src\simplifier.cpp
	g++ -c src\simplifier.cpp $(FLAGS)

#src\input_output.o:  src\input_output.cpp
#	g++ -c src\input_output.cpp $(FLAGS)

claen:
	rm $(TARGET) *-o
