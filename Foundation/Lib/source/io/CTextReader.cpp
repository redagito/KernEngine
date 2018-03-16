#include "foundation/io/CTextReader.h"

CTextReader::CTextReader(const std::shared_ptr<std::istream> &stream) : m_stream(stream) {}

bool CTextReader::readChar(char &c) { return false; }

bool CTextReader::readLine(std::string &line) { return std::getline(*m_stream, line).good(); }

bool CTextReader::readLines(std::vector<std::string> &lines) { return false; }

bool CTextReader::readAll(std::string &text) { return false; }
