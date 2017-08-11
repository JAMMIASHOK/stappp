/***************************************************************/
/*  FEM++ : A C++ finite element method code for teaching      */
/*     Computational Dynamics Laboratory                       */
/*     School of Aerospace Engineering, Tsinghua University    */
/*                                                             */
/*     http://www.comdyn.cn/                                   */
/***************************************************************/

#include "Domain.h"
#include "Outputter.h"

#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

//	Output current time and date
void Outputter::PrintTime(const struct tm * ptm, ostream& output)
{
	char *weekday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	char *month[]   = {"January", "February", "March", "April", "May", "June", "July", "August", 
		               "September", "October", "November", "December"};

	output << "        (";
	output << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " on ";
	output << month[ptm->tm_mon+1] << " " << ptm->tm_mday << ", " << ptm->tm_year + 1900 << ", " 
		   << weekday[ptm->tm_wday] << ")" << endl << endl;
}

Outputter* Outputter::_instance = NULL;

//	Constructor
Outputter::Outputter(string FileName)
{
	OutputFile.open(FileName);

	if (!OutputFile) 
	{
		cout << "*** Error *** File " << FileName << " does not exist !" << endl;
		exit(3);
	}
}

//	Return the single instance of the class
Outputter* Outputter::Instance(string FileName)
{
	if (!_instance) _instance = new Outputter(FileName);
	return _instance;
}

//	Print program logo
void Outputter::PrintLogo(ostream& output)
{
	output << "***********************************************************" << endl;
	output << "* xxxxxx  xxxxxx  xxx       xx        x            x      *" << endl;
	output << "* xx      xx      xxxx     xxx        x            x      *" << endl;
	output << "* xx      xx      xxxx     x x        x            x      *" << endl;
	output << "* xx      xx      xx x    xx x        x            x      *" << endl;
	output << "* xx      xx      xx xx   xx x        x            x      *" << endl;
	output << "* xxxxxx  xxxxxx  xx xx   x  x   xxxxxxxxxxx  xxxxxxxxxxx *" << endl;
	output << "* xx      xx      xx  xx xx  x        x            x      *" << endl;
	output << "* xx      xx      xx  xx x   x        x            x      *" << endl;
	output << "* xx      xx      xx   xxx   x        x            x      *" << endl;
	output << "* xx      xx      xx   xxx   x        x            x      *" << endl;
	output << "* xx      xxxxxxx xx    x    x        x            x      *" << endl;
	output << "***********************************************************" << endl << endl;
}
//	Print program logo
void Outputter::OutputHeading()
{
	PrintLogo(cout);
	PrintLogo(OutputFile);

	Domain* FEMData = Domain::Instance();

	cout << "TITLE : " << FEMData->GetTitle() << endl;
	OutputFile << "TITLE : " << FEMData->GetTitle() << endl;

	time_t now;
	struct tm *local = new struct tm;
	now = time(NULL);

#if defined(WIN32) 
	localtime_s(local, &now);	//  localtime_s is used only in windows
#else
    localtime_r(&now, local);	//  localtime_r is used only in unix
#endif

	PrintTime(local, cout);
	PrintTime(local, OutputFile);
}

