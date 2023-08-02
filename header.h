#ifndef HEADER_FILE
#define HEADER_FILE
// 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INST_MEM_SIZE 1024
#define DATA_MEM_SIZE 1024
#define HEAP_MEM_SIZE 8192

typedef enum { R=1, I=2, S=3, SB=4, U=5, UJ=6, SMTW=0 } TYPE;

TYPE check_type(int32_t opcode);

struct blob {
    int8_t heap_mem[HEAP_MEM_SIZE];
    int8_t mem_manager[HEAP_MEM_SIZE];

	uint8_t inst_mem[INST_MEM_SIZE];
	int8_t data_mem[DATA_MEM_SIZE];

    uint32_t registers[32];
    uint32_t PC;
};

int32_t read_register(struct blob *mem_ptr, uint32_t index);
void write_register(struct blob *mem_ptr, uint32_t index, int32_t value);

void halloc(struct blob *mem_ptr, int32_t size);
void fheap(struct blob *mem_ptr, int32_t address);

void write_memory(struct blob *mem_ptr, uint32_t address, uint8_t size, int32_t value);
int32_t read_memory(struct blob *mem_ptr, uint32_t address, uint8_t size);

void type_i(struct blob *mem_ptr, uint8_t opcode, uint8_t func3, uint8_t rd, uint8_t rs1, int32_t imm);
void type_uj(struct blob *mem_ptr, uint32_t rd, int32_t imm);
void type_u(struct blob *mem_ptr, uint32_t rd, int32_t imm);
void type_s(struct blob *mem_ptr, uint8_t func3, uint8_t rs1, uint8_t rs2, int32_t imm);
void type_sb(struct blob *mem_ptr, uint8_t func3, uint8_t rs1, uint8_t rs2, int32_t imm);
void type_r(struct blob *mem_ptr, uint8_t func3, uint8_t func7, uint8_t rd, uint8_t rs1, uint8_t rs2);

#endif
