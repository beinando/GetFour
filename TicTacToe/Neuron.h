// ********************** class neuron **************
#include <vector>

using namespace std;

struct Connection {

	double weight;
	double delta_weight;

};


class Neuron;
typedef vector<Neuron> Layer;

class Neuron {

public:
	Neuron(unsigned num_outputs, unsigned my_index);
	void set_output_val(double val) { m_output_val = val; };
	double get_output_val(void) const { return m_output_val; };
	void feed_forward(const Layer &prev_layer);
	void calc_output_gradients(double target_val);
	void calc_hidden_gradients(const Layer &next_layer);
	void update_input_weights(Layer &prev_layer);
	void get_results(vector<double> &result_vals);
	vector<Connection> m_output_weights;

private:

	static double eta; //[0.0 ... 1.0] overall net training rate
	static double alpha; //[0.0 ... n] multiplier of last weight change (momentum)

	static double transfer_function(double x);
	static double transfer_function_derivative(double x);
	double sum_dow(const Layer &next_layer) const;
	unsigned m_my_index;
	double m_output_val;
	

	static double random_weight(void) { return rand() / double(RAND_MAX); }
	double m_gradient;

};

