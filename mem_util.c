#include "header.h"

void print_illegal(struct blob *mem_ptr){
	uint32_t current_instruction = (mem_ptr->inst_mem[mem_ptr->PC+3]<<24)|(mem_ptr->inst_mem[mem_ptr->PC+2]<<16)|(mem_ptr->inst_mem[mem_ptr->PC+1]<<8)|mem_ptr->inst_mem[mem_ptr->PC];
	printf("Illegal Operation: 0x%08x\n", current_instruction);
	printf("PC = 0x%08x;\n", mem_ptr->PC);
	for(int8_t i = 0; i < 32; i++){
		printf("R[%i] = 0x%08x;\n", i, read_register(mem_ptr, i));
	}
}

int32_t read_register(struct blob *mem_ptr, uint32_t index){
	return mem_ptr->registers[index];
}

void write_register(struct blob *mem_ptr, uint32_t index, int32_t value){
	if(index > 0 && index < 32){
		mem_ptr->registers[index] = value;
	}
	else if(index == 0) {;}
	else{
		print_illegal(mem_ptr);
		exit(1);
	}
}

void halloc(struct blob *mem_ptr, int32_t size){
	if(size > HEAP_MEM_SIZE || size <= 0){
		write_register(mem_ptr, 28, 0);
		return;
	}

	int32_t free_c = 0;
	int32_t start = 0;
	for(int32_t i = 0; i < 8192; i+=64){
		if(mem_ptr->mem_manager[i] == 0){
			if(free_c == 0){
				start = i;
			}
			free_c += 64;

			if(free_c >= size){
				for(int32_t j = start; j < (start + free_c); j++){
					if(j == start){
						mem_ptr->mem_manager[j] = 2;
					}
					else if(j < start + (free_c)-1){
						mem_ptr->mem_manager[j] = 1;
					}
					else if(j == start + (free_c)-1){
						mem_ptr->mem_manager[j] = 3;
					}

				}
				write_register(mem_ptr, 28, start + 0xb700);
				break;
			}
		}
		else{
			free_c = 0;
		}
	}

	if(free_c < size){
		write_register(mem_ptr, 28, 0);
	}
}

void fheap(struct blob *mem_ptr, int32_t address){
	int32_t address_to_index = address - 0xb700;

	if(mem_ptr->mem_manager[address_to_index] == 0 || mem_ptr->mem_manager[address_to_index] != 2){
		print_illegal(mem_ptr);
		exit(1);
	}

	for(int32_t i = address_to_index; i < 8192; i++){
		if(mem_ptr->mem_manager[i] == 3){
			mem_ptr->mem_manager[i] = 0;
			break;
		}

		mem_ptr->mem_manager[i] = 0;
	}
}

