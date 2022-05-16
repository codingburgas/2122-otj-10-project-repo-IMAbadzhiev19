#include<iostream>
#include<fstream>
#include<vector>

struct User
{
	int id;
	bool isEnabled;
	char name[100];
	std::string surname;
};

int main()
{
	//std::cout << __cplusplus << std::endl;

	std::fstream file("file1.bin", std::ios::binary | std::ios::out | std::ios::in | std::ios::app);

	if (file.fail())
		std::cout << "File failed to open" << std::endl;
	else {

		int a = 65;
		int arr[] = { 65, 66, 67 };
		User u1{ 65, true };
		std::vector<User> users{
			{65, true, "Atanas", new char[400] {"Ivanov"}},
			{66, true, "Dragan", new char[400] {"Petkov"}},
			{67, true, "Pecata", new char[400] {"Pecov"}}
		};

		//file.write(reinterpret_cast<char*>(&a), sizeof(a));
		//file.write(reinterpret_cast<char*>(&arr), sizeof(arr));
		//file.write(reinterpret_cast<char*>(&u1), sizeof(u1));
		file.write(reinterpret_cast<char*>(users.data()), sizeof(users) * sizeof(User));

		file.close();
	}

}