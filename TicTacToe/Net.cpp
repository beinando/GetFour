#include "Net.h"
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

void Net::get_results(vector<double> &result_vals) const
{
	result_vals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; n++) {

		result_vals.push_back(m_layers.back()[n].get_output_val());

	}


}


//********* back prop **************

void Net::back_prop(const vector<double> &target_vals) {

	//calculate overall net error (RMS of output neuron errors)
	Layer &output_layer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < output_layer.size() - 1; n++) {

		double delta = target_vals[n] - output_layer[n].get_output_val();
		m_error += delta*delta;

	}

	m_error /= output_layer.size() - 1; // get average error squared
	m_error = sqrt(m_error);			//get RMS

										//implement a recent average measurement 
	m_recent_average_error = (m_recent_average_error * m_recent_average_smoothing_factor + m_error)
		/ (m_recent_average_smoothing_factor + 1.0);


	//calculate output layer gradients
	for (unsigned n = 0; n < output_layer.size() - 1; n++) {

		output_layer[n].calc_output_gradients(target_vals[n]);


	}


	//calculate gradients on hidden layers

	for (unsigned layer_num = m_layers.size() - 2; layer_num > 0; layer_num--) {

		Layer &hidden_layer = m_layers[layer_num];
		Layer &next_layer = m_layers[layer_num + 1];
		for (unsigned n = 0; n < hidden_layer.size(); n++) {

			hidden_layer[n].calc_hidden_gradients(next_layer);

		}
	}

	//for all layers from outputs to first hidden layer
	//update connection weights

	for (unsigned layer_num = m_layers.size() - 1; layer_num > 0; layer_num--) {

		Layer &layer = m_layers[layer_num];
		Layer &prev_layer = m_layers[layer_num - 1];

		for (unsigned n = 0; n < layer.size() - 1; n++) {

			layer[n].update_input_weights(prev_layer);

		}

	}


}








//********* back prop **************



//********** feed forward ************
void Net::feed_forward(const vector<double> &input_vals)
{

	assert(input_vals.size() == m_layers[0].size() - 1);

	//Assign (latch) the input values into the input neurons

	for (unsigned i = 0; i < input_vals.size(); i++) {

		m_layers[0][i].set_output_val(input_vals[i]);

	}

	//forward propagation
	for (unsigned layer_num = 1; layer_num < m_layers.size(); layer_num++) {
		Layer &prev_layer = m_layers[layer_num - 1];

		for (unsigned n = 0; n < m_layers[layer_num].size() - 1; n++) {
			m_layers[layer_num][n].feed_forward(prev_layer);

		}

	}

}



//********** feed forward ************


Net::Net(const vector<unsigned> &topology)
{

	unsigned num_layers = topology.size();
	for (unsigned layer_num = 0; layer_num < num_layers; layer_num++) {

		unsigned num_outputs = layer_num == topology.size() - 1 ? 0 : topology[layer_num + 1];

		m_layers.push_back(Layer());
		//We have made a new layer, now fill it with neurons, 
		//and a bias neuron to the layer:
		for (unsigned neuron_num = 0; neuron_num <= topology[layer_num]; neuron_num++) {

			m_layers.back().push_back(Neuron(num_outputs, neuron_num));
			//cout << "made a neuron!" << endl;
		}

		//force the bias nodes output values to 1.0. it's the last neuron created above
		m_layers.back().back().set_output_val(1.0);

	}

}
