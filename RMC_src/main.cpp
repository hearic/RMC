# include "CalMode.h"
# include "Input.h"

//////////  Global classes  //////////
CDRNG ORNG;
CDOutput Output;

# ifdef _IS_PARALLEL_
CDParallel OParallel;
# endif

int main(int argc_temp,char *argv_temp[])
{
	///////////////// Define objects ////////////////////////
	///////  decoupling via refactoring at 20130720   ///////
	CDInput OInput;
	CDGeometry OGeometry;
	CDMaterial OMaterial;
	CDAceData OAceData;
	CDCalMode OCalMode;
	CDCriticality OCriticality;
	CDBurnup OBurnup;
	CDTally OTally;
	CDConvergence OConvergence;
	CDParticleState OParticleState;
	CDPlot OPlot;
	CDFixedSource OFixedSource;
	CDNeutronTransport ONeutronTransport;

	///////////////// Parallel Case ////////////////////////
# ifdef _IS_PARALLEL_
	MPI_Init(&argc_temp,&argv_temp);
	OParallel.InitiateParallel();	
# endif

	/////////Check command line and inp file exist: start ////////////
	OInput.CheckIOFile(argc_temp,argv_temp);
	
	////////////// Write output heading ////////////////		
	Output.OutputHeading(OCriticality);

	////////////// Read input file ////////////////
	OInput.ReadInuptBlocks(OCalMode,OGeometry,OMaterial,OAceData,OCriticality,OTally,OConvergence,OBurnup,OPlot, OFixedSource); 
	
	////////////// Plot Image(geometry) ////////////////
	OPlot.RunPlot(OGeometry, OMaterial);

	////////////// Generate Input File////////////////
	Output.GenerateInpFile(OGeometry, OMaterial, OAceData, OCriticality, OTally, OConvergence, OBurnup, OFixedSource);

	////////////// Run Calculation ////////////////
	OCalMode.RunCalculation(ONeutronTransport, OAceData,OGeometry,Depth,OCriticality,OMaterial,OParticleState,OConvergence,OTally,OBurnup, OFixedSource);

	////////////// Write output ending ////////////////
	Output.OutputEnding(OCalMode); 

	///////////////// Parallel Case ////////////////////////
# ifdef _IS_PARALLEL_
	MPI_Finalize();
# endif 

	return 1; 
}