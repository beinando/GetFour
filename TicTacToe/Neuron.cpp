#include "Neuron.h"


double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;

void Neuron::update_input_weights(Layer &prev_layer) {

	//the wights to be updated are in the connection container 
	//in the neurons in the preceeding layer

	for (unsigned n = 0; n < prev_layer.size(); n++) {
		Neuron &neuron = prev_layer[n];
		double old_delta_weight = neuron.m_output_weights[m_my_index].delta_weight;
		double new_delta_weight =
			//Individual input, magnified by the gradient and train rate
			eta
			* neuron.get_output_val()
			*m_gradient
			//also add momentum = a fraction of the previous delta weight
			+ alpha
			* old_delta_weight;

		neuron.m_output_weights[m_my_index].delta_weight = new_delta_weight;
		neuron.m_output_weights[m_my_index].weight += new_delta_weight;

	}

}

double Neuron::sum_dow(const Layer &next_layer) const {

	double sum = 0.0;

	//sum of our contributions of the errors at the nodes we feed

	for (unsigned n = 0; n < next_layer.size() - 1; n++) {

		sum += m_output_weights[n].weight*next_layer[n].m_gradient;
	}

	return sum;

}


void Neuron::calc_hidden_gradients(const Layer &next_layer)
{

	double dow = sum_dow(next_layer);
	m_gradient = dow*Neuron::transfer_function_derivative(m_output_val);

}


void Neuron::calc_output_gradients(double target_val)
{

	double delta = target_val - m_output_val;
	m_gradient = delta*Neuron::transfer_function_derivative(m_output_val);



}





double Neuron::transfer_function(double x)
{

	return tanh(x);


}

// x := tanh(x) from transfer_function, so since d/dx tanh(x) = 1-tanh^2(x) it makes sense
double Neuron::transfer_function_derivative(double x)
{

	return 1.0 - x*x;


}


void Neuron::feed_forward(const Layer &prev_layer)
{

	double sum = 0.0;

	//sum up the previous layers outputs (which are our inputs)
	//include the bias node from the previous layer

	for (unsigned n = 0; n < prev_layer.size(); n++) {
		sum += prev_layer[n].get_output_val()*
			prev_layer[n].m_output_weights[m_my_index].weight;

	}

	m_output_val = Neuron::transfer_function(sum);


}
Neuron::Neuron(unsigned num_outputs, unsigned my_index)
{

	for (unsigned c = 0; c < num_outputs; c++) {

		m_output_weights.push_back(Connection());
		m_output_weights.back().weight = random_weight();

	}

	m_my_index = my_index;

}