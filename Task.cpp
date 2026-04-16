#include "Task.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

void SaveTaskToFile(const std::vector<Task>& tasks, const std::string& filename)
{
	std::ofstream fstream(filename);
	fstream << tasks.size();
		
	for (const Task& task : tasks) {
		std::string tempDescr = task.description;
		std::replace(tempDescr.begin(), tempDescr.end(), ' ', '_');
		fstream << '\n' << tempDescr << ' ' << task.done;
	}

	fstream.close();
}

std::vector<Task> LoadTasksFromFile(const std::string& filename)
{
	if (!std::filesystem::exists(filename)) return std::vector<Task>();
	std::ifstream istream(filename);

	int size; istream >> size;
	if (size <= 0 || !istream) return std::vector<Task>();

	std::vector<Task> tasks(size);
	for (int i = 0; i < size; i++) {
		std::string tempDescr;
		istream >> tempDescr >> tasks[i].done;
		std::replace(tempDescr.begin(), tempDescr.end(), '_', ' ');
		tasks[i].description = tempDescr;
	}

	istream.close();
	return tasks;
}
