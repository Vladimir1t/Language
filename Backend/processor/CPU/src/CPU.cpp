#include "..\include\CPU.h"

static FILE* ErrorFile = fopen ("Log\\error_CPU.txt", "w");

int main (int argc, char* argv[])
{
    struct Cpu proc = {0};
    CpuCtor (&proc);

    FillArray (&proc, argv[1]);                                 // get bytecode from sourse file

    FOPEN (resultF, "Log\\result.txt", "w");

    int result = ExecuteCommands (resultF, &proc);
    if (result == SUCCESS)
        printf ("PROGRAM HAS FINISHED SUCCESSFULLY\n");

    return 0;
}

int ExecuteCommands (FILE* resultF, struct Cpu* proc)
{
    elem_t num1    = POISON,
           num2    = POISON,
           num3    = POISON;
    elem_t address = POISON;

    for (size_t IP = 0; IP < proc->arraySize; IP += COM_SIZE)
    {
       char command = GetCommand (proc, IP);
       elem_t* arg = GetArgument (proc, IP, proc->arrayCommand[IP + 1] & MASK);

        switch (command)
        {
            case PUSH:
                //printf ("push %d\n", *arg);
                PUSH_COM (arg);
                break;

            case POP:
                //printf ("pop\n");
                POP_COM (arg);
                break;

            case ADD:
                MATH_COM (ADD, +);
                break;

            case SUB:
                MATH_COM (SUB, -);
                break;

            case DIV:
                MATH_COM (DIV, /);
                break;

            case MUL:
                MATH_COM (MUL, *);
                break;

            case SQRT:
                MATH_SQRT (SQRT);
                break;

            case OUT:
                OUT_COM ();
                break;

            case IN:
                IN_COM ();
                break;

            case HLT:
                HLT_COM ();
                break;

            case JMP:
                IP = JumpTo (proc, IP);
                break;

            case JB:
                JUMP (JB, <);
                break;

            case JBE:
                JUMP (JBE, <=);
                break;

            case JA:
                JUMP (JA, >);
                break;

            case JAE:
                JUMP (JAE, >=);
                break;

            case JE:
                JUMP (JE, ==);
                break;

            case JNE:
                JUMP (JNE, !=);
                break;

            case CALL:
                CALL_COM (IP);
                break;

            case RET:
                RET_COM (IP);
                break;
        }
    }
    fclose (resultF);
    CpuDtor (proc);

    return SUCCESS;
}

void CpuCtor (Cpu* proc)
{
    StackCtor (&proc->stk, 10);        // construct Stack of arguments
    StackCtor (&proc->stkAdr, 2);     // construct Stack of addresses

    proc->arrayCommand = NULL;        // buffer
    proc->arraySize = 0;              // size of buffer

    proc->ramSize = RAM_SIZE;         // construct random access memory
    proc->ram = (elem_t*) calloc (RAM_SIZE, sizeof(elem_t));
    for (size_t i = 0; i < proc->ramSize; i++)
        proc->ram[i] = POISON;
}

void CpuDtor (struct Cpu* proc)
{
    for (size_t i = 0; i < proc->stk.size; i++)
        proc->stk.data[i] = POISON;

    StackDtor (&proc->stk);
    StackDtor (&proc->stkAdr);

    for (size_t i = 0; i < proc->arraySize; i++)
        proc->arrayCommand[i] = '\0';
    free (proc->arrayCommand);
    proc->arrayCommand = NULL;

    free (proc->ram);
}

int FillArray (struct Cpu* proc, char* fileSourse)
{
    struct Strings Str = {0};
    FOPEN (bytecode, fileSourse, "rb");

    Str.fileSize = FileSize (bytecode);   // measure the size of a bytecode

    proc->arraySize = Str.fileSize;
    CALLOC (proc->arrayCommand, char, proc->arraySize);

    if (fread (proc->arrayCommand, sizeof(char), Str.fileSize, bytecode) != Str.fileSize)
        fprintf (ErrorFile, "--Error. file reading error--\n");

    fclose (bytecode);
    free (Str.textPointer);
}

void GetDigits (struct Cpu* proc, elem_t* num2, elem_t* num1)
{
    StackPop (&proc->stk, num2);
    StackPop (&proc->stk, num1);
}

size_t JumpTo (struct Cpu* proc, size_t IP)
{
    elem_t address =  *(elem_t*) (proc->arrayCommand + IP + COM_SIZE) - COM_SIZE;
    //printf ("[%d]", address);
    return address;
}

char GetCommand (struct Cpu* proc, size_t IP)
{
    char command = *(char*) (proc->arrayCommand + IP);
    return command;
}

elem_t* GetArgument (struct Cpu* proc, size_t IP, char mode)
{
    switch (mode)
    {
        case NUM_MOD:
            return (elem_t*) (proc->arrayCommand + IP + COM_SIZE);
            break;

        case REG_MOD:
            for (int j = 0; j < REGISTRS_NUM; j++)
            {
                if (*(elem_t*) (proc->arrayCommand + IP + COM_SIZE) ==
                    (elem_t)   reg[j].name[1] - (elem_t) reg[j].name[0])
                    return &reg[j].value;
            }
            break;

        case RAM_MOD:
            elem_t adr =  *(elem_t*) (proc->arrayCommand + IP + COM_SIZE);
            return proc->ram + adr;
            break;
    }
}










