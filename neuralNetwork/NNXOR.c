#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <string.h>

double sigmoid(double x)
{
	return 1/(1+exp(-x));
}

double dSigmoid(double x)
{
	return x * (1-x);
}

double init_weight()
{
	return ((double)rand())/((double)RAND_MAX);
}
void sgd(int *array,size_t n)
{
	if(n>1)
	{
		size_t i;
		for(i = 0; i < n - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n-i)+1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

void clear(char *filename)
{
	FILE* of = fopen(filename,"w");
	char *str;
	asprintf(&str,"%s:\n",filename);
	fwrite(str,1,strlen(str),of);
	fclose(of);
}

// void write(char* filename, char* result)
// {
// 	FILE* output_file = fopen(filename,"a+");
// 	if(!output_file)
// 	{
// 		errx(1,"fopen");
// 	}
// 	fwrite(result,1,strlen(result),output_file);
// 	fclose(output_file);
// }

int NNXOR()
{
	//initialisation des poids et de ce dont nous avons besoins
	double hiddenNode[2];
	double outputNode[1];

	const double learningSpeed = 0.1f;

	double deltaOutput[1];
	double deltaHidden[2];

	double hiddenWeights[2][2];
	double outputWeights[2][1];

	for(int i = 0; i < 2;i++)
	{
		for(int j = 0; j < 2;j++)
		{
			hiddenWeights[i][j] = init_weight();
		}
	}
	for(int i = 0; i < 2;i++)
	{
		for(int j = 0; j < 1;j++)
		{
			outputWeights[i][j] = init_weight();
		}
	}


	double hideNodeBias[2] = {};
	double outputNodeBias[1];

	for(int j = 0; j<1; j++)
	{
		outputNodeBias[j] = init_weight();
	}

	//training
	double inputs[4][2]={{0.0f,0.0f},
	{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f}};

	double outputs[4][1] = {{0.0f},
		{1.0f},{1.0f},{0.0f}};
	int epoch = 100000;
	int orderTrain[] = {0,1,2,3};

	for(int n = 0; n < epoch; n++)
	{
		sgd(orderTrain,4);

		for(int u = 0; u<4;u++)
		{
			int i = orderTrain[u];
			//forward_pass
			for(int j = 0; j < 2; j++)
			{
				double activation = hideNodeBias[j];//fonction d'activation
				for(int k = 0; k < 2;k++)
					activation +=inputs[i][k]*hiddenWeights[k][j];
				hiddenNode[j] = sigmoid(activation);
			}

			for(int j = 0; j < 1;j++)
			{
				double activation = outputNodeBias[j];
				for(int k = 0; k < 2;k++)
					activation += hiddenNode[k]*outputWeights[k][j];
				outputNode[j] = sigmoid(activation);
			}

			printf("Input: %g %g	Output: %g Predicted Output: %g \n",
					inputs[i][0],inputs[i][1],
					outputNode[0],inputs[i][0]);


			//backpropagation
			for(int j = 0; j<1; j++)
			{
				double error_rate = (outputs[i][j]-outputNode[j]);
				deltaOutput[j] = error_rate * dSigmoid(outputNode[j]);
			}


			for(int j = 0; j<2; j++)
			{
				double error_rate = 0.0f;
				for(int k = 0; k < 1; k++)
					error_rate += deltaOutput[k] * outputWeights[j][k];
				deltaHidden[j] = error_rate * dSigmoid(hiddenNode[j]);
			}

			for(int j = 0; j < 1; j++)
			{
				outputNodeBias[j]+=deltaOutput[j]*learningSpeed;
				for(int k = 0; k<2;k++)
					outputWeights[k][j]+=hiddenNode[k]*deltaOutput[j]*learningSpeed;
			}

			for(int j = 0; j<2;j++)
			{
				hideNodeBias[j]+=deltaHidden[j]*learningSpeed;
				for(int k = 0; k<2;k++)
					hiddenWeights[k][j]+=inputs[i][k]*deltaHidden[j]*learningSpeed;
			}
		}
	}

	//sauvegarde des meilleurs poids et biais
	//fputs("Final Hidden Weights \n [",stdout);
	clear("FinalHiddenWeights");
	for(int j = 0; j < 2;j++)
	{
		//fputs("[ ",stdout);
		for(int k = 0; k < 2; k++)
		{
			char bu[40] = {0};
			//printf("%f ", hiddenWeights[k][j]);
			sprintf(bu,"%lf\n",hiddenWeights[k][j]);
			// write("FinalHiddenWeights",bu);
		}
		//fputs("] ",stdout);
	}

	//fputs(" ]\n Final Hidden Biases \n[ ", stdout);
	clear("FinalHiddenBias");
	for(int j = 0; j < 2; j++)
	{
		char buf[40] = {0};
		//printf("%f ", hideNodeBias[j]);
		sprintf(buf,"%lf\n",hideNodeBias[j]);
		// write("FinalHiddenBias",buf);
	}
	//fputs("] \n",stdout);

	//fputs("Final Output Weights \n[ ",stdout);
	clear("FinalOutputWeight");
	for(int j = 0; j < 1;j++)
	{
		//fputs("[ ",stdout);
		for(int k = 0; k < 2; k++)
		{
			char buff[40] = {0};
			//printf("%f ", outputWeights[k][j]);
			sprintf(buff,"%lf\n",outputWeights[k][j]);
			// write("FinalOutputWeight",buff);
		}
		//fputs("] ",stdout);

	}

	//fputs(" ]\n Final Outputs Biases \n[ ", stdout);
	clear("FinalOutputBias");
	for(int j = 0; j < 1; j++)
	{
		char buffe[40] = {0};
		//printf("%f ", outputNodeBias[j]);
		sprintf(buffe,"%lf\n",outputNodeBias[j]);
		// write("FinalOutputBias",buffe);
	}

	//fputs("] \n",stdout);
	return 0;
}
