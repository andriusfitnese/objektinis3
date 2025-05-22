#include "dequeh.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void test_rule_of_five() {
    cout << "Bandomas rule of 5\n";

    Stud a;
    std::stringstream ss("Jonas Jonaitis 9 8 7 6 10 -1 4");
    a.readStudent(ss);
    cout << "Pradinis studentas (a):\n" << a << "\n";
    cout << string(80, '-') << endl;

    Stud b(a);  // Copy constructor
    cout << "Kopijavimo kosntruktorius (b = a). Naujas studentas(b): \n" << b << "\n";
    cout << "Senas studentas (a): \n" << a << endl;
    cout << string(80, '-') << endl;

    Stud c(std::move(a));  // Move constructor
    cout << "Kelimo konstruktorius (c = std::move(a)). Naujas studentas (c): \n" << c << "\n";
    cout << "Senas studentas (a): \n" << a << endl;
    cout << string(80, '-') << endl;

    Stud d;
    d = b;  // Copy assignment
    cout << "Kopijavimo assignment'as (d = b). Naujas studentas (d): \n" << d << "\n";
    cout << "Senas studentas (b): \n" << b << endl;
    cout << string(80, '-') << endl;

    Stud e;
    e = std::move(b);  // Move assignment
    cout << "Move assignment (e = std::move(b)). Naujas studentas (e): \n" << e << "\n";
    cout << "Senas studentas (b): \n" << b << endl;
    cout << string(80, '-') << endl;
}

void test_io() {
    cout << "\nVeikia input operatorius...\n";

    Stud s;
    std::stringstream ss("Vardenis Pavardenis 8");
    ss >> s;
    cout << s.getVardas() << " " << s.getPavarde() << " " << s.getEgzaminas() << endl;
    cout << string(80, '-') << endl;

    cout << "Veikia output operatorius..." << endl;
    std::stringstream out;

    Stud a;
    std::stringstream ss1("Antanas Antanaitis 7 6 5 10 -1 8");
    a.readStudent(ss1);
    out << a;
    assert(!out.str().empty());
    cout << "Ivesties/isvesties testas praejo. Isvestis: \n" << out.str() << "\n";
}

int main() {
    //Zmogus zm;
    test_rule_of_five();
    test_io();
    cout << "Paspauskite Enter uzdaryti programa.";
    std::cin.ignore();
    std::cin.get();
    return 0;
}