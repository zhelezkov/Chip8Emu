//
//  Memory.hpp
//  Chip8Emulator
//

#ifndef Memory_hpp
#define Memory_hpp

#define MEM_SIZE 0xFFF
#define MEM_START 0x200

class Memory {
private:
	unsigned char data[MEM_SIZE];

public:
	unsigned char operator[](int i);
	void clear();
};

#endif /* Memory_hpp */
