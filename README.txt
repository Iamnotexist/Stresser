Это руководство по программе Stresser
Создатель: Артемий Морозов
Телеграмм: @tokreal

Для компиляции необходим MinGw (C++ 14 и выше).
Программа работает лишь в Windows 10.
Для компиляции достаточно запустить build.bat
Перед этим необходимо добавить в переменную середы путь до g++ (D:\MinGw\bin)
Пользоваться программой нужно из консоли

В проекте есть сторонние библиотеки:

https://github.com/Iamnotexist/provod2

Stresser - легкая программа, помогающая проводить стресс-тестирование программы. Перед использованием необходимо добавить в переменную среды путь до stress.exe
(D:\Stresser\stress.exe, этот файл появится после запуска build.bat)

Информация про test:

	Пусть есть простая задача:

		дано 2 числа a, b.
		требуется вывести их раность

	В качестве примера некоректного решения я написал программу, которая выводит сумму двух чисел
	В качестве стресс-решения я реализовал правильно работающую программу
	Такую простую задачу я использую для того, чтобы проще показать, как работает Stresser


Информация про написание solve, stress_solve, generator:

	solve - программа должна считывать информацию из стандартного потока ввода, и выводить в стандартный поток вывода
	stress_solve - аналогично 
	generator - должен генерировать один тест, корректный для solve и stress_solve и выводить его в стандартный поток вывода	 		


