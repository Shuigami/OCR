#include <math.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "img.h"
#include "activations.h"
#include "nn.h"
#include "matrix.h"
#include "ops.h"

#include <limits.h>
#include <unistd.h>

int nn_function(int i) {
	srand(time(NULL));

	//TRAINING
	if (i){
		int number_imgs = 15000;
		Img** imgs = csv_to_imgs("neuralNetwork/data_set_final6.txt", number_imgs);
		//for(int i = 0; i < 15;i++)
		NeuralNetwork* net = network_create(784, 300, 10,0.1);
		network_train_batch_imgs(net, imgs, number_imgs);
		network_save(net, "neuralNetwork/testing_net");
		// PREDICTING
		//imgs_free(imgs,number_imgs);
		//network_free(net);
	} else {
		int number_imgss = 81;
		Img** imgss = csv_to_imgs2("grid_result/grid.txt", number_imgss);

		int a[81]={0};
		NeuralNetwork* neto = network_load("neuralNetwork/testing_net");
		for(int i = 0; i<number_imgss;i++)
		{
			Matrix* res = network_predict_img(neto,imgss[i]);
			printf("Network Predicted %d\n",matrix_argmax(res));
			a[i] = matrix_argmax(res);
		}
		
		for(int i = 0; i < 81;i++)
			printf("a[%i] = %i\n",i,a[i]);

		writeee(a);
	}

	return 0;
}
