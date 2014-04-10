/*
 * main.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: Pimenta
 */

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

typedef int64_t  word_t;
typedef uint64_t uword_t;
typedef uint32_t address_t;

#ifndef MEM_WORDS
#define MEM_WORDS 8192
#endif

const uword_t ADDRESS_WIDTH = log2(MEM_WORDS);
const uword_t Amask         = (MEM_WORDS - 1) << 2*ADDRESS_WIDTH;
const uword_t Bmask         = (MEM_WORDS - 1) << 1*ADDRESS_WIDTH;
const uword_t Jmask         = (MEM_WORDS - 1) << 0*ADDRESS_WIDTH;

uword_t   mem[MEM_WORDS];

ifstream f;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage mode: subleq <mem_init_file>\n");
    return 0;
  }
  
  void read();
  void run();
  
  f.open(argv[1]);
  read();
  f.close();
  
  //run();
  
  return 0;
}

void read() {
  void skipLines(int);
  bool end();
  void skipAddress();
  
  skipLines(7);
  for (address_t words = 0; !end(); ++words) {
    skipAddress();
    string tmp;
    for (char c = f.get(); c != ';'; c = f.get())
      tmp += c;
    sscanf(tmp.c_str(), "%llx", mem + words);
    skipLines(1);
  }
}

void run() {
  uword_t instruction;
  address_t Aaddr, Baddr, Jaddr;
  word_t sub;
  for (address_t ip = 0; ip < MEM_WORDS; ip = sub <= 0 ? Jaddr : ip + 1) {
    instruction = mem[ip];
    Aaddr = instruction & Amask;
    Baddr = instruction & Bmask;
    Jaddr = instruction & Jmask;
    sub = mem[Baddr] - mem[Aaddr];
    mem[Baddr] = sub;
  }
}

void skipLines(int lines) {
  for (int i = 0; i < lines; ++i)
    while (f.get() != '\n');
}

bool end() {
  char tmp = f.get();
  return tmp == '\r' || tmp == '\n';
}

void skipAddress() {
  while (f.get() != ':');
  f.get();
}
