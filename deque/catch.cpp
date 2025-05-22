#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"
#include "dequeh.h"
#include <sstream>
#include <string>

TEST_CASE("Rule of Five veikia", "[RuleOfFive]") {
    Stud a;
    std::stringstream ss("Jonas Jonaitis 9 8 7 6 10 -1 4");
    a.readStudent(ss);

    SECTION("Copy konstruktorius") {
        Stud b(a);
        REQUIRE(b.getVardas() == a.getVardas());
        REQUIRE(b.getPavarde() == a.getPavarde());
    }

    SECTION("Move konstruktorius") {
        Stud movedFrom(a);
        Stud c(std::move(movedFrom));
        REQUIRE(c.getVardas() == "Jonas");
        REQUIRE(c.getPavarde() == "Jonaitis");
    }

    SECTION("Copy priskyrimas") {
        Stud b(a);
        Stud d;
        d = b;
        REQUIRE(d.getVardas() == b.getVardas());
        REQUIRE(d.getPavarde() == b.getPavarde());
    }

    SECTION("Move priskyrimas") {
        Stud b(a);
        Stud e;
        e = std::move(b);
        REQUIRE(e.getVardas() == "Jonas");
        REQUIRE(e.getPavarde() == "Jonaitis");
    }
}

TEST_CASE("Ivedimo/isvedimo operatorius veikia", "[IO]") {
    SECTION("Operatorius >> teisingai praziuri studentus") {
        Stud s;
        std::stringstream ss("Vardenis Pavardenis 8");
        ss >> s;
        REQUIRE(s.getVardas() == "Vardenis");
        REQUIRE(s.getPavarde() == "Pavardenis");
        REQUIRE(s.getEgzaminas() == 8);
    }

    SECTION("Operatorius << isveda teisinga studentu informacija") {
        Stud a;
        std::stringstream ss("Antanas Antanaitis 7 6 5 10 -1 8");
        a.readStudent(ss);
        std::stringstream out;
        out << a;
        REQUIRE_FALSE(out.str().empty());
    }
}