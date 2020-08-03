#include <iostream>
#include <direct.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>

#include "lib\provod2.h"

#define MAX_PATH_CUR_DIR 1000 //максимальный путь до директории с нужным файлом - 1
#define MAX_PATH_EXE_DIR 1000 //максимальный путь до файла stress.exe - 1

//функция для проверки информации
void _check (provod2 &pr, std::string str);

//функция для выбора файла
void _set_file (provod2 &pr, std::string pice, std::string file, std::string pcd);

//проверка существования файла
bool _file_is_exist (std::string file);

//функция для записи новой информации
void _write_file (provod2 &pr, std::string name_command, int argc, char *argv[], std::string pcd);

//фунция для записи файла для компиляции
void _create_file_to_compile (provod2 &pr, std::string pice, std::string ped, std::string pcd);

//для компиляции одного файла
void _compile_one (provod2 &pr, std::string pice, int argc, char *argv[], std::string ped, std::string pcd);

//для проверки того, что вывел чекер 
bool _check_checker (std::string ped);

//выводит то, что написал чекер
void _get_test (provod2 &pr, std::string ped);

int main (int argc, char *argv[]) {

	if (argc == 1) {
		std::cout << "error_stress: add arguments\n";
		return 0;
	}

	char path_cur_dir[MAX_PATH_CUR_DIR]; //здесь будет хранится путь до директории нужного файла
	_getcwd(path_cur_dir, sizeof(path_cur_dir));

	
	char path_exe_dir[MAX_PATH_EXE_DIR]; //здесь будет хранится путь до директории с исполняемым файлом stress.exe
	GetModuleFileName(0, path_exe_dir, MAX_PATH_EXE_DIR);

	std::string pcd = path_cur_dir, //для упрощения работы
				ped = path_exe_dir;

	//удаляю stress.exe из пути
	while (ped.back() != '\\')
		ped.pop_back();			

	pcd += '\\'; //добавляю \ в путь

	provod2 pr((ped + "sys\\sys.info").c_str()); //открываю файл для чтения информации

	//обрабатываю аргумент check
	if (std::string(argv[1]) == "check") {

		std::cout << "\n";

		_check(pr, "solve"); //вывожу информацию про solve

		_check(pr, "stress_solve"); //про stress-solve

		_check(pr, "generator"); //про generator

		_check(pr, "checker"); //про checker

		_check(pr, "test_count"); //выдает количество стресс тестов

		_check(pr, "test_out"); //куда будет осуществляться вывод контртеста

		pr.write();
		return 0;
	}

	//обрабатываю аргумент test_count
	if (std::string(argv[1]) == "test_count") {

		//проверяю наличие аргумента
		if (argc == 2) {
			std::cout << "error_stress: add arguments\n";

			pr.write();
			return 0;
		}

		//записываю, что колво тестов бесконечно
		if (std::string(argv[2]) == "inf") {
			pr.deleteVertex("test_count");
			pr["test_count"]["inf"];

			pr.write();
			return 0;
		}

		//проверяю корректность аргумента
		bool correct = true;
		int dl = strlen(argv[2]); //длина аргумента

		if (dl >= 8) //если число тестов слишком большое
			correct = false;

		if (correct)
			for (int i = 0; i < dl; ++i)
				if (argv[2][i] < '0' || argv[2][i] > '9')
					correct = false;

		if (!correct) {
			std::cout << "error_stress: incorrect argument\n";

			pr.write();
			return 0;
		}		

		//записываю ифнормацию
		pr.deleteVertex("test_count");
		pr["test_count"][std::string(argv[2])];

		pr.write();
		return 0;
	}

	//установить путь до файла или выполнить с ним какие-то действия
	if (std::string(argv[1]) == "solve") {

		//если нужно скомпилировать отдельный элемент
		if (argc >= 3 && std::string(argv[2]) == "build") {
			//проверяю сущетсвование файла 
			_compile_one(pr, "solve", argc, argv, ped, pcd);

			pr.write();
			return 0;
		}

		//если нужно вывести информацию про этот элемент
		if (argc >= 3 && std::string(argv[2]) == "check") {
			std::cout << "\n";

			_check(pr, "solve"); //вывожу информацию про solve

			pr.write();
			return 0;
		}

		_write_file(pr, "solve", argc, argv, pcd); //вызываю готовую фунцкию

		pr.write();
		return 0;
	}

	if (std::string(argv[1]) == "stress_solve") {

		//если нужно скомпилировать отдельный элемент
		if (argc >= 3 && std::string(argv[2]) == "build") {
			
			_compile_one(pr, "stress_solve", argc, argv, ped, pcd);

			pr.write();
			return 0;
		}

		//если нужно вывести информацию про этот элемент
		if (argc >= 3 && std::string(argv[2]) == "check") {
			std::cout << "\n";

			_check(pr, "stress_solve"); //вывожу информацию про stress_solve

			pr.write();
			return 0;
		}

		_write_file(pr, "stress_solve", argc, argv, pcd);

		pr.write();
		return 0;
	}

	if (std::string(argv[1]) == "generator") {

		//если нужно скомпилировать отдельный элемент
		if (argc >= 3 && std::string(argv[2]) == "build") {
			
			_compile_one(pr, "generator", argc, argv, ped, pcd);

			pr.write();
			return 0;
		}

		//если нужно вывести информацию про этот элемент
		if (argc >= 3 && std::string(argv[2]) == "check") {
			std::cout << "\n";

			_check(pr, "generator"); //вывожу информацию про stress_solve

			pr.write();
			return 0;
		}

		_write_file(pr, "generator", argc, argv, pcd);

		pr.write();
		return 0;
	}

	if (std::string(argv[1]) == "checker") {
		
		if (argc >= 3 && std::string(argv[2]) == "standart") {
			pr.deleteVertex("checker");

			pr["checker"]["standart"];
			return 0;
		}

		//если нужно скомпилировать отдельный элемент
		if (argc >= 3 && std::string(argv[2]) == "build") {
			
			_compile_one(pr, "checker", argc, argv, ped, pcd);

			pr.write();
			return 0;
		}

		//если нужно вывести информацию про этот элемент
		if (argc >= 3 && std::string(argv[2]) == "check") {
			std::cout << "\n";

			_check(pr, "checker"); //вывожу информацию про stress_solve

			pr.write();
			return 0;
		}

		_write_file(pr, "checker", argc, argv, pcd);

		pr.write();
		return 0;
	}


	//почистить информацию
	if (std::string(argv[1]) == "clear") {
		pr.deleteVertex("solve");
		pr.deleteVertex("stress_solve");
		pr.deleteVertex("generator");

		pr.deleteVertex("checker");
		pr["checker"]["standart"];

		pr.deleteVertex("test_count");
		pr["test_count"]["inf"];

		pr.deleteVertex("test_out");
		pr["test_out"]["stdout"];


		pr.write();
		return 0;
	}

	if (std::string(argv[1]) == "build") {
		
		//компилирую каждый файл
		if (pr["solve"].getVertex().size() == 0 || pr["solve"].getVertex()[0] != "OK") //для того, чтобы не компилировать несколько раз
			_compile_one(pr, "solve", argc, argv, ped, pcd);
		if (pr["solve"].getVertex()[0] != "OK") {
			pr.write();
			return 0;
		}

		if (pr["stress_solve"].getVertex().size() == 0 || pr["stress_solve"].getVertex()[0] != "OK")
			_compile_one(pr, "stress_solve", argc, argv, ped, pcd);
		if (pr["stress_solve"].getVertex()[0] != "OK") {
			pr.write();
			return 0;
		}

		if (pr["generator"].getVertex().size() == 0 || pr["generator"].getVertex()[0] != "OK")		
			_compile_one(pr, "generator", argc, argv, ped, pcd);
		if (pr["generator"].getVertex()[0] != "OK") {
			pr.write();
			return 0;
		}

		if (pr["checker"].getVertex().size() == 0 || pr["checker"].getVertex()[0] != "OK") {
			if (pr["checker"].getVertex()[0] != "standart")
				_compile_one(pr, "checker", argc, argv, ped, pcd);
		}

		pr.write();
		return 0;
	}

	if (std::string(argv[1]) == "start") {

		bool inf = (pr["test_count"].getVertex()[0] == "inf"); //следует ли генерировать тесты до нахождения ошибки
		int test_count = 0;
		if (pr["test_count"].getVertex()[0] != "inf")
			test_count += atoi(pr["test_count"].getVertex()[0].c_str()); //кол-во тестов

		//проверяю, что все готово к стресс тестированию
		if (!_file_is_exist(ped + "compile\\solve.exe")) {
			std::cout << "error_stress: solve not exist\n";

			pr.deleteVertex("solve");
			pr["solve"]["error: file not exist"];

			pr.write();
			return 0;
		}

		if (!_file_is_exist(ped + "compile\\stress_solve.exe")) {
			std::cout << "error_stress: stress_solve not exist\n";

			pr.deleteVertex("stress_solve");
			pr["stress_solve"]["error: file not exist"];

			pr.write();
			return 0;
		}

		if (!_file_is_exist(ped + "compile\\generator.exe")) {
			std::cout << "error_stress: generator not exist\n";

			pr.deleteVertex("generator");
			pr["generator"]["error: file not exist"];

			pr.write();
			return 0;
		}

		if (pr["checker"].getVertex()[0] != "standart" && !_file_is_exist(ped + "compile\\checker.exe")) {
			std::cout << "error_checker: checker not exist\n";

			pr.deleteVertex("checker");
			pr["checker"]["error: file not exist"];

			pr.write();
			return 0;
		}

		if (pr["test_out"].getVertex()[0] != "stdout" && !_file_is_exist(pr["test_out"].getVertex()[0])) {
			std::cout << "error_checker: test_out file not exist\n";

			pr.deleteVertex("test_out");
			pr["test_out"]["error: file not exist"];

			pr.write();
			return 0;
		}

		//провожу тестирования по кол-ву тестов или пока не найду стресс тест
		while (inf || test_count--) {

			//генерирую тест
			system((ped + "compile\\generator>" + ped + "compile\\stress_test.txt").c_str());

			//запускаю решение
			system((ped + "compile\\solve>" + ped + "compile\\solve_out.txt<" + 
				ped + "compile\\stress_test.txt").c_str());

			//запускаю стресс-решение
			system((ped + "compile\\stress_solve>" + ped + "compile\\stress_out.txt<" + 
				ped + "compile\\stress_test.txt").c_str());

			//запускаю чекер
			if (pr["checker"].getVertex()[0] == "standart")
				system((ped + "compile\\standart_checker>" + ped + "compile\\checker_out.txt").c_str());
			else 
				system((ped + "compile\\checker>" + ped + "compile\\checker_out.txt").c_str());

			//проверяю вывод
			if (_check_checker(ped)) {
				//если тест найден, вывожу его
				_get_test(pr, ped);
				pr.write();
				return 0;
			}
		}

		//в любом случае что-то вывожу
		_get_test(pr, ped);
		pr.write();
	}


	if (std::string(argv[1]) == "test_out") {
		//проверяю наличие второго агрумента 
		if (argc == 2) {
			std::cout << "error_stress: add arguments\n";

			pr.write();
			return 0;
		}

		//обрабатываю отчистку
		if (std::string(argv[2]) == "clear" || std::string(argv[2]) == "stdout") {
			pr.deleteVertex("test_out");
			pr["test_out"]["stdout"];

			pr.write();
			return 0;
		}

		if (!_file_is_exist(argv[2])) {
			std::cout << "error_stress: incorrect argument or file not exist\n";

			pr.write();
			return 0;
		}

		pr.deleteVertex("test_out");
		pr["test_out"][std::string(argv[2])];

		pr.write();
		return 0;
	}

	return 0;
}


