network net;

net.randomNetwork (10,0.1,kuramoto(), weightedEdge());

net.observeAll ("output/observeAll.allStates");


for (int i = 0; i < 10; i = i + 1)	
	net.observe (i, "output/observeAll.allStates2");


net.evolve(0.0,10.0);


