

#include "fullNetworkWithNodes.h"

void registerStandards()
{

	stdSdeIntegrator::registerStandardValues();
	pcoBase::registerStandardValues();
	containerNode<baseType, 0>::registerStandardValues();
	dynNetwork::registerStandardValues();
//	createNetwork::registerStandardValues();
	escapeNoiseNeuron::registerStandardValues();
	stdOdeIntegrator::registerStandardValues();

	network::registerStandardValues();
	pcoDelay::registerStandardValues();
//	pulseCoupledExcitatoryNeuron::registerStandardValues();
//	sigEdgeParams::registerStandardValues();
	streamInNode::registerStandardValues();
	streamOutNode::registerStandardValues();
	streamOutNodeHist::registerStandardValues();
	sdeNode::registerStandardValues();
#if OPENGL
	GLNetwork<baseType>::registerStandardValues();
#endif
	#include "generatedRegisterStandards.h"

}



