#ifndef MEANING_TREE_FCTS_H
#define MEANING_TREE_FCTS_H
#include <map>
#include <string>
#include <functional>

#include "utility.hpp"
#include "usefulStructs.hpp"
#include "pyFile.hpp"
#include "pyLine.hpp"
#include "pyError.hpp"

std::map<std::string, std::function<bool(FctContext)>> getBoolTreeFcts();

std::map<std::string, std::function<std::string(FctContext)>> getStringTreeFcts();

#endif
