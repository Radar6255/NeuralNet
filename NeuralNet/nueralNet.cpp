#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

void run();
void backprop(double, double);
double testCases();
double errorAmount(int, int, double);

//First we have to create the arrays for input, hidden, and output nodes
int input[2];
const int numInputs = 2;
double hidden[1];
const int numHidden = 1;
double output[1];
const int numOut = 1;

double dErrorda;
double dErrordb;

//Need to also store weights
//First is hidden -> inputs so you can multiply each element in the inputs by hidden weights
double inToHidden[numHidden][numInputs];

//First is hidden -> inputs so you can multiply each element in the inputs by hidden weights
double hiddenToOut[numOut][numHidden];

int main(){
	srand(time(NULL));

	cout << "Start:"<<endl;

	//Setting weights to random values
	for (int hid = 0; hid < numHidden; hid++){
		for (int in = 0; in < numInputs; in++){
			inToHidden[hid][in] = rand()/((double) RAND_MAX);
		}
	}

	for (int out = 0; out < numOut; out++){
		for (int hid = 0; hid < numHidden; hid++){
			hiddenToOut[out][hid] = rand()/((double) RAND_MAX);
		}
	}

	cout << "The total error is " << testCases() << endl << endl;


	backprop(dErrorda, dErrordb);
	cout << "After back-propagation the error total is " << testCases();;

}

void run(){
	//TODO Implement activation function
	//Find weighted sum for the hidden nodes
	for (int hid = 0; hid < numHidden; hid++){
		hidden[hid] = 0;
		for (int in = 0; in < numInputs; in++){
			hidden[hid] += input[hid] * inToHidden[hid][in];
		}
	}

	//Find weighted sum for the output nodes
	for (int out = 0; out < numHidden; out++){
		output[out] = 0;
		for (int hid = 0; hid < numInputs; hid++){
			output[out] += input[out] * inToHidden[out][hid];
		}
	}
}
void findErrorInCases(){
	input[0] = 1;
	input[1] = 1;

	double da = 0.01;
	double db = 0.01;

	//Finding f(a)
	run();
	double out = errorAmount(input[0], input[1],output[0]);

	//Finding f(a-da)
	inToHidden[0][0] -= da;
	run();
	double daOut = errorAmount(input[0], input[1],output[0]);

	//Finding dOut/da
	dErrorda = (out - daOut) / da;
	cout << "dError/da " << dErrorda << endl;

	//Finding f(b-db)
	inToHidden[0][1] -= db;

	run();

	double dbOut = errorAmount(input[0], input[1],output[0]);

	dErrordb = (out - dbOut) / db;
	cout << "dError/db " << dErrordb << endl;

}


void backprop(double dErrorda, double dErrordb){
	inToHidden[0][0] -= dErrorda;
	inToHidden[0][1] -= dErrordb;
}

double errorAmount(int in1, int in2, double guess){
	if (in1 == in2 && in1 == 1){
//		return abs(1 - guess);
		return 1 - guess;
	}
	return guess;
}


double testCases(){
	double errorSum = 0;

	cout << endl << "Printing all combinations\n";
	input[0] = 1;
	input[1] = 1;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;
	errorSum += errorAmount(1,1,output[0]);

	input[0] = 1;
	input[1] = 0;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;
	errorSum += errorAmount(1,0,output[0]);

	input[0] = 0;
	input[1] = 1;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;
	errorSum += errorAmount(0,1,output[0]);

	input[0] = 0;
	input[1] = 0;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;
	errorSum += errorAmount(0,0,output[0]);

	return errorSum;
}