//	Print nodal data
void Outputter::OutputNodeInfo()
{
	Domain* FEMData = Domain::Instance();

	Node* NodeList = FEMData->GetNodeList();

	cout << "C O N T R O L   I N F O R M A T I O N" << endl<< endl;
	OutputFile << "C O N T R O L   I N F O R M A T I O N" << endl<< endl;

//	Number of lines printed in each page
	int Page = 30;

	cout << setiosflags(ios::scientific) <<setprecision(5);
	OutputFile << setiosflags(ios::scientific) <<setprecision(5);

	int NUMNP = FEMData->GetNUMNP();
	int NUMEG = FEMData->GetNUMEG();
	int NLCASE = FEMData->GetNLCASE();
	int MODEX = FEMData->GetMODEX();

	cout << "      NUMBER OF NODAL POINTS . . . . . . . . . . (NUMNP)  =" << setw(6) << NUMNP << endl;
	cout << "      NUMBER OF ELEMENT GROUPS . . . . . . . . . (NUMEG)  =" << setw(6) << NUMEG << endl;
	cout << "      NUMBER OF LOAD CASES . . . . . . . . . . . (NLCASE) =" << setw(6) << NLCASE << endl;
	cout << "      SOLUTION MODE  . . . . . . . . . . . . . . (MODEX)  =" << setw(6) << MODEX << endl;
	cout << "         EQ.0, DATA CHECK" << endl
		 << "         EQ.1, EXECUTION" << endl << endl;

	OutputFile << "      NUMBER OF NODAL POINTS . . . . . . . . . . (NUMNP)  =" << setw(6) << NUMNP << endl;
	OutputFile << "      NUMBER OF ELEMENT GROUPS . . . . . . . . . (NUMEG)  =" << setw(6) << NUMEG << endl;
	OutputFile << "      NUMBER OF LOAD CASES . . . . . . . . . . . (NLCASE) =" << setw(6) << NLCASE << endl;
	OutputFile << "      SOLUTION MODE  . . . . . . . . . . . . . . (MODEX)  =" << setw(6) << MODEX << endl;
	OutputFile << "         EQ.0, DATA CHECK" << endl
			   << "         EQ.1, EXECUTION" << endl << endl;

	cout << " N O D A L   P O I N T   D A T A" << endl << endl;
	cout << "    NODE       BOUNDARY                         NODAL POINT" << endl
		 << "   NUMBER  CONDITION  CODES                     COORDINATES" << endl;
	OutputFile << " N O D A L   P O I N T   D A T A" << endl << endl;
	OutputFile << "    NODE       BOUNDARY                         NODAL POINT" << endl
			   << "   NUMBER  CONDITION CODES                      COORDINATES" << endl;
 
	for (int i = 0; i < NUMNP; i++)
	{
		Node node = NodeList[i];
		cout << setw(9) << i + 1 << setw(5) << node.bcode[0] << setw(5) << node.bcode[1] << setw(5) << node.bcode[2]
			 << setw(18) << node.XYZ[0] << setw(15) << node.XYZ[1] << setw(15) << node.XYZ[2] << endl;
		OutputFile << setw(9) << i + 1 << setw(5) << node.bcode[0] << setw(5) << node.bcode[1] << setw(5) << node.bcode[2]
				   << setw(18) << node.XYZ[0] << setw(15) << node.XYZ[1] << setw(15) << node.XYZ[2] << endl;
	}

	cout << endl;
	OutputFile << endl;
}

//	Output equation numbers
void Outputter::OutputEquationNumber()
{
	Domain* FEMData = Domain::Instance();
	int NUMNP = FEMData->GetNUMNP();

	Node* NodeList = FEMData->GetNodeList();

	cout << " EQUATION NUMBERS" << endl << endl;
	cout << "   NODE NUMBER   DEGREES OF FREEDOM" << endl;
	cout << "        N           X    Y    Z" << endl;

	OutputFile << " EQUATION NUMBERS" << endl << endl;
	OutputFile << "   NODE NUMBER   DEGREES OF FREEDOM" << endl;
	OutputFile << "        N           X    Y    Z" << endl;

	for (int np = 0; np < NUMNP; np++)	// Loop over for all node
	{
		cout << setw(9) << np+1 << "       ";
		OutputFile << setw(9) << np+1 << "       ";

		for (int dof = 0; dof < Node::NDF; dof++)	// Loop over for DOFs of node np
		{
			cout << setw(5) << NodeList[np].bcode[dof];
			OutputFile << setw(5) << NodeList[np].bcode[dof];
		}

		cout << endl;
		OutputFile << endl;
	}

	cout << endl;
	OutputFile << endl;
}

//	Output element data
void Outputter::OutputElementInfo()
{
//	Print element group control line

	Domain* FEMData = Domain::Instance();

	unsigned int NUMEG = FEMData->GetNUMEG();
	unsigned int* NUME = FEMData->GetNUME();
	Element** ElementSetList = FEMData->GetElementSetList();

	unsigned int* NUMMAT = FEMData->GetNUMMAT();
	Material** MaterialSetList = FEMData->GetMaterialSetList();

	cout << " E L E M E N T   G R O U P   D A T A" << endl << endl << endl;
	OutputFile << "E L E M E N T   G R O U P   D A T A" << endl << endl << endl;

	for (int EleGrp = 0; EleGrp < NUMEG; EleGrp++)
	{
		cout << " E L E M E N T   D E F I N I T I O N" << endl << endl;
		OutputFile << " E L E M E N T   D E F I N I T I O N" << endl << endl;

		unsigned int ElementType = FEMData->GetElementTypes()[EleGrp];
		unsigned int NUME = FEMData->GetNUME()[EleGrp];

		cout << " ELEMENT TYPE  . . . . . . . . . . . . .( NPAR(1) ) . . =" << setw(5) << ElementType << endl;
		cout << "     EQ.1, TRUSS ELEMENTS" << endl 
			 << "     EQ.2, ELEMENTS CURRENTLY" << endl
			 << "     EQ.3, NOT AVAILABLE" << endl << endl;

		cout << " NUMBER OF ELEMENTS. . . . . . . . . . .( NPAR(2) ) . . =" << setw(5) << NUME << endl << endl;

		OutputFile << " ELEMENT TYPE  . . . . . . . . . . . . .( NPAR(1) ) . . =" << setw(5) << ElementType << endl;
		OutputFile << "     EQ.1, TRUSS ELEMENTS" << endl 
				   << "     EQ.2, ELEMENTS CURRENTLY" << endl
				   << "     EQ.3, NOT AVAILABLE" << endl << endl;

		OutputFile << " NUMBER OF ELEMENTS. . . . . . . . . . .( NPAR(2) ) . . =" << setw(5) << NUME << endl << endl;

		switch (ElementType)
		{
		case 1:	// Bar element
			PrintBarElementData(EleGrp);
			break;

		} 
	}
}

