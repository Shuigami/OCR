#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/img.h"
#include "neural/activations.h"
#include "neural/nn.h"
#include "matrix/matrix.h"
#include "matrix/ops.h"

int main() {
	srand(time(NULL));

	//TRAINING
	
	/*int number_imgs = 15000;
	Img** imgs = csv_to_imgs("data_set_final2.txt", number_imgs);
	//for(int i = 0; i < 15;i++)
	NeuralNetwork* net = network_create(784, 300, 10,0.1);
	network_train_batch_imgs(net, imgs, number_imgs);
	network_save(net, "testing_net");
	// PREDICTING
	//imgs_free(imgs,number_imgs);
	//network_free(net);

*/	int number_imgss = 81;
	Img** imgss = csv_to_imgs2("testsudo4.txt", number_imgss);
	//img_print(imgss[0]);
	int a[81]={0};
	NeuralNetwork* neto = network_load("testing_net");
	for(int i = 0; i<number_imgss;i++)
	{
		//img_print(imgss[i]);
		Matrix* res = network_predict_img(neto,imgss[i]);
		printf("Network Predicted %d\n",matrix_argmax(res));
		a[i] = matrix_argmax(res);
	}
	for(int i = 0; i < 81;i++)
		printf("a[%i] = %i\n",i,a[i]);

	writeee(a,"//home/epita/mnist-from-scratch-master/lorenzoolemanchooo");
	//a[0] = matrix_argmax(res);
	//printf("Network Predicted %d\n",matrix_argmax(res));
	//}
	//double score = network_predict_imgs(net, imgss, 81);
	//printf("Score: %1.5f", score);

	//imgs_free(imgss, number_imgss);
	//network_free(neto);
	return 0;
}
