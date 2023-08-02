#include "header.h"

void type_r(struct blob *mem_ptr, uint8_t func3, uint8_t func7, uint8_t rd, uint8_t rs1, uint8_t rs2){
    int32_t write_value;
    if(func7 == 0b0100000){ // sub
        if(func3 == 0){
            write_value = read_register(mem_ptr, rs1) - read_register(mem_ptr, rs2);
            write_register(mem_ptr, rd, write_value);
        }
        else if(func3 == 0b101){ // sra
            int32_t shifted_value = read_register(mem_ptr, rs1) >> read_register(mem_ptr, rs2);
            write_value = shifted_value | (read_register(mem_ptr, rs1) << (32 - read_register(mem_ptr, rs2)));
            write_register(mem_ptr, rd, write_value);
        }
        return;
    }
    else if(func7 == 0){
        switch(func3){
            case 0: // add
                write_value = read_register(mem_ptr, rs1) + read_register(mem_ptr, rs2);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b100: // xor
                write_value = read_register(mem_ptr, rs1) ^ read_register(mem_ptr, rs2);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b110: // or
                write_value = read_register(mem_ptr, rs1) | read_register(mem_ptr, rs2);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b111: // and
                write_value = read_register(mem_ptr, rs1) & read_register(mem_ptr, rs2);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b001: // sll
                write_value = read_register(mem_ptr, rs1) << read_register(mem_ptr, rs2);
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b101: // slr
                uint32_t shifted_value = (uint32_t)read_register(mem_ptr, rs1) >> read_register(mem_ptr, rs2);
                write_value = (int32_t)shifted_value;
                write_register(mem_ptr, rd, write_value);
                break;
            case 0b010: // slt
                if(read_register(mem_ptr, rs1) < read_register(mem_ptr, rs2)){
                    write_register(mem_ptr, rd, 1);
                }
                else{
                    write_register(mem_ptr, rd, 0);
                }
                break;
            case 0b011: // sltu
                uint32_t urs1 = (uint32_t)read_register(mem_ptr, rs1);
                uint32_t urs2 = (uint32_t)read_register(mem_ptr, rs2);

                if(urs1 < urs2){
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
    }
    else{
        uint32_t current_instruction = (mem_ptr->inst_mem[mem_ptr->PC+3]<<24)|(mem_ptr->inst_mem[mem_ptr->PC+2]<<16)|(mem_ptr->inst_mem[mem_ptr->PC+1]<<8)|mem_ptr->inst_mem[mem_ptr->PC];
        printf("Instruction Not Implemented: 0x%08x\n", current_instruction);
        printf("PC = 0x%08x;\n", mem_ptr->PC);
        for(int8_t i = 0; i < 32; i++){
            printf("R[%i] = 0x%08x;\n", i, read_register(mem_ptr, i));
        }
        exit(1);
    }
}