//	Output bar element data
void Outputter::PrintBarElementData(int EleGrp)
{

	Domain* FEMData = Domain::Instance();

	int NUMMAT = FEMData->GetNUMMAT()[EleGrp];
	BarMaterial* MaterialGroup = (BarMaterial*) FEMData->GetMaterialSetList()[EleGrp];

	cout << " M A T E R I A L   D E F I N I T I O N" << endl << endl;
	cout << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	cout << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT << endl << endl;

	cout << "  SET       YOUNG'S     CROSS-SECTIONAL" << endl
		 << " NUMBER     MODULUS          AREA" << endl
		 << "               E              A" << endl;
	
	OutputFile << " M A T E R I A L   D E F I N I T I O N" << endl << endl;
	OutputFile << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	OutputFile << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT << endl << endl;

	OutputFile << "  SET       YOUNG'S     CROSS-SECTIONAL" << endl
			   << " NUMBER     MODULUS          AREA" << endl
			   << "               E              A" << endl;

	cout << setiosflags(ios::scientific) <<setprecision(5);
	OutputFile << setiosflags(ios::scientific) <<setprecision(5);

//	Loop over for all property sets
	for (int mset = 0; mset < NUMMAT; mset++)
	{
		cout << setw(5) << mset+1 << setw(16) << MaterialGroup->E << setw(16) << MaterialGroup->Area << endl;
		OutputFile << setw(5) << mset+1 << setw(16) << MaterialGroup->E  << setw(16) << MaterialGroup->Area << endl;
	}

	cout << endl << endl << " E L E M E N T   I N F O R M A T I O N" << endl;
	cout << " ELEMENT     NODE     NODE       MATERIAL" << endl
		 << " NUMBER-N      I        J       SET NUMBER" << endl; 

	OutputFile << endl << endl << " E L E M E N T   I N F O R M A T I O N" << endl;
	OutputFile << " ELEMENT     NODE     NODE       MATERIAL" << endl
			   << " NUMBER-N      I        J       SET NUMBER" << endl; 

	Element** ElementSetList = FEMData->GetElementSetList();
	Bar* ElementGroup = (Bar* ) ElementSetList[EleGrp];

//	Loop over for all elements in group EleGrp
	for (int Ele = 0; Ele < FEMData->GetNUME()[EleGrp]; Ele++)
	{
		Node** nodes = ElementGroup[Ele].GetNodes();
		Material* ElementMaterial = ElementGroup[Ele].GetElementMaterial();

		cout << setw(5) << Ele+1 << setw(11) << nodes[0]->num << setw(9) << nodes[1]->num 
			 << setw(12) << ElementMaterial->num << endl;
		OutputFile << setw(5) << Ele+1 << setw(11) << nodes[0]->num << setw(9) << nodes[1]->num 
				   << setw(12) << ElementMaterial->num << endl;
	}

	cout << endl;
	OutputFile << endl;
}

