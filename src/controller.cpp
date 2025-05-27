#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <bits/stdc++.h>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"
#include "RPSGameObject.h"

RPSGameObject* collision(std::vector<RPSGameObject*>, Position, RPSGameObject*);
bool checkCoor(std::vector<std::pair<int, int>> coors, std::pair<int, int> coor) {
    for(auto i : coors) if(i == coor) {
        return false;
    }
    return true;
}

RPSType endGame(std::vector<RPSGameObject*> objs) {
    RPSGameObject* anchor = objs.front();
    for(size_t i = 1; i < objs.size(); i++) {
        if(anchor->getType() != objs[i]->getType()) return NONE;
    }
    return anchor->getType();
}

Controller::Controller(View& view) : _view(view){
}

void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    srand(time(NULL));
    int n = 5;
    std::vector<std::pair<int, int>> coors;
    for(int i = 0; i < n; i++) {
        std::pair<int, int> coor = {rand() % 19, rand() % 20};
        while(!checkCoor(coors, coor) || !checkCoor(coors, {coor.first+1, coor.second})) {
            coor = {rand() % 19, rand() % 20};
        }
        (i != 0) ? _objs.push_back(gameObjectFactory::createRock(coor.first, coor.second, " R", false)) : _objs.push_back(gameObjectFactory::createRock(coor.first, coor.second, "R", true));
        coors.push_back(coor);
        coors.push_back({++coor.first, coor.second});
    }
    for(int i = 0; i < n; i++) {
        std::pair<int, int> coor = {rand() % 19, rand() % 20};
        while(!checkCoor(coors, coor) || !checkCoor(coors, {coor.first+1, coor.second})) {
            coor = {rand() % 19, rand() % 20};
        }
        _objs.push_back(gameObjectFactory::createPaper(coor.first, coor.second));
        coors.push_back(coor);
        coors.push_back({++coor.first, coor.second});
    }
    for(int i = 0; i < n; i++) {
        std::pair<int, int> coor = {rand() % 19, rand() % 20};
        while(!checkCoor(coors, coor) || !checkCoor(coors, {coor.first+1, coor.second})) {
            coor = {rand() % 19, rand() % 20};
        }
        _objs.push_back(gameObjectFactory::createScissor(coor.first, coor.second));
        coors.push_back(coor);
        coors.push_back({++coor.first, coor.second});
    }
    
    // init state
    int input = -1;
    clock_t start, end;
    
    // Main loop
    while (true) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27)break;

        this->handleInput(input);

        _view.resetLatest();
        for(RPSGameObject* obj : _objs) 
        {
            int mvmt = (rand() % 4) + 1;
            Position pos = obj->getPosition();
            if(!obj->getControl()) {
                if(mvmt == 1) pos.e2--;
                else if(mvmt == 2) pos.e2++;
                else if(mvmt == 4) pos.e1++;
                else if(mvmt == 3) pos.e1--;
                if(collision(_objs, pos, obj) == NULL) obj->update(mvmt);
                else {
                    pos = obj->getPosition();
                    if(collision(_objs, {pos.e1-1, pos.e2}, obj) == NULL) obj->update(3);
                    else if(collision(_objs, {pos.e1, pos.e2-1}, obj) == NULL) obj->update(1);
                    else if(collision(_objs, {pos.e1, pos.e2+1}, obj) == NULL) obj->update(2);
                    else if(collision(_objs, {pos.e1+1, pos.e2}, obj) == NULL) obj->update(4);
                }
            }
            _view.updateGameObject(obj);
            for(RPSGameObject *other : _objs) {
                ICollider* obj1 = obj;
                ICollider* obj2 = other;
                if(obj == other) continue;
                if(obj->intersect(obj2)) {
                    obj->onCollision(obj2);
                    bool noControl = false;
                    for(auto i : _objs) {
                        if(i->getControl() && i->getType() == ROCK) {
                            noControl = false;
                            break;
                        }
                        noControl = true;
                    }
                    if(noControl) {
                        for(auto i : _objs) {
                            if(i->getType() == ROCK) {
                                i->setControl();
                                i->setIcon();
                                break;
                            }
                        }
                    }
                }
            }

            _view.updateGameObject(obj);
        }

        _view.render();
       
        end = clock();

        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 400); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
				if(endGame(_objs) != NONE) {
            RPSType res = endGame(_objs);
            if(res == ROCK) {
                std::cout << "Rock Win" << std::endl;
                std::cout << "Such a WINNER !!" << std::endl;
            } else if(res == PAPER) {
                std::cout << "Paper Win" << std::endl;
                std::cout << "Go back to your room to practice more, LOSER !!" << std::endl;
            } else {
                std::cout << "Scissors Win" << std::endl;
                std::cout << "Go back to your room to practice more, LOSER !!" << std::endl;
            }
            break;
        }
    }

}



