#ifndef VECTOR_LIB_H
#define VECTOR_LIB_H
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm> //sortui
#include <thread>
#include <execution>
#include <future> 
#include <functional> //del greater ir less funkciju
#include <deque>
#include <chrono>
#include <list>
#include <numeric>

using std::cout;
using std::cin;
using std::min;
using std::endl;
using std::vector;
using std::stringstream;
using std::launch;
using std::fixed;
using std::setw;
using std::left;
using std::streamsize;
using std::numeric_limits;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::string;
using std::sort;
using std::setprecision;
using std::ifstream;
using std::ofstream;
using std::ws;
using std::istringstream;
using std::cerr;
using std::nth_element;
using std::future;
using std::istream;
using std::runtime_error;
using std::ios;
using std::exception;
using std::to_string;
using std::ostringstream;
using std::deque;
using std::list;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::copy;
using std::back_inserter;
using std::accumulate;

const string vardai[] = { "Jonas", "Petras", "Marius", "Lukas", "Tomas", "Simas", "Andrius", "Darius" };
const string pavardes[] = { "Kazlauskas", "Petraitis", "Jonaitis", "Mikalauskas", "Bagdonas", "Vaitkus", "Urbonas", "Grigas" };

class Zmogus {
protected:
	string var_;
	string pav_;
public:
	Zmogus() = default;
	Zmogus(std::string v, std::string p)
		: var_(std::move(v)), pav_(std::move(p)) {
	}
	virtual ~Zmogus() = 0;
	virtual std::istream& read(std::istream& in) = 0;
	virtual std::ostream& print(std::ostream& out) const = 0;

	const std::string& getVardas()  const { return var_; }
	const std::string& getPavarde() const { return pav_; }
};
inline Zmogus::~Zmogus() = default;

class Stud : public Zmogus {

private:

	Vector<int>paz_;
	int egrez_;
	double gal_;
	double ndvid_;
	double med_;

public:
	Stud();
	~Stud() override = default;

	Stud(const Stud& other);  ///kopijavimo konstruktorius
	Stud& operator=(const Stud&); ///kopijavimo assignment
	Stud(Stud&&) noexcept;  ///kelimo konstruktorius
	Stud& operator=(Stud&&) noexcept;  ///kelimo assignment

	Stud(const string& vardas, const string& pavarde, const Vector<int>& pazymiai, const int egzaminas);
	Stud(istream& is);

	///setteriai
	void setVar(const string& v) { var_ = v; }
	void setPav(const string& p) { pav_ = p; }
	void addPaz(int p) { paz_.push_back(p); }
	void setEgrez(int e) { egrez_ = e; }
	void setNdvid(double v) { ndvid_ = v; }
	void setGal(double g) { gal_ = g; }
	void setMed(double m) { med_ = m; }
	void clearPaz() { paz_.clear(); }


	/// getteriai 
	inline Vector<int>& getPazymiai() { return paz_; }
	inline int getEgzaminas() const { return egrez_; }
	inline double getNdvid() const { return ndvid_; }
	inline double getGalutinis() const { return gal_; }
	inline double getMediana() const { return med_; }
	void paskaiciuoti_vid_ir_med();
	void paskaiciuoti_gal();
	double galutinis_vidurkis() const { return gal_; }
	double galutinis_mediana() const { return med_; }

	istream& readStudent(istream& is);
	std::istream& read(std::istream& in) override;
	std::ostream& print(std::ostream& out) const override;

	friend std::istream& operator>>(std::istream& in, Stud& s) {
		return s.read(in);
	}
	friend std::ostream& operator<<(std::ostream& os, const Stud& s) {
		return s.print(os);
	}
};
std::istream& operator>>(std::istream& in, Zmogus& z);
std::ostream& operator<<(std::ostream& os, Zmogus const& z);

bool sortVardu(const Stud& a, const Stud& b);
bool sortPav(const Stud& a, const Stud& b);
bool sortMed(const Stud& a, const Stud& b);
bool sortGal(const Stud& a, const Stud& b);
double mediana(const Vector<int>& paz, double egrez, string var);
double galvid(double egrez, double ndvd);
double ndvid(const Vector<int>& paz);
void rng(Vector<int>& paz);  ///nd pazymiu gen
void rng(int& egrez);  ///egz pazymiu gen
void rng(string& vardas, string& pavarde);  ///vardu generavimas
void skaitymas(Vector<Stud>& grupe, duration<double>& veiklaik);
bool gautteisinga(int& input, const string& prompt, int min, int max);   ///error handlingas neteisingos ivesties atveju
void failogen(const string& failopav, int irasuk);
void atrinkimas1(Vector<Stud>& grupe, Vector<Stud>& nerdai, Vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik);
void atrinkimas2(Vector<Stud>& grupe, Vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik);
void atrinkimas3(Vector<Stud>& grupe, Vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik);
Vector<Stud> processBatch(const Vector<string>& lines);
#endif