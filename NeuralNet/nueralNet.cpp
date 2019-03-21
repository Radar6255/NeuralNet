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
double hidden[1];
double output[1];

double dErrorda;
double dErrordb;

//Need to also store weights
//First is input nodes then to hidden
double inToHidden[2][1];
double hiddenToOut[1];

int main(){
	srand(time(NULL));

	cout << "Start:"<<endl;

	//Set to random values
	inToHidden[0][0] = rand()/((double) RAND_MAX);
	inToHidden[1][0] = rand()/((double) RAND_MAX);
	hiddenToOut[0] = rand()/((double) RAND_MAX);

	cout << "The total error is " << testCases() << endl << endl;


	backprop(dErrorda, dErrordb);
	cout << "After back-propagation the error total is " << testCases();;

}

void run(){
	//Find weighted sum for the hidden node
	double sum = 0;
	for (int i = 0; i < 2; i++){
		sum += input[i] * inToHidden[i][0];
	}
	hidden[0] = sum;

	output[0] = hidden[0] * hiddenToOut[0];
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
