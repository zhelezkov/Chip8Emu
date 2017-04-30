#pragma once

#include <string>

enum typeToken {
	NAME,
	DT,
	ST,
	ADDR, // [i]
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
	"LABEL",
	"CMD",
	"REG",
	"HEX",
	"DEC",
	"BIN",
	"COMMENT",
	"NIL"
};