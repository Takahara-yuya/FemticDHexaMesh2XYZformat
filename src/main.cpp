//--------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2021 Yoshiya Usui
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//--------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <complex>
#include <iomanip>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>

#include "MeshDataNonConformingHexaElement.h"
#include "ResistivityBlock.h"

enum PlaneType {
	UNKNOWN = -1,
	ZX_PLANE = 0,
	XY_PLANE,
};

enum ElementType {
	TETRA = 0,
	BRICK,
	NONCONFORMING_HEXA
};

struct Coord3D {
	double X;
	double Y;
	double Z;
};

struct Coord2D {
	double X;
	double Y;
};

ResistivityBlock m_ResistivityBlock;
int m_numIteration = 0;
double m_centerCoord[3];
std::vector<int> m_blockExcluded;

void run(const std::string& paramFile);
void readParameterFile(const std::string& paramFile);
void FemticDHexaMesh2XYZformat();

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "You must specify parameter file  !!" << std::endl;
		exit(1);
	}
	run(argv[1]);
	return 0;
}

void run(const std::string& paramFile) {
	readParameterFile(paramFile);
	FemticDHexaMesh2XYZformat();
}

void FemticDHexaMesh2XYZformat() {

	MeshDataNonConformingHexaElement m_meshDataNonConformingHexaElement;

	m_meshDataNonConformingHexaElement.inputMeshData();
	m_ResistivityBlock.inputResisitivityBlock(m_numIteration);

	std::ostringstream ofile;
	ofile << "resistivity_xyz_iter" << m_numIteration << ".dat";
	std::ofstream ofs(ofile.str().c_str(), std::ios::out);
	if (ofs.fail()) {
		std::cerr << "File open error : " << ofile.str() << " !!" << std::endl;
		exit(1);
	}

	ofs.precision(6);

	const int numElem = m_meshDataNonConformingHexaElement.getNumElemTotal();
	ofs << numElem << std::endl;
	const int numEdgeOnElem = 12;
	for (int iElem = 0; iElem < numElem; ++iElem) {
		std::vector<Coord2D> coordIntersectVec;
		const int blockID = m_ResistivityBlock.getBlockIDFromElemID(iElem);
		//if (find(m_blockExcluded.begin(), m_blockExcluded.end(), blockID) != m_blockExcluded.end()) {
		//	continue;// Excluded
		//};
		CommonParameters::locationXYZ coord1 = m_meshDataNonConformingHexaElement.getGravityCenter(iElem);
		coord1.X += m_centerCoord[1]; coord1.Y += m_centerCoord[0];
		coord1.X *= 0.001; coord1.Y *= 0.001; coord1.Z *= 0.001;
		ofs << std::setw(15) << std::scientific << coord1.Y << std::setw(15) << std::scientific << coord1.X
			<< std::setw(15) << std::scientific << coord1.Z << std::setw(15) << std::scientific << log10(m_ResistivityBlock.getResistivityValuesFromBlockID(blockID))
			<< std::setw(15) << std::scientific << blockID << std::endl;
	}
	ofs.close();
}

void readParameterFile(const std::string& paramFile) {

	std::ifstream ifs(paramFile.c_str(), std::ios::in);
	if (ifs.fail()) {
		std::cerr << "File open error : " << paramFile.c_str() << " !!" << std::endl;
		exit(1);
	}
	ifs >> m_numIteration;
	std::cout << "Iteration number : " << m_numIteration << std::endl;

	ifs >> m_centerCoord[0] >> m_centerCoord[1] >> m_centerCoord[2];
	std::cout << "Center coord (km) : (X, Y, Z) = (" << m_centerCoord[0] << ", " << m_centerCoord[1] << ", " << m_centerCoord[2] << ")" << std::endl;
	for (int i = 0; i < 3; ++i) {
		m_centerCoord[i] *= 1000.0;
	}

	int numBlockExcepted(0);
	ifs >> numBlockExcepted;
	/*std::cout << "Number of the parameter cells excluded : " << numBlockExcepted << std::endl;*/
	m_blockExcluded.reserve(numBlockExcepted);
	for (int iBlk = 0; iBlk < numBlockExcepted; ++iBlk) {
		int blkID(0);
		ifs >> blkID;
		m_blockExcluded.push_back(blkID);
	}

	/*std::cout << "The following parameter cells are excluded : " << std::endl;
	for (std::vector<int>::iterator itr = m_blockExcluded.begin(); itr != m_blockExcluded.end(); ++itr) {
		std::cout << *itr << std::endl;
	}*/

	ifs.close();

}



