#include "RLAgent.h"
#include <random>
#include <iostream>

void Qlearn::train(size_t steps, size_t batchSize, double eps)
{
	Position* next;
	Position* root = pos;
	size_t n;
	double error = 0, t;
	network deriv = *nn;

	size_t episode = 1;

	for (int step = 0; step < steps; step++) {
		if ((step + 1) % 1000000 == 0) {
			nn->save("rocket");
			std::cout << "save\n";
		}

		pos->FillNext();
		n = pos->next.size();

		//pos->save(std::cout);
		
		if (n == 0) {
			if ((episode++) % 100 == 0) {
				std::cout << error << std::endl;
				for (int i = 0; i < 6; i++)
					std::cout << nn->layers[0].W.get(0, i) << " ";
				std::cout << std::endl << std::endl;
			}

			error = 0;
			pos->newGame();
			pos->freePrevious();
			//system("cls");
			//return;
			continue;
		}

		pos->DeepEstimation(1);
		t = nn->feed(pos->vectorize()).get(0, 0) - pos->rating;
		error += t * t;
		//std::cout << t * t << " ";
		deriv.layers[0].W.zero();
		deriv.layers[0].B.zero();
		deriv.backprop(pos->vectorize(), pos->rating, *nn);
		std::cout << nn->feed(pos->vectorize()).get(0, 0) << " ";
		if (deriv.curBatch >= batchSize)
			nn->applyGrad(deriv, 0.001);
		std::cout << nn->feed(pos->vectorize()).get(0, 0) << std::endl;
		pos->Estimate();
		std::cout << pos->rating << std::endl;

		deriv.layers[0].B.T().show(std::cout);
		deriv.layers[0].W.T().show(std::cout);
		nn->layers[0].B.T().show(std::cout);
		nn->layers[0].W.T().show(std::cout);

		std::cout << "deriv\n";

		t = nn->feed(pos->vectorize()).get(0, 0) - pos->rating;
		//error += t * t;
		//std::cout << t * t << std::endl;

		if (double(rand()) / RAND_MAX > eps)
			pos = pos->makeBestMove(1);
		else pos = pos->makeMove(rand() % n);
	}
}
