#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

void runCase(char[]);
void run();
void backprop(double, double);
double testCases();
double errorAmount(int, int, double);
double activationFunc(double);
char readOut();

//First we have to create the arrays for input, hidden, and output nodes
const int numInputs = 5 * 27;
int input[numInputs];

const int numHidden = 1;
double hidden[numHidden];

const int numOut = 27;
double output[numOut];
double outputError[numHidden][numOut];

double dErrorda;
double dErrordb;

//Need to also store weights
//First is hidden -> inputs so you can multiply each element in the inputs by hidden weights
double inToHidden[numHidden][numInputs];

//First is out -> hidden so you can multiply each element in the inputs by hidden weights
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
	char test[] = {'h','e','l','l','o',' '};
	runCase(test);
	cout << readOut();

//	cout << "The total error is " << testCases() << endl << endl;

}

double activationFunc(double val){
	return 1.0/(1.0+exp(-val));
}

double derivativeOfActivation(double val){
	return -exp(val) * pow((1+exp(val)),-2);
}

void run(){
	//Find weighted sum for the hidden nodes
	for (int hid = 0; hid < numHidden; hid++){
		hidden[hid] = 0;
		for (int in = 0; in < numInputs; in++){
			hidden[hid] += input[in] * inToHidden[hid][in];
		}
		//Applying activation function
		hidden[hid] = activationFunc(hidden[hid]);
	}

	//Find weighted sum for the output nodes
	for (int out = 0; out < numOut; out++){
		output[out] = 0;
		for (int hid = 0; hid < numHidden; hid++){
			output[out] += hidden[hid] * hiddenToOut[out][hid];
		}
		output[out] = activationFunc(output[out]);
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
	outputError[0][0] = dErrorda * derivativeOfActivation(output[0]);

	//Finding f(b-db)
	inToHidden[0][1] -= db;

	run();



	double dbOut = errorAmount(input[0], input[1],output[0]);

	dErrordb = (out - dbOut) / db;
	cout << "dError/db " << dErrordb << endl;
	outputError[1][0] = dErrordb * derivativeOfActivation(output[0]);

}

void backprop(double dErrorda, double dErrordb){
	inToHidden[0][0] -= dErrorda;
	inToHidden[0][1] -= dErrordb;
}

//TODO Change when the goal changes
double errorAmount(int in1, int in2, double guess){
	if (in1 == in2 && in1 == 1){
//		return abs(1 - guess);
		return 1 - guess;
	}
	return guess;
}

//Function to set the appropriate inputs for the the neural network
void setInputs(char previous[numInputs/27]){
	//Refreshing input to all 0's
	for (int i = 0; i < numInputs; i++){
		input[i] = 0;
	}
	//Adding a 1 for the character in the appropriate input
	for (int i = 0; i < numInputs/27; i++){
		char in = tolower(previous[i]);
		if (in == ' '){
			in = ((int) 'z')+1;
		}
		input[(i*27)+(in-97)] = 1;
	}
}

//Function to read the output of the neural network
char readOut(){
	double greatest = output[0];
	int greatestIndex = 0;
	for (int i = 0; i < numOut; i++){
		if (output[i] > greatest){
			greatest = output[i];
			greatestIndex = i;
		}
	}
	return greatestIndex+97;
}

//Takes in the previous characters and the current character it should get
void runCase(char chars[1+(numInputs/27)]){
	char expected = chars[numInputs/27];
	char previous[numInputs/27];
	for (int i = 0; i < numInputs/27; i++){
		previous[i] = chars[i];
	}
	setInputs(previous);
	run();
}

double testCases(){
	//TODO Change tests
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
