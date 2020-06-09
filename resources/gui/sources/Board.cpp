//
// Created by marci on 06.06.2020.
//
#include "../headers/Board.h"
//#include "../game_elements/Deck.h"
#include <iostream>
#include <iterator>
#include <unordered_set>

Board::Board(float width, float height, int playerCount){
    round=0;
    IS_NEW_ROUND = true;
    deck = std::make_shared<Deck>();
    stack = std::make_shared<Stack>();

    stack->boardStack.push_back(deck->cardCollection.back());
    stack->setFresh();
    deck->cardCollection.pop_back();
    stack->update();
    for(int i = 0;i<playerCount;i++) {
        players.push_back(std::make_shared<Player>("Gracz "+std::to_string(i+1)));
        players.back()->setNo(i);
        std::cout << "+ Gracz " << i << std::endl;
    }
    activePlayer = players.at(0);
    previousPlayer = players.at(3);
    nextPlayer = players.at(1);
    lastPlayer = players.at(2);
    giveaway();

    //SET PROPERTIES OF BUTTONS
    if(!font.loadFromFile("../assets/fonts/arial.TTF")){
        std::cout<<"error font loading";
    }

    buttons[0].setFont(font);
    buttons[0].setColor(sf::Color::White);
    buttons[0].setString(L"Dobierz kartę i zakończ turę");
    buttons[0].setPosition(sf::Vector2f(800,10 + 160 * 3));

    buttons[1].setFont(font);
    buttons[1].setColor(sf::Color::White);
    buttons[1].setPosition(sf::Vector2f(800,10 + 160 * 3));

    for(int i = 0; i < 4; i++){
        nicknames[i].setFont(font);
        nicknames[i].setColor(sf::Color::White);
        nicknames[i].setString(players.at(i)->getNickname());

    }
    nicknames[0].setPosition(sf::Vector2f(20, 10 + 160 * 3));
    nicknames[1].setPosition(sf::Vector2f(20, 10 + 160 * 0));
    nicknames[2].setPosition(sf::Vector2f(20, 10 + 160 * 1));
    nicknames[3].setPosition(sf::Vector2f(20, 10 + 160 * 2));

    newRoundText.setFont(font);
    newRoundText.setColor(sf::Color::White);
    newRoundText.setString(L"Rozpoczyna Gracz 1. Naciśnij spację aby rozpocząć grę...");
    newRoundText.setPosition(sf::Vector2f(width/2 - 350, height/2 - 20));

    activeButton = 0;
    activeOption = 0;
    activeShape = 0;
    activeNumber = 0;
    option = false;

    //load choose cards
    shapesTextures[0].loadFromFile("../resources/cards/color_D.png");
    shapesTextures[1].loadFromFile("../resources/cards/color_C.png");
    shapesTextures[2].loadFromFile("../resources/cards/color_H.png");
    shapesTextures[3].loadFromFile("../resources/cards/color_S.png");
    numbersTextures[0].loadFromFile("../resources/cards/value_5.png");
    numbersTextures[1].loadFromFile("../resources/cards/value_6.png");
    numbersTextures[2].loadFromFile("../resources/cards/value_7.png");
    numbersTextures[3].loadFromFile("../resources/cards/value_8.png");
    numbersTextures[4].loadFromFile("../resources/cards/value_9.png");
    numbersTextures[5].loadFromFile("../resources/cards/value_10.png");
    for(int i = 0; i < 4; i++){
        shapes[i].setTexture(shapesTextures[i]);
        shapes[i].setScale(0.15,0.15);
        shapes[i].setPosition(sf::Vector2f(920 + 25*i,300));
    }
    for(int i = 0; i < 6; i++){
        numbers[i].setTexture(numbersTextures[i]);
        numbers[i].setScale(0.15, 0.15);
        numbers[i].setPosition(sf::Vector2f(920 + 25*i,300));
    }

    //load choose Text;

    chooseShape.setFont(font);
    chooseShape.setColor(sf::Color::White);
    chooseShape.setString(L"Wybierz żądany kolor:");
    chooseShape.setPosition(sf::Vector2f(850, 250));

    chooseNumber.setFont(font);
    chooseNumber.setColor(sf::Color::White);
    chooseNumber.setString(L"Wybierz żądaną liczbę:");
    chooseNumber.setPosition(sf::Vector2f(880, 250));

}

