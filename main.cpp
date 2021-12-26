#include <SFML/Graphics.hpp>

#include <cmath>


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


    
    sf::Text text;
    // load font
    sf::Font nokiafont;
    if (!nokiafont.loadFromFile("nokiafc22.ttf")){
        printf("font file not found");
        return -1;
    }
    // select the font
    text.setFont(nokiafont);
    // define text
    text.setString("Hello world");
    // set the character size
    text.setCharacterSize(phone_w/20); // in pixels, not points!
    // set the color
    //Text.setFillColor(sf::Color(0, 0, 0));//black
    text.setFillColor(sf::Color::Black);
    // set the text style
    //text.setStyle(sf::String::Bold | sf::String::Italic | sf::String::Underlined);
    text.setStyle(sf::Text::Regular);
    // set position
    text.setPosition(phone_w*0.19,phone_h*0.24);
        

    while (window.isOpen())
    {
        //window.setSize(sf::Vector2u(window.getSize().x ,window.getSize().x));


        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }


        window.clear();

        sf::Time elapsed = clock.getElapsedTime();
        if ((int)floor(2*elapsed.asSeconds())%2 == 0){
            text.setString(">");
        }else{
            text.setString(">_");
        }
    


        window.draw(phone_sprite);

        window.draw(nokia_screen);

        window.draw(text);

        window.display();

    }

    return 0;
}