Инструкция по использованию:

	(Стресс-тестирование - поиск стресс-теста
	 контр-тест - входные данные, на которых стресс-решение работает корректно, а решение - нет
	 стресс-тест - тест, который генерируется во время стресс тестирования
	 стресс-решение - корректное, но обычное неоптимальное решение
	 решение - некорректное, но обычно оптимальное решение)

	После того, как действия, указанные выше, были выполнены, программа готова к работе.

	Программа имеет большой функционал. Для того, чтобы получить информацию о статусе программы, необходимо ввести команду:

		stress check

	В консоль должна вывестись следующая информация:

		solve                         not exist
		stress_solve                  not exist
		generator                     not exist
		checker                       standart
		test_count                    inf
		test_out                      stdout

	Первая строчка говорит о том, что файл для компиляции решения, которое нужно стресс-тестировать, не определен. 
	Аналогично для второй и третьей строчки, однако во второй строчке содержится информация про стресс-решение, а в третьей - про генератор тестов.
	В четвертой строчке содержится информация про состояния чекера. Программа Stresser поддерживает пользовательские чекеры (об этом чуть позже), так же 
		в программе присутствует стандартный чекер. С его иходным кодом можно ознакомится compile\standart_checker.cpp
	Пятая строка содержит информацию о том, сколько стресс-тестов сгенерирует программа. Если inf, то стресс-тестирование будет проводится до тех пор, пока
		контр-тест не будет найден (если программа будет работать слишком долго, Ctrl + C прервет исполнение). 
	Шестая строчка содержит информацию о том, куда поступит информация после окончания стресс-тестирования. Если stdout - в консоль, иначе в файл, который вы выберите.

	Чтобы выбрать файл для компиляции, достаточно в консоли дойти до директории с файлом и вызвать соответсвующую команду.

	Например, чтобы выбрать файл test\test_solve.cpp в качестве решения, достаточно перейти в директорию test и вызвать следующую комунду:

		stress solve test_solve.cpp

	Если после исполнения мы возовем команду stress check, то, в моем случае, получим следующую информацию:
	
		solve                         D:\My_projects\Stresser\test\test_solve.cpp
		stress_solve                  not exist
		generator                     not exist
		checker                       standart
		test_count                    inf
		test_out                      stdout

	Как можно заметить, мы задали файл для компиляции для решения. Стоит отметить, что нельзя выбирать файл, содержащий в названии пробелы.
	Так же проверить то, что файл был выбран успешно, можно следующей командой:

		stress solve check 

	В моем случае выведется следующая информация:
	
		solve                         D:\My_projects\Stresser\test\test_solve.cpp

	То есть, выведется информация лишь о том, что мне нужно.
	
	В общем виде эта команда выглядит так:

		stress $x$ check 

	Здесь вместо $x$ можно писать solve, stress_solve, generator, checker, test_count, test_out.
	
	Итак, когда мы выбрали файл для компиляции решения, выберем файлы для компиляции стресс-решения и генератора.

		stress stress_solve test_stress_solve.cpp
		stress generator test_generator.cpp

	Посмотрим, что выведет программа.
	
		stress check

	Вывод:
	
		solve                         D:\My_projects\Stresser\test\test_solve.cpp
		stress_solve                  D:\My_projects\Stresser\test\test_solve.cpp
		generator                     D:\My_projects\Stresser\test\test_generator.cpp
		checker                       standart
		test_count                    inf
		test_out                      stdout

	Все произшло так, как и рассчитывалось. Были заданы файлы для компиляции. Теперь осталось дать программе их скомпилировать.
	Для этого можно вызвать команду вида:

		stress $x$ build

	Здесь вместо $x$ можно писать solve, stress_solve, generator, checker (только в том случае, если checker нестандартный)
	Программа скомпилирует $x$ и подготовит .exe файл к стресс-тестировнию.

	Скомпилируем solve и посмотрим на результат

		stress solve build
		stress solve check

	Вывод:
	
		solve                         OK

	OK означает, что файл успешно скомпилировался. Если бы файл не скомпилировался, то ошибки компиляции вывелись бы в консоль, а OK бы отсутствал.
	Проверим это.

	Я специально уберу в в 9-ой строке файла test\test_solve.cpp знак ';' После этого попробую повторить предыдущие команды.

		stress solve test_solve.cpp
		stress solve build

	Как и ожидалось, в консоль была выведена информация об ошибке:
	
		D:\My_projects\Stresser\compile\solve.cpp:9:15: error: expected ';' before 'return'

	Здесь написино, что ошибка найдена в файле solve.cpp
	Это связано с алгоритмом работы программы. 
	Сначала информация из файла test\test_solve.cpp копируется в файл compile\solve.cpp 
	Далее происходит компиляция файла compile\solve.cpp

	Аналогично работает компиляция для stress_solve, generator, checker.

	Если написать команду stress check, можно заметить, что информация в статусе программы изменилась

		stress solve check 

	Вывод:
	
		solve                         error: compilation error

	Это говорит о том, что решение нужно скомпилировать заново	

	Я верну знак ';' и скомпилирую solve	

		stress solve test_solve.cpp
		stress solve build
	
	Так же я скомпилирую остальные файлы. Для того, чтобы не компилировать все файлы по отдельности, я воспользуюсь командой:

		stress build

	Эта команда будет поочереди компилировать еще не скомпилированные файлы, аналогично команде stress $x$ build
	
		stress build 
		stress check

	Вывод:
	
		solve                         OK
		stress_solve                  OK
		generator                     OK
		checker                       standart
		test_count                    inf
		test_out                      stdout

	Для данной задачи, нам подойдет стандартный checker.
	
	Попробуем запустить стресс тестирование:

		stress start

	В моем случае, контр-тест был найден доастаточно быстро:
	
		50 71

	Действительно, этот тест является контр-тестом, так-как (50 - 71) != (50 + 71)

	Давайте немного изменим генератор и статус программы

		stress test_count 100

	Этой командой я устанавливаю, что во время стресс-тестирования будет генерироваться всего лишь 100 стресс-тестов (либо меньше, если контр-тест был найден раньше)
	
	В файле test\test_generator.cpp я изменю строку:

		cout << rand() % 100 << " " << rand() % 100;

	На строку:
	
		cout << 0 << " " << 0;

	Понятно, что такой генератор будет выдавать лишь тест 0 0, который не ялвяется стресс-тестом			 	

	Проверим, как пройдет стресс тестирование, но перед этим, так как файл test\test_generator.cpp был изменен, его следует перекомпилировать.

		stress generator test_generator.cpp
		stress build 
		stress start

	Вывод:

		Stress_test_not_found

	Ожидаемый результат.
	
	Верну test_generator.cpp в исходное состояние.

	Если требуется обнулить информацию о чем-то, можно воспользоваться командой stress $x$ clear.
	Вместо $x$ можно писать solve, stress_solve, generator, checker (сделает стандартным), test_count (сделает бесконечным), test_out (запишет stdout).

	Так же существует команда stress clear, которая применит к каждому функцию stress $x$ clear

	Стоит отметить, что в программе стрессер есть аналоги clear.

		stress test_count inf
		stress checker standart
		stress test_out stdout

	Эти функции отработают абсолютно так же, как и stress $x$ clear.
	
	Итак, это были все команды.

	Выпишу их еще раз (далее $y$ - название файла, $z$ - целое неотрицательное число <= 9 999 999):

		stress check 
		stress clear
		stress build 
		stress start

		stress solve $y$
		stress solve clear
		stress solve build
		stress solve check 

		stress stress_solve $y$
		stress stress_solve clear
		stress stress_solve build
		stress stress_solve check

		stress generator $y$
		stress generator clear
		stress generator build
		stress generator check

		stress checker $y$
		stress checker clear
		stress checker standart
		stress checker build
		stress checker check

		stress test_count $z$
		stress test_count clear
		stress test_count inf
		
		stress test_out $y$
		stress test_out clear
		stress test_out stdout

	Теперь сделаем вывод контр теста в файл, используется при этом свой чекер (я взял скопированный стандартный)

		stress clear
		stress test_out test_kontr_test_out.txt
		stress solve test_solve.cpp
		stress stress_solve test_stress_solve.cpp
		stress generator test_generator.cpp
		stress checker test_checker.cpp
		stress build
		stress start

	Вывод в файле test_kontr_test_out.txt

		88 26


	

	Руководство по написанию собственного чекера:

	С примером написания чекера можете ознакомится, прочитав исходный код файла test\test_checker.cpp

	Начинаю код с таких строк, рекомендую делать так же

		#include <bits/stdc++.h>
		#include <Windows.h> 
		using namespace std;

		#define MAX_PATH_EXE_DIR 1000 //максимальный путь до файла stress.exe - 1 	

	Обязательно описать функцию входа в программу таким образом:
	
		int main (int argc, char *argv[]) {

			// копировать это

			char path_exe_dir[MAX_PATH_EXE_DIR];
			GetModuleFileName(0, path_exe_dir, MAX_PATH_EXE_DIR);

			std::string ped = path_exe_dir; //для упрощения работы


			while(ped.back() != '\\') //получаю директорию исполняемого файла
				ped.pop_back();

	
			ifstream in_solve((ped + "solve_out.txt").c_str(), ios_base::in);
			ifstream in_stress((ped + "stress_out.txt").c_str(), ios_base::in);
			ifstream in_test((ped + "stress_test.txt").c_str(), ios_base::in);	

	Здесь in_solve будет средством чтения из файла, в который производит вывод решение, in_stress - средство чтения из файла, в который производит
		вывод стресс-решение. in_test же нужен для чтения стресс-теста.

	Если был найден контр-тест, то его нужно вывести в стандартный поток вывода (cout << ).
	Если контр-тест не был найден, необходимо вывести в стандартный поток вывода "\nStress_test_not_found\n"			




	
