#pragma once

void read(char* filename, char* buffer);
void translate(char* board,char result[81][10]);
char force(char s[81][10], size_t i);
void init(char s[81][10]);
void print(char s[81][10]);
