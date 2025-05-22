
#include "dequeh.h"
int main()
{
	deque<Stud> grupe;
	vector<Stud> nerdai;
	vector<Stud> galiorka;
	Stud laik;
	bool geras = false;
	int pasir = 0;
	auto veiklaik = duration<double>::zero();

	int pas = 0;
	if (gautteisinga(pas, "Ar norite pasinaudoti failo generavimo funkcija? Iveskite 1, jei norite, 2, jei nenorite: ", 1, 2))
	{
		switch (pas)
		{

		case 1:
			string temp;
			cout << "Iveskite failo pavadinima: ";
			cin >> temp;
			pas = 0;
			if (gautteisinga(pas, "Iveskite norimu irasu kieki (1-10000000): ", 1, 10000000))
				failogen(temp, pas);
			break;
		}
	}
	while (!geras)
	{
		try {
			cout << "Pasirinkite, ar noresite vesti bent kelis duomenis ranka arba generuot (1); ar skaitysite is failo (2)" << endl;
			cin >> pasir;
			if (cin.fail() || (pasir != 1 && pasir != 2)) {
				throw runtime_error("Netinkamas pasirinkimas. Iveskite 1 arba 2.");
			}
			geras = true;
		}
		catch (const runtime_error& e)
		{
			cerr << "Klaida: " << e.what() << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	if (pasir == 2)
	{
		skaitymas(grupe, veiklaik);
		cout << "Skaitymas baigtas." << endl;
	}
	else {
		while (true)
		{
			Stud s;
			s.readStudent(cin);
			if (s.getVardas() == "stop") break;
			grupe.push_back(s);
		}
	}
		int sortpas = 0;
		string pasirn;
		if (gautteisinga(sortpas, "Pasirinkite, kaip norite surikiuoti (1 - Vardas, 2 - Pavarde, 3 - Mediana, 4 - Galutinis pazymys): ", 1, 4)) {}
		high_resolution_clock::time_point start, end;
		switch (sortpas) {
		case 1:
			start = high_resolution_clock::now();
			sort(grupe.begin(), grupe.end(), sortVardu);
			///grupe.sort(sortVardu);
			end = high_resolution_clock::now();
			cout << "Surikiuota pagal vardus (abeceles tvarka)." << endl;
			pasirn = "vardus";
			break;
		case 2:
			start = high_resolution_clock::now();
			sort(grupe.begin(), grupe.end(), sortPav);
			///grupe.sort(sortPav);
			end = high_resolution_clock::now();
			cout << "Surikiuota pagal pavardes (abeceles tvarka)." << endl;
			pasirn = "pavardes";
			break;
		case 3:
			start = high_resolution_clock::now();
			sort(grupe.begin(), grupe.end(), sortMed);
			///grupe.sort(sortMed);
			end = high_resolution_clock::now();
			cout << "Surikiuota pagal mediana." << endl;
			pasirn = "mediana";
			break;
		case 4:
			start = high_resolution_clock::now();
			sort(grupe.begin(), grupe.end(), sortGal);
			///grupe.sort(sortGal);
			end = high_resolution_clock::now();
			cout << "Surikiuota pagal galutini pazymi." << endl;
			pasirn = "galutini pazymi";
			break;
		default:
			cout << "Neteisingas pasirinkimas!" << endl;
			break;
		}
		veiklaik += end - start;
		///cout << "Rusiavimo pagal " << pasirn << " laikas:" << duration<double>(end - start).count() << endl;
		pas = 0;
		int pasmv = 0;
		if (gautteisinga(pas, "Ar isskirti studentus, kurie pazangus ir nepazangus [bus galimas tik isvedimas i faila!] (1) ar ne? (2)", 1, 2))
			switch (pas)
			{
			case 1:
				gautteisinga(pasmv, "Isvesti mediana(1), vidurki(2)?", 1, 2);
				if (grupe.empty())
				{
					cout << "Studentu nerasta! programa baigiama!";
					///return 1;
				}
				else {
					int pas1 = 0;
					if (gautteisinga(pas1, "Kuria strategija (1-3) noretute panaudoti?", 1, 3))
						switch (pas1)
						{
						case 1:
							atrinkimas1(grupe, nerdai, galiorka, pasmv, veiklaik);
							break;
						case 2:
							atrinkimas2(grupe, galiorka, pasmv, veiklaik);
							break;
						case 3:
							atrinkimas3(grupe, galiorka, pasmv, veiklaik);
							break;
						}
				}
				break;
			case 2:
				pas = 0;
				if (gautteisinga(pas, "I ekrana(1) ar i faila(2)?", 1, 2))
				{
					gautteisinga(pasmv, "Isvesti mediana(1), vidurki(2)?", 1, 2);
					if (grupe.empty())
					{
						cout << "Studentu nerasta! programa baigiama!";
						///return 1;
					}
					else isvedimas(pas, pasmv, grupe);
				}
				break;
			}
		///cout << "Visos programos testo laikas: " << veiklaik.count() << endl;
		cout << "Paspauskite Enter uzdaryti programa.";
		std::cin.ignore();
		std::cin.get();
		return 0;
	}