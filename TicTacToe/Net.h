#include "Neuron.h"


class Net
{
public:
	Net(const vector<unsigned> &topology);
	//& : reference
	void feed_forward(const vector<double> &input_vals);
	void back_prop(const vector<double> &target_vals);
	void get_results(vector<double> &result_vals) const;
	double get_recent_average_error(void) const { return m_recent_average_error; }
	vector<Layer> m_layers;

private:

	double m_error;
	double m_recent_average_error;
	double m_recent_average_smoothing_factor;
};
