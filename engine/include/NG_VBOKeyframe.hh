#ifndef _NG_VBOKEYFRAME_H_
#define _NG_VBOKEYFRAME_H_

#include <vector>
#include <fstream>
#include <memory>

#include "NG_VBO.hh"

namespace NG {
	class VBOKeyframe{
	public:
		VBOKeyframe(const char* file_path);
		VBOKeyframe(std::ifstream& infile);
		std::shared_ptr<NG::VBO> Interpolate(std::vector<double> coefficients);
		unsigned int NumKeyframes(){return m_keyframes.size();}
	private:
		void init(std::ifstream& infile);
	private:
		std::vector<std::shared_ptr<NG::VBO> > m_keyframes;
	};
}

#endif /* _NG_VBOKEYFRAME_H_ */
