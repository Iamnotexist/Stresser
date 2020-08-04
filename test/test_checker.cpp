#include <bits/stdc++.h>
#include <Windows.h> //обязательно добавлять эти библиотеки
using namespace std;

#define MAX_PATH_EXE_DIR 1000 //максимальный путь до файла stress.exe - 1 (это тоже нужно вставлять в свой чекер)

int main (int argc, char *argv[]) {

	// копировать это

	char path_exe_dir[MAX_PATH_EXE_DIR];
	GetModuleFileName(0, path_exe_dir, MAX_PATH_EXE_DIR);

	std::string ped = path_exe_dir; //для упрощения работы


	while(ped.back() != '\\')
		ped.pop_back();

	
	ifstream in_solve((ped + "solve_out.txt").c_str(), ios_base::in);
	ifstream in_stress((ped + "stress_out.txt").c_str(), ios_base::in);
	ifstream in_test((ped + "stress_test.txt").c_str(), ios_base::in);
	//  ^
	//  |
	//

	string a, b;

	while (true) {

		if (in_solve.eof() && in_stress.eof()) {
			cout << "\nStress_test_not_found\n";

			in_solve.close();
			in_stress.close();
			in_test.close();
			return 0;
		}

		in_solve >> a;
		in_stress >> b;

		if (a != b) {

			cout << "\n";
			while (getline(in_test, a)) 
				cout << a << "\n";


			in_solve.close();
			in_stress.close();
			in_test.close();
			return 0;
		}
	}

	return 0;
}