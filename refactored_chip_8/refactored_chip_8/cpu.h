#pragma once
#include <cstdint>
#include "system_memory.h"
#include <unordered_map>
#include <random>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>

class bus;

class cpu {
private:

	// Pointers
	uint16_t PC; // Program counter
	uint8_t SP; // Stack counter

	// REGISTERS
	uint8_t registers[16];
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t I; // Address register

	// Flag
	bool VF;

	// Stack
	uint16_t stack[16];

	// Current instruction
	uint16_t instruction;

public:
	cpu(bus*);
	~cpu();

	void run();

private:
	// Operations
	void SYS();
	void CLS();
	void RET();
	void JP_1();
	void CALL();
	void SE_1();
	void SNE_1();
	void SE_2();
	void LD_1();
	void ADD_1();
	void LD_2();
	void OR();
	void AND();
	void XOR();
	void ADD_2();
	void SUB();
	void SHR();
	void SUBN();
	void SHL();
	void SNE_2();
	void LD_3();
	void JP_2();
	void RND();
	void DRW();
	void SKP();
	void SKNP();
	void LD_4();
	void LD_5();
	void LD_6();
	void LD_7();
	void ADD_3();
	void LD_8();
	void LD_9();
	void LD_10();
	void LD_11();

private:
	bus* bus_link;
};