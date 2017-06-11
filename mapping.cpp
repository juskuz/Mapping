#include <iostream>
#include <windows.h>
#include <fstream>
#include <math.h>
#include <ctime>


using namespace std;


double mierz_czas(double koniec, double start)
{
    double czas = (double)(koniec - start)/CLOCKS_PER_SEC ;
    return czas;
}

int zeruj_instancje(int instancja[])
{
    for(int i=0; i<200; i++)
    {
        instancja[i]=0;
    }
}

void zeruj_mape(int mapa[], int rozmiar)
{
	for (int i; i<rozmiar; i++)
	{
		mapa[i]=0;
	}
}

void wypisz_rozwiazanie(int rozmiar, int mapa[])
{
	cout<<"Zbudowano mape."<<endl<<endl<<"MAPA:"<<endl<<endl;
	for(int i=0; i<rozmiar; i++)
	{
		cout<<mapa[i]<<" ";
	}
	cout<<endl<<endl;
}

bool szukaj_w_instancji(int wartosc, int instancja[], int rozmiar, bool wykorzystane[], int zablokowane[], int licznik)
{
	for(int i=0; i<rozmiar; i++)
	{
		if((wartosc==instancja[i])&&(wykorzystane[i]==false))
		{
			zablokowane[licznik]=i;
			wykorzystane[i]=true;
			return true;
		}
	}
	return false;
}


int szukaj2 (int mapa2[], int instancja[], int licznosc_instancji, int mapa[], int poziom, int rozmiar_mapy /*czyli maks poziom*/, int maxelement, bool wykorzystane[], int *jest )
{
	if(poziom==rozmiar_mapy) // jesli wypelnilismy mape do konca to wypisz rozwiazanie
	{
		cout<<"ZNALEZIONO ROZWIAZANIE: "<<endl;
		wypisz_rozwiazanie(poziom, mapa);
		for(int h=0; h<poziom+1; h++)
		{
			mapa2[h]=mapa[h];
		}
		*jest=1;
		return 0;
	}
	if (*jest==1)
	{
		wypisz_rozwiazanie(poziom+1, mapa);
		return 0;
	}

	for (int i=0; i<licznosc_instancji; i++) // przebiegamy po calej instancji sprawdzajac wszystkie elementy po kolei
	{
		if (wykorzystane[i]==true) continue; // jesli dany element jest wykorzystany, to przejdz do kolejnego
		int szukany=0;
		szukany =instancja[i]; // dopisujemy wartosc obecnie szukana
		bool znaleziono=true;

		int licz=0;
		int zablokowane[rozmiar_mapy+1];

		for(int j=0; j<rozmiar_mapy+1; j++)//zerowanie zablokowanych w danej petli
		{
			zablokowane[j]=0;
		}

		for(int j=poziom-1; j>=0; j--) // tutaj bedziemy sprawdzac sumy wczesniejszych elementow tablicy, jest (poziom - 1), bo wstawiamy pod indeks poziom, wiec sumujemy indeks wczesniej
		{
			szukany=szukany+mapa[j]; // sumujemy kolejne elementy mapy (powiekszamy wartosc szukana)
			if (szukany>maxelement) break; // jesli suma elementow mapy przekroczy max element to sprawdz kolejny element mapy
			if ((szukany==maxelement) &&(poziom==(rozmiar_mapy-1)))
			{
				mapa[poziom]=instancja[i];
				wykorzystane[i]=true;
				szukaj2(mapa2, instancja, licznosc_instancji, mapa, (poziom+1), rozmiar_mapy, maxelement, wykorzystane, jest);
				wykorzystane[i]=false;
				mapa[poziom]=0;

			}
			znaleziono=szukaj_w_instancji(szukany, instancja, licznosc_instancji, wykorzystane, zablokowane, licz); // znaleziono wolna wartosc w tablicy
			if (znaleziono==false) // odblokuj wszystkie po tej pory
			{
				for(int k=0; k<licz; k++)
				{
					wykorzystane[zablokowane[k]]=false;
				}
				break; // przejdz do kolejnego elementu
			}
			licz++; // jesli zablokowano liczbe, to zwieksz licz o 1
		}
		if(znaleziono==false) // zakonczono petle i nie znaleziono wszystkich niezbednych elementow w instancji
		{
			for(int k=0; k<licz; k++)
			{
				wykorzystane[zablokowane[k]]=false;
			}
			continue;
		}
		//jesli znaleziono wszystkie to sprawdzamy jeszcze czy istnieje dopelnienie
		bool dopelnienie;
		int brakujacy=(maxelement-szukany); // sprawdzamy ile brakuje do pelnej mapy, jesli 0, to pomijamy blokowanie (dodawany element powinien byc wstawiany jako ostatni do mapy)
		if (szukany==maxelement)
		{
		}
		else // jesli dodawany element nie jest ostatni, to oblicz ile brakuje, sprawdz czy wolne i zablokuj
		{
		dopelnienie = szukaj_w_instancji(brakujacy, instancja, licznosc_instancji, wykorzystane, zablokowane, licz);
		if((dopelnienie==true)&&(poziom==rozmiar_mapy-2))
		{
			mapa[poziom]=instancja[i];
			mapa[poziom+1]=brakujacy;
			poziom++;
			*jest=1;
			szukaj2(mapa2, instancja, licznosc_instancji, mapa, (poziom), rozmiar_mapy, maxelement, wykorzystane, jest);
			if(*jest==1) return 0;
		}
		if (dopelnienie==false)  // jesli dopelnienia nie ma w tablicy to nie mozna wstawic, wiec odblokuj elementy
		{
			for(int k=0; k<licz; k++)
			{
				wykorzystane[zablokowane[k]]=false;
			}
			continue;
		}
		if (dopelnienie==true)		licz++;
		}

		mapa[poziom]=instancja[i]; // warunki sa spelnione, mozna wstawic do mapy
		wykorzystane[i]=true;

		++poziom;
		szukaj2(mapa2, instancja, licznosc_instancji, mapa, (poziom), rozmiar_mapy, maxelement, wykorzystane, jest);
		if(*jest==1) return 0;
		--poziom;
		wykorzystane[i]=false;
		mapa[poziom]=0;

		for(int j=0; j<licz; j++) // trzeba odblokowac dane
		{
			wykorzystane[zablokowane[j]]=false;
		}
	}
}


