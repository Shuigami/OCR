#pragma once

void read(char* filename, char* buffer);
void translate(char* board,char result[81][10]);
void translateback(char board[81][10],char* result);
void write(char* filename,char* result);
char force(char s[81][10], size_t i);
void init(char s[81][10]);
void print(char s[81][10]);
size_t x_of(size_t i);
size_t y_of(size_t i);
size_t i_of(size_t i);
size_t valid_numb(char s[81][10], size_t i, size_t j);
