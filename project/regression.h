#include <vector>

#define EPOCH 200
#define ALPHA 0.01
#define BETA 0.015

using namespace std;

void regression(vector<vector<double>> source, vector<double> *result) {
	int num_data = source[0].size();
	double w0, w1, err, p, err_acc;
	for (int k = 0; k < num_data; k++) {	// k = [0,11] Month
		
		// Initialize w0, w1, err_acc
		w0 = w1 = err_acc = 0;
		//printf("\nProccessing %d-Month Regression...\n", k+1);
		for (int i = 1; i <= EPOCH; i++) {
			for (int j = 0; j < source.size(); j++) {	// j = [0,] Year
				p = w0 + w1*(j + 1)*BETA;					// year 1 ~
				err = p - source[j][k];					// get err (j-th year k-th month value)
				w0 = w0 - ALPHA*err;
				w1 = w1 - ALPHA*err*(j + 1)*BETA;			// update parameter
				err_acc += err*err;	
				/*if ((k == 0) && ((i % 20) == 0)) {
					printf("MONTH - %d [%d] err - %.5f\n", k + 1, i, err); // show error rate
					printf("Root Mean Square Error : %.5f\n", sqrt(err_acc / (i*(j+1))));
				}*/
			}
			//if(i%200==0) printf("EPOCH[%d] Root Mean Square Error : %.5f\n", i, sqrt(err_acc / (i*source.size())));
		}
		// Push predicted value
		//printf("w0: %.5f\tw1:%.5f\n", w0, w1);
		result->push_back(w0 + w1*(k + 1)*BETA);
	}
}