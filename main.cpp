#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdlib>

#define NB_CHAR_PER_LINE 16
#define NB_LINES_ON_SCREEN 10

int main()
{
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

    

    // define a 120x50 rectangle
    sf::RectangleShape nokia_screen(sf::Vector2f(100.f, 50.f));
    // change the size
    //nokia_screen.setSize(sf::Vector2f(phone_w*0.68,phone_h*0.235));
    nokia_screen.setSize(sf::Vector2f(phone_w*0.64,phone_h*0.19));
    // set color
    sf::Color screen_color(162, 160, 39);
    nokia_screen.setFillColor(screen_color);
    // set position
    //nokia_screen.setPosition(phone_w*0.158,phone_h*0.22);
    nokia_screen.setPosition(phone_w*0.1775,phone_h*0.245);



    std::vector<char> full_text;

    std::vector<char> current_word;

    std::vector<sf::Text> linesTexts;

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
    blinkText.setString("_");
    // set the character size
    blinkText.setCharacterSize(charSize); // in pixels, not points!
    // set the color
    //Text.setFillColor(sf::Color(0, 0, 0));//black
    blinkText.setFillColor(sf::Color::Black);
    // set the text style
    //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
    blinkText.setStyle(sf::Text::Regular);
    // set position
    blinkText.setPosition(phone_w*0.19,phone_h*0.24);


    sf::Text best_guess_text;
    // select the font
    best_guess_text.setFont(nokiafont);
    // define text
    best_guess_text.setString("Proof");
    // set the character size
    best_guess_text.setCharacterSize(charSize); // in pixels, not points!
    // set the color
    best_guess_text.setFillColor(screen_color);
    // set the text style
    //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
    best_guess_text.setStyle(sf::Text::Regular);
    // set position
    best_guess_text.setPosition(phone_w*0.19,phone_h*0.24);

    sf::Text guess_list_text;
    // select the font
    guess_list_text.setFont(nokiafont);
    // define text
    guess_list_text.setString("of\nconcept");
    // set the character size
    guess_list_text.setCharacterSize(charSize); // in pixels, not points!
    // set the color
    guess_list_text.setFillColor(sf::Color::Black);
    // set the text style
    //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
    guess_list_text.setStyle(sf::Text::Regular);
    // set position
    guess_list_text.setPosition(phone_w*0.19,phone_h*0.24);

    sf::RectangleShape best_gess_background(sf::Vector2f(0, 0));;
    best_gess_background.setFillColor(sf::Color::Black);
    


    sf::Text text;
    // select the font
    text.setFont(nokiafont);
    // define text
    text.setString("");
    // set the character size
    text.setCharacterSize(charSize); // in pixels, not points!
    // set the color
    //Text.setFillColor(sf::Color(0, 0, 0));//black
    text.setFillColor(sf::Color::Black);
    // set the text style
    //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
    text.setStyle(sf::Text::Regular);
    // set position
    text.setPosition(phone_w*0.19,phone_h*0.24);
    // store the new line of text
    linesTexts.push_back(text);

    // the string is stored in an another variable 
    // to make it easier to change it
    for (int l=0;l<NB_CHAR_PER_LINE;l++){
        full_text.push_back(' ');
    }

    int current_line_pos = 0;
    int num_current_line = 0;
    int num_max_line = 0;
    int num_current_line_on_screen = 0;
    int line_offset;
    int nb_guess_printed = 0;
    int nb_lines_printed = 0;

    bool key_is_new_char;



    while (window.isOpen())
    {
        //window.setSize(sf::Vector2u(window.getSize().x ,window.getSize().x));


        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if ( event.type == sf::Event::TextEntered ){
                if ( event.text.unicode < 0x80 ) {// it's printable
                    keyString = (char)event.text.unicode;
                    key_is_new_char = false;
                    if ((keyString-'0' >= 0 && keyString-'0' <= 9)||keyString=='#'||keyString=='*'){
                        key_is_new_char = true;
                        if (keyString == '0'){
                            nb_guess_printed = 0; // fin d'un mot
                            if (current_word.size() == 1){
                                switch(current_word[0]){
                                    case ' ':
                                        keyString = '.';
                                        break;
                                    case '.':
                                        keyString = '.';
                                        break;
                                    default:
                                        keyString = ' ';
                                        break;
                                }
                            }else{
                                keyString = ' ';
                            }
                            current_word.clear();
                        }else{
                            if (current_word.size() == 1){
                                switch(current_word[0]){
                                    case ' ':
                                        current_word.clear();
                                        break;
                                    case '.':
                                        current_word.clear();
                                        break;
                                    default:
                                        break;
                                }
                            }
                            //prediction etc
                            //===============================
                            //
                            //  proposer une liste de
                            //  suggestion plausibles
                            //
                            //===============================

                            //appeller ici la fonction qui prend word en argument
                            //et qui renvoie une liste de suggestions
                            nb_guess_printed = 3;//valeur arbritraire en attendant
                            
                        }
                        current_word.push_back(keyString);
                        while (!(num_current_line*NB_CHAR_PER_LINE+current_line_pos<full_text.size())){
                            full_text.push_back(' ');
                        }
                        if (num_current_line*NB_CHAR_PER_LINE+current_line_pos > 0){
                            switch(current_word[0]){
                                case '.':
                                    full_text[num_current_line*NB_CHAR_PER_LINE+current_line_pos-1] = keyString;
                                    full_text[num_current_line*NB_CHAR_PER_LINE+current_line_pos] = ' ';
                                    break;
                                default:
                                    full_text[num_current_line*NB_CHAR_PER_LINE+current_line_pos] = keyString;
                                    break;
                            }
                            
                        }else{
                            full_text[num_current_line*NB_CHAR_PER_LINE+current_line_pos] = keyString;
                        }
                        char buf [NB_CHAR_PER_LINE];
                        for (int l=0;l<NB_CHAR_PER_LINE;l++){
                            if (num_current_line*NB_CHAR_PER_LINE+l<full_text.size()){
                                buf[l] = full_text[num_current_line*NB_CHAR_PER_LINE+l];
                            }else{
                                buf[l] = ' ';   
                            }                            
                        }
                        linesTexts[num_current_line].setString(buf);
                    }
                    //printf("char : \n%c,\n%d\n",keyString,keyString);
                    if (keyString == 'C'){
                        if (num_current_line+current_line_pos>0){//on est pas en 0,0
                            //full_text.push_back(' ');
                            //full_text.erase(full_text.begin() + NB_CHAR_PER_LINE*num_current_line+current_line_pos);
                            for (int k=(num_current_line*NB_CHAR_PER_LINE)+current_line_pos;k<full_text.size();k++){
                                full_text[k-1] = full_text[k];
                            }
                            //full_text[full_text.size()-1] = ' ';
                            if (num_max_line>0){
                                full_text.pop_back();
                                if (full_text.size()%NB_CHAR_PER_LINE == 0){
                                    num_max_line --;
                                    linesTexts.pop_back();
                                }
                            }
                            
                            
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
                            char buf[NB_CHAR_PER_LINE];
                            for (int k=0;k<linesTexts.size()-1;k++){
                                for (int l=0;l<NB_CHAR_PER_LINE;l++){
                                    buf[l] = full_text[k*NB_CHAR_PER_LINE+l];
                                }
                                linesTexts[k].setString(buf);
                            }
                            for (int l=0;l<NB_CHAR_PER_LINE;l++){
                                if ((linesTexts.size()-1)*NB_CHAR_PER_LINE+l<full_text.size()){
                                    buf[l] = full_text[(linesTexts.size()-1)*NB_CHAR_PER_LINE+l];
                                }else{
                                    buf[l] = ' ';
                                }
                            }
                            linesTexts[linesTexts.size()-1].setString(buf);
                            
                        }
                        
                    }
                    if (key_is_new_char){
                        current_line_pos ++;
                        if (!(current_line_pos < NB_CHAR_PER_LINE)){
                            num_current_line ++;
                            num_current_line_on_screen ++;
                            if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                                num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                            }
                            current_line_pos = 0;
                        }
                                               
                        /*
                        if (keyString == (char)13){
                            num_current_line ++;
                            num_current_line_on_screen ++;
                            if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                                num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                            }
                            current_line_pos = 0;
                        }*/
                    }
                    
                    while (num_max_line < num_current_line /*|| (current_line_pos == NB_CHAR_PER_LINE-1 && num_max_line == num_current_line)/**/){
                        num_max_line ++;
                        //
                        //add a line of text on the nokia screen
                        //
                        sf::Text text;
                        // select the font
                        text.setFont(nokiafont);
                        // define text
                        text.setString("");
                        // set the character size
                        text.setCharacterSize(charSize); // in pixels, not points!
                        // set the color
                        //Text.setFillColor(sf::Color(0, 0, 0));//black
                        text.setFillColor(sf::Color::Black);
                        // set the text style
                        //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
                        text.setStyle(sf::Text::Regular);
                        // set position
                        text.setPosition(phone_w*0.19,phone_h*0.24);
                        // store the new line of text
                        linesTexts.push_back(text);
                        
                        // the string is stored in an another variable 
                        // to make it easier to change it
                        for (int l=0;l<NB_CHAR_PER_LINE;l++){
                            full_text.push_back(' ');
                        }
                    }
                }
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                // left key is pressed

                nb_guess_printed = 0; //coder la cmpletion autoamtique

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
                
                nb_guess_printed = 0; //coder la cmpletion autoamtique

                if (current_line_pos < NB_CHAR_PER_LINE - 1){
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                // up key is pressed

                nb_guess_printed = 0; //coder la cmpletion autoamtique

                if (num_current_line > 0){
                    num_current_line --;
                    num_current_line_on_screen --;
                    if (num_current_line_on_screen < 0){
                        num_current_line_on_screen = 0;
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                // down key is pressed

                nb_guess_printed = 0; //coder la cmpletion autoamtique

                if (num_current_line < num_max_line){
                    num_current_line ++;
                    num_current_line_on_screen ++;
                    if (num_current_line_on_screen >= NB_LINES_ON_SCREEN){
                        num_current_line_on_screen = NB_LINES_ON_SCREEN - 1;
                    }
                }
            }
        }
        
        sf::Time elapsed = clock.getElapsedTime();
        


        window.clear();

        


        window.draw(phone_sprite);

        window.draw(nokia_screen);

        
        if (num_max_line < NB_LINES_ON_SCREEN-1){
            nb_lines_printed = num_max_line+1;
        }else{
            nb_lines_printed = NB_LINES_ON_SCREEN;
        }

        if (nb_guess_printed > 0){
            if (num_current_line_on_screen + nb_guess_printed < NB_LINES_ON_SCREEN){
                line_offset = num_current_line_on_screen;
            }else{
                line_offset = NB_LINES_ON_SCREEN-1-nb_guess_printed ;
            }
            while (nb_lines_printed + nb_guess_printed > NB_LINES_ON_SCREEN){
                nb_lines_printed --;
            }

            best_gess_background.setPosition(phone_w*0.19-1,phone_h*0.24 + nb_lines_printed*charSize);
            best_gess_background.setSize(sf::Vector2f(best_guess_text.getLocalBounds().width+2,charSize+2));
            window.draw(best_gess_background);
            
            best_guess_text.setPosition(phone_w*0.19,phone_h*0.24 + nb_lines_printed*charSize);
            window.draw(best_guess_text);
            
            guess_list_text.setPosition(phone_w*0.19,phone_h*0.24 + (nb_lines_printed+1)*charSize);
            window.draw(guess_list_text);
        }else{
            line_offset = num_current_line_on_screen;
        }

        for (int k=0;k<nb_lines_printed;k++){
            linesTexts[k+num_current_line-line_offset].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
            
            window.draw(linesTexts[k+num_current_line-line_offset]);
            
        }

        //window.draw(text);
        /*if (!(num_max_line < NB_LINES_ON_SCREEN-1)){
            /*int local_num_lines_to_print = NB_LINES_ON_SCREEN;
            if (current_line_pos >= NB_CHAR_PER_LINE-1 && num_current_line_on_screen == NB_LINES_ON_SCREEN-1){
                local_num_lines_to_print --;
            }
            for (int k=0;k<local_num_lines_to_print;k++){
                
                linesTexts[k+num_current_line-local_num_lines_to_print+NB_LINES_ON_SCREEN- num_current_line_on_screen].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
                window.draw(linesTexts[k+num_current_line-local_num_lines_to_print+NB_LINES_ON_SCREEN-num_current_line_on_screen]);
            }*//*

            for (int k=0;k<NB_LINES_ON_SCREEN;k++){
                linesTexts[k+num_current_line-line_offset].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
                
                window.draw(linesTexts[k+num_current_line-line_offset]);
                
            }
        }else{
            for (int k=0;k<num_max_line+1;k++){
                
                linesTexts[k].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
                
                window.draw(linesTexts[k]);
                
            }
        }*/

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

            //printf("%d,%d,%d,%d,%d\n",cursor_y_pos,num_current_line_on_screen,num_current_line,num_max_line,NB_LINES_ON_SCREEN);
      
            
            blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NB_CHAR_PER_LINE),phone_h*0.24 + cursor_y_pos*charSize);
            /*
            if (!(num_current_line < NB_LINES_ON_SCREEN-1)){
                blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NB_CHAR_PER_LINE),phone_h*0.24 + (NB_LINES_ON_SCREEN-1)*charSize);
            }else{
                blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NB_CHAR_PER_LINE),phone_h*0.24 + cursor_y_pos*charSize);
            }*/
            window.draw(blinkText);
        }


        window.display();

    }

    return 0;
}