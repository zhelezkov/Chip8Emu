//
//  typeToken.h
//  Chip8Asm
//

#ifndef typeToken_h
#define typeToken_h

#include <string>

enum typeToken {
	NAME,
	DT,
	ST,
	ADDR, // [i]
	I,	  // I
	B,    // BCD == 'b'
	F,
	HF,
	R,
	LABEL,
	CMD,
	REG,
	HEX,
	DEC,
	BIN,
	COMMENT,
	NIL
};

static const std::string typeTokenStr[] = {
	"NAME",
	"DT",
	"ST",
	"ADDR",
	"I",
	"B",
	"F",
	"HF",
	"RPL",
	"LABEL",
	"CMD",
	"REG",
	"HEX",
	"DEC",
	"BIN",
	"COMMENT",
	"NIL"
};

#endif /* typeToken.h */