export void root_calculate(uniform float data_points[], uniform float ground_truth[], uniform int N){
	foreach(index=0 ... N){
		//Do square root calculation with newton method
		//The stop sign for each calculation is root - ground_truth >= 1e-4
			float root = 2.82f;
			while (-(1e-4) <= (root - ground_truth[index]) <= 1e-4) {
				root = root - (root * root - data_points[index]) / (2 * root);
			}
		}
	}

