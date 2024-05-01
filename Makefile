FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code\
	-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
	-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
	-Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo\
	-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits\
	-Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

TARGET = Language
CC = g++

# Frontend
front: frontend
	Frontend\frontend.exe text_files\sourse_file.txt

frontend: Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp
	$(CC) -o Frontend\frontend Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp

Frontend\main.o:  Frontend\src\main.cpp
	$(CC) Frontend\src\main.cpp -c  $(FLAGS)

Frontend\input_output.o:  Frontend\src\input_output.cpp
	$(CC) Frontend\src\input_output.cpp -c  $(FLAGS)

 Frontend\parcer.o:  Frontend\src\parcer.cpp
	$(CC) -c Frontend\parcer Frontend\src\parcer.cpp $(FLAGS)

 Frontend\simplifier.o:  Frontend\src\simplifier.cpp
	$(CC) -c Frontend\simplifier Frontend\src\simplifier.cpp $(FLAGS)

# Backend

#src\input_output.o:  src\input_output.cpp
#	g++ -c src\input_output.cpp $(FLAGS)

clean:
	rm $(TARGET) *-o
