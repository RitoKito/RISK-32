#include "header.h"

void type_u(struct blob *mem_ptr, uint32_t rd, int32_t imm){
    write_register(mem_ptr, rd, imm); // lui
}