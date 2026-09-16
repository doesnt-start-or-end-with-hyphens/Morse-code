//The map containing all The morse Translations
#include <ctype.h>
class morseIndex {
  bool a[2] = {true, false};
  bool b[4] = {true, false, false, false};
  bool c[4] = {true, false, true, false};
  bool d[3] = {true, false, false};
  bool e[1] = {false};
  bool f[4] = {false, false, true, false};
  bool g[3] = {true, true, false};
  bool h[4] = {false, false, false, false};
  bool i[2] = {false, false};
  bool j[4] = {false, true, true, true};
  bool k[3] = {true, false, true};
  bool l[4] = {false, true, false, false};
  bool m[2] = {true, true};
  bool n[2] = {true, false};
  bool o[3] = {true, true, true};
  bool p[4] = {false, true, true, false};
  bool q[4] = {true, true, false, true};
  bool r[3] = {false, true, false};
  bool s[3] = {false, false, false};
  bool t[1] = {true};
  bool u[3] = {false, false, true};
  bool v[4] = {false, false, false, true};
  bool w[3] = {false, true, true};
  bool x[4] = {true, false, false, true};
  bool y[4] = {true, false, true, true};
  bool z[4] = {true, true, false, false};
  bool _1[5] = {false, true, true, true, true};
  bool _2[5] = {false, false, true, true, true};
  bool _3[5] = {false, false, false, true, true};
  bool _4[5] = {false, false, false, false, true};
  bool _5[5] = {false, false, false, false, false};
  bool _6[5] = {true, false, false, false, false};
  bool _7[5] = {true, true, false, false, false};
  bool _8[5] = {true, true ,true, false, false};
  bool _9[5] = {true, true, true, true, false};
  bool _0[5] = {true, true, true, true, true};
  bool qmrk[6] = {false, false, true, true, false, false};
  bool emrk[6] = {true, false, true, false, true, true};
  bool prd[6] = {false, true, false, true, false, true};
  bool cma[6] = {true, true, false, false, true, true};
  bool scol[6] = {true, false, true, false, true, false};
  bool col[6] = {true, true, true, false, false, false};
  bool pls[5] = {false, true, false, true, false};
  bool mns[6] = {true, false, false, false, false, true};
  bool slh[5] = {true, false, false, true, false};
  bool eql[5] = {true, false, false, false, true};
};
bool getMorse[int returnSize] (char letter){
  if (tolower(letter) == "a"){
    returnSize = 2;
    return morseI.a;
  }
  else if (tolower(letter) == "b"){
    returnSize = 4;
    return morseI.a;
  }
  else if (tolower(letter) == "c"){
    returnSize = 4;
    return morseI.c;
  }
  else if (tolower(letter) == "d"){
    returnSize = 3;
    return morseI.d;
  }
  else if (tolower(letter) == "e"){
    returnSize = 1;
    return morseI.e;
  }
  else if (tolower(letter) == "f"){
    returnSize = 4;
    return morseI.f;
  }
  else if (tolower(letter) == "g"){
    returnSize = 3;
    return morseI.g;
  }
  else if (tolower(letter) == "h"){
    returnSize = 4;
    return morseI.h;
  }
  else if (tolower(letter) == "i"){
    returnSize = 2;
    return morseI.i;
  }
  else if (tolower(letter) == "j"){
    returnSize = 4;
    return morseI.j;
  }
  else if (tolower(letter) == "k"){
    returnSize = 3;
    return morseI.k;
  }
  else if (tolower(letter) == "l"){
    returnSize = 4;
    return morseI.l;
  }
  else if (tolower(letter) == "m"){
    returnSize = 2;
    return morseI.m;
  }
  else if (tolower(letter) == "n"){
    returnSize = 2;
    return morseI.n;
  }
  else if (tolower(letter) == "o"){
    returnSize = 3;
    return morseI.o;
  }
  else if (tolower(letter) == "p"){
    returnSize = 4;
    return morseI.p;
  }
  else if (tolower(letter) == "q"){
    returnSize = 4;
    return morseI.q;
  }
  else if (tolower(letter) == "r"){
    returnSize = 3;
    return morseI.r;
  }
  else if (tolower(letter) == "s"){
    returnSize = 3;
    return morseI.s;
  }
  else if (tolower(letter) == "t"){
    returnSize = 1;
    return morseI.t;
  }
  else if (tolower(letter) == "u"){
    returnSize = 3;
    return morseI.u;
  }
  else if (tolower(letter) == "v"){
    returnSize = 4;
    return morseI.v;
  }
  else if (tolower(letter) == "w"){
    returnSize = 3;
    return morseI.w;
  }
  else if (tolower(letter) == "x"){
    returnSize = 4;
    return morseI.x;
  }
  else if (tolower(letter) == "y"){
    returnSize = 4;
    return morseI.y;
  }
  else if (tolower(letter) == "z"){
    returnSize = 4;
    return morseI.z;
  }
  else if (letter == "1"){
    returnSize = 5;
    return morseI._1;
  }
  else if (letter == "2"){
    returnSize = 5;
    return morseI._2;
  }
  else if (letter == "3"){
    returnSize = 5;
    return morseI._3;
  }
  else if (letter == "4"){
    returnSize = 5;
    return morseI._4;
  }
  else if (letter == "5"){
    returnSize = 5;
    return morseI._5;
  }
  else if (letter == "6"){
    returnSize = 5;
    return morseI._6;
  }
  else if (letter == "7"){
    returnSize = 5;
    return morseI._7;
  }
  else if (letter == "8"){
    returnSize = 5;
    return morseI._8;
  }
  else if (letter == "9"){
    returnSize = 5;
    return morseI._9;
  }
  else if (letter == "0"){
    returnSize = 5;
    return morseI._0;
  }
  else if (letter == "?"){
    returnSize = 6;
    return morseI.qmrk;
  }
  else if (letter == "!"){
    returnSize = 6;
    return morseI.emrk;
  }
  else if (letter == "."){
    returnSize = 6;
    return morseI.prd;
  }
  else if (letter == ","){
    returnSize = 6;
    return morseI.cma;
  }
  else if (letter == ";"){
    returnSize = 6;
    return morseI.scol;
  }
  else if (letter == ":"){
    returnSize = 6;
    return morseI.col;
  }
  else if (letter == "+"){
    returnSize = 5;
    return morseI.pls;
  }
  else if (letter == "-"){
    returnSize = 6;
    return morseI.mns;
  }
  else if (letter == "/"){
    returnSize = 5;
    return morseI.slh;
  }
  else if (letter == "="){
    returnSize = 6;
    return morseI.eql;
  }
  else{
    Serial.println("Letter not in database and has been skipped")
  }
}