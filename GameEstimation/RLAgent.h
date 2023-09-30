#pragma once

#include "network.h"
#include "Position.h"

class RLAgent
{
public:
	network *nn;
	Position* pos;

	virtual void train(size_t steps, size_t batchSize, double eps) = 0;
};

class Qlearn : public RLAgent { 
public:
	void train(size_t steps, size_t batchSize = 10, double eps = 0);
};