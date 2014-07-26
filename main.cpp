/*
 * main.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: Pimenta
 */

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

typedef int32_t  word_t;
typedef uint32_t uword_t;

#ifndef MEM_WORDS
#define MEM_WORDS 0x2000
#endif

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage mode: subleq-sim <meminit_file>\n");
    return 0;
  }
  
  uword_t* mem = new uword_t[MEM_WORDS];
  
  // read data
  ifstream f(argv[1]);
  for (uword_t words = 0; ; words++) {
    string tmp;
    
    // skip address
    f >> tmp;
    if (tmp == "END;") {
      break;
    }
    while (f.get() != ':');
    f.get();
    
    // read word
    tmp = "";
    for (char c = f.get(); c != ';'; c = f.get())
      tmp += c;
    sscanf(tmp.c_str(), "%x", &mem[words]);
  }
  f.close();
  
  // run
  uword_t A, B, J;
  word_t sub;
  uint64_t instructions = 0;
  for (uword_t ip = 0; ; ip = sub <= 0 ? J : ip) {
    if (mem[ip] == mem[ip + 1] && mem[ip + 2] == ip) {
      break;
    }
    
    A = mem[ip++];
    B = mem[ip++];
    J = mem[ip++];
    sub = mem[B] - mem[A];
    printf("0x%08X: A=0x%08X(0x%08X) B=0x%08X(0x%08X) J=0x%08X sub=0x%08X\n", ip - 3, A, mem[A], B, mem[B], J, sub);
    mem[B] = sub;
    instructions++;
  }
  printf("Instructions executed: %lld\n", instructions);
  
  delete[] mem;
  
  return 0;
}
