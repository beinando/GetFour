#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include "Net.h"
#include "TrainingData.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <random>
#include "Connector.h"
#include <chrono>
#include <thread>
#include <string>
#include <Windows.h>
#include "GraphicFunctions.h"



using namespace sf;
using namespace std;









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

struct BoardData {
	vector<double> board_state;
	int			times_played;
	double		value;
	//for second net
	double		value2;

};
	

short check_for_win(vector <double> &board_state) {

	if ((board_state[0] == board_state[1]) && (board_state[0] == board_state[2])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[3] == board_state[4]) && (board_state[3] == board_state[5])) {
		if (board_state[3] == 1) {

			return 1;

		}
		else if (board_state[3] == -1) {

			return -1;

		}

	}

	else if ((board_state[6] == board_state[7]) && (board_state[6] == board_state[8])) {
		if (board_state[6] == 1) {

			return 1;

		}
		else if (board_state[6] == -1) {

			return -1;

		}

	}

	else if ((board_state[0] == board_state[3]) && (board_state[0] == board_state[6])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[1] == board_state[4]) && (board_state[1] == board_state[7])) {
		if (board_state[1] == 1) {

			return 1;

		}
		else if (board_state[1] == -1) {

			return -1;

		}

	}

	else if ((board_state[2] == board_state[5]) && (board_state[2] == board_state[8])) {
		if (board_state[2] == 1) {

			return 1;

		}
		else if (board_state[2] == -1) {

			return -1;

		}

	}

	else if ((board_state[0] == board_state[4]) && (board_state[0] == board_state[8])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[2] == board_state[4]) && (board_state[2] == board_state[6])) {
		if (board_state[2] == 1) {

			return 1;

		}
		else if (board_state[2] == -1) {

			return -1;

		}

	}


	return 0;

}

int get_index_from_database(vector<double> &i_state, vector<BoardData> &i_database) {


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

		col = i%3;
		row = i/3;

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

	vector<Sprite> output_sprites;

	for (size_t i = 0; i < input_sprites.size(); i++)
	{
		output_sprites.push_back(Sprite());
		const sf::Texture* test;
		test = input_sprites[i].getTexture();
		output_sprites[i].setTexture(*test);

	}


	for (int i = 0; i < input_sprites.size(); i++) {

		col = input_sprites[i].getPosition().x/150;
		row = input_sprites[i].getPosition().y/150;

		mirrored_col = (-1 * (col - 1)) + 1;
		mirrored_row = (-1 * (row - 1)) + 1;
		

		output_sprites[i].setPosition(mirrored_col * 150, mirrored_row * 150);


	

	}



	return output_sprites;

}


