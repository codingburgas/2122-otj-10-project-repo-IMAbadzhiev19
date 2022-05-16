#include<iostream>
#include<string>
#include<vector>

#include"Calculator.h"

using namespace std;

int main()
{
	int a, b;
	cin >> a >> b;

	Calculator calc;
	cout << calc.div(a, b);
}