#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdlib>

#define NUM_CHAR_PER_LINE 16
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
    nokia_screen.setFillColor(sf::Color(162, 160, 39));
    // set position
    //nokia_screen.setPosition(phone_w*0.158,phone_h*0.22);
    nokia_screen.setPosition(phone_w*0.1775,phone_h*0.245);



    std::vector<std::vector<char>> linesBuffers;
    std::vector<sf::Text> linesTexts;

    int charSize = phone_w/20;

    // load font
    sf::Font nokiafont;
    if (!nokiafont.loadFromFile("nokiafc22.ttf")){
        printf("font file not found");
        return -1;
    }

    int current_buffer_pos = NUM_CHAR_PER_LINE;
    int num_current_buffer = -1;
    int num_max_buffer = -1;



    

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

    


    while (window.isOpen())
    {
        //window.setSize(sf::Vector2u(window.getSize().x ,window.getSize().x));


        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if ( event.type == sf::Event::TextEntered ){
                if ( event.text.unicode < 0x80 ) // it's printable
                {
                    keyString = (char)event.text.unicode;
                    if ((keyString-'0' >= 0 && keyString-'0' <= 9) || keyString == (char)13){
                        current_buffer_pos ++;
                    }
                    if (!(current_buffer_pos < NUM_CHAR_PER_LINE)){
                        num_current_buffer ++;
                        current_buffer_pos = 0;
                    }
                    if (keyString == (char)13){
                        num_current_buffer ++;
                        current_buffer_pos = -1;
                    }
                    while (num_max_buffer < num_current_buffer){
                        num_max_buffer ++;
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
                        // the string is store in an another variable 
                        // to make it easier to change it
                        std::vector<char> buf(NUM_CHAR_PER_LINE);
                        linesBuffers.push_back(buf);
                        for (int l=0;l<NUM_CHAR_PER_LINE;l++){
                            linesBuffers[num_max_buffer][l] = ' ';
                        }
                    }


                    if (keyString-'0' >= 0 && keyString-'0' <= 9){
                        linesBuffers[num_current_buffer][current_buffer_pos] = keyString;
                        char buf [NUM_CHAR_PER_LINE];
                        for (int l=0;l<NUM_CHAR_PER_LINE;l++){
                            buf[l] = linesBuffers[num_current_buffer][l];
                        }
                        linesTexts[num_current_buffer].setString(buf);
                    }
                    //printf("char : \n%c,\n%d\n",keyString,keyString);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    // left key is pressed
                    
                }
            }
        }
        
        sf::Time elapsed = clock.getElapsedTime();
        


        window.clear();

        


        window.draw(phone_sprite);

        window.draw(nokia_screen);

        //window.draw(text);
        if (num_current_buffer >= NB_LINES_ON_SCREEN){
            for (int k=0;k<NB_LINES_ON_SCREEN;k++){
                
                linesTexts[k+num_current_buffer+1-NB_LINES_ON_SCREEN].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
                window.draw(linesTexts[k+num_current_buffer+1-NB_LINES_ON_SCREEN]);
            }
        }else{
            for (int k=0;k<num_current_buffer+1;k++){
                
                linesTexts[k].setPosition(phone_w*0.19,phone_h*0.24 + k*charSize);
                
                window.draw(linesTexts[k]);
            }
        }

        if ((int)floor(2*elapsed.asSeconds())%2 == 0){
            int cursor_x_pos = current_buffer_pos;
            if (cursor_x_pos < 0 || cursor_x_pos >= NUM_CHAR_PER_LINE){
                cursor_x_pos = 0;
            }
            int cursor_y_pos = num_current_buffer;
            if (cursor_y_pos < 0){
                cursor_y_pos = 0;
            }
            if (num_current_buffer >= NB_LINES_ON_SCREEN){
                blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NUM_CHAR_PER_LINE),phone_h*0.24 + (NB_LINES_ON_SCREEN-1)*charSize);
            }else{
                blinkText.setPosition(phone_w*0.19 + cursor_x_pos*(nokia_screen_w/NUM_CHAR_PER_LINE),phone_h*0.24 + cursor_y_pos*charSize);
            }
            window.draw(blinkText);
        }
        

        window.display();

    }

    return 0;
}