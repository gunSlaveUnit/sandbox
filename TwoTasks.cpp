// Interesting tasks for lecture

#include <iostream>

int main() {
	int amountTasks;
	std::cin >> amountTasks;

	int* goodTasksForTeacher = new int[amountTasks];
	for (int i = 0; i < amountTasks; ++i)
		std::cin >> goodTasksForTeacher[i];

	int* goodTasksForStudents = new int[amountTasks];
	for (int i = 0; i < amountTasks; ++i)
		std::cin >> goodTasksForStudents[i];

	int amountGoodTasks = 0;
	for (int i = 0; i < amountTasks - 1; ++i)
		for (int j = 1; j < amountTasks; ++j)
			if (i < j &&
						(goodTasksForTeacher[i] + goodTasksForTeacher[j] > 
						goodTasksForStudents[i] + goodTasksForStudents[j])) 
				++amountGoodTasks;
			else 
				continue;

	std::cout << amountGoodTasks;

	delete[] goodTasksForTeacher;
	delete[] goodTasksForStudents;
}