void write_memory(struct blob *mem_ptr, uint32_t address, uint8_t size, int32_t value){
    uint32_t address_to_index = 0;
	uint8_t bytes[4] = {0};
	int8_t *dest_ptr = NULL;
	int8_t is_heap = 0;

	if(address == 0x0800){
		printf("%c", value);
		return;
	}
	else if(address == 0x0804){
		printf("%i", value);
		return;
	}
	else if(address == 0x0808){
		printf("%x", value);
		return;
	}
	else if(address == 0x080C){
		printf("CPU Halt Requested\n");
		exit(1);
	}
	else if(address == 0x0830){
		halloc(mem_ptr, value);
		return;
	}
	else if(address == 0x0834){
		fheap(mem_ptr, address);
		return;
	}
	else if(address == 0x0820){
		printf("PC = 0x%08x;\n", mem_ptr->PC);
		return;
	}
	else if(address == 0x0824){
		for(int8_t i = 0; i < 32; i++){
			printf("R[%i] = 0x%08x;\n", i, read_register(mem_ptr, i));
		}
		return;
	}
	else if(address == 0x0828){
		uint32_t stored_value = (uint32_t)read_memory(mem_ptr, value, 4);
		printf("Value stored at %08x = %08x;\n", value, stored_value);
		return;
	}
	else if(address >= 0 && address <= 1023){
		print_illegal(mem_ptr);
		exit(1);
	}
	else if(address >= 1024 && address <= 2047){
		address_to_index = address - 1024;
		dest_ptr = mem_ptr->data_mem;
	}
	else if(address >= 0xb700 && address <= 0xd6ff){
		is_heap = 1;
		address_to_index = address - 0xb700;
		dest_ptr = mem_ptr->heap_mem;
		if(mem_ptr->mem_manager[address_to_index] == 0){
			print_illegal(mem_ptr);
			exit(1);
		}
	}
	else{
		print_illegal(mem_ptr);
		exit(1);
	}

	if(size == 4){
		bytes[0] = value & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;
		bytes[2] = (value >> 16) & 0xFF;
		bytes[3] = (value >> 24) & 0xFF;

		for(int8_t i = 0; i < size; i++){
			if(is_heap == 1 && mem_ptr->mem_manager[address_to_index+i] == 0){
				print_illegal(mem_ptr);
				exit(1);
			}
			dest_ptr[address_to_index+i] = bytes[i];
		}
	}
	else if(size == 2){
		bytes[0] = value & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;

		for(int8_t i = 0; i < size; i++){
			if(is_heap == 1 && mem_ptr->mem_manager[address_to_index+i] == 0){
				print_illegal(mem_ptr);
				exit(1);
			}
			dest_ptr[address_to_index+i] = bytes[i];
		}
	}
	else if(size == 1){
		int8_t byte = value;
		dest_ptr[address_to_index] = byte;
	}
}

int32_t read_memory(struct blob *mem_ptr, uint32_t address, uint8_t size){
	int32_t address_to_index = 0;
	uint8_t bytes[4] = {0};
	int8_t *dest_ptr = NULL; // Pointer to other blocks
	uint8_t *udest_ptr = NULL; // Pointer to instruction data block
	int32_t original;
	int8_t is_heap = 0;


	if(address == 0x0812){
		char input;
		scanf("%c", &input);

		return (int32_t)input;
	}
	else if(address == 0x0816){
		int32_t input = 0;
		scanf("%i", &input);

		return input;
	}
	else if(address >= 0 && address <= 1023){ // If address is in instruction block, use unsigned pointer
		address_to_index = address;
		udest_ptr = mem_ptr->inst_mem;
	}
	else if(address >= 1024 && address <= 2047){ // If not in instruction block, use signed pointer
		address_to_index = address - 1024;
		dest_ptr = mem_ptr->data_mem;
	}
	else if(address >= 0xb700 && address <= 0xd6ff ){ // Heap block
		is_heap = 1;
		address_to_index = address - 0xb700;
		dest_ptr = mem_ptr->heap_mem;
		if(mem_ptr->mem_manager[address_to_index] == 0){
			print_illegal(mem_ptr);
			exit(1);
		}
	}

	if(size == 4){
		if(dest_ptr == NULL){
			for(int8_t i = 0; i < size; i++){
				bytes[i] = udest_ptr[address_to_index+i];
			}
		}
		else{
			for(int8_t i = 0; i < size; i++){
				if(is_heap == 1 && mem_ptr->mem_manager[address_to_index+i] == 0){
					print_illegal(mem_ptr);
					exit(1);
				}
				bytes[i] = dest_ptr[address_to_index+i];
			}
		}

		original = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
		return original;
	}
	else if(size == 2){

		if(dest_ptr == NULL){
			for(int8_t i = 0; i < size; i++){
				if(is_heap == 1 && mem_ptr->mem_manager[address_to_index+i] == 0){
					print_illegal(mem_ptr);
					exit(1);
				}
				bytes[i] = udest_ptr[address_to_index+i];
			}
		}
		else{
			for(int8_t i = 0; i < size; i++){
				bytes[i] = dest_ptr[address_to_index+i];
			}
		}

		original = (bytes[1] << 8) | bytes[0];
		return original;
	}
	else if(size == 1){
		if(dest_ptr == NULL){
			original = udest_ptr[address_to_index];
		}
		else{
			original = dest_ptr[address_to_index];
		}
		return original;
	}

	print_illegal(mem_ptr);
	exit(1);
}