//здесь я определяю некоторые функции

//фукнция для проверки информации о частях стрессера
void _check (provod2 &pr, std::string str) {
	int dl = str.size();
	std::cout << str;
	for (int i = 0; i < 30 - dl; ++i) //чтобы вывод был красивым
		std::cout << " ";

	if (pr[str].getVertex().size() == 0) {
		std::cout << "not exist\n";
		pr.deleteVertex(str);
	}
	else
		std::cout << pr[str].getVertex()[0] << "\n";
}

//функция для записи новой информации 
void _write_file (provod2 &pr, std::string name_command, int argc, char *argv[], std::string pcd) {
	//проверяю наличие аргумента
	if (argc == 2) {
		std::cout << "error_stress: add arguments\n";

		pr.write();
		return;
	}

	int dl = strlen(argv[2]); //длина аргумента

	//проверяю корректность аргумента
	bool correct = true;
	if (std::string(argv[2]) != "clear" && !_file_is_exist(std::string(argv[2])))
		correct = false;

	if (!correct) {
		std::cout << "error_stress: incorrect argument or file not exist\n";

		pr.write();
		return;
	}

	_set_file(pr, argv[1], argv[2], pcd);
}

//установка файла для компиляции
void _set_file (provod2 &pr, std::string pice, std::string file, std::string pcd) {
	pr.deleteVertex(pice); //удаляю предыдущую информацию

	if (file == "clear") {//если поступила команда отчистить информацию

		if (pice == "checker") {
			pr.deleteVertex("checker");
			pr["checker"]["standart"];
			pr.write();
			return;
		}

		pr.deleteVertex(pice);
		pr[pice]["not exist"];

		pr.write();

		return;	
	}

	pcd += file; //получаю полный путь до файла

	pr[pice][pcd]; //записываю информацию

	pr.write();
}

