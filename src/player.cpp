#include "player.hpp"

Player::Player(std::string name){
    m_name = name;
    m_human = 1;
}

Player::~Player(){

}

void Player::setHuman(bool human){
    m_human = human;
}

bool Player::getHuman(){
    return m_human;
}

void Player::setName(std::string name){
    m_name = name;
}

std::string Player::getName(){
    return m_name;
}