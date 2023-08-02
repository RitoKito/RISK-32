#include "header.h"

void type_i(struct blob *mem_ptr, uint8_t opcode, uint8_t func3, uint8_t rd, uint8_t rs1, int32_t imm){

    int32_t write_value;
    uint32_t unsigned_value1;
    uint32_t unsigned_value2;
    if(opcode == 0b0010011){
        switch(func3){
            case 0: // addi
                write_value = read_register(mem_ptr, rs1) + imm;
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b100: // xori
                write_value = read_register(mem_ptr, rs1) ^ imm;
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b110: // ori
                write_value = read_register(mem_ptr, rs1) | imm;
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b111: // andi
                write_value = read_register(mem_ptr, rs1) & imm;
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b010: // slti
                if(read_register(mem_ptr, rs1) < imm){
                    write_register(mem_ptr, rd, 1);
                }
                else{
                    write_register(mem_ptr, rd, 0);
                }
                break;
            case 0b011: // sltiu
                unsigned_value1 = (uint32_t)read_register(mem_ptr, rs1);
                unsigned_value2 = (uint32_t)imm;
                
                if(unsigned_value1 < unsigned_value2){
                    write_register(mem_ptr, rd, 1);
                }
                else{
                    write_register(mem_ptr, rd, 0);
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
        mem_ptr->PC += 4;
    }
    else if(opcode == 0b0000011){
        int8_t sign;
        switch(func3){
            case 0: // lb
                write_value = read_memory(mem_ptr, (read_register(mem_ptr, rs1) + imm), 1);
                sign = (write_value>>7)&1;
				if(sign){
					write_value |= 0xFFFFFF00;
				}
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b001: // lh
                write_value = read_memory(mem_ptr, (read_register(mem_ptr, rs1) + imm), 2);
                sign = (write_value>>15)&1;
				if(sign){
					write_value |= 0xFFFF0000;
				}
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b010: // lw
                write_value = read_memory(mem_ptr, (read_register(mem_ptr, rs1) + imm), 4);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b100: // lbu
                unsigned_value1 = (uint32_t)read_memory(mem_ptr, (read_register(mem_ptr, rs1) + imm), 1);
                write_register(mem_ptr, rd, unsigned_value1);
                break;
            case 0b101: // lhu
                unsigned_value1 = (uint32_t)read_memory(mem_ptr, (read_register(mem_ptr, rs1) + imm), 2);
                write_register(mem_ptr, rd, unsigned_value1);
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
    else if(opcode == 0b1100111){ // jal
        write_register(mem_ptr, rd, mem_ptr->PC + 4);
        mem_ptr->PC = read_register(mem_ptr, rs1 + imm);
    }
}