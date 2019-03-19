#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void run();
void backprop();

//First we have to create the arrays for input, hidden, and output nodes
int input[2];
double hidden[1];
double output[1];

//Need to also store weights
//First is input nodes then to hidden
double inToHidden[2][1];
double hiddenToOut[1];

int main(){
	srand(time(NULL));

	//Set to random values
	inToHidden[0][0] = rand()/((double) RAND_MAX);
	inToHidden[1][0] = rand()/((double) RAND_MAX);
	hiddenToOut[0] = rand()/((double) RAND_MAX);

	cout << "Printing all combinations\n";
	input[0] = 1;
	input[1] = 1;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;

	input[0] = 1;
	input[1] = 0;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;

	input[0] = 0;
	input[1] = 1;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;

	input[0] = 0;
	input[1] = 0;
	cout << "Trying: " << input[0] << " " << input[1] << endl;
	run();
	cout << output[0] << endl << endl;


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

void backprop(){

}
