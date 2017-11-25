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

enum MODE { SECAO_AUREA, ARMIJO };

bool isVectorZero(vector<double> vect) {
	if (vect.size() != 4) {
		string err = "Received vector of size " + std::to_string(vect.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	return (vect[1] == 0 && vect[2] == 0 && vect[3] == 0 && vect[4] == 0);
}

vector<double> multiplicaEscalar(vector<double> vect, double value) {

	vector<double> returnVector;

	for (int i = 0; i < vect.size(); i++)
	{
		double value = value * vect[i];
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

double GetPenalidadeGradiente( double x, double rho, double rhoP) {
	// Todas as penalidades tem a mesma fórmula, então podemos utilizar uma única função que aceita qualquer x!

	double penalidadeIgualdades = -((rhoP) / (std::pow(x, 2))) - ((rhoP) / (std::pow(x, 2) - (2 * x) + 1));

	double penalidadeDesigualdades = (2 * rho * x) - (6 * rho * std::pow(x, 2)) + (4 * rho * std::pow(x, 3));

	return penalidadeIgualdades + penalidadeDesigualdades;
}

vector<double> GetGradienteEmX( vector<double> x , double rho, double rhoP) {
	if ( x.size() != 4 ) {
		string err = "Received vector of size " + std::to_string(x.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	double grad1 = (-3.0) - (10.0 * x[1]) - (2.0 * x[2]) - (3.0 * x[3]) + GetPenalidadeGradiente(x[1], rho, rhoP);
	double grad2 = (-10.0 * x[0]) - 10.0 - (10.0 * x[2]) - (10.0 * x[3]) + GetPenalidadeGradiente(x[2], rho, rhoP);
	double grad3 = (-2.0 * x[0]) - (10.0 * x[1]) - 40.0 - x[3] + GetPenalidadeGradiente(x[3], rho, rhoP);
	double grad4 = (-3.0 * x[0]) - (10.0 * x[1]) - x[2] - 12.0 + GetPenalidadeGradiente(x[4], rho, rhoP);

	vector<double> returnVector = { grad1, grad2, grad3, grad4 };

	return returnVector;
}

vector<double> DescidaGradiente(vector<double> x0, double startRho, double startRhoP, MODE mode) {
	vector<double> x = x0;
	double rho = startRho;
	double rhoP = startRhoP;
	vector<double> grad = GetGradienteEmX(x, rho, rhoP);

	while ( !isVectorZero(grad) )
	{
		vector<double> d = multiplicaEscalar(grad, -1);
		// To-Do: Seção áurea / armijo para achar t
		double t = 1;
		x = somaVectors(grad, multiplicaEscalar(d, t));
	}
}

int main(int argc, char* args[]) {

	vector<double> debug = { 0.0, 1.0, 5.0, 0.0 };

	cin.get();

}