#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdlib>

//#define NB_CHAR_PER_LINE 16
// ratio et taille des charactere determine arbitrairement
// pour que ca soit lisible et joli (ou du moins pas trop moche)
#define NB_CHAR_PER_LINE 13
#define NB_CHAR_MAX_PER_WORD 64
#define NB_LINES_ON_SCREEN 10
#define NB_MAX_GUESS_PRINTED 3


std::vector<std::vector<char>> autocompletion(std::vector<char> word){
    std::vector<std::vector<char>> result  = std::vector<std::vector<char>>();
    //
    //inserer ici autocompletion
    //

    //
    //test pour voir si l'affichage marche
    //
    for (int k=0; k<(int)word[0]-(int)'0';k++){
        std::vector<char> guess_k = std::vector<char>();

        for (int l=0; l<word.size() && l<NB_CHAR_MAX_PER_WORD;l++){
            guess_k.push_back('0'+(word[l]-'0'+k+1)%10);
        }
        result.push_back(guess_k);
    }

    return result;
}

//cest hardcode cest moche mais rapide a implementer
char letter_to_num(char l){
    if (l-'0' >= 0 &&l-'0'<=9){
        return l;
    }if (l == 's'){
        return '7';
    }if (l == 'v'){
        return '8';
    }if ((int)l > (int)'v'){
        return '9';
    }
    return (char)('2'+((l-'a')/3));
}



