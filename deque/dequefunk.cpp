#include "dequeh.h"

int g_printMode = 1;
/// 1 - mediana, 2 - vidurkis (isvedimui)

bool failasegzistuoja(const string& failopav)
{
	ifstream failas(failopav);
	return failas.good();
}
bool sortVardu(const Stud& a, const Stud& b) {
	return a.getVardas() < b.getVardas();
}
bool sortPav(const Stud& a, const Stud& b) {
	return a.getPavarde() < b.getPavarde();
}
bool sortMed(const Stud& a, const Stud& b) {
	return a.getMediana() < b.getMediana();
}
bool sortGal(const Stud& a, const Stud& b) {
	return a.getGalutinis() < b.getGalutinis();
}

Stud::Stud() : egrez_(0) {}
Stud::Stud(const string& vardas, const string& pavarde, const vector<int>& pazymiai, const int egzaminas)
	: Zmogus(vardas, pavarde), paz_(pazymiai), egrez_(egzaminas), gal_(0), ndvid_(0), med_(0) {
}

Stud::Stud(const Stud& other)
	: Zmogus(other), paz_(other.paz_),
	egrez_(other.egrez_), gal_(other.gal_),
	ndvid_(other.ndvid_), med_(other.med_) {
}

Stud& Stud::operator=(const Stud& other) {
	if (this != &other) {
		var_ = other.var_;
		pav_ = other.pav_;
		paz_ = other.paz_;
		egrez_ = other.egrez_;
		gal_ = other.gal_;
		ndvid_ = other.ndvid_;
		med_ = other.med_;
	}
	return *this;
}

Stud::Stud(Stud&& other) noexcept
	: Zmogus(std::move(other)),
	paz_(std::move(other.paz_)), egrez_(other.egrez_),
	gal_(other.gal_), ndvid_(other.ndvid_), med_(other.med_) 
{
	other.var_.clear();
	other.pav_.clear();
	other.egrez_ = 0;
	other.gal_ = 0;
	other.ndvid_ = 0;
	other.med_ = 0;
}

Stud& Stud::operator=(Stud&& other) noexcept {
	if (this != &other) {
		Zmogus::operator=(std::move(other));
		paz_ = std::move(other.paz_);
		egrez_ = other.egrez_;
		gal_ = other.gal_;
		ndvid_ = other.ndvid_;
		med_ = other.med_;
		other.var_.clear();
		other.pav_.clear();
		other.egrez_ = 0;
		other.gal_ = 0;
		other.ndvid_ = 0;
		other.med_ = 0;
	}
	return *this;
}

void Stud::paskaiciuoti_vid_ir_med() {
	if (paz_.empty()) {
		ndvid_ = 0;
		med_ = 0;
		return;
	}
	int sum = std::accumulate(paz_.begin(), paz_.end(), 0);
	int medianos_poz;

	ndvid_ = double(sum) / paz_.size();
	sort(paz_.begin(), paz_.end());

	if (paz_.size() % 2 == 0) {
		medianos_poz = paz_.size() / 2;
		med_ = (paz_.at(medianos_poz) + paz_.at(medianos_poz - 1)) / 2.0;
	}
	else {
		medianos_poz = paz_.size() / 2;
		med_ = paz_.at(medianos_poz);
	}

}

void Stud::paskaiciuoti_gal() {
	gal_ = (0.4 * ndvid_ + 0.6 * egrez_);
	med_ = (0.4 * med_ + 0.6 * egrez_);
}

void rng(vector<int>& paz)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> pazkiek(1, 10);
	uniform_int_distribution<int> pazym(1, 10);
	int pazymk = pazkiek(gen);
	paz.reserve(paz.size() + pazymk);
	cout << "Generuojami pazymiai..." << endl;
	for (int i = 0;i < pazymk;i++)
	{
		int pazy = pazym(gen);
		paz.push_back(pazy);
		cout << pazy << " ";
	}
	cout << endl;
}

void rng(int& egrez)  ///gauname adresa egrez, sugeneruojame ir grazinam
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> pazym(1, 10);
	cout << "Generuojamas pazymys..." << endl;
	egrez = pazym(gen);
	cout << egrez << " ";
	cout << endl;
}
void rng(string& vardas, string& pavarde)
{
	cout << "Generuojamas vardas..." << endl;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> nameDist(0, 7);

	vardas = vardai[nameDist(gen)];
	pavarde = pavardes[nameDist(gen)];
	cout << vardas << " " << pavarde << endl;
}

Stud::Stud(istream& is) {
	is >> var_ >> pav_;
	int pazym;
	while (is >> pazym) {
		paz_.push_back(pazym);
	}
	if (!paz_.empty()) {
		egrez_ = paz_.back();
		paz_.pop_back();
	}
	gal_ = 0;
	ndvid_ = 0;
	med_ = 0;
}

