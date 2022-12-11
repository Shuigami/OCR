#pragma once

void read_solver(char* filename, char* buffer);
void translate_solver(char* board,char result[81][10]);
void translateback(char board[81][10],char* result);
void write_solver(char* filename,char* result);
void print_solver(char s[81][10]);
size_t x_of(size_t i);
size_t y_of(size_t i);
size_t i_of(size_t i);
size_t valid_numb(char s[81][10], size_t i, size_t j);