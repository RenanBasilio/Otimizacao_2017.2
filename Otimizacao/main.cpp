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

const double GOLDEN_RATIO = (3 - std::sqrt(5)) / 2;

bool isVectorZero(vector<double> vect) {
	if (vect.size() != 4) {
		string err = "Received vector of size " + std::to_string(vect.size()) + ". Expected 4.";
		throw std::invalid_argument(err);
	}

	return (vect[1] == 0 && vect[2] == 0 && vect[3] == 0 && vect[4] == 0);
}

vector<double> multiplicaEscalar(vector<double> vect, double escalar) {

	vector<double> returnVector;

	for (int i = 0; i < vect.size(); i++)
	{
		double value = escalar * vect[i];
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

vector<double> subtraiVectors(vector<double> vect1, vector<double> vect2) {

	if (vect1.size() != vect2.size()) {
		string err = "Vector sizes do not match! vect1 is of size " + std::to_string(vect1.size()) + " while vect2 is of size " + std::to_string(vect2.size()) + ".";
		throw std::invalid_argument(err);
	}

	vector<double> returnVector;

	for (int i = 0; i < vect1.size(); i++)
	{
		double value = vect1[i] - vect2[i];
		returnVector.push_back(value);
	}

	return returnVector;
}

double normaVector(vector<double> vect) {
	double result =
		std::sqrt(
			std::pow(vect[0], 2) +
			std::pow(vect[1], 2) +
			std::pow(vect[2], 2) +
			std::pow(vect[3], 2)
		);

	return result;
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
	// Todas as penalidades tem a mesma fórmula, então podemos utilizar uma única função que aceita qualquer x!

	vector<double> penalidadeDesigualdades = { 0, 0, 0, 0 };
	vector<double> penalidadeIgualdades = { 0, 0, 0, 0 };

	for (int i = 0; i < x.size(); i++)
	{
		if (x[i] < 0) penalidadeDesigualdades[i] += 2 * x[i];
		if (x[i] > 1) penalidadeDesigualdades[i] += (2 * x[i]) - 2;
		penalidadeIgualdades[i] += (2 * x[i]) - (6 * std::pow(x[i], 2)) + (4 * std::pow(x[i], 3));
	}
	double i = ((33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59);
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

double secaoAurea(vector<double> x, vector<double> d, double eps)
{
	vector<double> a = x;
	vector<double> s = somaVectors(x, d);
	vector<double> b = somaVectors(x, multiplicaEscalar(d, 2));
	// Critério de parada:
	// Da segunda restrição, temos que a região viável é [0, 1] x [0, 1] em cada variável.
	// Assim, não faz sentido termos intervalo [a, b] maior do que 2, já que esta é a distância
	// máxima entre quaisquer dois pontos viáveis (norma do vetor [1, 1, 1, 1]).
	while (f(b) < f(s) && normaVector(subtraiVectors(b, a)) > 2) {
		a = s;
		s = b;
		b = multiplicaEscalar(b, 2);
	}

	vector<double> u = somaVectors(a, multiplicaEscalar(subtraiVectors(b, a), GOLDEN_RATIO));
	vector<double> v = somaVectors(a, multiplicaEscalar(subtraiVectors(b, a), (1 - GOLDEN_RATIO)));
	int iteracao = 0;

	// Critério de parada:
	// 100 iterações
	while (normaVector(subtraiVectors(b, a)) > eps) {
		iteracao++;
		if (f(u) < f(v)) {
			b = v;
			v = u;
			u = somaVectors(a, multiplicaEscalar(subtraiVectors(b, a), GOLDEN_RATIO));
		}
		else {
			a = v;
			u = v;
			v = somaVectors(a, multiplicaEscalar(subtraiVectors(b, a), (1 - GOLDEN_RATIO)));
		}
		if (iteracao == 100) cout << "Falha na seção áurea (número máximo de iterações excedido)." << endl;
	}

	cout << "Método da Seção Áurea convergiu com " << iteracao << " iterações." << endl;

	double result = (normaVector(subtraiVectors(a, u)) + normaVector(subtraiVectors(a, v))) / 2;

	cout << "Tamanho do passo é " << result << endl;

	return result;
}

vector<double> DescidaGradiente(vector<double> x0, double startRho, MODE mode) {
	vector<double> x = x0;
	double rho = startRho;
	vector<double> grad = GetGradienteEmX(x, rho);

	while ( !isVectorZero(grad) )
	{
		vector<double> d = multiplicaEscalar(grad, -1);
		// To-Do: Seção áurea / armijo para achar t
		double t;
		switch (mode)
		{
		case SECAO_AUREA:
			t = secaoAurea(x, d, 0.01);
			break;
		case ARMIJO:
			break;
		default:
			break;
		}
		
		x = somaVectors(x, multiplicaEscalar(d, t));
		rho = 3 * rho;

		grad = GetGradienteEmX(x, rho);
	}

	return x;
}

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "");

	vector<double> debug = { 1.0, 1.0, 1.0, 1.0 };
	vector<double> debug2 = { 0.0, 0.0, 0.0, 0.0 };
	vector<double> s1 = subtraiVectors(debug, debug2);
	vector<double> s2 = subtraiVectors(debug2, debug);
	double n1 = normaVector(subtraiVectors(debug, debug2));
	double n2 = normaVector(subtraiVectors(debug2, debug));

	vector<double> var = GetGradienteEmX(debug, 1);
	double var2 = f(debug);

	vector<double> var5 = DescidaGradiente(debug2, 1, MODE::SECAO_AUREA);
	double result = f(var5);

	cin.get();

}