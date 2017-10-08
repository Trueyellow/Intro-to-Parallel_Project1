//ISPC for root calculate by Kaixiang Huang

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

	// A simple task function
	task void
	root_task(uniform float data_points[], uniform float ground_truth[],
		uniform int len_of_each_task, uniform int total_data){
			//Do square root calculation with newton method
			//The stop sign for each calculation is root - ground_truth >= 1e-4
				float root = 2.82f;
				uniform int start = taskIndex * len_of_each_task;
		    uniform int end = min((taskIndex+1) * len_of_each_task, (unsigned int)total_data);

				foreach(index = start ... end)
						while (-(1e-4) <= (root - ground_truth[index]) <= 1e-4) {
							root = root - (root * root - data_points[index]) / (2 * root);
						}
			}


		// A simple task wrapper for root_task
		export void
		root_task_wrap(uniform float data_points[], uniform float ground_truth[],
			uniform int N, uniform int number_of_threads){
				uniform int len_of_data = N / number_of_threads;
				launch[number_of_threads] root_task(data_points, ground_truth, len_of_data, N);
			}
