DEBUG_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code\
	-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
	-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
	-Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo\
	-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits\
	-Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

TARGET = Language
CXX = g++
# file execute 
program = text_files\factorial.txt

# OS WINDOWS

all: frontend backend cpu asm
	Frontend\frontend.exe $(program)
	Backend\backend.exe text_files\file_output.txt text_files\asm_file.txt
	Backend\processor\Assembler\assembler.exe  text_files\asm_file.txt  Backend\processor\Assembler\res_ass.txt
	Backend\processor\CPU\cpu.exe  Backend\processor\Assembler\res_ass.txt  text_files\result_file.txt

# Frontend
front: frontend
	Frontend\frontend.exe $(program)

frontend: Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp
	$(CXX) -o Frontend\frontend Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp

# Backend
back: backend
	Backend\backend.exe text_files\file_output.txt text_files\asm_file.txt

backend: Backend\src\main.cpp Backend\src\input_output.cpp Backend\stack\src\stack.cpp Backend\src\translator.cpp
	$(CXX) -o Backend\backend Backend\src\main.cpp Backend\src\input_output.cpp Backend\stack\src\stack.cpp Backend\src\translator.cpp


proc: asm cpu
	Backend\processor\Assembler\assembler.exe  text_files\asm_file.txt  Backend\processor\Assembler\res_ass.txt
	Backend\processor\CPU\cpu.exe  Backend\processor\Assembler\res_ass.txt  text_files\result_file.txt

cpu: Backend\processor\CPU\src\CPU.cpp  Backend\processor\Onegin\src\file_size.cpp  Backend\processor\Onegin\src\string_read.cpp  Backend\processor\Onegin\src\destruct.cpp  Backend\processor\Onegin\src\str_print.cpp  Backend\Stack\src\stack.cpp
	$(CXX) -o Backend\processor\CPU\cpu Backend\processor\CPU\src\CPU.cpp  Backend\processor\Onegin\src\file_size.cpp  Backend\processor\Onegin\src\string_read.cpp  Backend\processor\Onegin\src\destruct.cpp  Backend\processor\Onegin\src\str_print.cpp  Backend\processor\Stack\src\stack.cpp

asm: Backend\processor\Assembler\src\assembler.cpp  Backend\processor\Onegin\src\file_size.cpp  Backend\processor\Onegin\src\string_read.cpp  Backend\processor\Onegin\src\destruct.cpp  Backend\processor\Onegin\src\str_print.cpp
	$(CXX) -o Backend\processor\Assembler\assembler Backend\processor\Assembler\src\assembler.cpp  Backend\processor\Onegin\src\file_size.cpp  Backend\processor\Onegin\src\string_read.cpp  Backend\processor\Onegin\src\destruct.cpp  Backend\processor\Onegin\src\str_print.cpp


Frontend\main.o:  Frontend\src\main.cpp
	$(CXX) Frontend\src\main.cpp -c  

Frontend\input_output.o:  Frontend\src\input_output.cpp
	$(CXX) Frontend\src\input_output.cpp -c 

Frontend\parcer.o:  Frontend\src\parcer.cpp
	$(CXX) -c Frontend\parcer Frontend\src\parcer.cpp

Frontend\simplifier.o:  Frontend\src\simplifier.cpp
	$(CXX) -c Frontend\simplifier Frontend\src\simplifier.cpp 

clear:
	rm -rf *.x *.exe *.o
