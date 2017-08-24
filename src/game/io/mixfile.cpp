#include "mixfile.h"
#include "ccfileclass.h"

#ifndef RAPP_STANDALONE
List<MixFileClass<CCFileClass> *> &MixFileClass<CCFileClass>::s_mixList =
*reinterpret_cast<List<MixFileClass<CCFileClass> *> *>(0x006ECE30);
#else
List<MixFileClass<CCFileClass> *> MixFileClass<CCFileClass>::s_mixList;
#endif

// Explicit template instantiation.
template class MixFileClass<CCFileClass>;