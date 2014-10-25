
#include <iostream>
#include <string>

void information(std::string text, bool newline = false, bool wrap = false);

int fatal(std::string text, int exitcode);

void processed(int current, long total, std::string filename, int width, int height);
