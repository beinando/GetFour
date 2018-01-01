#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <stdlib.h>
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
#include "GeneralFunctions.h"
#include <iomanip>



using namespace sf;
using namespace std;
using namespace general_functions;
using namespace graphic_functions;


int main() {

	
	

	double x = rotate_x(-1, 0, 180);
	cout << "x: " << x << endl;
	double y = rotate_y(-1, 0, 180);
	cout << "y: " << y << endl;

	int vh = round(y);

	sf::RenderWindow window(sf::VideoMode(7 * 150, 6 * 150), "MattseTicTacToe!");
	sf::Vector2i window_pos;
	load_textures();
	window_pos.x = 0;
	window_pos.y = 0;
	window.setPosition(window_pos);
	graphic_functions::load_textures();
	graphic_functions::init_board_graphics(7, 6, 150);
	graphic_functions::draw_raw_board(window, 7, 6);

	//****************for neural net********************************
	vector<unsigned> topology = { 2,4,1 };
	Net my_net(topology);
	Net my_net2(topology);
	vector<double> input_vals, target_vals, result_vals;
	vector<double> input_vals2, target_vals2, result_vals2;
	int training_pass = 0;


	//for test output

	write_in_file(my_net.m_layers);

	read_from_file();
	/*for (int layer = 0; layer < topology.size() - 1; layer++) {
		cout << "Layer: " << layer << endl;
		
		for (int neuron = 0; neuron < my_net.m_layers[layer].size() - 1; neuron++) {
			cout << "Neuron: " << neuron << endl;
			for (int neuron2 = 0; neuron2 < my_net.m_layers[layer + 1].size() - 1; neuron2++) {


				cout << " next Neuron: " << neuron2 << " weight: " << my_net.m_layers[layer][neuron].m_output_weights[neuron2].weight << endl;
			}

		}*/


	//}
	// end for test output


	//****************for neural net********************************


	//*************for change in values *************************
	double momentum = 1.0;
	double force = 0.2;


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

	int row;
	int col;
	int row_rand;
	int	col_rand;
	
	//for graphical output
	int dummyint = 1;
	
	graphic_functions::draw_raw_board(window, 7, 6);
	window.display();

	while (window.isOpen())
	{
		int select_player = generate_random_number(0, 1);
		int block_next_step=1;
		sf::Event e;
		int move = 0;
		int win = 0;
	
		sprites.clear();
		board_all_states.clear();
		occupied_fields.clear();
		board_state.clear();

		
		for (int i = 0; i < 42; i++) {

			occupied_fields.push_back(false);

		}

		for (int i = 0; i < 42; i++) {

			board_state.push_back(0);

		}

		vector<int> free_col;

		for (int i = 0; i < 7; i++) {

			free_col.push_back(true);

		}
		
		while( move< 42) {

			free_col.clear();
			free_fields.clear();
			possible_states.clear();

			//iterate through size of the occupied_fields vector
			for (int i = 0; i < 42; i++) {
				if (occupied_fields[i] == false) {
					free_fields.push_back(i);
				}
			}


			//i=col, j=row
			for (int i = 0; i < 7; i++) {
				for (int j = 0; j<6; j++) {
					if (occupied_fields[j * 7 + i] == false) {

						free_col.push_back(i);
						j = 5;

					}
				


				}


			}
			
			block_next_step = 1;

			//for dragging the window around and selecting col for input

			while (window.pollEvent(e)) {

				block_next_step = 1;

				switch (e.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonReleased:
					int x = e.mouseButton.x;
					int y = e.mouseButton.y;
				
					col = x / 150;
					row = y / 150;

					for (size_t i = 0; i < free_col.size(); i++)
					{
						if (free_col[i] == col) {

							block_next_step = 0;
							break;

						}
						else if(i== free_col.size()-1) {

							block_next_step = 1;

						}
					}
				}
			}

				//dummy sprite generated to fill into <vector<sprite>>-container
				Sprite helpsprite = Sprite();
				Sprite helpsprite2 = Sprite();

				if(block_next_step==0){
					
					//*************************input move*************
					for (int r = 5; r >= 0; r--) {
						if (occupied_fields[r * 7 + col] == false) {
							helpsprite.setPosition(col * 150, r * 150);
							helpsprite.setTexture(_cross);
							occupied_fields[r * 7 + col] = true;
							//******input into the board_state structue +1 = cross -1 = circle
							board_state[r * 7 + col] = +1;
							break;
						}
					}

					col_rand = generate_random_number(0, free_col.size()-1);
					col_rand = free_col[col_rand];
					vector<double> possible_next_state;
					vector<int> possible_indices;
					possible_indices.clear();
					for (int c = 0; c < 7; c++) {
						
						
						for (int r = 5; r >= 0; r--) {
							
							if (occupied_fields[r * 7 + c] == false) {
								
								
								//******input into the board_state structue +1 = cross -1 = circle
								possible_indices.push_back(r * 7 + c);
								
								break;
							}
						}
					}

					//generate random number for selecting one state out of all possible states
					int random_num = generate_random_number(0, possible_indices.size()-1);
					board_state[possible_indices[random_num]] = -1;
					occupied_fields[possible_indices[random_num]] = true;
					//set sprite for graphical output
		
					int foo_x = possible_indices[random_num] % 7;
					int foo_y = possible_indices[random_num] / 7;

					helpsprite2.setPosition(foo_x*150, foo_y*150);
					helpsprite2.setTexture(_circle);

					int stop = 1;


					 win = check_for_win(board_state);
					
					// next player will only count up

					move++;

					Sprite Troll = Sprite();
					Troll.setTexture(_troll);
					Troll.setPosition(150, 3 * 150);

					sprites.push_back(helpsprite);
					sprites.push_back(helpsprite2);
					board_all_states.push_back(board_state);
					BoardData insert_state;

					//insert states into the database
					int dummy = 0;
					for (int i = 0; i < board_data_base.size(); i++) {

						if (board_state == board_data_base[i].board_state) {
							board_data_base[i].times_played++;
							break;
						}
						else if (i == board_data_base.size() - 1) {
							dummy++;
							insert_state.board_state = board_state;
							insert_state.times_played = 1;
							insert_state.value = 0;
							board_data_base.push_back(insert_state);
						}
					}

					
					select_player++;
				
					graphic_functions::draw_raw_board(window, 7, 6);
					graphic_functions::draw_figures(window, sprites);
					

					window.display();
					window.clear();

					if (win == -1) {

						BoardData last_state;
						int number_of_states = board_all_states.size();
						for (int i = 0; i < number_of_states; i++) {
							
								last_state.board_state = board_all_states[board_all_states.size() - 1 - i];
								last_state.value = +1 - (double(i) / number_of_states);
								board_data_base[board_data_base.size()-1].value = last_state.value;
							
						}
						//break single game loop (until 42)
						break;


					}
					//else if (who_won == -1) {

					//	BoardData last_state;
					//	int number_of_states = board_all_states.size();

					//	for (int i = 0; i < number_of_states; i++) {
					//		target_vals.clear();
					//		//setting REWARDS for the former states
					//		last_state.board_state = board_all_states[board_all_states.size() - 1 - i];

					//		for (int data_base_index = 0; data_base_index < board_data_base.size(); data_base_index++) {

					//			if (last_state.board_state == board_data_base[data_base_index].board_state) {
					//				last_state.value = -1 + (double(i) / number_of_states);
					//				//update values depending on former value
					//				last_state.value = board_data_base[data_base_index].value + force*(-board_data_base[data_base_index].value + last_state.value);
					//				board_data_base[data_base_index].value = last_state.value;
					//				break;
					//			}
					//			else if (data_base_index == board_data_base.size()) {

					//				last_state.value = -1 + (double(i) / number_of_states);
					//				board_data_base[data_base_index].value = last_state.value;

					//			}
					//		}



					//		input_vals = last_state.board_state;

					//		cout << "*********************************************" << endl;
					//		show_vector_vals(": inputs:", input_vals);

					//		my_net.feed_forward(input_vals);

					//		my_net.get_results(result_vals);

					//		show_vector_vals("Outputs: ", result_vals);

					//		target_vals.push_back(last_state.value);

					//		show_vector_vals("Targets: ", target_vals);

					//		assert(target_vals.size() == topology.back());

					//		my_net.back_prop(target_vals);

					//		cout << "Net recent average error: "
					//			<< my_net.get_recent_average_error() << endl;
					//		cout << "Done!" << endl;


					//	}


					//	crosswon++;




					//	break;
					//}


					
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