/*Stud::~Stud() {
	paz_.clear();
	var_ = "";
	pav_ = "";
	egrez_ = 0;
	ndvid_ = 0;
	med_ = 0;
	gal_ = 0;
}
*/
std::istream& Stud::readStudent(std::istream& is) { ///rankinis/automatinis irasymas ( ne is failo )
	string v;
	cout << "Iveskite studento varda (parasykite stop, jei esate jau ivede visus, parasykite gen, jei norite varda sugeneruoti)" << endl;
	is >> v;
	if (v == "stop")
	{
		var_ = "stop";
		return is;
	}
	setVar(v);
	string p;
	if (v == "gen")
	{
		string rv, rp;
		rng(rv, rp);
		setVar(rv);
		setPav(rp);
	}
	else
	{
		cout << "Iveskite jo pavarde" << endl;
		is >> p;
		setPav(p);
	}

	cout << "Iveskite jo namu darbu rezultatus ( jei norit, kad butu sugeneruoti, parasykite -2, ivede visus, parasykite -1)" << endl;
	paz_.clear();
	int pazym;
	int i = 0;
	bool tinka = false;
	while (!tinka)
	{
		try
		{
			is >> pazym;
			if (is.fail() or ((pazym > 10 or pazym < 1) and pazym != -2 and pazym != -1))
			{
				throw runtime_error("Ivestas netinkamas simbolis/skaicius! Iveskite sveika skaiciu nuo 1 iki 10!");
			}
			if (pazym == -2 and i < 1)
			{
				rng(paz_);
				break;
			}
			else if (pazym == -2 and i > 0)
			{
				throw runtime_error("Generuoti galima tik is pradziu.Veskite ranka arba uzbaikite su -1.");
			}
			if (pazym == -1 and i > 0)break;
			else if (pazym == -1 and i < 1) throw runtime_error("Neivedete nei vieno namu darbu pazymio!");
			else
			{
				paz_.push_back(pazym);
				i++;
			}
		}
		catch (runtime_error& e)
		{
			cerr << "Klaida: " << e.what() << endl;
			is.clear();
			is.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	cout << "Iveskite jo egzamino rezultata. (jei norite, kad butu sugeneruotas, rasykite -1)" << endl;
	tinka = false;
	int egz;
	while (!tinka)
	{
		try {
			is >> egz;
			if (is.fail() or (egz < 1 and egz != -1) or (egz > 10 and egz != -1))
			{
				throw runtime_error("Ivestas netinkamas simbolis/skaicius! Iveskite sveika skaiciu nuo 1 iki 10!");
			}
			else if (egz == -1)
			{
				rng(egz);
				break;
			}
			else {
				setEgrez(egz);
				break;
			}
		}
		catch (const runtime_error& e)
		{
			cerr << "Klaida: " << e.what() << endl;
			is.clear();
			is.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	paskaiciuoti_vid_ir_med();
	paskaiciuoti_gal();
	return is;
}

std::istream& operator>>(std::istream& in, Zmogus& z) {
	return z.read(in);
}
std::ostream& operator<<(std::ostream& os, const Zmogus& z) {
	return z.print(os);
}

std::istream& Stud::read(std::istream& in) {
	in >> var_ >> pav_;
	std::vector<int> tmp;
	int x;
	while (in >> x) {
		tmp.push_back(x);
		if (in.peek() == '\n' || in.eof()) break;
	}
	if (!tmp.empty()) {
		egrez_ = tmp.back();
		tmp.pop_back();
		paz_ = std::move(tmp);
	}
	paskaiciuoti_vid_ir_med();
	paskaiciuoti_gal();
	return in;
}

std::ostream& Stud::print(std::ostream& os) const {
	os
		<< std::fixed
		<< std::setprecision(2)
		<< std::left
		<< std::setw(15) << var_
		<< std::setw(18) << pav_;

	if (g_printMode == 1) {
		os << std::setw(8) << med_;
	}
	else {
		os << std::setw(8) << gal_;
	}
	return os;
}

vector<Stud> processBatch(const vector<string>& lines) {
	vector<Stud> batchResults;
	batchResults.reserve(lines.size());

	for (const auto& line : lines) {
		istringstream iss(line);
		Stud laik(iss);

		laik.paskaiciuoti_vid_ir_med();
		laik.paskaiciuoti_gal();
		batchResults.push_back(laik);
	}
	return batchResults;
}

void skaitymas(deque<Stud>& grupe, duration<double>& veiklaik) {
	string failopav;
	cout << "Iveskite failo pavadinima: " << endl;
	while (true) {
		cin >> failopav;
		if (!failasegzistuoja(failopav)) {
			cerr << "Klaida: failas " << failopav << " neegzistuoja!" << endl;
			///return;
		}
		else {
			break;
		}
	}

	ifstream in(failopav, ios::in | ios::ate); // atidarom faila pamatyti ilgi
	if (!in) {
		cerr << "Nepavyko atidaryti failo!" << endl;
		///return;
	}

	size_t fileSize = in.tellg(); // gaunam failo dydi
	in.seekg(0, ios::beg); // einam i failo pradzia

	cout << "Failas atidarytas: " << failopav << endl;
	auto start = high_resolution_clock::now();

	const size_t BUFFER_SIZE = 64 * 1024 * 1024; // 64MB bufferis, kompams su daugiau ramo padeda didesnis
	vector<char> buffer(min(fileSize, BUFFER_SIZE));

	string header;
	getline(in, header);

	vector<future<vector<Stud>>> futures;
	vector<string> lines;
	string leftover; // jeigu eilute neuzbaigta bufferiui baigiantis, issaugosim likuti

	while (in.read(buffer.data(), buffer.size()) || in.gcount() > 0) {
		string chunk = leftover + string(buffer.data(), in.gcount());
		stringstream ss(chunk);
		string line;

		leftover.clear(); // resetinam likuti kitam chunk'ui
		while (getline(ss, line)) {
			if (ss.eof()) {
				leftover = line; // issaugom paskutine eilute jeigu nesibaige
			}
			else {
				lines.push_back(line);
			}
		}

		if (lines.size() >= 100000) {
			futures.push_back(async(launch::async, processBatch, lines));
			lines.clear();
		}
	}

	in.close();

	// praeinam likusias eilutes, jei yra
	if (!leftover.empty()) {
		lines.push_back(leftover);
	}

	if (!lines.empty()) {
		futures.push_back(async(launch::async, processBatch, lines));
	}

	// surenkam rezultatus
	for (auto& fut : futures) {
		vector<Stud> batch = fut.get();
		grupe.insert(grupe.end(), batch.begin(), batch.end());
	}

	auto end = high_resolution_clock::now();
	veiklaik += end - start;
	cout << "Failo skaitymo laikas: " << duration<double>(end - start).count() << " s" << endl;
}

void isvedimas(int pas, int pasmv, const deque<Stud>& grupe)
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
bool gautteisinga(int& input, const string& prompt, int min, int max) {
	cout << prompt;
	while (true) {
		cin >> input;
		if (cin.fail() || input < min || input > max) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ivestas neteisingas simbolis/skaicius. Prasome ivesti reiksme tarp " << min << " ir " << max << ": ";
		}
		else {
			return true;
		}
	}
}
void failogen(const string& failopav, int irasuk)
{
	ofstream out(failopav);
	if (!out)
	{
		cerr << "Klaida! Neatidarytas failas " << failopav << endl;
		return;
	}
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> pazymk(1, 20);
	uniform_int_distribution<int> pazym(1, 10);

	const size_t bufferSize = 20000;
	vector<string> buffer;
	buffer.reserve(bufferSize);
	///int kiekpaz = pazymk(gen);
	int kiekpaz = 10;
	out << left << setw(25) << "Vardas" << setw(25) << "Pavarde";
	for (int i = 1;i <= kiekpaz;i++)
	{
		out << setw(6) << ("ND" + to_string(i));
	}
	out << setw(10) << "Egzaminas\n";
	for (int i = 1;i <= irasuk;i++)
	{
		ostringstream eil;
		eil << left << setw(25) << ("VardasNR" + to_string(i)) << setw(25) << ("PavardeNR" + to_string(i));
		for (int j = 0;j < kiekpaz;j++)
		{
			eil << setw(6) << pazym(gen);
		}
		eil << setw(10) << pazym(gen);
		eil << endl;
		buffer.push_back(eil.str());

		if (buffer.size() >= bufferSize)
		{
			for (const auto& line : buffer)
			{
				out << line;
			}
			buffer.clear();
		}
	}
	for (const auto& eil : buffer) {
		out << eil;
	}


	out.close();
}
void atrinkimas1(deque<Stud>& grupe, vector<Stud>& nerdai, vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik)
{
	auto pasis = (pasmv == 1) ?
		[](const Stud& s) { return s.getMediana(); } :
		[](const Stud& s) { return s.getGalutinis(); };

	string pasir = (pasmv == 1) ? "Mediana" : "Galutinis";

	auto start1 = high_resolution_clock::now();
	for (const auto& n : grupe)
	{
		if (n.getGalutinis() >= 5) nerdai.push_back(n);
		else galiorka.push_back(n);
	}
	grupe.clear();
	grupe = deque<Stud>();
	auto end1 = high_resolution_clock::now();
	veiklaik += end1 - start1;
	cout << "Atskyrimo i dvi grupes veikimo laikas panaikinant originalu vektoriu: " << duration<double>(end1 - start1).count() << endl;
	auto start2 = high_resolution_clock::now();
	ofstream outp("nerdai.txt");
	outp << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : nerdai)
	{
		outp << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outp << endl;
	}
	auto end2 = high_resolution_clock::now();
	veiklaik += end2 - start2;
	cout << "Nerdu irasymo i faila veikimo laikas: " << duration<double>(end2 - start2).count() << endl;
	auto start3 = high_resolution_clock::now();
	ofstream outf("galiorka.txt");
	outf << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : galiorka)
	{
		outf << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outf << endl;
	}
	auto end3 = high_resolution_clock::now();
	veiklaik += end3 - start3;
	cout << "Galiorkos irasymo i faila veikimo laikas:  " << duration<double>(end3 - start3).count() << endl;
}

void atrinkimas2(deque<Stud>& grupe, vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik)
{
	auto pasis = (pasmv == 1) ?
		[](const Stud& s) { return s.getMediana(); } :
		[](const Stud& s) { return s.getGalutinis(); };

	string pasir = (pasmv == 1) ? "Mediana" : "Galutinis";

	auto start1 = high_resolution_clock::now();
	for (const auto& n : grupe)
	{
		if (n.getGalutinis() < 5) {
			galiorka.push_back(n);
		}
	}
	grupe.erase(remove_if(grupe.begin(), grupe.end(), [](const Stud& s) { return s.getGalutinis() < 5; }), grupe.end());
	auto end1 = high_resolution_clock::now();
	veiklaik += end1 - start1;
	cout << "Atskyrimo i dvi grupes veikimo laikas nepanaikinant originalaus vektoriaus: " << duration<double>(end1 - start1).count() << endl;
	auto start2 = high_resolution_clock::now();
	ofstream outp("nerdai.txt");
	outp << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : grupe)
	{
		outp << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outp << endl;
	}
	auto end2 = high_resolution_clock::now();
	veiklaik += end2 - start2;
	cout << "Nerdu irasymo i faila veikimo laikas: " << duration<double>(end2 - start2).count() << endl;
	auto start3 = high_resolution_clock::now();
	ofstream outf("galiorka.txt");
	outf << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : galiorka)
	{
		outf << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outf << endl;
	}
	auto end3 = high_resolution_clock::now();
	veiklaik += end3 - start3;
	cout << "Galiorkos irasymo i faila veikimo laikas:  " << duration<double>(end3 - start3).count() << endl;
}



