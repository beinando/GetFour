#pragma once


struct BoardData {
	vector<double> board_state;
	int			times_played;
	double		value;
	//for second net
	double		value2;

};


namespace general_functions{

	

	void sleep(int milliseconds) {

		std::chrono::milliseconds timespan(milliseconds);

		std::this_thread::sleep_for(timespan);

	}






	double generate_random_number(int start, int end) {

		std::mt19937 rng;
		rng.seed(chrono::high_resolution_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(start, end); // distribution in range [1, 6]

		return (double)dist6(rng);

	}


	void show_vector_vals(string label, vector<double> &v)
	{

		cout << label << " ";
		for (unsigned i = 0; i < v.size(); i++) {

			cout << v[i] << " ";

		}

		cout << endl;



	}


	

	inline int get_index_from_database(vector<double> &i_state, vector<BoardData> &i_database) {

		for (int i = 0; i < i_database.size(); i++) {


			if (i_state == i_database[i].board_state) {

				return i;

			}

			else if (i == i_database.size() - 1) {

				return -1;

			}

		}

	}

	vector<double> get_mirrored_state(vector<double> &input_state, int mirror_axis) {

		//mirror axis : horizontal(0) or vertical(1) mirroring

		int col;
		int row;

		int mirrored_row;
		int mirrored_col;
		int mirrored_index;

		vector<double> output_state;

		for (size_t i = 0; i < input_state.size(); i++)
		{
			output_state.push_back(0);
		}


		for (int i = 0; i < input_state.size(); i++) {

			col = i % 3;
			row = i / 3;

			mirrored_row = (-1 * (row - 1)) + 1;
			mirrored_col = (-1 * (col - 1)) + 1;

			mirrored_index = 3 * mirrored_row + col;

			output_state[mirrored_index] = input_state[i];


		}

		return output_state;

	}






	vector<sf::Sprite> get_mirrored_sprites(vector<sf::Sprite> &input_sprites, int mirror_axis) {

		//mirror axis : horizontal(0) or vertical(1) mirroring

		int col;
		int row;



		int mirrored_row;
		int mirrored_col;
		int mirrored_index;

		vector<sf::Sprite> output_sprites;

		for (size_t i = 0; i < input_sprites.size(); i++)
		{
			output_sprites.push_back(sf::Sprite());
			const sf::Texture* test;
			test = input_sprites[i].getTexture();
			output_sprites[i].setTexture(*test);

		}


		for (int i = 0; i < input_sprites.size(); i++) {

			col = input_sprites[i].getPosition().x / 150;
			row = input_sprites[i].getPosition().y / 150;

			mirrored_col = (-1 * (col - 1)) + 1;
			mirrored_row = (-1 * (row - 1)) + 1;


			output_sprites[i].setPosition(mirrored_col * 150, mirrored_row * 150);




		}



		return output_sprites;

	}




	double rotate_x(int x, int y, double degree) {

		double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00;
		degree = pi*degree * 2 / 360;

		double a = x*cos(degree);
		double b = -y*sin(degree);
		double c = a + b;
		return (c);



	}

	double rotate_y(int x, int y, double degree) {

		double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00;
		degree = pi*degree * 2 / 360;

		double a = x*sin(degree);
		double b = y*cos(degree);
		double c = a + b;
		return (c);



	}

	int check_for_win(vector<double> &board_state) {

		for (int pos = 0; pos < 42; pos++) {

			int t_row = pos / 7;
			int t_col = pos % 7;

			//check row // WORKS 
			if (!(t_row + 1 >= 6 || t_row + 2 >= 6 || t_row + 3 >= 6)) {

				if
					(
					(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 1) * 7 + t_col]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 2) * 7 + t_col]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 3) * 7 + t_col]) &&
						((board_state[(t_row) * 7 + t_col] == 1) || (board_state[(t_row) * 7 + t_col] == -1))
						)
				{

					if (board_state[(t_row) * 7 + t_col] == 1) {

						return 1;

					}
					else {

						return -1;

					}
					

				}

			}

			//check col // WORKS
			if (!(t_col + 1 >= 7 || t_col + 2 >= 7 || t_col + 3 >= 7)) {

				if
					(
					(board_state[(t_row) * 7 + t_col] == board_state[(t_row) * 7 + t_col + 1]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row) * 7 + t_col + 2]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row) * 7 + t_col + 3]) &&
						((board_state[(t_row) * 7 + t_col] == 1) || (board_state[(t_row) * 7 + t_col] == -1))
						)
				{
					if (board_state[(t_row) * 7 + t_col] == 1) {

						return 1;

					}
					else {

						return -1;

					}

				}

			}


			// check bottom left to top right diagonal // WORKS
			if (!((t_row + 1 >= 6 || t_row + 2 >= 6 || t_row + 3 >= 6) || (t_col - 1 < 0 || t_col - 2 < 0 || t_col - 3 < 0))) {

				if
					(
					(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 1) * 7 + t_col - 1]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 2) * 7 + t_col - 2]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 3) * 7 + t_col - 3]) &&
						((board_state[(t_row) * 7 + t_col] == 1) || (board_state[(t_row) * 7 + t_col] == -1))
						)
				{
					if (board_state[(t_row) * 7 + t_col] == 1) {

						return 1;

					}
					else {

						return -1;

					}

				}

			}

			// check bottom right to top left diagonal // WORKS
			if (!((t_row + 1 >= 6 || t_row + 2 >= 6 || t_row + 3 >= 6) || (t_col + 1 >= 7 || t_col + 2 >= 7 || t_col + 3 >= 7))) {
				if
					(
					(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 1) * 7 + t_col + 1]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 2) * 7 + t_col + 2]) &&
						(board_state[(t_row) * 7 + t_col] == board_state[(t_row + 3) * 7 + t_col + 3]) &&
						((board_state[(t_row) * 7 + t_col] == 1) || (board_state[(t_row) * 7 + t_col] == -1))
						)
				{
					if (board_state[(t_row) * 7 + t_col] == 1) {

						return 1;

					}
					else {

						return -1;

					}
				}
				//end outer if
			}
			//end for loop
		}

		return 0;

	}

	//**** WRITE / READ FROM / IN FILES

	void write_in_file(vector<Layer> i_layers, string output_file) {

		ofstream out_file;
		out_file.open(output_file);
		
		int foo_1 = i_layers.size();

		for (int layer_count = 0; layer_count < i_layers.size() - 1; layer_count++) {
			out_file << "LAYER " << layer_count << "\n";
			out_file << "******************************" << "\n";
			//iterate through all neurons in specific layer
			for (int neuron_count = 0; neuron_count < i_layers[layer_count].size(); neuron_count++) {
				//iterate through all next possible neurons
				int foo_2 = i_layers[layer_count].size();
				for (int next_neuron_count = 0; next_neuron_count < i_layers[layer_count][neuron_count].m_output_weights.size(); next_neuron_count++){
					int foo_3 = i_layers[layer_count][neuron_count].m_output_weights.size();
					out_file << "LAYER:" << layer_count << "NEURON " <<  neuron_count << "CON_NEURON " << next_neuron_count << "WEIGHT "  << setprecision(2) << fixed << i_layers[layer_count][neuron_count].m_output_weights[next_neuron_count].weight  << "DELTA WEIGHT " << setprecision(2) << fixed << i_layers[layer_count][neuron_count].m_output_weights[next_neuron_count].delta_weight <<"\n";
				}
			}

		
		}
	

		

		//out_file << "neuron " << count << "weight " << values[values.size()][0].m_output_weights[0].m_output_val << "\n";


		out_file.close();

	}


	void zeroes_in_file(vector<double> start_vector) {



		ofstream out_file;
		out_file.open("list.txt");

		int count = 0;


		while (count < start_vector.size())
		{

			out_file << 0.2 << "\n";
			count++;
		}


		out_file.close();

	}

	//Read values
	Net read_from_file(Net i_net) {


		vector<string> values;
		ifstream in_file;
		int old_intern_neuron_num=-1;
		Neuron intern_neuron = Neuron(0,0);

		in_file.open("list.txt");

		//check for error
		if (in_file.fail()) {

			cerr << "Error opening file" << endl;
			exit(1);
		}

		//Layer ... 1 ... until next ... neuron ... 1... next neuron ... weight



		int count = 0;
		string dummy;
		string line;

		if (in_file.is_open())
		{
			while (getline(in_file, line))
			{
				values.push_back(line);
				cout << line << '\n';
			}
			in_file.close();
		}

		//search through string vector
		for (int i = 0; i < values.size(); i++) {
			//search through single string
			for (int j = 0; j < values[i].size(); j++) {

				if (values[i][j] == ':') {
					std::string layer_string = values[i].substr(j + 1, 1);
					std::string neuron = values[i].substr(j + 9, 1);
					std::string neuron2 = values[i].substr(j + 21, 1);
					std::string weight1 = values[i].substr(j + 29, 4);
std::string weight2 = values[i].substr(j + 46, 4);


int intern_layer_num = stoi(layer_string);
int intern_neuron_num = stoi(neuron);
int intern_neuron2_num = stoi(neuron2);

double intern_weight = stod(weight1);
double intern_delta_weight = stod(weight2);

if (intern_neuron_num != old_intern_neuron_num) {

	intern_neuron = Neuron(i_net.m_layers[intern_layer_num + 1].size(), intern_neuron_num);

}


intern_neuron.m_output_weights[intern_neuron2_num].weight = intern_weight;
intern_neuron.m_output_weights[intern_neuron2_num].delta_weight = intern_delta_weight;

i_net.m_layers[intern_layer_num][intern_neuron_num] = intern_neuron;

//old_intern for checking if new neuron index comes next
old_intern_neuron_num = intern_neuron_num;



				}




			}




		}

		return i_net;


	}





	std::tuple<vector<BoardData>, vector<BoardData>> update_database(int i_win, vector<BoardData> &i_board_all_states, vector<BoardData> &i_board_data_base) {

		int dummy = 0;


		if (i_win == 1) {

			BoardData last_state;
			int number_of_states = i_board_all_states.size();

			for (int i = 0; i < number_of_states; i++) {

				//setting REWARDS for the former states


				last_state.value = 1 - (double(i) / number_of_states);
				for (int data_base_index = 0; data_base_index < i_board_data_base.size() - dummy; data_base_index++) {

					if (last_state.board_state == i_board_data_base[data_base_index].board_state) {

						////update values depending on former value
						i_board_all_states[i_board_all_states.size() - 1 - i].value = (i_board_data_base[data_base_index].value + last_state.value) / 2;
						i_board_data_base[data_base_index].value = i_board_all_states[i_board_all_states.size() - 1 - i].value;

						break;
					}
					else if (data_base_index == i_board_data_base.size() - dummy) {

						i_board_all_states[i_board_all_states.size() - 1 - i].value = 1 - (double(i) / number_of_states);
						i_board_data_base[data_base_index].value = last_state.value;

					}
				}



			}
		}

		return std::make_tuple(i_board_data_base, i_board_all_states);


	}


	Net update_net(vector<BoardData> &i_board_all_states, vector<BoardData> &i_board_data_base, Net &i_net, vector<unsigned> &i_topology) {

		vector<double> input_vals;
		vector<double> target_vals;
		vector<double> result_vals;

		//iterate over all states played in this game
		for (int i = 0; i < i_board_all_states.size(); i++){
			target_vals.clear();

			input_vals = i_board_all_states[i].board_state;
			//feed forward
			i_net.feed_forward(input_vals);
			i_net.get_results(result_vals);
			target_vals.push_back(i_board_all_states[i].value);

			assert(target_vals.size() == i_topology.back());
			//back projection
			i_net.back_prop(target_vals);

			//output
			std::cout << "*********************************************" << endl;
			show_vector_vals(": inputs:", input_vals);



		}
		



		return i_net;
	}

		/*		vector<double> input_vals;
				vector<double> target_vals;
				vector<double> result_vals;


				input_vals = last_state.board_state;

				std::cout << "*********************************************" << endl;
				show_vector_vals(": inputs:", input_vals);

				i_my_net.feed_forward(input_vals);

				i_my_net.get_results(result_vals);

				show_vector_vals("Outputs: ", result_vals);

				target_vals.push_back(last_state.value);

				show_vector_vals("Targets: ", target_vals);

				assert(target_vals.size() == i_topology.back());

				i_my_net.back_prop(target_vals);

				cout << "Net recent average error: "
					<< i_my_net.get_recent_average_error() << endl;
				cout << "Done!" << endl;


			}*/

			








	



}
