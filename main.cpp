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
typedef int32_t  address_t;

#ifndef MEM_WORDS
#define MEM_WORDS 0x2000
#endif

const uword_t ADDRESS_WIDTH = uword_t(log2(double(MEM_WORDS)));
const uword_t ADDRESS_MASK  = uword_t(MEM_WORDS - 1);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage mode: subleq-sim <mem_init_file>\n");
    return 0;
  }
  
  uword_t mem[MEM_WORDS];
  
  // read data
  ifstream f(argv[1]);
  for (address_t words = 0, end = 0; !end; ++words) {
    // skip address
    while (f.get() != ':');
    f.get();
    
    // read word
    {
      string tmp;
      for (char c = f.get(); c != ';'; c = f.get())
        tmp += c;
      sscanf(tmp.c_str(), "%llx", mem + words);
    }
    
    // skip line
    while (f.get() != '\n');
    
    // check end
    {
      char tmp = f.get();
      end = tmp == '\r' || tmp == '\n';
    }
  }
  f.close();
  
  // run
  uword_t instruction;
  address_t Aaddr, Baddr, Jaddr;
  word_t sub;
  for (address_t ip = 0; ip < MEM_WORDS; ip = sub <= 0 ? Jaddr : ip + 1) {
    instruction = mem[ip];
    Aaddr = (instruction >> 2*ADDRESS_WIDTH) & ADDRESS_MASK;
    Baddr = (instruction >> 1*ADDRESS_WIDTH) & ADDRESS_MASK;
    Jaddr = (instruction >> 0*ADDRESS_WIDTH) & ADDRESS_MASK;
    sub = mem[Baddr] - mem[Aaddr];
    mem[Baddr] = sub;
  }
  
  return 0;
}
