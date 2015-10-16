#include "turing.h"

const int maxScales = 10;
const int nScales = 10;

const int Turing::scales = nScales;
const int Turing::varrad = 1;
const int Turing::blurnum = 1;

const int raDef[maxScales] = { 150, 125, 110, 100, 8, 5, 4, 3, 2, 1 },
riDef[maxScales] = { 350, 300, 250, 200, 12, 10, 8, 6, 4, 2 },
rwDef[maxScales] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
rsDef[maxScales] = { 3, 3, 3, 3, 3, 2, 2, 2, 1, 1 };
const double rsaDef[maxScales] = { .06, .065, .055, .05, .04, .03, .02, .01, .007, .003 };

std::vector<int> Turing::aDef(raDef, raDef + nScales);
std::vector<int> Turing::iDef(riDef, riDef + nScales);
std::vector<int> Turing::wDef(rwDef, rwDef + nScales);
std::vector<int> Turing::sDef(rsDef, rsDef + nScales);

std::vector<double> Turing::saDef(rsaDef, rsaDef + nScales);

const double Turing::cIntDef[3] = { 0, 0, 0 };
