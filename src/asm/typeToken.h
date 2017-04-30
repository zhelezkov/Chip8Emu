#pragma once

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