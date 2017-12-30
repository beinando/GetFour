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




	



}
