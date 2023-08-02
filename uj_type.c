#include "header.h"

void type_uj(struct blob *mem_ptr, uint32_t rd, int32_t imm){
    write_register(mem_ptr, rd, mem_ptr->PC + 4); // jal
    mem_ptr->PC += imm;
}