#ifndef _NG_BINARYUTILS_H_
#define _NG_BINARYUTILS_H_

#include <fstream>

namespace {
	template<typename T>
	inline void binary_read(std::ifstream& file, T* location, long length){
		file.read((char*)location, sizeof(T) * length);
	}
}

#endif /* _NG_BINARYUTILS_H_ */
