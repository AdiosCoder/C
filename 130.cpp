/*
130. Napisz program rysowania znakiem ASCII ponizszej figury.
	 Program powinien umozliwic:
	- wybór znaku kodu ASCII;
	- wczytanie poczatkowych rozmiarow figury;
	- przesuwanie figury klawiszami
	- powiekszanie oraz zmniejszanie rozmiaru figury za pomoca klawiszy + i -;
	- ograniczenie przesuwania i rozmiarow figury do obszaru ekranu;

				---------o
						/
						\
				---------

Uwaga: punkt poczatkowy (kropka) znajduje sie w srodku ekranu.

Założenia:
1.  Program powinien być pisany z użyciem funkcji.
2.  Treść funkcji intmain() powinna składać się tylko z definicji zmiennych oraz wywoływani funkcji.
3.  Nie używamy zmiennych globalnych.

Dodatkowe:
Wymiary standardowej konsoli 80 x 25(szer. x wys.) */

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <stdio.h>
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //-Pozwala na zmiane kolorów w konsoli.

/*-Deklaracja funkcji (całe funkcje znajdują się poniżej funkcji intmain())-*/

void HideCursor();
void gotoxy(int x, int y);
void Head();
void Znak(char &znak, int &rozmiar, int &spr, char cnt);
void info(int &Info);
void Draw(int startx, int starty, int &zmiennax, int &zmiennay, int &rozmiar, char znak);
void Foot(string &temp);
void Change(int &startx, int &starty, int &rozmiar, char &przycisk, string &temp, char &znak, int &spr);
void czysc_rys(int x, int y);
bool SPR(int x, int y, int rozmiar);
void Zmiana(char &znak);
void koniec(int spr);

