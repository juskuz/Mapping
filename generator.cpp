#include <iostream>
#include <windows.h>
#include <fstream>

using namespace std;

int Newton( unsigned int n, unsigned int k)      // Funkcja obliczaj¹ca symbol newtona
{
double Wynik = 1;       // Deklaracja zmiennej ( u¿ywamy double dla dok³adnoœci obliczeñ )
 
for( unsigned int i = 1; i <= k; i++)   // Od 1 do k wykonujemy :
{
Wynik = Wynik * ( n - i + 1 ) / i;      // Obliczanie ze wzoru iteracyjnego
}
 
return (unsigned long long) Wynik;      // Zwróæ Wynik
}

int main()
{
	int liczba;
	cout<<"podaj liczbe ciec: ";
	cin>>liczba;
	int liczba_elementow;
	int n=(liczba+2);
	int k=2;
	
	if( k == 0 || k == n ) {liczba_elementow=1;}       // Je¿eli k = 0 lub k = n to wypisz 1
	else liczba_elementow= Newton(n, k);                 // W przeciwnym wypadku oblicz wartoœæ symbolu 
	
	int tablica[liczba_elementow];
	int mapa[liczba+1];
	
	for(int i=0; i<(liczba+1); i++)
	{
		cout<<endl<<"Podaj element numer "<<i+1<<": ";
		cin>>mapa[i];
	}
	int element=0;
	for (int i=0; i<(liczba+1); i++)
	{
		int suma=mapa[i];
		tablica[element]=suma;
		element++;
		if(i<liczba)
		{
		
			for (int j=i+1; j<(liczba+1); j++)
			{
				if (i==j) continue;
				suma=suma+mapa[j];
				tablica[element]=suma;
				element++;
			}
		}
	}
	
	cout<<endl<<"wyliczono wartosc elementow: "<<endl<<endl;
	for (int i=0; i<liczba_elementow; i++)
	{
		cout<<"ELEMENT "<<i+1<<": "<<tablica[i]<<endl;
	}
	
	fstream plik;
	plik.open("nowe.txt", ios::out);
	if(plik.good())
	{
	
		for (int i=0; i<liczba_elementow; i++)
		{
			plik<<i+1<<tablica[i];
		}
	}
	plik.close();
	cout<<endl<<"zapisano do pliku"<<endl;
	
}
