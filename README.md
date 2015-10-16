![Example 1](/examples/example1.png?raw=true "Example")
# color-ms-turing-patterns
C++ library for creating RGB Multi-Scale Turing Patterns
Based off of the work by [Jonathan McCabe](http://www.jonathanmccabe.com/Cyclic_Symmetric_Multi-Scale_Turing_Patterns.pdf)
and the implementation at [Softology](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/).
I have added the features of Color, Symmetry Degree, Image Seeding, and Compound/Multi-Scale Synthesis.

# Use
The main.cpp file contains the variables
```
const int WIDTH = 1000;
const int HEIGHT = 1000;

bool SYM = false;

int TYPE = 2;
```

1. WIDTH and HEIGHT act as expected.
2. SYM allows you to turn symmetry on or off.
3. TYPE determines the number of scales to implement. TYPE=2 means apply the two scales with smallest 
calculated variation and TYPE=1 acts as a typical MSTP.

The vars.cpp file contains the finer-detailed variables
```
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
```

1. maxscales is the maximum number of scales you would use. Iw ouldn't use more than ten.
2. nScales is the number of scales you actually want to use.
3. varrad is the square-radius size of pixels to sum for the variation calculation
4. blurnum is the number of times the blurring algorithm is implemented
5. raDef is the array of activator radii for each scale
5. riDef is the array of inhibitor radii for each scale
6. rwDef is the array of weights for each scale
7. rsDef is the array of symmetry orders for each scale
8. rsaDef is the array of "small amounts" for each scale
9. cIntDef is the array of crossover between the RG, RB, GB channels.

# Examples
Another good example for paramters is:
```
\\vars.cpp
const int maxScales = 10;
const int nScales = 5;

const int Turing::scales = nScales;
const int Turing::varrad = 1;
const int Turing::blurnum = 1;

const int raDef[maxScales] = { 150, 70, 10, 2, 1, 5, 4, 3, 2, 1 },
riDef[maxScales] = { 350, 150, 25, 5, 2, 10, 8, 6, 4, 2 },
rwDef[maxScales] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
rsDef[maxScales] = { 3, 2, 2, 2, 2, 3, 2, 2, 1, 1 };
const double rsaDef[maxScales] = { .06, .05, .04, .03, .01, .03, .02, .01, .007, .003 };

std::vector<int> Turing::aDef(raDef, raDef + nScales);
std::vector<int> Turing::iDef(riDef, riDef + nScales);
std::vector<int> Turing::wDef(rwDef, rwDef + nScales);
std::vector<int> Turing::sDef(rsDef, rsDef + nScales);

std::vector<double> Turing::saDef(rsaDef, rsaDef + nScales);

const double Turing::cIntDef[3] = { 0, 0, 0 };
```
```
\\main.cpp
const int WIDTH = 1000;
const int HEIGHT = 1000;

bool SYM = true;

int TYPE = 2;
```

#TODO
1. I would like to reconfigure the app to use floats or ints rather than 3 doubles for the RBG data.
3. Fix image seeding height/width
