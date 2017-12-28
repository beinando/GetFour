#include "TrainingData.h"
#include <vector>
#include <sstream>

using namespace std;


TrainingData::TrainingData(const string filename)
{
	m_training_data_file.open(filename.c_str());


}

void TrainingData::get_topology(vector<unsigned> &topology)
{

	string line;
	string label;

	getline(m_training_data_file, line);
	stringstream ss(line);
	ss >> label;
	if (this->isEof() || label.compare("topology:") != 0) {

		abort();
	}

	while (!ss.eof()) {

		unsigned n;
		ss >> n;
		topology.push_back(n);

	}

	return;

}

unsigned TrainingData::get_next_inputs(vector<double> &input_vals)
{

	input_vals.clear();

	string line;
	getline(m_training_data_file, line);
	stringstream ss(line);


	string label;
	ss >> label;
	if (label.compare("in:") == 0) {

		double one_value;
		while (ss >> one_value) {

			input_vals.push_back(one_value);

		}

	}

	return input_vals.size();

}


unsigned TrainingData::get_target_outputs(vector<double> &target_output_vals)
{

	target_output_vals.clear();
	string line;
	getline(m_training_data_file, line);
	stringstream ss(line);


	string label;
	ss >> label;

	if (label.compare("out:") == 0) {

		double one_value;
		while (ss >> one_value) {

			target_output_vals.push_back(one_value);

		}

	}


	return target_output_vals.size();



}