#pragma once

#include "matrix.h"

double sigmoid(double input);
Matrix* sigmoidPrime(Matrix* m);
Matrix* softmax(Matrix* m);