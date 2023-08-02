#include "header.h"

void type_s(struct blob *mem_ptr, uint8_t func3, uint8_t rs1, uint8_t rs2, int32_t imm){
    int32_t address = read_register(mem_ptr, rs1) + imm;
    switch(func3){
        case 0: // sb
            write_memory(mem_ptr, address, 1, read_register(mem_ptr, rs2));
            break;
        case 0b001: // sh
            write_memory(mem_ptr, address, 2, read_register(mem_ptr, rs2));
            break;
        case 0b010: // sw
            write_memory(mem_ptr, address, 4, read_register(mem_ptr, rs2));
            break;
        default:
            uint32_t current_instruction = (mem_ptr->inst_mem[mem_ptr->PC+3]<<24)|(mem_ptr->inst_mem[mem_ptr->PC+2]<<16)|(mem_ptr->inst_mem[mem_ptr->PC+1]<<8)|mem_ptr->inst_mem[mem_ptr->PC];
			printf("Instruction Not Implemented: 0x%08x\n", current_instruction);
			printf("PC = 0x%08x;\n", mem_ptr->PC);
			for(int8_t i = 0; i < 32; i++){
				printf("R[%i] = 0x%08x;\n", i, read_register(mem_ptr, i));
			}
            exit(1);
    }

    mem_ptr->PC += 4;
}