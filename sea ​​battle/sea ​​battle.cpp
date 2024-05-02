#include <iostream>
#include <iomanip>				// для выравнивания текста
#include <windows.h>			//для изменения цвета
#include <string>
using namespace std;

void Welcome() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_RED | BACKGROUND_INTENSITY);
	cout << "Морской бой" << endl << endl;
	cout << "MENU:" << endl;
	std::cout << std::left << std::setw(20) << "1. Играть" << std::endl;
	std::cout << std::left << std::setw(20) << "2. Правила игры" << std::endl;
	std::cout << std::left << std::setw(20) << "3. Выход" << std::endl;
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
}
void rules_sea_battle() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_RED | BACKGROUND_INTENSITY);
	cout << "На своем поле размещается 10 кораблей(1 шт.– из четырех ячеек, 2 шт.– из трех ячеек\n"
		"3 шт.– из двух ячеек и 4 шт.– из одной ячейки), тогда как поле соперника в начале игры остается пустым.\n"
		"Игроки по очереди делают ход, называя номер ячейки. При попадании – корабль считается раненым или потопленным.\n"
		"Если корабль ранен, то игроку предоставляется еще один ход. Если корабль потоплен, то игрок рисует область вокруг него,\n"
		"равную одной ячейке, где другие корабли не могут располагаться.\n"
		"Победителем считается тот игрок, кто первым смог потопить все корабли соперника." << endl;
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
}

/*1 А1 0
* 1 А3 0
* 1 А5 0
* 1 А7 0
* 2 В1 0
* 2 В3 0
* 2 В5 0
* 3 Ж1 0
* 3 Ж3 0
* 4 Ж7 0
*/

struct Cell {
	bool ship;
	bool hit;
};

struct Table {
	int wight = 10;
	int lenth = 10;
	Cell Table_pole[10][10]{};
	int _4DeckShipCount = 1;   // число 4-х палубных
	int _3DeckShipCount = 2;   // число 3-х палубных
	int _2DeckShipCount = 3;   // число 2-х палубных
	int _1DeckShipCount = 4;   // число 1-х палубных
};

Table table;
Table table2;

void create_Table(Table& table) {
	for (int i = 0; i < table.wight; i++) {
		for (int j = 0; j < table.lenth; j++) {
			table.Table_pole[i][j] = {};
		}
	}
}

void print_Table(Table& table, bool hide_ships = false) {
	cout << "  АБВГДЕЖЗИЙ" << endl;
	for (int i = 0; i < table.wight; i++) {
		cout << setw(2) << i + 1;
		for (int j = 0; j < table.lenth; j++) {
			if (table.Table_pole[i][j].hit) {
				if (table.Table_pole[i][j].ship) {
					cout << "X";
				}
				else {
					cout << ".";
				}
			}
			else {
				if (table.Table_pole[i][j].ship && !hide_ships) {
					cout << "0";
				}
				else {
					cout << " ";
				}
			}
		}
		cout << endl;
	}
}

bool can_place_ship(Table& table, int ship_x, int ship_y, int ship_length, int rotate) {
	int w = 1;
	int h = ship_length;
	if (rotate == 1) {
		w = ship_length;
		h = 1;
	}

	for (int x = ship_x - 1; x < ship_x + w + 1; x++) {
		for (int y = ship_y - 1; y < ship_y + h + 1; y++) {
			int xx = x;
			int yy = y;

			if (xx < 0) xx = 0;
			if (xx > 9) xx = 9;

			if (yy < 0) yy = 0;
			if (yy > 9) yy = 9;

			if (table.Table_pole[yy][xx].ship) {
				return false;
			}
		}
	}
	return true;
}