double rotate_x(int x, int y, double degree) {

	double pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00;
	degree = pi*degree*2 / 360;

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






int main() {

	double x = rotate_x(-1, 0, 180);
	cout << "x: " << x << endl;
	double y = rotate_y(-1, 0, 180);
	cout << "y: " << y << endl;

	int vh = round(y);

	sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
	sf::Vector2i window_pos;
	window_pos.x = 0;
	window_pos.y = 0;
	window.setPosition(window_pos);
	graphic_functions::load_textures();
	graphic_functions::init_board_graphics(3, 150);
	graphic_functions::draw_raw_board(window, 3);
	
	//****************for neural net********************************
	vector<unsigned> topology = { 9,9,9,1 };
	Net my_net(topology);
	Net my_net2(topology);
	vector<double> input_vals, target_vals, result_vals;
	vector<double> input_vals2, target_vals2, result_vals2;
	int training_pass = 0;

	//****************for neural net********************************


	//*************for change in values *************************
	double momentum=1.0;
	double force =0.2;


	//*************for change in values *************************







	//container for all free fields
	vector<int> free_fields;
	//container for all fields with symbols , cross or circle
	vector<bool> occupied_fields;
	//current board state
	vector<double> board_state;
	//container for all states during one game
	vector<vector<double>> board_all_states;
	vector<Sprite> sprites;
	vector<Sprite> mirrored_sprites;
	vector<BoardData> board_data_base;
	vector<vector<double>> possible_states;

	static int crosswon = 0;
	static int circlewon = 0;

	BoardData start_state;
	start_state.board_state = { 0,0,0,0,0,0,0,0,0 };
	start_state.value = 0;
	start_state.value2 = 0;
	start_state.times_played = 0;
	board_data_base.push_back(start_state);

	//for graphical output
	int dummyint=1;
	
	while (window.isOpen())
	{
		//		//start with circle or with cross?
		int select_player = generate_random_number(0, 1);
		sprites.clear();
		board_all_states.clear();

		occupied_fields.clear();
		for (int i = 0; i < 9; i++) {

			occupied_fields.push_back(false);

		}

		board_state.clear();
		for (int i = 0; i < 9; i++) {

			board_state.push_back(0);

		}


		for (int move = 0; move < 9; move++) {

			select_player++;

			//clear free fields for new initialization of the game
			free_fields.clear();
			possible_states.clear();

			//iterate through size of the occupied_fields vector
			for (int i = 0; i < 9; i++) {
				if (occupied_fields[i] == false) {
					free_fields.push_back(i);
				}
			}

			int random = generate_random_number(0, free_fields.size() - 1);
			int random_position = free_fields[random];

			Sprite new_sprite;
			Sprite mirrored_sprite;
			(select_player % 2 == 0) ? new_sprite.setTexture(graphic_functions::_circle) : new_sprite.setTexture(graphic_functions::_cross);

			

			double free_index;
			vector<int> free_indices;
			free_indices.clear();
			//circle
			if (select_player % 2 == 0) {



				vector<double> possible_next_state;
				possible_next_state = board_state;
				for (int i = 0; i < free_fields.size(); i++) {
					possible_next_state = board_state;
					free_index = free_fields[i];
					possible_next_state[free_index] = 1;
					free_indices.push_back(free_index);
					possible_states.push_back(possible_next_state);

				}
				double max = -1e7;
				int position_of_best_move;

				for (int i = 0; i < possible_states.size(); i++) {


					input_vals = possible_states[i];

					//integer showing how often a certain state was already played
					int times_played;
					//index of a the current state within the database, if state is available in database
					int index_in_database = get_index_from_database(input_vals, board_data_base);

					if (index_in_database == -1) {

						times_played = 0;

					}

					else {

						times_played = board_data_base[index_in_database].times_played;

					}



					my_net.feed_forward(input_vals);
					my_net.get_results(result_vals);


					//add +0.1 to prevent division by zero
					if ((crosswon + circlewon)<10000) {
						if ((result_vals[0] + 1) / (sqrt(double(times_played + 0.1))) > max) {
							max = result_vals[0];
							position_of_best_move = free_indices[i];
						}
					}
					else {
						if ((result_vals[0] + 1) > max) {
							max = result_vals[0];
							position_of_best_move = free_indices[i];
						}
					}


				}


				//board_state = possible_states[random_state];
				
				//THIS random position is not random anymore, just for now
				//random_position = position_of_best_move;
				if(generate_random_number(0,1000)>1){
					random_position = position_of_best_move;
				}

				board_state[random_position] = 1;

				occupied_fields[random_position] = true;
			}
			//cross
			else {

				vector<double> possible_next_state;
				possible_next_state = board_state;
				for (int i = 0; i < free_fields.size(); i++) {
					possible_next_state = board_state;
					free_index = free_fields[i];
					possible_next_state[free_index] = -1;
					free_indices.push_back(free_index);
					possible_states.push_back(possible_next_state);

				}
				double max = -1e7;
				int position_of_best_move;

				for (int i = 0; i < possible_states.size(); i++) {


					input_vals2 = possible_states[i];

					//integer showing how often a certain state was already played
					int times_played;
					//index of a the current state within the database, if state is available in database
					int index_in_database = get_index_from_database(input_vals, board_data_base);

					if (index_in_database == -1) {

						times_played = 0;

					}

					else {

						times_played = board_data_base[index_in_database].times_played;

					}

					my_net2.feed_forward(input_vals2);
					my_net2.get_results(result_vals2);


					if((crosswon+circlewon)<10000){
						if ((result_vals2[0]+1)/(sqrt(double(times_played + 0.1))) > max) {

							max = result_vals2[0];
							position_of_best_move = free_indices[i];

						}
					}

					else {
	
						if ((result_vals2[0] + 1) > max) {

							max = result_vals2[0];
							position_of_best_move = free_indices[i];

						}

					}


				}

				//programm does best move with 99.9% probability
				if (generate_random_number(0, 1000)>1) {
					
						random_position = position_of_best_move;
				}
			
				board_state[random_position] = -1;

				occupied_fields[random_position] = true;

			}

			new_sprite.setPosition(random_position % 3 * 150, random_position / 3 * 150);
			sprites.push_back(new_sprite);



			mirrored_sprites = get_mirrored_sprites(sprites,0);


			//all states during the loop inside a game
			board_all_states.push_back(board_state);
			


			//for dragging the window around
			sf::Event e;
			while (window.pollEvent(e)) {

				if (e.type == sf::Event::Closed)
					window.close();
				break;

			}
			
		//	if((circlewon+crosswon)%2000==0 || dummyint==0){
			graphic_functions::draw_raw_board(window, 3);
			graphic_functions::draw_figures(window, sprites);
			window.display();
			
			graphic_functions::draw_raw_board(window, 3);
			graphic_functions::draw_figures(window, mirrored_sprites);
			window.display();
			sleep(750);
		//	}
			
		
		
				
		

			int who_won = check_for_win(board_state);

			//dummy state to declare a state for inserting into the database
			BoardData insert_state;
			//insert states into the database
			int dummy = 0;
			for (int i = 0; i < board_data_base.size(); i++) {
				
				if (board_state == board_data_base[i].board_state) {

					//if state already in the database, increment times_played
					board_data_base[i].times_played++;
					break;

				}
				else if (i == board_data_base.size() - 1) {
					dummy++;
					insert_state.board_state = board_state;
					insert_state.times_played = 1;
					insert_state.value = 0;
					insert_state.value2 = 0;
					//if state is not in the database, insert a new state in the database
					board_data_base.push_back(insert_state);

					//cout << tester << endl;
				}
			}
			static int nowin = 0;

			
			//*********************************LOOK WHO WON GAME*************************
			if (who_won == 1) {


				BoardData last_state;
				int number_of_states = board_all_states.size();

				for (int i = 0; i < number_of_states; i++){
					target_vals.clear();
					//setting REWARDS for the former states
					last_state.board_state = board_all_states[board_all_states.size() - 1 - i];

					last_state.value = 1 - (double(i) / number_of_states);
					for (int data_base_index = 0; data_base_index < board_data_base.size()- dummy; data_base_index++) {

						if (last_state.board_state == board_data_base[data_base_index].board_state) {
							
							////update values depending on former value
							last_state.value = (board_data_base[data_base_index].value + last_state.value)/2;
							board_data_base[data_base_index].value=last_state.value;
							break;
						}
						else if (data_base_index == board_data_base.size()-dummy) {

							last_state.value = 1 - (double(i) / number_of_states);
							board_data_base[data_base_index].value = last_state.value;

						}
					}

					input_vals = last_state.board_state;
				
					
					my_net.feed_forward(input_vals);
					
					my_net.get_results(result_vals);
					
					
					target_vals.push_back(last_state.value);
					
					
					assert(target_vals.size() == topology.back());
					
					my_net.back_prop(target_vals);
					
					if(circlewon%10000==0){
					show_vector_vals(": inputs:", input_vals);
					show_vector_vals("Outputs: ", result_vals);
					show_vector_vals("Targets: ", target_vals);
					cout << "Net recent average error: "
						<< my_net.get_recent_average_error() << endl;
					cout << "Done!" << endl;
					cout << "*********************************************" << endl;
					}
					
					
				}

				for (int i = 0; i < number_of_states; i++) {
					target_vals2.clear();
					//setting REWARDS for the former states
					last_state.board_state = board_all_states[board_all_states.size() - 1 - i];

					last_state.value2 = -1 + (double(i) / number_of_states);
					for (int data_base_index = 0; data_base_index < board_data_base.size() - dummy; data_base_index++) {

						if (last_state.board_state == board_data_base[data_base_index].board_state) {

							////update values depending on former value
							last_state.value2 = (board_data_base[data_base_index].value2 + last_state.value2) / 2;
							board_data_base[data_base_index].value2 = last_state.value2;
							break;
						}
						else if (data_base_index == board_data_base.size() - dummy) {

							last_state.value2 = -1 + (double(i) / number_of_states);
							board_data_base[data_base_index].value2 = last_state.value2;

						}
					}

					input_vals2 = last_state.board_state;

					

					my_net2.feed_forward(input_vals2);

					my_net2.get_results(result_vals2);
				
					target_vals2.push_back(last_state.value2);
					
					

					assert(target_vals2.size() == topology.back());

					my_net2.back_prop(target_vals2);

					if(circlewon%10000==0){
					show_vector_vals(": inputs2:", input_vals2);
					show_vector_vals("Outputs2: ", result_vals2);
					show_vector_vals("Targets2: ", target_vals2);
					cout << "Net recent average error2: "
						<< my_net.get_recent_average_error() << endl;
					cout << "Done2!" << endl;
					cout << "*********************************************" << endl;
					}


				}

				circlewon++;

				dummyint = 1;
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{

					dummyint = 0;

				}
				
				break;
			}
			else if (who_won == -1) {

				BoardData last_state;
				int number_of_states = board_all_states.size();

				for (int i = 0; i < number_of_states; i++) {
					target_vals.clear();
					//setting REWARDS for the former states
					last_state.board_state = board_all_states[board_all_states.size() - 1 - i];

					for (int data_base_index = 0; data_base_index < board_data_base.size(); data_base_index++) {

						if (last_state.board_state == board_data_base[data_base_index].board_state) {
							last_state.value = -1 + (double(i) / number_of_states);
							//update values depending on former value
							last_state.value = board_data_base[data_base_index].value + force*(-board_data_base[data_base_index].value + last_state.value);
							board_data_base[data_base_index].value = last_state.value;
							break;
						}
						else if (data_base_index == board_data_base.size()) {

							last_state.value = -1 + (double(i) / number_of_states);
							board_data_base[data_base_index].value = last_state.value;

						}
					}
					
				

					input_vals = last_state.board_state;
				
				
					
					my_net.feed_forward(input_vals);

					my_net.get_results(result_vals);
					
					
					target_vals.push_back(last_state.value);
				
					
					assert(target_vals.size() == topology.back());

					my_net.back_prop(target_vals);
					
					if (circlewon % 10000 == 0) {
						show_vector_vals(": inputs:", input_vals);
						show_vector_vals("Outputs: ", result_vals);
						show_vector_vals("Targets: ", target_vals);
						cout << "Net recent average error: "
							<< my_net.get_recent_average_error() << endl;
						cout << "Done!" << endl;
						cout << "*********************************************" << endl;
					}

				}


				for (int i = 0; i < number_of_states; i++) {
					target_vals2.clear();
					//setting REWARDS for the former states
					last_state.board_state = board_all_states[board_all_states.size() - 1 - i];

					last_state.value2 = +1 - (double(i) / number_of_states);
					for (int data_base_index = 0; data_base_index < board_data_base.size() - dummy; data_base_index++) {

						if (last_state.board_state == board_data_base[data_base_index].board_state) {

							////update values depending on former value
							last_state.value2 = (board_data_base[data_base_index].value2 + last_state.value2) / 2;
							board_data_base[data_base_index].value2 = last_state.value2;
							break;
						}
						else if (data_base_index == board_data_base.size() - dummy) {

							last_state.value2 = +1 - (double(i) / number_of_states);
							board_data_base[data_base_index].value2 = last_state.value2;

						}
					}

					input_vals2 = last_state.board_state;

				
					my_net2.feed_forward(input_vals2);

					my_net2.get_results(result_vals2);

					
					target_vals2.push_back(last_state.value2);

					
					assert(target_vals2.size() == topology.back());

					my_net2.back_prop(target_vals2);
					if (circlewon % 10000 == 0) {
						show_vector_vals(": inputs2:", input_vals2);
						show_vector_vals("Outputs2: ", result_vals2);
						show_vector_vals("Targets2: ", target_vals2);
						cout << "Net recent average error2: "
							<< my_net.get_recent_average_error() << endl;
						cout << "Done2!" << endl;
						cout << "*********************************************" << endl;
					}


				}
				
				crosswon++;
				dummyint = 1;
			
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{

					dummyint = 0;

				}
				
				break;
			}
			
			else if (who_won == 0) {

				nowin++;
				dummyint = 1;

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{

					dummyint = 0;

				}
				
			}


			double relative_winrate = circlewon / (((double)circlewon + (double)crosswon) + 1);
			cout << "relative_winrate: " << relative_winrate << endl;
			cout << "Different positions evaluated: " << (double(board_data_base.size()) / 19683) * 100 << "%" << endl;
			cout << "Total games played : " << (nowin+crosswon+circlewon)  << endl;
			cout << "*******************" << endl;
		
			if(circlewon%10000==0){
			cout << "circle won: " << circlewon << endl;
			cout << "cross won: " << crosswon << endl;
			cout << "*******************" << endl;
			double relative_winrate = circlewon / (((double)circlewon + (double)crosswon)+1);
			cout << "relative_winrate: " << relative_winrate << endl;
			cout << "Different positions evaluated: " << (double(board_data_base.size())/ 19683)*100 << "%" << endl;
			cout << "*******************" << endl;
			}



			
		}


		


	}
	window.close();
	CloseConnection();

}



//************** neural net stuff*****************************************

//TrainingData train_data("TrainingData.txt");
//
//vector<unsigned> topology;
//
//train_data.get_topology(topology);
//Net my_net(topology);
//
//
//vector<double> input_vals, target_vals, result_vals;
//int training_pass = 0;
//
//while (training_pass<10000) {
//
//	training_pass++;
//	cout << endl << "Pass" << training_pass;
//
//	if (train_data.get_next_inputs(input_vals) != topology[0]) {
//
//		break;
//
//	}
//
//	show_vector_vals(": Inputs:", input_vals);
//	my_net.feed_forward(input_vals);
//
//	my_net.get_results(result_vals);
//	show_vector_vals("Outputs: ", result_vals);
//
//	train_data.get_target_outputs(target_vals);
//	show_vector_vals("Targets: ", target_vals);
//	assert(target_vals.size() == topology.back());
//
//	my_net.back_prop(target_vals);
//
//	cout << "Net recent average error: "
//		<< my_net.get_recent_average_error() << endl;
//
//	cout << "Done!" << endl;
//
//
//
//
//
//
//
//
//
//}
//
//for (int layer = 0; layer < topology.size() - 1; layer++) {
//	cout << "Layer: " << layer << endl;
//	for (int neuron = 0; neuron < my_net.m_layers[layer].size() - 1; neuron++) {
//		cout << "Neuron: " << neuron << endl;
//		for (int neuron2 = 0; neuron2 < my_net.m_layers[layer + 1].size() - 1; neuron2++) {
//
//
//			cout << " next Neuron: " << neuron2 << " weight: " << my_net.m_layers[layer][neuron].m_output_weights[neuron2].weight << endl;
//		}
//
//	}
//
//
//}




//************** neural net stuff*****************************************

// ******************** OLD MAIN **************************************
//	sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
//	sf::Vector2i window_pos;
//	window_pos.x = 0;
//	window_pos.y = 0;
//	window.setPosition(window_pos);
//
//
//
//	
//	sf::Sprite board[3][3];
//	
//	
//	//set sprites for fields
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//	
//			if ((i + j) % 2 == 0) {
//				board[i][j] = sf::Sprite(_t1);
//			}
//			else {
//				board[i][j] = sf::Sprite(_t2);
//			}
//	
//			board[i][j].setPosition(i * 150, j * 150);
//	
//		}
//	
//	}
//
//	vector<int> free_fields;
//	vector<bool> occupied_fields;
//	vector<int> board_state;
//	vector<vector<int>> board_all_states;
//	vector<vector<vector<int>>> board_all_states_many_games;
//	int random_position_circle = generate_random_number(0, 9);
//	
//	vector<Sprite> sprites;
//	sf::Sprite cross = sf::Sprite(_cross);
//	sf::Sprite circle = sf::Sprite(_circle);
//
//
//	
////	cross.setPosition(300, 300);
//	
//	
//	
//	
//	while (window.isOpen())
//	{
//		//start with circle or with cross?
//		int select_player = generate_random_number(0, 1);
//		sprites.clear();
//
//		occupied_fields.clear();
//		for (int i = 0; i < 9; i++) {
//
//			occupied_fields.push_back(false);
//
//		}
//
//		board_state.clear();
//		for (int i = 0; i < 9; i++) {
//
//			board_state.push_back(0);
//
//		}
//
//
//		
//		//move counter for iterating many moves
//		for (int move = 0; move < 9; move++) {
//	
//			select_player++;
//
//			//clear free fields for new initialization of the game
//			free_fields.clear();
//
//			//iterate through size of the occupied_fields vector
//			for (int i = 0; i < 9; i++) {
//				if (occupied_fields[i] == false) {
//					free_fields.push_back(i);
//				}
//			}
//
//			int random = generate_random_number(0, free_fields.size() - 1);
//			int random_position = free_fields[random];
//
//			Sprite new_sprite;
//			(select_player % 2 == 0) ? new_sprite.setTexture(_circle) : new_sprite.setTexture(_cross);
//
//			new_sprite.setPosition(random_position % 3 * 150, random_position / 3 * 150);
//			sprites.push_back(new_sprite);
//
//			occupied_fields[random_position] = true;
//			(select_player % 2 == 0) ? board_state[random_position]=1 : board_state[random_position] = -1;
//
//			board_all_states.push_back(board_state);
//
//			sf::Event e;
//
//			while (window.pollEvent(e)) {
//
//				if (e.type == sf::Event::Closed)
//					window.close();
//				break;
//
//			}
//
//
//
//			for (int i = 0; i < 3; i++) {
//				for (int j = 0; j < 3; j++) {
//					window.draw(board[i][j]);
//				}
//			}
//
//			for (int i = 0; i < sprites.size(); i++) {
//
//				window.draw(sprites[i]);
//
//			}
//
//			//	window.draw(cross);
//
//
//			for (int i = 0; i < board_state.size(); i++) {
//
//				cout << "Field: " << i << endl;
//				cout << board_state[i] << endl;
//
//			}
//
//			cout << "*************************************" << endl;
//
//			
//			window.display();
//			
//
//		}
//
//		board_all_states_many_games.push_back(board_all_states);
//
//
//
//
//		CloseConnection();
//
//
//
//	}
	

//}

//
//if (Keyboard::isKeyPressed(Keyboard::Right))
//{
//
//
//}
//
//if (Keyboard::isKeyPressed(Keyboard::Left))
//{
//
//
//}
//}*/

//
//


//win condition : 

/*	for (int i = 0; i < 3; i + 3) {

if (Field[i].status == Field[i + 1].status == Field[i + 2].status)
}
for (int i = 0; i < 3; i++) {
if(Field[i].status == Field[i+3].status == Field[i+6].status)



}

if (Field[0].status == Field[4].status == Field[8].status) {


}

if (Field[2].status == Field[4].status == Field[6].status) {


}*/