int main()
{
	/*-Deklaracja zmiennych-*/

	int startx, starty, rozmiar, zmiennax, zmiennay, spr, Info;
	string temp;
	char znak, przycisk, cnt;

	/*-Ustawienie początkowych wartośći-*/

	startx = 39; 									// Początkowy punkt na osi X (poziomo)
	starty = 11; 									// Początkowy punkt na osi Y (pionowo)
	rozmiar = 4; 									// początkowy mnożnik wielkośći figury (później wybiera go użytkownik)
	spr = 0; 										// zmienna pomocnicza ( pozwala na wyjście z programu przy określonych sytuacjach)
	cnt = NULL;										// Pozwala na ponowne wybranie znaku i rozmiaru ( po wybraniu złego rozmiaru).
	Info = 0;										// Zmienna pomocnicza aby instrukcja wyświetlała się 1 raz.

	/*-Ukrywanie kursora w konsoli-*/

	HideCursor();

	/*-Obramowanie pola roboczego + teskst początkowy (Dane)-*/

	Head();

	/*-Użytkownik wybiera wielkość początkową figury oraz znak z jakiego ma być utworzona-*/

	Znak(znak, rozmiar, spr, cnt);

	/*- Kontynuacja programu jeżeli zostały podane dobre wartości początkowe-*/

	while (spr == 0)											
	{

		/*-Wyświetlanie instrukcji - jednorazowo-*/

		if (Info == 0)
		{
			info(Info); 											
		}


		do
		{

			/*-Tworzenie figury-*/

			Draw(startx, starty, zmiennax, zmiennay, rozmiar, znak);

			/*-Wyświetlenie Statusy figury-*/

			Foot(temp);

			/*-Przemieszczanie figury, zmiana wielkośc, powrót do wyboru wartości początkowych, zakończenie-*/

			Change(startx, starty, rozmiar, przycisk, temp, znak, spr);
		}
		while (przycisk != 27);

		/*-Koniec programu*/

		koniec(spr);
	}
	return 0;
}
void HideCursor()
{
	::HANDLE hConsoleOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo(hConsoleOut, &hCCI);
	hCCI.bVisible = FALSE;
	::SetConsoleCursorInfo(hConsoleOut, &hCCI);
}
void gotoxy(int x, int y)										// ustawia kursor w wybranym przez nas miejscu w konsoli
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void Head()
{
	gotoxy(0, 0);

	SetConsoleTextAttribute(console, 10);  						// zmiana koloru tekstu na zielony

	for (int i = 1; i <= 79; i++)								// Tworzenie górnego obramowania
	{
		cout << '-';
	}

	gotoxy(0, 24);

	for (int i = 1; i <= 79; i++)								// Tworenie dolnego obramowania
	{
		cout << '-';
	}

	SetConsoleTextAttribute(console, 15); 						// zmiana koloru tekstu na biały

	gotoxy(30, 6);
	cout << "Adrian Wojewoda." << endl;

	gotoxy(16, 9);

	cout << "Wcisnij dowolny klawisz aby kontynuowac..." << endl;
	_getch();

	czysc_rys(0, 1);

}
void Znak(char &znak, int &rozmiar, int &spr, char cnt) 		// Pozwala wybrać wielkośc początkową figury i znak, z którego będzie stworzona figura
{
	SetConsoleTextAttribute(console, 10);						// zmiana koloru tekstu na zielony

	gotoxy(27, 9);

	cout << "Wybierz znak dla figury: " << endl;

	znak = _getch(); 											// Użytkownik wybiera znak, z którego będzie stworzona figura

	czysc_rys(0, 1);

	gotoxy(12, 9);

	cout << "Podaj wielkosc (domyslna 4) figury i zatwierdz enterem: " << endl;

	gotoxy(12, 10);

	cin >> rozmiar;												// Użytkownik podaje startową wielkość figury

	SetConsoleTextAttribute(console, 15);						// zmiana koloru tekstu na biały
								
	czysc_rys(0, 1);

	if (rozmiar < 1 || rozmiar > 8)								// Sprawdzenie czy początkowa wielkość figury nie jest za mała, bądź za duża
	{
		SetConsoleTextAttribute(console, 10);					// zmiana koloru tekstu na zielony

		gotoxy(18, 10);
		cout << "Zla wartosc (Zakres watosci od 1 do 8)" << endl;

		gotoxy(5, 12);
		cout << "Wcisnij dowolny klawisz aby sprobowac ponownie lub esc aby zamknac..." << endl;

		cnt = _getch();

		SetConsoleTextAttribute(console, 15);					// zmiana koloru tekstu na biały

		czysc_rys(0, 1);

		if (cnt == 27) // 27 - "ESC" (ACSCII)						
		{
			spr = 1;											// Wyjście z programu
		}
		else
		{
			Znak(znak, rozmiar, spr, cnt);						// ponownie pozwala wybrać figure i znak
		}


	}
}
void info(int &Info) 											// Wyświetla instrukcje po wybraniu znaku i rozmiaru figury
{
	SetConsoleTextAttribute(console, 10);						// zmiana koloru tekstu na zielony

	gotoxy(8, 9);

	cout << "Nacisnij + lub - aby zmniejszyc lub powiekszyc figure." << endl;

	gotoxy(8, 11);

	cout << "Figure mozna przesuwac strzalkami" << endl;

	gotoxy(8, 13);

	cout << "Aby zmienic figure wcisnij c" << endl;

	gotoxy(8, 15);

	cout << "Aby wyjsc wcisnij esc" << endl;

	gotoxy(8, 15);

	cout << "Nacisnij dowolny klawisz aby kontynuowac..." << endl;

	gotoxy(8, 17);

	_getch();

	czysc_rys(0, 1);

	Info = 1;													// Blokuje ponowne wyświetlanie instrukcji

	SetConsoleTextAttribute(console, 15);						// zmiana koloru tekstu na biały
}
void Draw(int startx, int starty, int &zmiennax, int &zmiennay, int &rozmiar, char znak)
{
	zmiennax = startx;  // Zmienna względem osi X
	zmiennay = starty;  // Zmeinna względem osi Y


	for (int i = 1; i <= ((rozmiar * 3)-1); i++)				// Tworzenie górnej krawędzi
	{
		
		gotoxy(zmiennax -i, zmiennay);
		cout << znak;
	}
	for (int i = 1; i <= rozmiar - 2; i++) 						// Tworzenie pierwszej skośnej krawędzi
	{
		gotoxy(zmiennax, zmiennay);
		cout << znak;
		zmiennax--;
		zmiennay++;
	}

	for (int i = 1; i <= rozmiar - 2; i++) 						// Tworzenie drugiej skośnej krawędzi
	{
		gotoxy(zmiennax, zmiennay);
		cout << znak;
		zmiennay++;
		zmiennax++;
	}

	gotoxy(zmiennax, zmiennay);

	for (int i = 1; i <= rozmiar * 3; i++) 						// Tworzenie dolnej krawędzi
	{
		cout << znak;
		gotoxy(zmiennax -i, zmiennay);
	}
}
void Foot(string &temp)
{
	SetConsoleTextAttribute(console, 10); 						//zmiana koloru tekstu na zielony

	gotoxy(28, 24);

	cout << temp;

	SetConsoleTextAttribute(console, 15); 						// zmiana koloru tekstu na biały
}
void Change(int &startx, int &starty, int &rozmiar, char &przycisk, string &temp, char &znak, int &spr)
{
	przycisk = _getch();
	czysc_rys(0, 1);
	temp = "Figura w gotowosci...------------";

	switch (przycisk)
	{
	case 75:													//75 - "←" ASCII
	{
		startx--; 												// Przesunięcie figury w lewo
		if (startx>((0+3*rozmiar)-1))
		{
			break;
		}
		else
		{
			startx++;
			temp = "UWAGA! GRANICA POLA!------------";
			break;
		}
	}
	case 77:													//77 - "→" ASCII
	{
		startx++; 												// Przesunięcie figury w prawo
		if (SPR(startx, starty, rozmiar))
		{
			break;
		}
		else
		{
			startx--;
			temp = "UWAGA! GRANICA POLA!------------";
			break;
		}
	}
	case 80:													//80 - "↓" ASCII
	{
		starty++; 												// Przesunięcie figury w dół
		if (SPR(startx, starty, rozmiar))
		{
			break;
		}
		else
		{
			starty--;
			temp = "UWAGA! GRANICA POLA!------------";
			break;
		}
	}
	case 72:													//72 - "↑" ASCII
	{
		starty--; 												// Przesunięcie figury do góry
		if (starty >= 1)
		{
			break;
		}
		else
		{
			starty++;
			temp = "UWAGA! GRANICA POLA!------------";
			break;
		}
	}
	case '+':
	{
		rozmiar++; 												// Powiększenie figury
		if (SPR(startx, starty, rozmiar)&& startx>((0 + 3 * rozmiar) - 1))
		{
			break;
		}
		else
		{
			rozmiar--;
			temp = "MAKSYMALNY ROZMIAR PRZEKROCZONY!";
			break;
		}
	}
	case '-':
	{
		rozmiar--; 												// Powiększenie figury
		if (rozmiar>0)
		{
			break;
		}
		else
		{
			rozmiar++;
			temp = "MINIMALNY ROZMIAR PRZEKROCZONY!";
			break;
		}
	}
	case 99:													// 99 - "c" ASCII
	{
		Zmiana(znak);											// Wybór znaku figury
		break;
	}
	case 27:													// 27 - "ESC" ASCII
	{
		spr = 1;
		break; 													// wychodzi z programu
	}

	}
}
bool SPR(int x, int y, int rozmiar)
{
	if (x <= 78  && y + 2 * rozmiar- 4 <= 23)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void czysc_rys(int x, int y) 									// Czyści Pole robocze figury
{
	gotoxy(x, y);

	for (int i = 0; i <= 78; i++)								// uzupełnia pole spacjami
	{
		for (int j = 0; j <= 22; j++)
		{
			gotoxy(x + i, y + j);
			cout << " ";
		}
	}
}
void Zmiana(char &znak)											// Pozwala na zmiane znaku atkualnej figury
{
	gotoxy(28, 12);

	SetConsoleTextAttribute(console, 10);						// zmiana koloru tekstu na zielony

	cout << "Wybierz nowy znak" << endl;
	znak = _getch();

	SetConsoleTextAttribute(console, 15);						// zmiana koloru tekstu na bialy

	czysc_rys(0, 1);
}
void koniec(int spr)											// Wyświetla ekran pożegnalny
{
	czysc_rys(0, 1);

	SetConsoleTextAttribute(console, 10);						// zmiana koloru tekstu na zielony

	gotoxy(0, 24);

	for (int i = 1; i <= 79; i++)								// Usówa pozostałości po ststusie
	{
		cout << '-';
	}

	gotoxy(29, 10);

	cout << "Koniec programu." << endl;

	gotoxy(17, 12);

	cout << "Wcisnij dowolny klawisz aby zakonczyc..." << endl;

	_getch();													// wcisnięcie dowolnego znaku spowoduje zakończenie programu
}