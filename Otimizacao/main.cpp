#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

enum MODE { SECAO_AUREA, ARMIJO };

bool isVectorZero(vector<double> vect) {
	if (vect.size() != 4) {
		string err = "Received vector of size " + std::to_string(vect.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	return (vect[1] == 0 && vect[2] == 0 && vect[3] == 0 && vect[4] == 0);
}

vector<double> multiplicaEscalar(vector<double> vect, double scalar) {

	vector<double> returnVector;

	for (int i = 0; i < vect.size(); i++)
	{
		double value = scalar * vect[i];
		returnVector.push_back(value);
	}

	return returnVector;
}

vector<double> somaVectors(vector<double> vect1, vector<double> vect2) {

	if (vect1.size() != vect2.size()) {
		string err = "Vector sizes do not match! vect1 is of size " + std::to_string(vect1.size()) + " while vect2 is of size " + std::to_string(vect2.size()) + ".";
		throw std::invalid_argument(err);
	}

	vector<double> returnVector;

	for (int i = 0; i < vect1.size(); i++)
	{
		double value = vect1[i] + vect2[i];
		returnVector.push_back(value);
	}

	return returnVector;
}

double penalidade(vector<double> x) {
	double penalidadeIgualdades =
		std::pow(x[0], 2) - (2 * std::pow(x[0], 3)) + std::pow(x[0], 4) +
		std::pow(x[1], 2) - (2 * std::pow(x[1], 3)) + std::pow(x[1], 4) +
		std::pow(x[2], 2) - (2 * std::pow(x[2], 3)) + std::pow(x[2], 4) +
		std::pow(x[3], 2) - (2 * std::pow(x[3], 3)) + std::pow(x[3], 4);

	double penalidadeDesigualdades =
		std::pow(std::max(0.0, (-1)*x[0]), 2) + std::pow(std::max(0.0, (x[0] - 1)), 2) +
		std::pow(std::max(0.0, (-1)*x[1]), 2) + std::pow(std::max(0.0, (x[1] - 1)), 2) +
		std::pow(std::max(0.0, (-1)*x[2]), 2) + std::pow(std::max(0.0, (x[2] - 1)), 2) +
		std::pow(std::max(0.0, (-1)*x[3]), 2) + std::pow(std::max(0.0, (x[3] - 1)), 2) +
		std::pow(std::max(0.0, ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59)), 2);

	double result = penalidadeIgualdades + penalidadeDesigualdades;

	return result;
}

double f(vector<double> x) {
	double result =
		(-30.0 * x[0]) + (-10.0 * x[0] * x[1]) + (-2.0 * x[0] * x[2]) + (-3.0 * x[0] * x[3]) +
		(-10.0 * x[1]) + (-10.0 * x[1] * x[2]) + (-10.0 * x[1] * x[3]) +
		(-40.0 * x[2]) + (-1.0 * x[2] * x[3]) +
		(-12.0 * x[3]);

	result += penalidade(x);

	return result;
}

vector<double> GetPenalidadeGradiente( vector<double> x, double rho) {
	// Todas as penalidades tem a mesma f�rmula, ent�o podemos utilizar uma �nica fun��o que aceita qualquer x!

	vector<double> penalidadeDesigualdades = { 0, 0, 0, 0 };
	vector<double> penalidadeIgualdades = { 0, 0, 0, 0 };

	for (int i = 0; i < x.size(); i++)
	{
		if (x[i] < 0) penalidadeDesigualdades[i] += 2 * x[i];
		if (x[i] > 1) penalidadeDesigualdades[i] += (2 * x[i]) - 2;
		penalidadeIgualdades[i] += (2 * x[i]) - (6 * std::pow(x[i], 2)) + (4 * std::pow(x[i], 3));
	}
	if (((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59) > 0) {
		penalidadeDesigualdades[0] = 66 * ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59);
		penalidadeDesigualdades[1] = 28 * ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59);
		penalidadeDesigualdades[2] = 94 * ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59);
		penalidadeDesigualdades[3] = 22 * ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59);
	}

	vector<double> result = multiplicaEscalar(somaVectors(penalidadeIgualdades, penalidadeDesigualdades), rho);

	return result;
}

vector<double> GetGradienteEmX( vector<double> x , double rho) {
	if ( x.size() != 4 ) {
		string err = "Received vector of size " + std::to_string(x.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	vector<double> grad = { 0, 0, 0, 0 };

	grad[0] = (-30.0) - (10.0 * x[1]) - (2.0 * x[2]) - (3.0 * x[3]);
	grad[1] = (-10.0 * x[0]) - 10.0 - (10.0 * x[2]) - (10.0 * x[3]);
	grad[2] = (-2.0 * x[0]) - (10.0 * x[1]) - 40.0 - x[3];
	grad[3] = (-3.0 * x[0]) - (10.0 * x[1]) - x[2] - 12.0;

	vector<double> penalidade = GetPenalidadeGradiente(x, rho);

	vector<double> returnVector = somaVectors(grad, penalidade);

	return returnVector;
}

vector<double> DescidaGradiente(vector<double> x0, double startRho, MODE mode) {
	vector<double> x = x0;
	double rho = startRho;
	vector<double> grad = GetGradienteEmX(x, rho);

	while ( !isVectorZero(grad) )
	{
		vector<double> d = multiplicaEscalar(grad, -1);
		// To-Do: Se��o �urea / armijo para achar t
		double t = 0.5;
		
		x = somaVectors(x, multiplicaEscalar(d, t));
		rho = 3 * rho;

		grad = GetGradienteEmX(x, rho);
	}

	return x;
}

int main(int argc, char* args[]) {

	vector<double> debug = { 1.0, 1.0, 1.0, 1.0 };

	vector<double> var = GetGradienteEmX(debug, 1);
	double var2 = f(debug);

	vector<double> var5 = DescidaGradiente(debug, 1, MODE::ARMIJO);

	cin.get();

}