//проверяю, существует ли файл 
bool _file_is_exist (std::string file) {
	std::ifstream _in(file, std::ios_base::in);
	return _in.is_open();
}

//записываю информацию в файл для последующей компиляции
void _create_file_to_compile (provod2 &pr, std::string pice, std::string ped, std::string pcd) {
	//проверяю наличие информации о файле
	if (pr[pice].getVertex().size() == 0) {
		pr[pice]["error_stress: file not exist"];
		return;
	}

	std::ofstream _out((ped + "compile\\" + pice + ".cpp").c_str(), std::ios_base::out | std::ios_base::trunc);
	std::ifstream _in(pr[pice].getVertex()[0].c_str(), std::ios_base::in);

	std::string d; //буффер 

	//копирую файлы, куда мне нужно
	while (getline(_in, d))
		_out << d << "\n";

	_out.close();
	_in.close();

}

//функция для компиляции одного файла
void _compile_one (provod2 &pr, std::string pice, int argc, char *argv[], std::string ped, std::string pcd) {

	//проверяю существование файла
	if (pr[pice].getVertex().size() == 0 || !_file_is_exist(pr[pice].getVertex()[0])) {
		pr.deleteVertex(pice);
		pr[pice]["error: file not exist"];
		pr.write();
		std::cout << "error_stresser: compilation error, use 'stress check'";
		return;
	}

	//удаляю .exe от прошлого файла, если такой был
	if (_file_is_exist(ped + "compile\\" + pice + ".exe")) 
		system((std::string() + "del /Q " + ped + "compile\\" + pice + ".exe").c_str());
			
	//копирую файл для последующей компиляции
	_create_file_to_compile(pr, pice, ped, pcd);

	//компилирую файл 
	system((std::string() + "g++ " + ped + "compile\\" + pice + ".cpp" + " -o " +
		ped + "compile\\" + pice + ".exe").c_str());

	//проверяю, скомпилировался ли файл
	if (!_file_is_exist(ped + "compile\\" + pice + ".exe")) {
		pr.deleteVertex(pice);
		pr[pice]["error: compilation error"];
		pr.write();
		std::cout << "error_stresser: compilation error, use 'stress check'";
		return;
	}
			
	//говорю о том, что компиляция прошла успешно
	pr.deleteVertex(pice);
	pr[pice]["OK"];

	pr.write();
	return;
}

//возвращает true, если тест найдет, false иначе
bool _check_checker (std::string ped) {
	std::ifstream _in((ped + "compile\\checker_out.txt").c_str(), std::ios_base::in);

	std::string d;
	_in >> d;

	_in.close();

	if (d == "Stress_test_not_found")
		return false;

	return true;
}

//выводит то, что написал чекер
void _get_test (provod2 &pr, std::string ped) {

	//открываю файл, в котором содержится информация от чекера
	std::ifstream _in((ped + "compile\\checker_out.txt").c_str(), std::ios_base::in);

	if (pr["test_out"].getVertex()[0] == "stdout") {

		std::string d;
		while (getline(_in, d))
			std::cout << d << "\n";

		_in.close();
	} else {

		//открываю файл для вывода теста
		std::ofstream _out(pr["test_out"].getVertex()[0].c_str(), std::ios_base::out | std::ios_base::trunc);

		std::string d;
		while (getline(_in, d)) 
			_out << d << "\n";

		_in.close();
		_out.close();
	}
}