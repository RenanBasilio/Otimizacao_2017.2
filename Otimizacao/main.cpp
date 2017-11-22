#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<double> GetGradienteEmX( vector<double> x ) {
	if ( x.size() != 4 ) {
		string err = "Received vector of size " + std::to_string(x.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	double grad1 = (-3.0) - (10.0 * x[1]) - (2.0 * x[2]) - (3.0 * x[3]);
	double grad2 = (-10.0 * x[0]) - 10.0 - (10.0 * x[2]) - (10.0 * x[3]);
	double grad3 = (-2.0 * x[0]) - (10.0 * x[1]) - 40.0 - x[3];
	double grad4 = (-3.0 * x[0]) - (10.0 * x[1]) - x[2] - 12;

	vector<double> returnVector = { grad1, grad2, grad3, grad4 };

	return returnVector;
}

vector<vector<double>> GetHessiana() {
	// A matriz hessiana é constante, portanto não precisamos de nenhuma entrada para calcula-la.

	vector<vector<double>> hessiana = {
		{   0.0, -10.0,  -2.0,  -3.0 },
		{ -10.0,   0.0, -10.0, -10.0 },
		{  -2.0, -10.0,   0.0,  -1.0 },
		{  -3.0, -10.0,  -1.0,   0.0 }
	};

	return hessiana;

}

int main(int argc, char* args[]) {

	vector<double> debug = { 0.0, 1.0, 5.0, 0.0 };
	
	vector<double> grad = GetGradienteEmX(debug);

	vector<vector<double>> hess = GetHessiana();

	cin.get();

}