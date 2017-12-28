#include <fstream>
#include <vector>


using namespace std;

class TrainingData {

public:
	TrainingData(const string filename);
	bool isEof(void) { return m_training_data_file.eof(); }
	void get_topology(vector<unsigned> &topology);

	//returns the number of input values read from the file:
	unsigned get_next_inputs(vector<double> &input_vals);
	unsigned get_target_out;
	unsigned get_target_outputs(vector<double> &target_output_vals);

private:
	ifstream m_training_data_file;

};
