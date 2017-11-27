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
const double EPSILON = 0.000001;

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

double multiplicaVectors(vector<double> v1, vector<double> v2) {
	if (v1.size() != v2.size()) {
		string err = "Received vectors of different sizes: " + std::to_string(v1.size()) + " and "+ std::to_string(v2.size()) + ". Expected equal sizes.";
		throw std::invalid_argument(err);
	}

	double mult = 0;

	for (int i = 0; i < v1.size(); i++)	{
		mult += (v1[i] * v2[i]);
	}

	return mult;
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

double phi(vector<double> x, double rho) {
	double result =
		(-30.0 * x[0]) + (-10.0 * x[0] * x[1]) + (-2.0 * x[0] * x[2]) + (-3.0 * x[0] * x[3]) +
		(-10.0 * x[1]) + (-10.0 * x[1] * x[2]) + (-10.0 * x[1] * x[3]) +
		(-40.0 * x[2]) + (-1.0 * x[2] * x[3]) +
		(-12.0 * x[3]);

	result += rho * penalidade(x);

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
	double g1 = (33 * x[0]) + (14 * x[1]) + (47 * x[2]) + (11 * x[3]) - 59;
	if (g1 > 0) {
		penalidadeDesigualdades[0] += 66 * g1;
		penalidadeDesigualdades[1] += 28 * g1;
		penalidadeDesigualdades[2] += 94 * g1;
		penalidadeDesigualdades[3] += 22 * g1;
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

double secaoAurea(vector<double> x, double rho, vector<double> d, double eps) {
	// O m�todo de busca pela se��o �urea n�o funciona pois a fun��o n�o � unimodal.

	double a = 0;
	double s = normaVector(d);
	double b = 2 * s;
	// Crit�rio de parada:
	// Da segunda restri��o, temos que a regi�o vi�vel � [0, 1] x [0, 1] em cada vari�vel.
	// Assim, n�o faz sentido termos intervalo [a, b] maior do que 2, j� que esta � a dist�ncia
	// m�xima entre quaisquer dois pontos vi�veis (norma do vetor [1, 1, 1, 1]).

	while (phi(x, rho) < phi(somaVectors(x, multiplicaEscalar(d, s)), rho) && (b - a) < 2) {
		a = s;
		s = b;
		b = 2 * b;
	}

	double u = a + GOLDEN_RATIO * (b - a);
	double v = a + (1 - GOLDEN_RATIO) * (b - a);
	int iteracao = 0;

	// Crit�rio de parada:
	// Tamanho do passo � maior que 2 * eps
	// 100 itera��es
	while (b - a > eps && iteracao < 100) {
		iteracao++;
		// Se f deslocado u na dire��o de d � menor que f deslocado v na dire��o de d
		if (phi(somaVectors(x, somaVectors(x, multiplicaEscalar(d, u))), rho) < 
			phi(somaVectors(x, somaVectors(x, multiplicaEscalar(d, v))), rho)){
			b = v;
			v = u;
			u = a + GOLDEN_RATIO * (b - a);
		}
		else {
			a = v;
			u = v;
			v = a + (1 - GOLDEN_RATIO) * (b - a);
		}
		if (iteracao == 100) cout << "Falha na se��o �urea (n�mero m�ximo de itera��es excedido)." << endl;
	}

	cout << "M�todo da Se��o �urea convergiu com " << iteracao << " itera��es." << endl;

	double result = (u + v) / 2;

	cout << "Tamanho do passo � " << result << endl;

	return result;
}

double armijo(vector<double> x, double rho, vector<double> d, double gama, double eta) {
	if (!(eta > 0 && eta < 1)) {
		string err = "Invalide range for eta " + std::to_string(eta) + ". Expected between 0 and 1.";
		throw std::invalid_argument(err);
	}
	double t = 1;
	vector<double> x_td, nt_grad;
	do {
		for (int i = 0; i < x.size(); i++) {
			x_td[i] = x[i] + t*d[i];
		}
		nt_grad = multiplicaEscalar(GetPenalidadeGradiente(x, rho), eta*t);
		t = gama * t;
	} while ((phi(x_td, rho)) > (phi(x, rho) + multiplicaVectors(nt_grad, d)));
}


vector<double> DescidaGradiente(vector<double> x0, double startRho, MODE mode) {
	vector<double> x = x0;
	double rho = startRho;
	vector<double> grad = GetGradienteEmX(x, rho);
	double t = INFINITY;

	int iteracao = 0;
	// Crit�rio de parada:
	// O ponto gerado pela fun��o deve ser vi�vel, ou seja, penalidade = 0
	// Tamanho do passo � menor que epsilon
	// Mais do que 100 itera��es
	while (!(penalidade(x) == 0 && std::abs(t) >= EPSILON) && iteracao < 100)
	{
		iteracao++;
		vector<double> d = multiplicaEscalar(grad, -1);
		// To-Do: Se��o �urea / armijo para achar t
		switch (mode)
		{
		case SECAO_AUREA:
			t = secaoAurea(x, startRho, d, EPSILON);
			break;
		case ARMIJO:
			t = armijo(x, startRho, d, 0.01);
			break;
		default:
			break;
		}
		
		x = somaVectors(x, multiplicaEscalar(d, t));
		//rho = 3 * rho;

		grad = GetGradienteEmX(x, rho);
	}

	double i = penalidade(x);
	cout << "Descida por gradiente finalizada com penalidade " << i << endl;
	if (i > 0) cout << "N�o foi poss�vel encontrar ponto vi�vel." << endl;
	else {
		cout << "Ponto �timo encontrado: [ "
			<< x[0] << ", "
			<< x[1] << ", "
			<< x[2] << ", "
			<< x[3] << "]" << endl;
		cout << "Gradiente: [ "
			<< grad[0] << ", "
			<< grad[1] << ", "
			<< grad[2] << ", "
			<< grad[3] << "]" << endl << "Valor de f no ponto: " << phi(x, rho);
	}

	return x;
}

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "");

	vector<double> debug = { 1.0, 5.0, 4.0, 1.0 };
	vector<double> debug2 = { 0.0, 0.0, 1.0, 1.0 };
	vector<double> s1 = subtraiVectors(debug, debug2);
	vector<double> s2 = subtraiVectors(debug2, debug);
	double n1 = normaVector(subtraiVectors(debug, debug2));
	double n2 = normaVector(subtraiVectors(debug2, debug));

	vector<double> var = GetGradienteEmX(debug, 1);
	double var2 = phi(debug, 1);

	vector<double> var5 = DescidaGradiente(debug2, 1, MODE::SECAO_AUREA);
	double result = phi(var5, 1);

	cin.get();

}