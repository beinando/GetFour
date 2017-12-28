using namespace std;



namespace graphic_functions {


	vector<vector<sf::Sprite>> board;
	sf::Texture _t1, _cross, _circle;


	void load_textures() {
		_t1.loadFromFile("images/Field.png");
		_cross.loadFromFile("images/Cross.png");
		_circle.loadFromFile("images/Circle.png");
	

	}

	

	void init_board_graphics(int size, int _pixel_field) {


		//init vector for insert row-values
		vector<sf::Sprite> help_vector;


		for (int i = 0; i < size; i++) {
			help_vector.clear();
			for (int j = 0; j < size; j++) {

				sf::Sprite insert_element;

			
					insert_element.setTexture(_t1);
				


				insert_element.setPosition(i *  _pixel_field, j *  _pixel_field);
				help_vector.push_back(insert_element);


			}
			graphic_functions::board.push_back(help_vector);
		}
	}



	void draw_raw_board(sf::RenderWindow& window, int size) {
		for (int i = 0; i < size; i++) {
			vector<sf::Sprite> help_vec = graphic_functions::board[i];
			for (int j = 0; j < size; j++) {

				window.draw(help_vec[j]);
			}
		}
	}

	void draw_figures(sf::RenderWindow& window, vector<sf::Sprite> &sprites) {

		for (int i = 0; i < sprites.size(); i++) {

			window.draw(sprites[i]);

		}
	}

	//void draw_figures(sf::RenderWindow& window, vector<Field> _fields_with_figures, int _size, int  _pixel_field) {

	//	for (int i = 0; i < _fields_with_figures.size(); i++) {

	//		Field dummy_field = _fields_with_figures[i];
	//		if (dummy_field.occupied_by.color == true) {
	//			//white
	//			if (dummy_field.occupied_by.type == 1) {

	//				//pawn
	//			}
	//			else if (dummy_field.occupied_by.type == 2) {

	//				//knight
	//			}
	//			else if (dummy_field.occupied_by.type == 3) {

	//				//bishop
	//			}
	//			else if (dummy_field.occupied_by.type == 4) {
	//				//rook
	//				sf::Sprite rook;
	//				rook.setPosition((dummy_field.position%_size)*_pixel_field, (dummy_field.position / _size)*_pixel_field);
	//				rook.setTexture(white_rook);
	//				window.draw(rook);

	//			}
	//			else if (dummy_field.occupied_by.type == 5) {

	//				//queen
	//			}
	//			else if (dummy_field.occupied_by.type == 6) {

	//				//king
	//				sf::Sprite king;
	//				king.setPosition((dummy_field.position%_size)*_pixel_field, (dummy_field.position / _size)*_pixel_field);
	//				king.setTexture(white_king);
	//				window.draw(king);
	//			}

	//		}

	//		if (dummy_field.occupied_by.color == false) {
	//			//black


	//		}



	//	}




	//}



	/*void draw_attacked_fields(sf::RenderWindow& window, Board _board) {

		for (int i = 0; i < _board.board.size(); i++) {



			if (_board.board[i].is_attacked_by.size() > 0) {


				sf::Sprite attacked_by_white_sprite;
				attacked_by_white_sprite.setPosition((i % 8) * 56, (i / 8) * 56);
				attacked_by_white_sprite.setTexture(attacked_by_white);
				window.draw(attacked_by_white_sprite);


			}


		}




	}*/



}