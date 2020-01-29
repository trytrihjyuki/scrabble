#include "player.hpp"

Player::Player(std::string name){
    m_name = name;
    m_activate = 1;
    m_human = 1;
    m_score = 0;
}

Player::~Player(){

}

bool Player::getActivate(){
    return m_activate;
}

void Player::setActivate(bool activate){
    m_activate = activate;
}

bool Player::getHuman(){
    return m_human;
}

void Player::setHuman(bool human){
    m_human = human;
}

std::string Player::getName(){
    return m_name;
}

void Player::setName(std::string name){
    m_name = name;
}

int Player::getScore(){
    return m_score;
}

void Player::setScore(int score){
    m_score = score;
}