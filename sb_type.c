#include "header.h"

void type_sb(struct blob *mem_ptr, uint8_t func3, uint8_t rs1, uint8_t rs2, int32_t imm){
	uint32_t unsigned_value1;
	uint32_t unsigned_value2;
	switch(func3){
		case 0: // beq
			if(read_register(mem_ptr, rs1) == read_register(mem_ptr, rs2)){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
			break;
		case 0b001: // bne
			if(read_register(mem_ptr, rs1) != read_register(mem_ptr, rs2)){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
			break;
		case 0b100: // blt
			if(read_register(mem_ptr, rs1) < read_register(mem_ptr, rs2)){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
			break;
		case 0b110: // bltu
			unsigned_value1 = (uint32_t)read_register(mem_ptr, rs1);
			unsigned_value2 = (uint32_t)read_register(mem_ptr, rs2);

			if(unsigned_value1 < unsigned_value2){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
			break;
		case 0b101: // bge
			if(read_register(mem_ptr, rs1) >= read_register(mem_ptr, rs2)){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
			break;
		case 0b111: // bgeu
			unsigned_value1 = (uint32_t)read_register(mem_ptr, rs1);
			unsigned_value2 = (uint32_t)read_register(mem_ptr, rs2);

			if(unsigned_value1 >= unsigned_value2){
				mem_ptr->PC = mem_ptr->PC + imm;
			}
			else{
				mem_ptr->PC += 4;
			}
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
}