void first_player(Table& table) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	SetConsoleTextAttribute(handle, FOREGROUND_RED | BACKGROUND_INTENSITY);
	create_Table(table);
	print_Table(table);

	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
	cout << "Это Ваше поле расставьте корабли! Пример ввода : 4 A7 0, где 4 - это количестсво палуб, A7-координата, а 0 и 1 - это поворот корабля" << endl;
	int ship_length;
	string coord;

	int count_len[5] = {};
	int count_len_max[5] = {0, 4, 3, 2, 1};

	int rotate;
	int count_ship = 10;
	while (count_ship > 0) {
		cin >> ship_length >> coord >> rotate;

		if (count_len[ship_length] >= count_len_max[ship_length]) {
			cout << "Выберите корабль другой размерности!" << endl;
		}
		else {
			int j = coord[0] - 'А';
			int i = stoi(coord.substr(1)) - 1;
			if (can_place_ship(table, j, i, ship_length, rotate)) {
				for (int l = 0; l < ship_length; l++) {
					if (rotate == 1) {
						table.Table_pole[i][j + l].ship = true;
					}
					else {
						table.Table_pole[i + l][j].ship = true;
					}
				}
				count_len[ship_length]++;
				count_ship--;
			}
			else {
				cout << "Нельзя разместить корабль в этом месте!" << endl;
			}
		}

		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		print_Table(table);
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
	}
}

void attack(Table& table, int player) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	bool done = false;
	while (!done) {
		SetConsoleTextAttribute(handle, FOREGROUND_RED | BACKGROUND_GREEN);
		print_Table(table, true);

		if (player == 0) {
			cout << "Атакует первый игрок! Введите координаты корабля: ";
		} else if (player == 1) {
			cout << "Атакует второй игрок! Введите координаты корабля: ";
		}

		string coord;
		cin >> coord;
		int j = coord[0] - 'А';
		int i = stoi(coord.substr(1)) - 1;

		table.Table_pole[i][j].hit = true;

		if (table.Table_pole[i][j].ship) {
			cout << "Попадание!" << endl;
		}
		else {
			cout << "Промах! Ход переходит противнику! " << endl;
			done = true;
		}
	}
}

/*
1 А1 0
1 А3 0
1 А5 0
1 А7 0
2 В1 0
2 В4 0
2 В7 0
3 Д1 0
3 Д4 0
4 И1 0
*/

int main() {
	int game_choice;
	char choice_in_case1;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	char menu_choice{};
	bool back = false;
	bool ValidInput = false;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	do {
		Welcome();
		cin >> game_choice;
		switch (game_choice)
		{
		case 1: {
			ValidInput = true;
			first_player(table);
			first_player(table2);

			SetConsoleTextAttribute(handle, FOREGROUND_RED || BACKGROUND_GREEN);
			cout << "Поле первого игрока: " << endl;
			print_Table(table);
			cout << "Поле второго игрока: " << endl;
			print_Table(table);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY || BACKGROUND_INTENSITY);
			cout << "Поля заполнены! Начать бой? y/n" << endl;
			
			do {
				cin >> choice_in_case1;
				if (choice_in_case1== 'Y' || choice_in_case1 == 'y') {
					system("cls");

					while (true) {
						attack(table2, 0);
						attack(table, 1);
					}
				}
				if (choice_in_case1 == 'n' || choice_in_case1 == 'N') {
					cout << "Выход..." << endl;
					return 0;
				}
			} while (choice_in_case1 != 'Y' && choice_in_case1 != 'y');
			
			break;

		}
		case 2: {
			ValidInput = true;
			system("cls");
			rules_sea_battle();
			cout << "Вернуться в меню? y/n" << endl;
			do {
				cin >> menu_choice;
				if (menu_choice == 'Y' || menu_choice == 'y') {
					back = true;
					system("cls");
				}
				else if (menu_choice == 'N' || menu_choice == 'n') {
					cout << "Выход..." << endl;
					return 0;
				}
			} while (menu_choice != 'Y' && menu_choice != 'y');
			break;
		}
		case 3: {
			ValidInput = true;
			cout << "Выход..." << endl;
			return 0;
		}
		default: {
			cout << "Некорректный ввод! Выход..." << endl;
			return 0;
		}
		}
	} while (!ValidInput || back);
	return 0;
}