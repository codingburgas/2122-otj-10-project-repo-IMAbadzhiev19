#include<iostream>
#include<fstream>
#include<vector>

struct User
{
	int id;
	bool isEnabled;
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
			{65, true},
			{66, true},
			{67, true}
		};

		//file.write(reinterpret_cast<char*>(&a), sizeof(a));
		//file.write(reinterpret_cast<char*>(&arr), sizeof(arr));
		//file.write(reinterpret_cast<char*>(&u1), sizeof(u1));
		file.write(reinterpret_cast<char*>(users.data()), sizeof(users));

		file.close();
	}

}