void Board::giveaway() {
    for(auto player : players){
        for(int i = 0;i<5;i++){
            player->hand.push_back(deck->cardCollection.back());
            deck->cardCollection.pop_back();
        }
    }
}

int Board::getPlayerCount() const {
    return players.capacity();
}

int Board::getActivePlayerHandSize(){
    return activePlayer->hand.size();
}

void Board::draw(sf::RenderWindow &window) {
    float width = window.getSize().x;
    float height = window.getSize().y;
    float widthBetween = 30;
    float distance = 0;
    int no = 0;
    if(!IS_NEW_ROUND){
        activePlayer->drawHand(window, activeOption);
        for(auto player : players){
            if(player==activePlayer){ continue;}
            else{
                player->drawHiddenHand(window,no);
            }
            no++;
        }
        updateNicknames();
        deck->drawDeck(window);
        //std::cout << stack->getCardCount();
        stack->drawStack(window);

        for(int i = 0; i < 4; i++){
            window.draw(nicknames[i]);
        }



        if(stack->getWar()){
            buttons[0].setString("Pobierz karne karty: "+std::to_string(stack->getCardsToPull()));
            window.draw(buttons[0]);

        }
        else{
            buttons[0].setString(L"Dobierz kartę i zakończ turę");
            window.draw(buttons[0]);
        }

    }
    else{
        window.draw(newRoundText);
    }

    //choose window draw
    //shapes


    //numbers


}

Board::~Board() {}

/*std::shared_ptr<Player> Board::nextPlayer(std::shared_ptr<Player> current) {
    if(current==players.back()){
        return players.front();
    }else{
        return ;
    }
}*/

void Board::drawChooseShape(sf::RenderWindow window){
    for(int i = 0; i < 4; i++)
      window.draw(shapes[i]);
    window.draw(chooseShape);
}

void Board::drawChooseNumber(sf::RenderWindow window){
    for(int i = 0; i < 6; i++)
        window.draw(numbers[i]);
    window.draw(chooseNumber);
}

void Board::moveRight() {
    if(activeOption + 1 < activePlayer->hand.size())
        activeOption++;
    else{
        activeOption = activePlayer->hand.size() + 1;
        if(activeButton == 0){
            if(option){
                buttons[activeButton].setColor(sf::Color::White);
                buttons[activeButton].setColor(sf::Color::Red);
            }
            else{
                option = true;
                buttons[activeButton].setColor(sf::Color::Red);
            }
        }
        else{
            if(activeButton == 1){
                buttons[0].setColor(sf::Color::White);
                buttons[1].setColor(sf::Color::Red);
            }
        }
    }
}

void Board::pressSpace() {
    IS_NEW_ROUND = false;
}

void Board::moveLeft() {
    if(activeOption - 1 >= 0 && activeOption != activePlayer->hand.size() + 1)
        activeOption--;
    if(activeOption == activePlayer->hand.size() + 1){
        if(activeButton - 1 >= 0){
            buttons[activeButton].setColor(sf::Color::White);
            buttons[activeButton].setColor(sf::Color::Red);
        }
        else{
            buttons[activeButton].setColor(sf::Color::White);
            option = false;
            activeOption = activePlayer->hand.size() - 1;
        }
    }
}

int Board::getPressedOption(){
    if(activeOption < activePlayer->hand.size() + 1){
        return activeOption;
    }
    else{
        return activePlayer->hand.size() + activeButton;
    }
}

std::shared_ptr<Card> Board::getPressedCard() {
    if(activeOption != activePlayer->hand.size() + 1){
        return activePlayer->hand.at(activeOption);
    }
    else{

    }
}