//	Print load data
void Outputter::OutputLoadInfo()
{
	Domain* FEMData = Domain::Instance();

	for (int lcase = 1; lcase <= FEMData->GetNLCASE(); lcase++)
	{
		LoadCaseData* LoadData = &FEMData->GetLoadCases()[lcase - 1];

		cout << setiosflags(ios::scientific);
		OutputFile << setiosflags(ios::scientific);

		cout << " L O A D   C A S E   D A T A" << endl << endl;
		OutputFile << " L O A D   C A S E   D A T A" << endl << endl;

		cout << "     LOAD CASE NUMBER . . . . . . . =" << setw(6) << lcase << endl;
		cout << "     NUMBER OF CONCENTRATED LOADS . =" << setw(6) << LoadData->nloads << endl << endl;
		cout << "    NODE       DIRECTION      LOAD" << endl
			 << "   NUMBER                   MAGNITUDE" << endl;
		OutputFile << "     LOAD CASE NUMBER . . . . . . . =" << setw(6) << lcase << endl;
		OutputFile << "     NUMBER OF CONCENTRATED LOADS . =" << setw(6) << LoadData->nloads << endl << endl;
		OutputFile << "    NODE       DIRECTION      LOAD" << endl
				   << "   NUMBER                   MAGNITUDE" << endl;

		for (int i = 0; i < LoadData->nloads; i++)
		{
			cout << setw(7) << LoadData->node[i] << setw(13) << LoadData->dof[i]  << setw(19) << LoadData->load[i] << endl;
			OutputFile << setw(7) << LoadData->node[i] << setw(13) << LoadData->dof[i]  << setw(19) << LoadData->load[i] << endl;
		}

		cout << endl;
		OutputFile << endl;
	}
}

//	Print nodal displacement
void Outputter::OutputNodalDisplacement(int lcase)
{
	Domain* FEMData = Domain::Instance();
	Node* NodeList = FEMData->GetNodeList();
	double* Displacement = FEMData->GetDisplacement();

	cout << " LOAD CASE" << setw(5) << lcase+1 << endl << endl << endl;
	OutputFile << " LOAD CASE" << setw(5) << lcase+1 << endl << endl << endl;

	cout << setiosflags(ios::scientific);
	OutputFile << setiosflags(ios::scientific);

	cout << " D I S P L A C E M E N T S" << endl << endl;
	cout << "  NODE           X-DISPLACEMENT    Y-DISPLACEMENT    Z-DISPLACEMENT" << endl;
	OutputFile << " D I S P L A C E M E N T S" << endl << endl;
	OutputFile << "  NODE           X-DISPLACEMENT    Y-DISPLACEMENT    Z-DISPLACEMENT" << endl;

	for (int i = 0; i < FEMData->GetNUMNP(); i++)
	{
		cout << setw(5) << i + 1 << "        ";
		OutputFile << setw(5) << i + 1 << "        ";

		for (int j = 0; j < Node::NDF; j++)
		{
			if (NodeList[i].bcode[j] == 0)
			{
				cout << setw(18) << 0.0;
				OutputFile << setw(18) << 0.0;
			}
			else
			{
				cout << setw(18) << Displacement[NodeList[i].bcode[j] - 1];
				OutputFile << setw(18) << Displacement[NodeList[i].bcode[j] - 1];
			}
		}

		cout << endl;
		OutputFile << endl;
	}
}

//	Print total system data
void Outputter::OutputTotalSystemData()
{
	Domain* FEMData = Domain::Instance();

	cout << "	TOTAL SYSTEM DATA" << endl << endl;
	OutputFile << "	TOTAL SYSTEM DATA" << endl << endl;

	cout << "     NUMBER OF EQUATIONS . . . . . . . . . . . . . .(NEQ) = " << FEMData->GetNEQ() << endl
		 << "     NUMBER OF MATRIX ELEMENTS . . . . . . . . . . .(NWK) = " << FEMData->GetNWK() << endl
		 << "     MAXIMUM HALF BANDWIDTH  . . . . . . . . . . . .(MK ) = " << FEMData->GetMK() << endl
		 << "     MEAN HALF BANDWIDTH . . . . . . . . . . . . . .(MM ) = " << FEMData->GetNWK()/FEMData->GetNEQ() 
		 << endl << endl << endl;

	OutputFile << "     NUMBER OF EQUATIONS . . . . . . . . . . . . . .(NEQ) = " << FEMData->GetNEQ() << endl
			   << "     NUMBER OF MATRIX ELEMENTS . . . . . . . . . . .(NWK) = " << FEMData->GetNWK() << endl
			   << "     MAXIMUM HALF BANDWIDTH  . . . . . . . . . . . .(MK ) = " << FEMData->GetMK() << endl
			   << "     MEAN HALF BANDWIDTH . . . . . . . . . . . . . .(MM ) = " << FEMData->GetNWK()/FEMData->GetNEQ() 
			   << endl << endl << endl;
}

