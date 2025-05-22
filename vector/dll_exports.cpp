#define SPAUSDINIMO_EXPORTS
#include "dll_export.h"
#include "vector.hpp"

extern "C" SPAUSDINIMO_API void isvedimas(int pas, int pasmv, const Vector<Stud>& grupe)
{
	g_printMode = pasmv;
	switch (pas)
	{
	case 1:
		switch (pasmv)
		{
		case 1:
			cout << left << setw(15) << "Vardas" << setw(18) << "Pavarde" << setw(8) << "Galutinis (med.)" << endl;
			cout << string(52, '-') << endl;
			for (const auto& n : grupe)
			{
				cout << n << '\n';
			}
			break;

		case 2:
			cout << left << setw(15) << "Vardas" << setw(18) << "Pavarde" << setw(8) << "Galutinis (vid.)" << endl;
			cout << string(52, '-') << endl;
			for (const auto& n : grupe)
			{
				cout << n << '\n';
			}
			break;
		}
		break;
	case 2:
		ofstream out("kursiokaiapdorotas.txt");
		switch (pasmv)
		{
		case 1:
			out << left << setw(15) << "Vardas" << setw(18) << "Pavarde" << setw(8) << "Galutinis (med.)" << endl;
			out << string(52, '-') << endl;
			for (const auto& n : grupe)
			{
				out << fixed << left << setw(15) << setprecision(2) << n.getVardas() << setw(18) << n.getPavarde() << setw(8) << n.getMediana() << endl;
			}
			break;
		case 2:
			out << left << setw(15) << "Pavarde" << setw(18) << "Vardas" << setw(8) << "Galutinis (vid.)" << endl;
			out << string(52, '-') << endl;
			for (const auto& n : grupe)
			{
				out << fixed << left << setw(15) << setprecision(2) << n.getVardas() << setw(18) << n.getPavarde() << setw(8) << n.getGalutinis() << endl;
			}
			break;
		}
		out.close();
		break;
	}
}