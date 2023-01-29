#pragma once
#include <iostream>

#ifdef DEBUG
#define LOG(text) std::cout << text << std::endl
#else
#define LOG(text)
#endif