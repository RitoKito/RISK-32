// name: Nikita Trukhin
// unikey: ntru9611
// SID: 520637492

#include "header.h"
//
TYPE check_type(int32_t opcode){

	switch(opcode){
		case 0b0010011:
		case 0b0000011:
		case 0b1100111:
			return I;
		case 0b1101111:
			return UJ;
		case 0b0110111:
			return U;
		case 0b0100011:
			return S;
		case 0b0110011:
			return R;
		case 0b1100011:
			return SB;
		default:
			return SMTW;
	}

	return SMTW;
}

int8_t main(int8_t argc, char *argv[]){
	struct blob mem = {0};
	struct blob *mem_ptr = &mem;

	FILE *file;
	file = fopen(argv[1], "rb");

    if(file == NULL) { 
        printf("Cannot find image specified\n");
        return 1;
    }

	uint8_t buffer[2048] = {0};

	fread(buffer, 1, sizeof(buffer), file);
	fclose(file);

	uint16_t j = 0;
	for(int16_t i = 0; i < 2048; i++){
		if(i <= 1023){
			mem_ptr->inst_mem[i] = buffer[i];
		}
		else if(i >= 1024){
			mem_ptr->data_mem[j] = buffer[i];
			j++;
		}
    }

	int8_t running = 1;

	while(running != 0){
 		uint32_t current_instruction = (mem_ptr->inst_mem[mem_ptr->PC+3]<<24)|(mem_ptr->inst_mem[mem_ptr->PC+2]<<16)|(mem_ptr->inst_mem[mem_ptr->PC+1]<<8)|mem_ptr->inst_mem[mem_ptr->PC];

		uint32_t opcode = current_instruction & 0x7F;

 		TYPE instructionType = check_type(opcode);

		uint8_t func3;
		uint8_t func7;
		uint8_t rd;
		uint8_t rs1;
		uint8_t rs2;
		int32_t imm1;
		uint32_t imm2;
		uint32_t imm3;
		uint32_t imm4;
		int32_t imm;
		int8_t sign;
 		switch(instructionType){
			case R:
				func3 = (current_instruction & 0x7000)>>12;
				func7 = (current_instruction & 0xFE000000)>>25;

				rd = (current_instruction & 0xF80)>>7;
				rs1 = (current_instruction & 0xF8000)>>15;
				rs2 = ((current_instruction & 0x1F00000)>>20);
				
				type_r(mem_ptr, func3, func7, rd, rs1, rs2);

				mem_ptr->PC += 4;
				break;
 			case I:
				func3 = (current_instruction & 0x7000)>>12;

				rd = (current_instruction & 0xF80)>>7;
				rs1 = (current_instruction & 0xF8000)>>15;

				imm = (current_instruction & 0xFFF80000)>>20;
				sign = (current_instruction>>31)&1;
				if(sign){
					imm |= 0xFFFFF000;
				}

				type_i(mem_ptr, opcode, func3, rd, rs1, imm);
 				break;
			case S:
				func3 = (current_instruction & 0x7000)>>12;

				rs1 = (current_instruction & 0xF8000)>>15;
				rs2 = ((current_instruction & 0x1F00000)>>20);

				imm1 = (current_instruction & 0xFE000000)>>20;
				imm2 = (current_instruction & 0xF80)>>7;
				imm = imm1|imm2;

				sign = (current_instruction>>31)&1;
				if(sign){
					imm |= 0xFFFFF000;
				}

				type_s(mem_ptr, func3, rs1, rs2, imm);

				break;
			case SB:
				func3 = (current_instruction & 0x7000)>>12;

				rs1 = (current_instruction & 0xF8000)>>15;
				rs2 = ((current_instruction & 0x1F00000)>>20);

				imm1 = ((current_instruction & 0x80000000)>>19);
				imm2 = ((current_instruction & 0x80)<<4);
				imm3 = ((current_instruction & 0x7E000000)>>20);
				imm4 = ((current_instruction & 0xF00) >> 7);
				imm = (imm1|imm2|imm3|imm4);

				sign = (current_instruction>>31)&1;
				if(sign){
					imm |= 0xFFFFF000;
				}
				
				type_sb(mem_ptr, func3, rs1, rs2, imm);

				break;
			case U:
				rd = (current_instruction & 0xF80)>>7;
				imm = (current_instruction & 0xFFFFF000);

				type_u(mem_ptr, rd, imm);

				mem_ptr->PC += 4;
				break;
 			case UJ:
				rd = (current_instruction & 0xF80)>>7;

				imm1 = (current_instruction & 0x80000000)>>11;
				imm2 = current_instruction & 0xFF000;
				imm3 = ((current_instruction & 0x100000)>>9);
				imm4 = ((current_instruction & 0x7FE00000)>>20);
				imm = (imm1|imm2|imm3|imm4);

				sign = (current_instruction>>31)&1;
				if(sign){
					imm |= 0xFFF00000;
				}
				
				type_uj(mem_ptr, rd, imm);
 				break;
 			case SMTW:
 				printf("Instruction Not Implemented: 0x%08x\n", current_instruction);
				printf("PC = 0x%08x;\n", mem_ptr->PC);
				for(int8_t i = 0; i < 32; i++){
					printf("R[%i] = 0x%08x;\n", i, read_register(mem_ptr, i));
				}
 				return 1;
 		}
	}

    return 0;
}