void Controller::handleInput(int keyInput){

    // If there is no input, do nothing.
    if(keyInput==-1)return;
    RPSGameObject* current;;
    int a = -1;
    for(size_t i = 0; i + 1 < _objs.size(); ++i) if(_objs[i]->getType() == ROCK && _objs[i]->getControl()) {
        current = _objs[i];
        a = i;
        break;
    }
    if(a != -1) {
        Position pos = current->getPosition();
        switch(keyInput) {
            case 'w':
                // if(collision(_objs, pos) != NULL) {
                //     std::cout << "Collision occur" <<std::endl;
                //     // ICollider* curr = dynamic_cast<RPSGameObject*>(current);
                //     ICollider* other = dynamic_cast<RPSGameObject*>(collision(_objs, pos));
                //     if(current->intersect(other)) current->onCollision(other);
                //     break;
                // }
                pos.e2--;
                if(collision(_objs, pos, current) == NULL) current->update(1);
                return;
            case 's':
                // if(collision(_objs, pos) != NULL) {
                //     std::cout << "Collision occur" <<std::endl;
                //     // ICollider* curr = dynamic_cast<RPSGameObject*>(current);
                //     ICollider* other = dynamic_cast<RPSGameObject*>(collision(_objs, pos));
                //     if(current->intersect(other)) current->onCollision(other);
                //     break;
                // }
                pos.e2++;
                if(collision(_objs, pos, current) == NULL) current->update(2);
                return;
            case 'd':
                // if(collision(_objs, pos) != NULL) {
                //     // ICollider* curr = dynamic_cast<RPSGameObject*>(current);
                //     ICollider* other = dynamic_cast<RPSGameObject*>(collision(_objs, pos));
                //     if(current->intersect(other)) current->onCollision(other);
                //     break;
                // }
                pos.e1++;
                if(collision(_objs, pos, current) == NULL) current->update(4);
                return;
            case 'a':
                // if(collision(_objs, pos) != NULL) {
                //     // ICollider* curr = dynamic_cast<RPSGameObject*>(current);
                //     ICollider* other = dynamic_cast<RPSGameObject*>(collision(_objs, pos));
                //     if(current->intersect(other)) current->onCollision(other);
                //     break;
                // }
                pos.e1--;
                if(collision(_objs, pos, current) == NULL) current->update(3);
                return;
            case 9:
                RPSGameObject* next = _objs[a+1];
                while(next->getType() != ROCK) {
                    next = _objs[++a];
                    if(next == _objs.back()) {
                        a = 0;
                        next = _objs[a];
                    } else if(next->getControl() == true) {
                        next = _objs[++a];
                    }
                        // if(next == _objs.back()) {
                        //     next = _objs[0];
                        //     a = 0;
                        //     continue;
                        // } else if(next == current) {
                        //     next = _objs[++a];
                        //     continue;
                        // }
                        // next = _objs[++a];
                    }
                if(current->getControl()) {
                    current->unsetControl();
                    next->setControl();
                    current->setIcon();
                    next->setIcon();
                    current = next;
                    break;
                }
                return;
        }
    }
    // TODO 
    // handle key events.
}

void Controller::update(){

}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

RPSGameObject* collision(std::vector<RPSGameObject*> _objs, Position player, RPSGameObject* curr) {
    for(RPSGameObject* obj : _objs) {
        if(curr == obj) continue;
        // std::cout << obj->getPosition().e1 << " " << obj->getPosition().e2 << std::endl;
        // std::cout << player.e1 << " " << player.e2 << std::endl;
        if(obj->getPosition().e1 == player.e1 && obj->getPosition().e2 == player.e2) return obj;
        else if(((obj->getPosition().e1 + 1) == player.e1) && (obj->getPosition().e2 == player.e2)) return obj;
        else if((obj->getPosition().e1 == (player.e1 + 1)) && (obj->getPosition().e2 == player.e2)) return obj;
    }
    return NULL;
}