int main()
{
	int instancja[200];
    int a;
    int licznosc_instancji=0;
    string opcja;
    int liczba_ciec=0;
    int liczba_elementow_mapy;
    int mapa[15], mapa2[15]; // liczba ciec+1 czyli maks 15
    bool wykorzystane[200];
    int jest=0;

    bool wczytano=false;
    bool sprawdzono=false;
    bool utworzono=false;
    bool zdatny=false;

    for (int i=0; i<200; i++)
    {
        wykorzystane[i]=false;
    }

    do
    {
        system("CLS");
        cout<<" Podaj opcje: "<<endl<<endl;
        cout<<" - 'wczytaj' - wczytanie instancji z pliku "<<endl;
        cout<<" - 'wyswietl' - wyswietlanie wczytanej instancji"<<endl;
        cout<<" - 'sprawdz' - sprawdzenie wczytanej instancji"<<endl;
        cout<<" - 'zbuduj' - zbuduj mape"<<endl;
        cout<<" - 'pokaz' - wyswietlenie wyniku dzialania programu"<<endl;
        cout<<" - 'koniec' - konczy dzialanie programu "<<endl<<endl;
        cout<<" opcja: ";
        cin>>opcja;

        if (opcja=="wczytaj") // zly format wczytywania
        {
            system("CLS"); // wyczysc ekran

            zeruj_instancje(instancja);

            fstream plik;
            plik.open("instancja.txt", ios::in);
            while(plik>>a) // max 120
            {
                instancja[licznosc_instancji]=a;
                licznosc_instancji++;
            }
            wczytano=true;
            cout<<"Poprawnie wczytano dane z pliku instancja.txt."<<endl;

            system("PAUSE");
        }

        if (opcja=="wyswietl")
        {
            system("CLS"); // wyczysc ekran

            if(wczytano)
            {
                cout<<"Wczytana instancja:"<<endl<<endl;

                for(int i=0; i<licznosc_instancji; i++)
                {
                    cout<<instancja[i]<<"\t";
                }
                cout<<endl<<endl;
            }
            else
            {
                cout<<"Nie wczytano danych z pliku. Wybierz opcje 'wczytaj'."<<endl<<endl;
            }

            system("PAUSE");
        }

        if (opcja=="sprawdz") // sprawdz czy da sie utworzyc mape
        {
            system("CLS"); // wyczysc ekran

            if(wczytano)
            {
                sprawdzono=true;
                double ciecia;
                ciecia=(-3 + sqrt(1+(8*licznosc_instancji)))/2;
                double liczba_c= floor(ciecia);
                if (liczba_c==ciecia)
                {
                    liczba_ciec=liczba_c;
                    liczba_elementow_mapy=(liczba_ciec+1);
                    cout<<"Mozna utworzyc mape na podstawie takiej instancji.\nBedzie miala "<<liczba_c<<" ciec, czyli "<<liczba_c+1<<" elementow."<<endl<<endl;
                    zdatny=true;
                    zeruj_mape(mapa, liczba_elementow_mapy);
                    zeruj_mape(mapa2, liczba_elementow_mapy);
                }
                else
                {
                    cout<<"Blad! Nie mozna utworzyc mapy. Licznosc instancji jest nieprawidlowa."<<endl<<endl;
                }
            }
            else
            {
                cout<<"Nie wczytano danych z pliku. Nie mozna sprawdzic. Wybierz opcje 'wczytaj'."<<endl<<endl;
            }

            system("PAUSE");
        }

        if (opcja=="zbuduj") // utworz mape
        {
            system("CLS"); // wyczysc ekran
            if(wczytano)
            {
                if(sprawdzono)
                {
                    if(zdatny) // jesli liczba elementow sie zgadza to zbuduj mape
                    {
                        int max_wartosc=instancja[0];
                        int nieco_mniej=instancja[1];
                        for(int j=0; j<licznosc_instancji-1; j++) // znajdz nawieksza wartosc w tablicy
                        {
                            if(instancja[j+1]>instancja[j])
                            {
                            	nieco_mniej=max_wartosc;
                                max_wartosc=instancja[j+1];
                            }
                        } // znajdz dwie najwieksze wartosci w tablicy

                        int pierwszy_element=(max_wartosc-nieco_mniej);

                  		cout<<"MAKSYMALNA WARTOSC W TABLICY: "<<max_wartosc<<endl;
                  		cout<<"PIERWSZY ELEMENT: "<<pierwszy_element<<endl;
                  		int j;
						for (j=0; j<licznosc_instancji; j++)
						{
							if(instancja[j]==pierwszy_element)
							break;
						}
                  		int zablokowane_zbedne[2];
                  		int licz=0;

						mapa[0]=instancja[j];
                  		wykorzystane[j]=true;
                  		szukaj_w_instancji(max_wartosc, instancja, licznosc_instancji, wykorzystane, zablokowane_zbedne, licz); // aby zablokowac max_element
                  		szukaj_w_instancji(nieco_mniej, instancja, licznosc_instancji, wykorzystane, zablokowane_zbedne, licz); // aby zablokowac aby zablokowac dopelnienie

                  		double start=clock();
                  		szukaj2(mapa2, instancja, licznosc_instancji, mapa, 1, liczba_elementow_mapy, max_wartosc, wykorzystane, &jest);
						double koniec=clock();
                  		double czas=mierz_czas(koniec, start);
                  		cout<<"Czas dzialania algorytmu: "<<czas<<endl;
                        if(jest==0)
                        {
                            cout<<"\nNie udalo sie utworzyc mapy."<<endl;
                        }
                        if(jest==1) utworzono=true;
                    }
                    else
                    {
                        cout<<"Nie mozna utworzyc mapy. Liczba elementow jest nieodpowiednia."<<endl<<endl;
                    }
                }
                else
                {
                    cout<<"Nie sprawdzono danych z pliku. Nie mozna zbudowac mapy. Wybierz najpierw opcje 'sprawdz'."<<endl<<endl;
                }
            }
            else
            {
                cout<<"Nie wczytano danych z pliku. Nie mozna sprawdzic. Wybierz opcje 'wczytaj'."<<endl<<endl;
            }

            system("PAUSE");
        }

        if (opcja=="pokaz") // wyswietl utworzona mape
        {
            system("CLS"); // wyczysc ekran

            if(wczytano==true)
            {
                if(sprawdzono==true)
                {
                    if(zdatny==true) // jesli liczba elementow sie zgadza to zbuduj mape
                    {
                        if(utworzono==true)
                        {
                            cout<<endl<<"**************************************"<<endl;
                            cout<<"                MAPA:"<<endl<<endl;

                            for (int i=0; i<liczba_elementow_mapy; i++)
                            {
                                cout<<mapa[i]<<" ";
                            }
                            cout<<endl<<endl<<endl<<"*************************************"<<endl;
                            cout<<endl;
                        }
                        else
                        {
                            cout<<"Nie utworzono jeszcze mapy, wiec nie mozna jej wyswietlic. Wybierz najpierw opcje 'zbuduj' i sporobuj ponownie."<<endl<<endl;
                        }
                    }
                    else
                    {
                        cout<<"Mapa nie istnieje. Nie mozna jej wyswietlic."<<endl<<endl;
                    }
                }
                else
                {
                    cout<<"Nie sprawdzono danych z pliku. Nie mozna zbudowac mapy. Wybierz najpierw opcje 'sprawdz'."<<endl<<endl;
                }
            }
            else
            {
                cout<<"Nie wczytano danych z pliku. Nie mozna sprawdzic. Wybierz opcje 'wczytaj'."<<endl<<endl;
            }


            system("PAUSE");
        }

        if (opcja=="koniec") // zakoncz program
        {
            system("CLS"); // wyczysc ekran

            cout<<"Konczenie pracy programu..."<<endl;

            system("PAUSE");
        }
    } while (opcja!="koniec");


    return 0;
}
