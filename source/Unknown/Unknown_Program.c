#include "Unknown_Program.h"

#include "Unknown_Instructions.h"

#define codeInitCapacity 8

/* Constructs and returns a new UNProgram by value */
UNProgram unProgramMake(){
    UNProgram toRet = {0};
    toRet.code = arrayListMake(uint8_t,
        codeInitCapacity
    );
    toRet.lineNumbers = arrayListMake(uint16_t,
        codeInitCapacity
    );
    toRet.literals = unLiteralsMake();
    return toRet;
}

/*
 * Returns a pointer to the first instruction in the
 * specified UNProgram; returns NULL if not possible
 */
uint8_t *unProgramGetEntryPoint(UNProgram *programPtr){
    if(!programPtr){
        return NULL;
    }
    if(programPtr->code.size == 0){
        return NULL;
    }
    return arrayListFrontPtr(uint8_t,
        &(programPtr->code)
    );
}

/*
 * Pushes the specified byte to the back of the
 * specified program's code
 */
void unProgramPushBackCode(
    UNProgram *programPtr,
    uint8_t byte,
    uint16_t lineNumber
){
    arrayListPushBack(uint8_t,
        &(programPtr->code),
        byte
    );
    arrayListPushBack(uint16_t,
        &(programPtr->lineNumbers),
        lineNumber
    );
}

/*
 * Pushes the specified UNValue to the back of the
 * specified program's literals and returns the index
 * where it was written
 */
size_t unProgramPushBackLiteral(
    UNProgram *programPtr,
    UNValue literal
){
    return unLiteralsPushBack(
        &(programPtr->literals),
        literal
    );
}

/*
 * Prints out the disassembly of the specified program 
 */
void unProgramDisassemble(UNProgram *programPtr){
    for(size_t offset = 0;
        offset < programPtr->code.size;
    ){
        offset = unProgramDisassembleInstruction(
            programPtr,
            offset
        );
    }
}

/*
 * Prints out the disassembly of a simple instruction
 * that occupies a single byte and returns the new
 * offset
 */
static size_t printSimpleInstruction(
    const char *name,
    size_t offset
){
    printf("%s\n", name);
    return offset + 1;
}

/*
 * Prints out the disassembly of a 2-byte instruction
 * with the second byte printed out as an integer
 */
static size_t printByteInstruction(
    const char *name,
    UNProgram *programPtr,
    int offset
){
    /* get the slot from the next byte in the code */
    uint8_t slot = arrayListGet(uint8_t,
        &(programPtr->code),
        offset + 1
    );

    printf("%-8s %4d\n", name, slot);
    return offset + 2;
}

/*
 * Prints out the disassembly of a literal instruction
 * and returns the new offset
 */
static size_t printLiteralInstruction(
    const char *name,
    UNProgram *programPtr,
    size_t offset
){
    uint8_t litIndex = arrayListGet(uint8_t,
        &(programPtr->code),
        offset + 1
    );
    UNValue literal = unLiteralsGet(
        &(programPtr->literals),
        litIndex
    );
    printf("%-8s %4d '", name, litIndex);
    unValuePrint(literal);
    printf("'\n");
    return offset + 2;
}

/*
 * Prints out the disassembly of a single instruction
 * in the specified program and returns the new offset
 */
size_t unProgramDisassembleInstruction(
    UNProgram *programPtr,
    size_t offset
){
    /* print the address of the instruction */
    printf("%04lu ", offset);

    /* print line info */
    if(offset > 0
        && arrayListGet(uint16_t,
            &(programPtr->lineNumbers),
            offset
        )
            == arrayListGet(uint16_t,
                &(programPtr->lineNumbers),
                offset - 1
            )
    ){
        printf("    | ");
    }
    else{
        printf("%5d ", arrayListGet(uint16_t,
            &(programPtr->lineNumbers),
            offset
        ));
    }

    uint8_t instruction = arrayListGet(uint8_t,
        &(programPtr->code),
        offset
    );
    switch(instruction){
        case un_literal:
            return printLiteralInstruction(
                "LIT",
                programPtr,
                offset
            );
        case un_pop:
            return printSimpleInstruction(
                "POP",
                offset
            );
        case un_defineGlobal:
            return printLiteralInstruction(
                "DEFGLOB",
                programPtr,
                offset
            );
        case un_getGlobal:
            return printLiteralInstruction(
                "GETGLOB",
                programPtr,
                offset
            );
        case un_setGlobal:
            return printLiteralInstruction(
                "SETGLOB",
                programPtr,
                offset
            );
        case un_getLocal:
            return printByteInstruction(
                "GETLOCAL",
                programPtr,
                offset
            );
        case un_setLocal:
            return printByteInstruction(
                "SETLOCAL",
                programPtr,
                offset
            );
        case un_true:
            return printSimpleInstruction(
                "TRUE",
                offset
            );
        case un_false:
            return printSimpleInstruction(
                "FALSE",
                offset
            );
        case un_add:
            return printSimpleInstruction(
                "ADD",
                offset
            );
        case un_subtract:
            return printSimpleInstruction(
                "SUB",
                offset
            );
        case un_multiply:
            return printSimpleInstruction(
                "MUL",
                offset
            );
        case un_divide:
            return printSimpleInstruction(
                "DIV",
                offset
            );
        case un_negate:
            return printSimpleInstruction(
                "NEG",
                offset
            );
        case un_equal:
            return printSimpleInstruction(
                "EQUAL",
                offset
            );
        case un_greater:
            return printSimpleInstruction(
                "GREATER",
                offset
            );
        case un_less:
            return printSimpleInstruction(
                "LESS",
                offset
            );
        case un_not:
            return printSimpleInstruction(
                "NOT",
                offset
            );
        case un_print:
            return printSimpleInstruction(
                "PRINT",
                offset
            );
        case un_return:
            return printSimpleInstruction(
                "RET",
                offset
            );
        default:
            printf(
                "unknown bad opcode %d\n",
                instruction
            );
            return offset + 1;
    }
}

/*
 * Frees the memory associated with the specified
 * program
 */
void unProgramFree(UNProgram *programPtr){
    arrayListFree(uint8_t, &(programPtr->code));
    arrayListFree(uint16_t,
        &(programPtr->lineNumbers)
    );
    unLiteralsFree(&(programPtr->literals));
    memset(programPtr, 0, sizeof(*programPtr));
}