#ifdef _DEBUG_

//	Print column heights for debuging
void Outputter::PrintColumnHeights()
{
	cout << "************************** Column Heights ****************************" << endl;
	OutputFile << "************************** Column Heights ****************************" << endl;

	Domain* FEMData = Domain::Instance();

	int NEQ = FEMData->GetNEQ();
	unsigned int* ColumnHeights = FEMData->GetColumnHeights();

	for (int col = 0; col < NEQ; col++)
	{
		if (col+1 % 10 == 0)
		{
			cout << endl;
			OutputFile << endl;
		}

		cout << setw(8) << ColumnHeights[col];
		OutputFile << setw(8) << ColumnHeights[col];
	}
	cout << endl << endl;
	OutputFile << endl << endl;
}

//	Print address of diagonal elements for debuging
void Outputter::PrintDiagonalAddress()
{
	cout << "******************** Address of Diagonal Element *********************" << endl;
	OutputFile << "******************** Address of Diagonal Element *********************" << endl;

	Domain* FEMData = Domain::Instance();

	int NEQ = FEMData->GetNEQ();
	unsigned int* DiagonalAddress = FEMData->GetDiagonalAddress();

	for (int col = 0; col <= NEQ; col++)
	{
		if (col+1 % 10 == 0)
		{
			cout << endl;
			OutputFile << endl;
		}

		cout << setw(8) << DiagonalAddress[col];
		OutputFile << setw(8) << DiagonalAddress[col];
	}

	cout << endl << endl;
	OutputFile << endl << endl;
}

//	Print banded and full stiffness matrix for debuging
void Outputter::PrintStiffnessMatrix()
{
	cout << "********************** Banded stiffness matrix ***********************" << endl;
	OutputFile << "********************** Banded stiffness matrix ***********************" << endl;

	Domain* FEMData = Domain::Instance();

	int NEQ = FEMData->GetNEQ();
	unsigned int* DiagonalAddress = FEMData->GetDiagonalAddress();
	double* StiffnessMatrix = FEMData->GetStiffnessMatrix();

	cout << setiosflags(ios::scientific) <<setprecision(5);
	OutputFile << setiosflags(ios::scientific) <<setprecision(5);

	for (int i = 0; i < DiagonalAddress[NEQ]-1; i++)
	{
		if ((i+1) % 6 == 0)
		{
			cout << endl;
			OutputFile << endl;
		}

		cout << setw(14) << StiffnessMatrix[i];
		OutputFile << setw(14) << StiffnessMatrix[i];
	}

	cout << endl << endl;
	OutputFile << endl << endl;

	cout << "*********************** Full stiffness matrix ************************" << endl;
	OutputFile << "*********************** Full stiffness matrix ************************" << endl;

	for (int I = 0; I < NEQ; I++)
	{
		for (int J = 0; J < NEQ; J++)
		{
			int i = I;
			int j = J;
			if (i > j)
				swap(i,j);

			int H = DiagonalAddress[j + 1] - DiagonalAddress[j];
			if (j - i - H >= 0) 
			{
				cout << setw(14) << 0.0;
				OutputFile << setw(14) << 0.0;
			}
			else
			{
				cout << setw(14) << StiffnessMatrix[DiagonalAddress[j] + j - i - 1];
				OutputFile << setw(14) << StiffnessMatrix[DiagonalAddress[j] + j - i - 1];
			}
		}

		cout << endl;
		OutputFile << endl;
	}

	cout << endl;
	OutputFile << endl;
}

//	Print displacement vector for debuging
void Outputter::PrintDisplacement(int loadcase)
{
	cout << "********************** Displacement vector ***********************" << endl;
	OutputFile << "********************** Displacement vector ***********************" << endl;

	Domain* FEMData = Domain::Instance();

	int NEQ = FEMData->GetNEQ();
	double* Force = FEMData->GetForce();

	cout << "  Load case = " << loadcase << endl;
	OutputFile << "  Load case = " << loadcase << endl;

	cout << setiosflags(ios::scientific) <<setprecision(5);
	OutputFile << setiosflags(ios::scientific) <<setprecision(5);

	for (int i = 0; i < NEQ; i++)
	{
		if ((i+1) % 6 == 0)
		{
			cout << endl;
			OutputFile << endl;
		}

		cout << setw(14) << Force[i];
		OutputFile << setw(14) << Force[i];
	}

	cout << endl << endl;
	OutputFile << endl << endl;
}

#endif