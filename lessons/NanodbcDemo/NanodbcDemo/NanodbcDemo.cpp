#include<iostream>
#include<string>
#include"../nanodbc/nanodbc.h"

using namespace std;

template<typename T>
struct Node
{
	T data;
	Node* next;
};

template<typename T>
T sum(T a, T b)
{
	return a + b;
}

template<>
const char* sum(const char* a, const char* b)
{
	return a;
}

void demoMain()
{
	Node<int> node1;
	Node<float> node2;

	int a = sum<int>(5, 6);
	float b = sum<float>(5.0, 6.5);
}

int main() try
{
    auto const connstr = NANODBC_TEXT("Driver={ODBC Driver 17 for SQL Server};Server=.\\SQLExpress;Database=Management;Trusted_Connection=yes;"); // an ODBC connection string to your database
    nanodbc::connection conn(connstr);

    string name;

    cout << "Enter name: ";
    cin >> name;


    // Vulnerrable to SQL Injection!
    // YOU MUST USE PREPARED STATEMENTS!!!
    string query = NANODBC_TEXT("SELECT * FROM Tasks");

    auto result = nanodbc::execute(conn, query);
    while (result.next())
    {
        auto id = result.get<int>(0);
        auto firstName = result.get<std::string>(1);

        std::cout << id << ", " << firstName << "\n";
    }
    return EXIT_SUCCESS;
}
catch (std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}