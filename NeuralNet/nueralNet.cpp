#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

void runCase(char[]);
void run();
//double testCases();
void errorAmount(char);
double activationFunc(double);
char readOut();
void printArray(double[], int);
void fileInput(char*);
void backprop(char);



//First we have to create the arrays for input, hidden, and output nodes
const int numInputs = 5 * 27;
int input[numInputs];

const int numHidden = 10;
double hidden[numHidden];

//double hiddenNodes[][];

const int numOut = 27;
double output[numOut];
double outputError[numOut];

double totalError = 0;
int numChars = 0;

double dErrorda;
double dErrordb;

ifstream fileIn;

//Also need to store weights
//First is hidden -> inputs so you can multiply each element in the inputs by hidden weights
double inToHidden[numHidden][numInputs];

//First is out -> hidden so you can multiply each element in the inputs by hidden weights
double hiddenToOut[numOut][numHidden];

//Array to store which characters the network has
char inputChars[(numInputs/27)+1];
int main(){
	fileIn.open("inputData.txt");

	srand(time(NULL));
//	srand(314159);

	cout << "Start:"<<endl;

	//Setting weights to random values
	//Using He-et-al Initialization
	for (int hid = 0; hid < numHidden; hid++){
		for (int in = 0; in < numInputs; in++){
//			inToHidden[hid][in] = rand()/(2.5*(double) RAND_MAX);
			inToHidden[hid][in] = ((double)(numInputs-numHidden)*(rand()/(double)RAND_MAX)+numInputs)*sqrt(2.0/numInputs);
		}
	}for (int out = 0; out < numOut; out++){
		for (int hid = 0; hid < numHidden; hid++){
//			hiddenToOut[out][hid] = rand()/(2.5*(double) RAND_MAX);
			hiddenToOut[out][hid] = ((double)(numHidden-numOut)*(rand()/(double)RAND_MAX)+numHidden)*sqrt(2.0/numHidden);
		}
	}

	//Filling input characters with spaces to begin with
	for (int i = 0; i < (numInputs/27)+1; i++){
		inputChars[i] = ' ';
	}
	while (fileIn.good()){
		fileInput(inputChars);
		runCase(inputChars);
		numChars++;
		cout << readOut();
	}cout << "Total error for the run is " << totalError;

//	cout << "The total error is " << testCases() << endl << endl;

}

//Activation function for nodes
double activationFunc(double val){
	return 1.0/(1.0+exp(-val));
}
//Derivative of the activation function for the nodes
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
void errorAmount(char expected){
	for (int i = 0; i < numOut; i++){
		if (i == expected-97){
			outputError[i] = 1-output[i];
		}else{
			outputError[i] = output[i];
		}
	}
}

//Function to set the appropriate inputs for the the neural network
void setInputs(char previous[numInputs/27]){
	//Refreshing input to all 0's
	for (int i = 0; i < numInputs; i++){
		input[i] = 0;
	}
	//Adding a 1 for the character in the appropriate input
	for (int i = 0; i < numInputs/27; i++){
		char in = previous[i];
		if (in == ' '){
			in = ((int) 'z')+1;
		}
//		cout << ((i*27)+(in-97))<< ", ";
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
	}if (greatestIndex == ((int)'z')-96){
		return ' ';
	}return greatestIndex+97;
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
	backprop(expected);
}

//Gets next char to be guessed on
//Requires file to be opened
void fileInput(char* inputChars){
	for (int i = 0; i < (numInputs/27); i++){
		*(inputChars+i)= *(inputChars+i+1);
	}
	while (true && fileIn.good()){
		char temp = fileIn.get();
		if (isalpha(temp)){
			*(inputChars+(numInputs/27)) = tolower(temp);
			break;
		}if (temp == '\n'){
			cout << totalError/numChars << endl;
			numChars = 0;
			totalError = 0;
		}
	}
}

void printArray(double array[], int size){
	for (int i = 0; i < size; i++){
		cout << array[i] << ", ";
	}cout << endl;
}

void backprop(char expected){
	errorAmount(expected);
	double runError = 0;
	for (int i = 0; i < numOut; i++){
		runError += outputError[i];
	}totalError += runError;
	for (int hid = 0; hid < numHidden; hid++){
			//Derivative of the activation function at the current output
			double dAct = derivativeOfActivation(hidden[hid]);
			for (int in = 0; in < numInputs; in++){
				//dW is the change in the weight
				double dW = 0.1;
				inToHidden[hid][in] += dW;
				run();
				errorAmount(expected);
				double changeError = 0;
				for (int i = 0; i < numOut; i++){
					changeError += outputError[i];
				}
				//dErrorDW is the change in error due to W
				double dErrorDW = (changeError - runError) / dW;
				//Change the weight back
				inToHidden[hid][in] -= dW;

				//Scaling factor
				double changeScale = 0.01;
				//Adjust the weight to decrease the error
				inToHidden[hid][in] -= (dErrorDW * changeScale);
			}
		}
	for (int out = 0; out < numOut; out++){
		//Derivative of the activation function at the current output
		double dAct = derivativeOfActivation(output[out]);
		for (int hid = 0; hid < numHidden; hid++){
			//dW is the change in the weight
			double dW = 0.1;
			hiddenToOut[out][hid] += dW;
			run();
			errorAmount(expected);
			double changeError = 0;
			for (int i = 0; i < numOut; i++){
				changeError += outputError[i];
			}
			//dErrorDW is the change in error due to W
			double dErrorDW = (changeError - runError) / dW;
			//Change the weight back
			hiddenToOut[out][hid] -= dW;

			//Scaling factor
			double changeScale = 0.01;
			//Adjust the weight to decrease the error
			hiddenToOut[out][hid] -= (dErrorDW * changeScale);
		}
	}

}