void atrinkimas3(deque<Stud>& grupe, vector<Stud>& galiorka, int pasmv, duration<double>& veiklaik)
{
	auto pasis = (pasmv == 1) ?
		[](const Stud& s) { return s.getMediana(); } :
		[](const Stud& s) { return s.getGalutinis(); };

	string pasir = (pasmv == 1) ? "Mediana" : "Galutinis";

	auto start1 = high_resolution_clock::now();
	auto it = partition(grupe.begin(), grupe.end(), [](const Stud& s) { return s.getGalutinis() >= 5; });
	copy(it, grupe.end(), back_inserter(galiorka));

	grupe.erase(it, grupe.end());
	auto end1 = high_resolution_clock::now();
	veiklaik += end1 - start1;
	cout << "Atskyrimo i dvi grupes veikimo laikas nepanaikinant originalaus vektoriaus: " << duration<double>(end1 - start1).count() << endl;
	auto start2 = high_resolution_clock::now();
	ofstream outp("nerdai.txt");
	outp << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : grupe)
	{
		outp << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outp << endl;
	}
	auto end2 = high_resolution_clock::now();
	veiklaik += end2 - start2;
	cout << "Nerdu irasymo i faila veikimo laikas: " << duration<double>(end2 - start2).count() << endl;
	auto start3 = high_resolution_clock::now();
	ofstream outf("galiorka.txt");
	outf << fixed << setprecision(2) << left << setw(25) << "Vardas" << setw(25) << "Pavarde" << setw(6) << pasir << endl;
	for (const auto& n : galiorka)
	{
		outf << left << setw(25) << n.getVardas() << setw(25) << n.getPavarde() << setw(6) << pasis(n);
		outf << endl;
	}
	auto end3 = high_resolution_clock::now();
	veiklaik += end3 - start3;
	cout << "Galiorkos irasymo i faila veikimo laikas:  " << duration<double>(end3 - start3).count() << endl;
}