int main()
{
    
    //
    // environ 150 lignes dinitialisations diverses et variees, notament pour la gestion de l'affichage
    //

    //clock to mesure time
    sf::Clock clock; // starts the clock



    int physical_screen_height = sf::VideoMode::getDesktopMode().height;

    int phone_h_ratio = 615;
    int phone_w_ratio = 260;

    int window_h = physical_screen_height*0.8;
    int window_w = window_h*phone_w_ratio/phone_h_ratio;

    int phone_h = window_h;
    int phone_w = window_w;

    int nokia_screen_h = phone_h*0.19;
    int nokia_screen_w = phone_w*0.64;


    //sf::Style::Titlebar | sf::Style::Close -> empeche de manuellement redimensionner la fenetre
    sf::RenderWindow window(sf::VideoMode(window_w, window_h), "T9 emulator",sf::Style::Titlebar | sf::Style::Close);



    sf::Texture phone_texture;
    if (!phone_texture.loadFromFile("1110i.jpg")){
        printf("texture file not found");
        return -1;
    }

    sf::Sprite phone_sprite;
    phone_sprite.setTexture(phone_texture);

    phone_sprite.setScale(  phone_w / phone_sprite.getLocalBounds().width, 
                            phone_h / phone_sprite.getLocalBounds().height);

    

    // define a rectangle
    sf::RectangleShape nokia_screen(sf::Vector2f(0,0));
    // change the size
    nokia_screen.setSize(sf::Vector2f(phone_w*0.64, phone_h*0.19));
    // set color
    sf::Color screen_color(162, 160, 39);
    nokia_screen.setFillColor(screen_color);
    // set position
    nokia_screen.setPosition(phone_w*0.1775,phone_h*0.245);


    double screen_ref_width = nokia_screen.getGlobalBounds().width;


    std::vector<char> full_text;

    std::vector<char> current_word = std::vector<char>();

    int pos_in_current_word=-1;//valeur de -1 : indicateur qu'il faut la recalculer

    int charSize = phone_w/20;

    // load font
    sf::Font nokiafont;
    if (!nokiafont.loadFromFile("nokiafc22.ttf")){
        printf("font file not found");
        return -1;
    }

    

    char keyString;


    sf::Text blinkText;
    // select the font
    blinkText.setFont(nokiafont);
    // define text
    //blinkText.setString("|");
    blinkText.setString("_");
    // set the character size
    blinkText.setCharacterSize(charSize); // in pixels, not points!
    // set the color
    blinkText.setFillColor(sf::Color::Black);
    // set the text style
    blinkText.setStyle(sf::Text::Regular);
    // set position
    blinkText.setPosition(phone_w*0.19,phone_h*0.24);


    sf::RectangleShape best_gess_background(sf::Vector2f(0, 0));;
    best_gess_background.setFillColor(sf::Color::Black);


    std::vector<sf::Text> char_on_screen;

    for (int i=0;i<NB_LINES_ON_SCREEN;i++){
        for (int j=0;j<NB_CHAR_PER_LINE;j++){
            //un char aux coordonnees i,j
            sf::Text text;
            // select the font
            text.setFont(nokiafont);
            // define text
            text.setString(" ");
            // set the character size
            text.setCharacterSize(charSize); // in pixels, not points!
            // set the color
            //Text.setFillColor(sf::Color(0, 0, 0));//black
            text.setFillColor(sf::Color::Black);
            // set the text style
            //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
            text.setStyle(sf::Text::Regular);
            // set position
            text.setPosition(phone_w*0.19 + j*charSize,phone_h*0.24 + i*charSize);
            // store the new line of text

            char_on_screen.push_back(text);
        }
    }


    full_text = std::vector<char>();

    int current_line_pos = 0;
    int num_current_line = 0;
    int num_max_line = 0;
    int num_current_line_on_screen = 0;
    int line_offset;

    bool currently_guessing_a_word = false;
    int nb_guess_printed = 0;
    int total_nb_guess = 0;
    int current_guess_num = 0;
    int nb_lines_printed = 0;
    std::vector<std::vector<char>> guess_list;

    bool key_is_new_char;

    //
    //  boucle principale
    //

    while (window.isOpen())
    {
        //window.setSize(sf::Vector2u(X ,Y));

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if ( event.type == sf::Event::TextEntered ){
                if ( event.text.unicode < 0x80 ) {// it's printable
                    keyString = (char)event.text.unicode;
                    key_is_new_char = false;
                    //
                    //  disjonction de cas pour etre sur que lentree clavier correspond a ce qui est autorise
                    //
                    if ((keyString-'0' >= 0 && keyString-'0' <= 9)||keyString=='#'||keyString=='*'){
                        key_is_new_char = true;
                        //
                        //  recalcul de la position du curseur dans le mot en train d etre devine
                        //  et reevaluation de tout le mot concerne si besoin
                        //
                        if (pos_in_current_word<0){
                            current_word.clear();
                            pos_in_current_word = 0;
                            int pos_in_full_text = current_line_pos+num_current_line*NB_CHAR_PER_LINE;
                            while(pos_in_full_text>0 && full_text[pos_in_full_text-1] != ' ' && full_text[pos_in_full_text-1] != '.'){
                                pos_in_full_text--;
                                pos_in_current_word++;
                            }
                            int s = full_text.size();
                            while(pos_in_full_text < s && full_text[pos_in_full_text] != ' ' && full_text[pos_in_full_text] != '.'){
                                current_word.push_back(letter_to_num(full_text[pos_in_full_text]));
                                pos_in_full_text++;
                            }
                        }

                        //le zero est special il permet soit de faire unespace soit de valider un mot puis de mettre un espace     
                        if (keyString == '0'){
                            if (currently_guessing_a_word){
                                //trouver le debut du mot
                                int pos_in_full_text = current_line_pos+num_current_line*NB_CHAR_PER_LINE;
                                while(pos_in_full_text>0 && full_text[pos_in_full_text-1] != ' ' && full_text[pos_in_full_text-1] != '.'){
                                    pos_in_full_text--;
                                    current_line_pos--;
                                }
                                //remplacer le mot lettre par lettre
                                int pos_in_autocompleted_word = 0;
                                int s = full_text.size();
                                while(pos_in_full_text+pos_in_autocompleted_word < s && full_text[pos_in_full_text+pos_in_autocompleted_word] != ' ' && full_text[pos_in_full_text+pos_in_autocompleted_word] != '.'){
                                    //current_word.push_back(letter_to_num(full_text[pos_in_full_text]));
                                    full_text[pos_in_full_text+pos_in_autocompleted_word] = guess_list[current_guess_num][pos_in_autocompleted_word];
                                    pos_in_autocompleted_word++;
                                }
                                //cas ou le mot devine est plus long que le nombre de char reellement entre par lutilisateur
                                std::vector<char>::iterator it;
                                it = full_text.begin();
                                while (pos_in_autocompleted_word < guess_list[current_guess_num].size()){
                                    full_text.insert(it+pos_in_full_text+pos_in_autocompleted_word,guess_list[current_guess_num][pos_in_autocompleted_word]);
                                    pos_in_autocompleted_word++;
                                }

                                //ajouter l espace final
                                keyString = ' ';

                                //decaler le curseur a la fin du mot
                                current_line_pos += pos_in_autocompleted_word;
                                while (!(current_line_pos<NB_CHAR_PER_LINE)){
                                    current_line_pos-=NB_CHAR_PER_LINE;
                                    num_current_line++;
                                    num_current_line_on_screen ++;
                                    if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                                        num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                                    }
                                    if (num_current_line == num_max_line && current_line_pos > full_text.size()%NB_CHAR_PER_LINE){
                                        current_line_pos = full_text.size()%NB_CHAR_PER_LINE;
                                    }
                                }
                            }else{
                                int pos_in_full_text = current_line_pos+num_current_line*NB_CHAR_PER_LINE;
                                if (pos_in_full_text>0 && (full_text[pos_in_full_text-1] == ' ' || full_text[pos_in_full_text-1] == '.')){
                                    keyString = '.';
                                }else{
                                    keyString = ' ';
                                }
                            }
                            currently_guessing_a_word = false;
                            current_word.clear();
                            pos_in_current_word = -1;
                        }

                        if (keyString == '.'){
                            if (num_current_line*NB_CHAR_PER_LINE+current_line_pos > 0){
                                full_text[num_current_line*NB_CHAR_PER_LINE+current_line_pos-1] = keyString;
                            }
                            //on met un espace apres une ponctuation
                            keyString = ' ';
                        }
                        std::vector<char>::iterator it;
                        it = full_text.begin();
                        full_text.insert(it+num_current_line*NB_CHAR_PER_LINE+current_line_pos,keyString);
                        //full_text.insert(it+num_current_line*NB_CHAR_PER_LINE+current_line_pos,keyString);
                        //full_text.insert(it+num_current_line*NB_CHAR_PER_LINE+current_line_pos,anothervector.begin(),anothervector.end());
                        

                    }
                    if (keyString == 'C'){

                        currently_guessing_a_word = false;

                        if (num_current_line+current_line_pos>0){//on est pas en 0,0
                            for (int k=(num_current_line*NB_CHAR_PER_LINE)+current_line_pos;k<full_text.size();k++){
                                full_text[k-1] = full_text[k];
                            }
                            full_text.pop_back();
                            if (num_max_line>0 && full_text.size()%NB_CHAR_PER_LINE == 0){
                                num_max_line --;
                            }

                            //
                            //  recalcul de la position du curseur
                            // 
                            if (current_line_pos > 0){
                                current_line_pos --;
                            }else if(current_line_pos == 0 && num_current_line>0){
                                current_line_pos = NB_CHAR_PER_LINE - 1;
                                num_current_line --;
                                num_current_line_on_screen --;
                                if (num_current_line_on_screen < 0){
                                    num_current_line_on_screen = 0;
                                }
                            }

                            //
                            //  recalcul de la position du curseur dans le mot en train d etre devine
                            //  et reevaluation de tout le mot concerne
                            //
                            current_word.clear();
                            pos_in_current_word = 0;
                            int pos_in_full_text = current_line_pos+num_current_line*NB_CHAR_PER_LINE;
                            while(pos_in_full_text>0 && full_text[pos_in_full_text-1] != ' ' && full_text[pos_in_full_text-1] != '.'){
                                pos_in_full_text--;
                                pos_in_current_word++;
                            }
                            int s = full_text.size();
                            while(pos_in_full_text < s && full_text[pos_in_full_text] != ' ' && full_text[pos_in_full_text] != '.'){
                                current_word.push_back(letter_to_num(full_text[pos_in_full_text]));
                                pos_in_full_text++;
                            }  
                        }
                        
                    }
                    if (key_is_new_char){
                        
                        //
                        //  mettre a jour la position du curseur
                        //
                        current_line_pos ++;
                        if (!(current_line_pos < NB_CHAR_PER_LINE)){
                            num_current_line ++;
                            num_current_line_on_screen ++;
                            if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                                num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                            }
                            current_line_pos = 0;
                        }
                        //
                        //  mettre a jour le mot en cours decriture
                        //
                        if (keyString != ' '){
                            std::vector<char>::iterator it;
                            it = current_word.begin();
                            current_word.insert(it+pos_in_current_word,keyString);
                            pos_in_current_word++;
                        }

                        if (current_word.size()>0){
                            currently_guessing_a_word = true;
                            guess_list = autocompletion(current_word);
                            total_nb_guess = guess_list.size();
                            if (total_nb_guess<=NB_MAX_GUESS_PRINTED){
                                nb_guess_printed = total_nb_guess;
                            }else{
                                nb_guess_printed = NB_MAX_GUESS_PRINTED;
                            }
                            current_guess_num = 0;
                        }else{
                            currently_guessing_a_word = false;
                        }
                    }
                    
                    while ((num_max_line+1)*NB_CHAR_PER_LINE < full_text.size() || num_max_line < num_current_line /*|| (current_line_pos == NB_CHAR_PER_LINE-1 && num_max_line == num_current_line)/**/){
                        num_max_line ++;
                    }
                }
                
            }
            //
            //  disjonction de cas selon les fleches directionnelles
            //  droite et gauche permettent de se deplacer
            //
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                // left key is pressed

                nb_guess_printed = 0;
                currently_guessing_a_word = false;
                total_nb_guess = 0;
                current_guess_num = 0;
                pos_in_current_word = -1;
                current_word.clear();

                if (current_line_pos > 0){
                    current_line_pos --;
                }else if(current_line_pos <= 0 && num_current_line>0){
                    current_line_pos = NB_CHAR_PER_LINE - 1;
                    num_current_line --;
                    num_current_line_on_screen --;
                    if (num_current_line_on_screen < 0){
                        num_current_line_on_screen = 0;
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                // right key is pressed
                
                nb_guess_printed = 0;
                currently_guessing_a_word = false;
                total_nb_guess = 0;
                current_guess_num = 0;
                pos_in_current_word = -1;
                current_word.clear();

                if (current_line_pos < NB_CHAR_PER_LINE - 1 && NB_CHAR_PER_LINE*num_current_line+current_line_pos<full_text.size()){
                    current_line_pos ++;
                }else if(current_line_pos == NB_CHAR_PER_LINE - 1 && num_current_line < num_max_line){
                    current_line_pos = 0;
                    num_current_line ++;
                    num_current_line_on_screen ++;
                    if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                        num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                    }
                } 
            }
            //
            //  haut et bas permettent de choisir parmi les propositions de mot
            //  ou alors se deplacer si aucun mot n'est en train d etre propose
            //
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                // up key is pressed
                if (currently_guessing_a_word && total_nb_guess>0){
                    current_guess_num = (current_guess_num-1+total_nb_guess)%total_nb_guess;
                }
                else if (num_current_line > 0){

                    pos_in_current_word = -1;
                    current_word.clear();

                    num_current_line --;
                    num_current_line_on_screen --;
                    if (num_current_line_on_screen < 0){
                        num_current_line_on_screen = 0;
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                // down key is pressed
                if (currently_guessing_a_word && total_nb_guess>0){
                    current_guess_num = (current_guess_num+1)%total_nb_guess;
                }
                else if (num_current_line < num_max_line){

                    pos_in_current_word = -1;
                    current_word.clear();

                    num_current_line ++;
                    num_current_line_on_screen ++;
                    if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                        num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                    }
                    if (num_current_line == num_max_line && current_line_pos > full_text.size()%NB_CHAR_PER_LINE){
                        current_line_pos = full_text.size()%NB_CHAR_PER_LINE;
                    }
                }
            }
        }
        
        
        sf::Time elapsed = clock.getElapsedTime();
        
        //
        //  affichage des elements de linterface graphique
        //

        window.clear();

        


        window.draw(phone_sprite);

        window.draw(nokia_screen);

        //
        //  affichage des elements de texte
        //
        
        if (num_max_line < NB_LINES_ON_SCREEN-1){
            nb_lines_printed = num_max_line+1;
        }else{
            nb_lines_printed = NB_LINES_ON_SCREEN;
        }

        if (currently_guessing_a_word && nb_guess_printed > 0){
            if (num_current_line_on_screen + nb_guess_printed < NB_LINES_ON_SCREEN){
                line_offset = num_current_line_on_screen;
            }else{
                line_offset = NB_LINES_ON_SCREEN-1-nb_guess_printed ;
            }
            while (nb_lines_printed + nb_guess_printed > NB_LINES_ON_SCREEN){
                nb_lines_printed --;
            }


            for (int k=0;k<nb_guess_printed;k++){
                sf::Text guess_k_text;
                // select the font
                guess_k_text.setFont(nokiafont);
                // define text
                char buf[NB_CHAR_MAX_PER_WORD+3];
                int pos_last_char = 0;
                for (int l=0;l<NB_CHAR_PER_LINE+1;l++){
                    if(l<guess_list[(k+current_guess_num)%total_nb_guess].size()){
                        buf[l] = guess_list[(k+current_guess_num)%total_nb_guess][l];
                        pos_last_char = l;
                    }else{
                        buf[l] = 0;
                    }
                }
                
                //s'asurer que ca ne depasse pas de l'ecran
                //taille non fiable car depend de la fonte et de chaque char
                /*guess_k_text.setString(buf);
                if (guess_k_text.getGlobalBounds().width*charSize > 24*screen_ref_width){
                    while (pos_last_char > 0 && guess_k_text.getGlobalBounds().width*charSize > 25*screen_ref_width){
                        buf[pos_last_char] = '.';
                        buf[pos_last_char+1] = '.';
                        buf[pos_last_char+2] = '.';
                        buf[pos_last_char+3] = 0;
                        pos_last_char --;
                        guess_k_text.setString(buf);
                    }
                }*/
                if (pos_last_char>=NB_CHAR_PER_LINE){
                    buf[NB_CHAR_PER_LINE+1] = '.';
                    buf[NB_CHAR_PER_LINE+2] = '.';
                    buf[NB_CHAR_PER_LINE+3] = '.';
                    buf[NB_CHAR_PER_LINE+4] = 0;
                }

                guess_k_text.setString(buf);

                // set the character size
                guess_k_text.setCharacterSize(charSize); // in pixels, not points!
                // set the color
                if (k==0){
                    guess_k_text.setFillColor(screen_color);
                }else{
                    guess_k_text.setFillColor(sf::Color::Black);
                }
                // set the text style
                guess_k_text.setStyle(sf::Text::Regular);
                // set position
                guess_k_text.setPosition(phone_w*0.19,phone_h*0.24 + (nb_lines_printed+k)*charSize);
                if (k==0){
                    best_gess_background.setPosition(phone_w*0.19-1,phone_h*0.24 + nb_lines_printed*charSize);
                    best_gess_background.setSize(sf::Vector2f(guess_k_text.getLocalBounds().width+2,charSize+2));
                    window.draw(best_gess_background);
                }
                window.draw(guess_k_text);
                
            }
            
        }else{
            line_offset = num_current_line_on_screen;
        }

        for (int k=0;k<nb_lines_printed;k++){
            
            for (int j=0;j<NB_CHAR_PER_LINE;j++){
                int char_pos = (k+num_current_line-line_offset)*NB_CHAR_PER_LINE+j;
                if (char_pos < full_text.size()){
                    char_on_screen[k*NB_CHAR_PER_LINE+j].setString(full_text[char_pos]);
                    window.draw(char_on_screen[k*NB_CHAR_PER_LINE+j]);
                }
                
            }
            
        }

        

        //
        // affichage curseur ( + effet clignotant)
        //

        if ((int)floor(2*elapsed.asSeconds())%2 == 0){
            int cursor_x_pos = current_line_pos;
            int cursor_y_pos = line_offset;
            if (cursor_x_pos >= NB_CHAR_PER_LINE){
                cursor_x_pos = 0;
                cursor_y_pos ++;
            }
            if (cursor_x_pos < 0 ){
                cursor_x_pos = 0;
            }
            if (cursor_y_pos < 0){
                cursor_y_pos = 0;
            }
            if (cursor_y_pos >= NB_LINES_ON_SCREEN){
                cursor_y_pos = NB_LINES_ON_SCREEN-1;
            }

            
            
            blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NB_CHAR_PER_LINE),phone_h*0.24 + cursor_y_pos*charSize);
            
            window.draw(blinkText);
        }


        window.display();

    }

    return 0;
}