void Board::throwCard() {

    std::cout << "Runda numer: " << round << std::endl;
    if(getPressedOption() < getActivePlayerHandSize()){
        std::cout<<"Wyrzucono: ";
        getPressedCard()->printCard();

        if(stack->throwToStack(getPressedCard())){
            activePlayer->hand.erase(std::find(activePlayer->hand.begin(),activePlayer->hand.end(),getPressedCard()));
            newRound();
        }else{
            return;
        }
    }
    else{
        if(getPressedOption() == getActivePlayerHandSize()){
            if(!(stack->getWar())){
                drawCard();
                newRound();
                stack->cancelWar();
                std::cout<<"draw"<<std::endl;
            }
            else{
                for(int i = 0;i<stack->getCardsToPull();i++){
                    drawCard();
                }
                newRound();
                stack->cancelWar();
            }
        }
    }
}

void Board::moveStackToDeck() {
    auto topCard = stack->boardStack.back();
    stack->boardStack.pop_back();

    for (auto card : stack->boardStack){
        deck->cardCollection.push_back(card);
        stack->boardStack.erase(std::find(stack->boardStack.begin(),stack->boardStack.end(),card));
    }
    stack->boardStack.push_back(topCard);
    stack->setFresh();
    deck->shuffleDeck();
}

void Board::newRound() {
    IS_NEW_ROUND = true;
    round++;
    bool found = false;
    std::cout << "update..";
    stack->update();
    buttons[0].setColor(sf::Color::White);
    activeOption = 0;
    activeButton = 0;

    std::cout << "..update";
    std::cout << "KOLOR: " << stack->getDesideredColor() << " WARTOSC: " << stack->getDesideredValue() << std::endl;
    /*for(auto player : players){
        std::shared_ptr<Player> current = player;
        if(found){previousPlayer = activePlayer; activePlayer = current;}
        if(player==activePlayer){found=true;}
    }*/

    auto oldAct = activePlayer;
    auto oldPrev = previousPlayer;
    auto oldNext = nextPlayer;
    auto oldLast = lastPlayer;

    activePlayer = oldNext;
    nextPlayer = oldLast;
    lastPlayer = oldPrev;
    previousPlayer = oldAct;

    std::string toDisplay = "Tura Gracza "+(std::to_string(activePlayer->playerNo+1))+". Nacisnij spacje aby kontynuowac...";
    newRoundText.setString(toDisplay);

    std::cout << "Aktywny gracz:" << activePlayer->getNickname();

}

void Board::updateNicknames() {
    nicknames[0].setString(players.at((0+round)%4)->getNickname());
    nicknames[1].setString(players.at((1+round)%4)->getNickname());
    nicknames[2].setString(players.at((2+round)%4)->getNickname());
    nicknames[3].setString(players.at((3+round)%4)->getNickname());
}

void Board::drawCard() { //TODO TO REWORK
    std::cout << "XXX" << deck->cardCollection.size();
    if(checkDeck()){
        std::cout << "wchodze po checkDeck do drawowania";
        activePlayer->hand.push_back(deck->cardCollection.back());
        std::cout << "wrzucilem na reke";
        deck->cardCollection.pop_back();
        std::cout << "usunelem z talii";
    }else{
        std::cout << "Brak kart do ciagniecia.";
        return;
    }
}

bool Board::checkDeck() {
    if(deck->cardCollection.size()==0 && stack->boardStack.size()<=1){return false;}
    else if(deck->cardCollection.size()==0) {
        std::shared_ptr<Card> topCard = stack->topCard();
        stack->boardStack.pop_back();

        while (!(stack->boardStack.empty())) {
            deck->cardCollection.push_back(stack->boardStack.back());
            stack->boardStack.pop_back();
        }
        stack->boardStack.push_back(topCard);

        std::cout << "\nsPrzelozone karty, teraz:" << deck->cardCollection.size();
        return true;
    }else{
        std::cout << "Deck jest git";
        return true;
    }
}

bool Board::getIsNewRound() const {
    return IS_NEW_ROUND;
}



