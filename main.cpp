/*
    This file is under gpl3 license
    Boyaroslav 2023
    http://github.com/Boyaroslav

*/




#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<unistd.h>
#include <ctime>

using namespace std;
const int gx = 15, gy = 25;

const sf::Color brick_colors[] = {
    sf::Color(200, 100, 100),
    sf::Color(100, 200, 140),
    sf::Color(100, 100, 150),
    sf::Color(150, 100, 100)
};

struct block{

    int x;
    int y;

};

struct town_block{
    bool im_exist;

    sf::Color color; // 0 0 0 if nothing
};
town_block town[gy][gx];
// все блоки умещаются в матрицу 4х4


const block shapes[4][4] = {{{1, 2}, {1, 3}, {2, 2}, {2, 3}},
                         {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
                          {{1, 0}, {2, 0}, {2, 1}, {2, 2}},
                          {{3, 0}, {2, 0}, {2, 1}, {2, 2}}};

class Brick{
    private:
        vector<block>shape;
        bool hey_i_gonna_die = 0;
        bool make_new_pls = 0;
        sf::Color color = sf::Color(50, 50, 100);
        int interrupt = 0, rotinter=0;
        int x, y;

    public:
        Brick(const block blck[], int nx, int ny){for(int i=0; i<4; i++){shape.push_back(blck[i]);} x = nx; y = ny;}
        
        void draw(sf::RenderWindow &window){
            sf::RectangleShape rect(sf::Vector2f(20.f, 20.f));
            rect.setOutlineThickness(2.f);
            rect.setOutlineColor(sf::Color(50, 50, 100));
            rect.setFillColor(color);

            for(int i=0; i<shape.size(); i++){
                rect.setPosition(299 + 20.f * shape[i].x + 20.f * x, 99 + 20.f * shape[i].y + 20.f * y);
                window.draw(rect);
            }
        }
        void rotate90(){
            if (x < 0 || x > (gx - 1)){return;}
            if(rotinter>0){return;}
            short bx, by;
            for(int i=0; i<4; i++){
                if ((x + shape[i].y < 0) || (x + shape[i].y >= gx)){return;}
                else if (town[y + (4 - shape[i].x)][x + shape[i].y].im_exist){return;}
            }
            for(int i=0; i<4; i++){
                bx = 4 - shape[i].x;
                by = shape[i].y;
                shape[i].x = by;
                shape[i].y = bx;
                rotinter = 5;

            }
            bool isd=0;
            for(int i=0; i<4; i++){

                if ((x + shape[i].x >= gx) || (x + shape[i].x <= 0) || (y + shape[i].y <= 0) || (y + shape[i].y >= gy)){
                    isd = 1;
                    i = 4;
                }
                else if(town[y + shape[i].y][x + shape[i].x].im_exist){
                    isd=1;
                    i = 4;
                }
            }
            if(isd){
                cout<<"gback\n";
                for(int j=0; j<4; j++){                
                    bx = 4 - shape[j].x;
                    by = shape[j].y;
                    shape[j].x = by;
                    shape[j].y = bx;}
                    rotinter = 5;
                    return;
                
            }}
            
        void godown(){
            if(interrupt){return;}
            y++;
        }
        int logic(){
            if(rotinter>0){rotinter--;}
            if(interrupt>0){interrupt--;return 0;}
            bool isd = 0;
            
            if (make_new_pls == 1){
                make_new_pls = 0;
                x = 2;        
                int c = rand() % 4;
                for(int i=0; i<4; i++){
                    shape[i].x = shapes[c][i].x;
                    shape[i].y = shapes[c][i].y;
                }
                color = brick_colors[c];

            }
            
            for(int i=0; i<4; i++){
                if(((y + shape[i].y + 1) >= gy) && interrupt == 0){
                    isd = 1;
                }
                else if ((town[y + shape[i].y + 1][x + shape[i].x].im_exist) && interrupt == 0){
                    isd = 1;
                }
                else if(interrupt){interrupt--;}

            }
        if(isd){
                if (!hey_i_gonna_die){
                    hey_i_gonna_die = 1;interrupt = 10;return 1;}
                    
                    else{
            
                        for(int j=0; j<4; j++){
                            town[y + shape[j].y][x + shape[j].x].im_exist = 1;
                            town[y + shape[j].y][x + shape[j].x].color = color;
                        }
                        y = 0;
                        hey_i_gonna_die = 0;
                        
                        make_new_pls = 1;
                        return 0;

                        
                    }
        }
        
         return 0;}
        int moveright(){
            for(int i=0; i<4; i++){
                if (shape[i].x + x + 1 >= gx){return 0;}
                if (town[y + shape[i].y][x + shape[i].x + 1].im_exist == 1){return 0;}
            }
            this->x += 1;
        }
        int moveleft(){
            for(int i=0; i<4; i++){
                if (shape[i].x + x - 1 < 0){ return 0;}
                if (town[y + shape[i].y][x + shape[i].x - 1].im_exist == 1){return 0;}
            }
            this->x -= 1;
        }
        void set_color(sf::Color c){
            this->color = c;

        }

        
        
};

Brick new_brick(){
    int c = rand() % 3;
    Brick f = Brick(shapes[c], 2, 4);
    f.set_color(brick_colors[c]);
    return f;
}

void delt(int i){
    for(int j=0; j<gx; j++){
        for(int z=i; z>0; z--){
            town[z][j] = town[z-1][j];
        }
    }
}
char SCORE_BUFFER[1024];
void set_str(int i){
    int p=1; while(i>=p){p*=10;}
    p /= 10;
    int j=0;

    while(p>0){
        SCORE_BUFFER[j] = ((i / p) % 10) + '0';
        j++;
        p /= 10;
    }
    SCORE_BUFFER[j] = '\0';

    
}

int main()
{
    sf::Font font;
    if (!font.loadFromFile("Quicksand-Bold.ttf"))
    {
        return -1;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    int score = 0;
    std::srand(std::time(nullptr));

    int counter = 0;
    
    for(int i=0; i<gy; i++){
        for(int j=0; j<gx; j++){
            town[i][j].im_exist = 0;
        }
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "tetris");
    sf::RectangleShape shape(sf::Vector2f(20.f, 20.f));
    shape.setFillColor(sf::Color(200, 100, 50));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(50, 50, 100));
    window.setFramerateLimit(60);
    Brick f = new_brick();
    int c;
    


    while (window.isOpen())
    {
        

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                std::cout << "new width: " << event.size.width << std::endl;
                std::cout << "new height: " << event.size.height << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                f.moveright();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                f.moveleft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                f.rotate90();
            }
        }
        if (counter % 15 == 0){

            counter = 0;
            f.godown();
            
            
        }
        f.logic();



        window.clear(sf::Color(100, 100, 50));
        c=0;
        for(int i=0; i<gy; i++){
            c=0;
            for(int j=0; j<gx; j++){
                if(town[i][j].im_exist){
                    shape.setFillColor(town[i][j].color);
                    c++;
                }
                shape.setPosition(300 + 20.f * j, 100 + 20.f * i);
                window.draw(shape);
                shape.setFillColor(sf::Color(200, 100, 50));
            }
            if(c == gx){
                delt(i);
                score += 100;
                set_str(score);
                text.setString(SCORE_BUFFER);
            }
        }
        for(int d=0; d<gx; d++){
            if(town[0][d].im_exist){
                text.setString("Game over!\0");
                window.draw(text);
                window.display();
                usleep(2000);
                cout<<"Game over. exit..."
                return 0;
            }
        }
        window.draw(text);

        f.draw(window);


        window.display();

        counter++;

    }

    return 0;
}
