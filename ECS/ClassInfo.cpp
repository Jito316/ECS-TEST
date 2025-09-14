#include "ClassInfo.h"
std::unordered_map<size_t, ComponentInfo::Data*>  ComponentInfo::m_registry;
std::unordered_map<std::string, size_t> ComponentInfo::m_nameToID;
size_t ComponentInfo::m_nextID = 0;