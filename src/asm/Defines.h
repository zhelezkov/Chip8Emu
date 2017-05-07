#pragma once
#include <tuple>
#include <vector>

struct StringToken;

struct TokenIndexes {
	std::pair<int, int> LabelToken;
	std::pair<int, int> CommandToken;
	std::pair<int, int> CommentToken;
};

#define VECTOR_TYPE std::tuple<std::vector<StringToken>, std::vector<TypeArg>, TokenIndexes>
#define NEWSTR std::make_tuple(std::vector<StringToken>(), std::vector<TypeArg>(), TokenIndexes())

#define STRING(i) std::get<0>(ovector[i])
#define TOKEN(i, j) std::get<0>(ovector[i])[j]

#define CMD_STR(i, j) std::get<0>(ovector[i])[j].commandStr
#define COMMENT_STR(i, j) std::get<0>(ovector[i])[j].commentStr
#define ERROR_STR(i, j) std::get<0>(ovector[i])[j].errorStr

#define ARGS(i) std::get<1>(ovector[i])
#define CMD_IND(i) std::get<2>(ovector[i]).CommandToken
#define LABEL_IND(i) std::get<2>(ovector[i]).LabelToken
#define COMMENT_IND(i) std::get<2>(ovector[i]).CommentToken
