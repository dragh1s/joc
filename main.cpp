#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <optional>
#include <windows.h> // Beep

using namespace std;

const int tileSize = 20;
const int gridWidth = 30;
const int gridHeight = 30;

struct Node {
    int x, y;
};

void LoadHighScore(int& highScore) {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void SaveHighScore(int score, int highScore) {
    if (score > highScore) {
        ofstream file("highscore.txt");
        if (file.is_open()) {
            file << score;
            file.close();
        }
    }
}

int main() {
    system("chcp 65001 >nul");
    cout << "========================================\n";
    cout << "            ULTIMATE SNAKE GAME         \n";
    cout << "========================================\n";
    cout << "Chose difficulty level:\n";
    cout << "1. Easy\n";
    cout << "2. Normal\n";
    cout << "3. Hard\n";
    cout << "Choose Difficulty (1 to 3): ";

    int choice = 2;
    cin >> choice;

    float baseDelay = 0.15f;
    bool useObstacles = false;
    if (choice == 1) { baseDelay = 0.18f; useObstacles = false; }
    else if (choice == 2) { baseDelay = 0.12f; useObstacles = false; }
    else if (choice == 3) { baseDelay = 0.08f; useObstacles = true; }

    int highScore = 0;
    LoadHighScore(highScore);

    sf::RenderWindow window(sf::VideoMode({gridWidth * tileSize, gridHeight * tileSize}), "Snake 2D - SFML 3 / TERRA");
    window.setFramerateLimit(60);
    vector<Node> snake = {{15, 15}, {14, 15}, {13, 15}};
    int dirX = 1;
    int dirY = 0;

    Node fruit = {rand() % gridWidth, rand() % gridHeight};
    Node bonusFruit = {-1, -1};
    bool hasBonus = false;
    int bonusCounter = 0;
    int score = 0;
    bool isPaused = false;

    vector<Node> obstacles;
    if (useObstacles) {
        for (int i = 0; i < 6; i++) {
            obstacles.push_back({5 + i * 4, 4 + (i % 3) * 3});
        }
    }

    sf::Clock clock;
    float timer = 0.0f;
    sf::RectangleShape block(sf::Vector2f(static_cast<float>(tileSize - 2), static_cast<float>(tileSize - 2)));

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::P) {
                    isPaused = !isPaused;
                }
            }
        }

        if (!isPaused) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dirY == 0) { dirX = 0; dirY = -1; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dirY == 0) { dirX = 0; dirY = 1; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dirX == 0) { dirX = -1; dirY = 0; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dirX == 0) { dirX = 1; dirY = 0; }

            float timeStep = clock.restart().asSeconds();
            timer += timeStep;
            float currentDelay = max(0.04f, baseDelay - (score / 30) * 0.015f);
            
            if (timer > currentDelay) {
                timer = 0.0f;
                for (int i = snake.size() - 1; i > 0; --i) {
                    snake[i] = snake[i - 1];
                }
                snake[0].x += dirX;
                snake[0].y += dirY;

                // Verificare fruct normal
                if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                    score += 10;
                    Beep(800, 40);
                    fruit.x = rand() % gridWidth;
                    fruit.y = rand() % gridHeight;
                    snake.push_back(snake.back());
                    bonusCounter++;

                    if (bonusCounter >= 3) {
                        bonusFruit.x = rand() % gridWidth;
                        bonusFruit.y = rand() % gridHeight;
                        hasBonus = true;
                        bonusCounter = 0;
                    }
                }

                // Verificare fruct bonus
                if (hasBonus && snake[0].x == bonusFruit.x && snake[0].y == bonusFruit.y) {
                    score += 30;
                    Beep(1200, 70);
                    hasBonus = false;
                }

                // Coliziunea cu pereții
                if (snake[0].x < 0 || snake[0].x >= gridWidth || snake[0].y < 0 || snake[0].y >= gridHeight) {
                    Beep(300, 350);
                    SaveHighScore(score, highScore);
                    LoadHighScore(highScore);
                    score = 0;
                    snake = {{15, 15}, {14, 15}, {13, 15}};
                    dirX = 1; dirY = 0;
                    hasBonus = false;
                    bonusCounter = 0;
                }

                // Coliziunea cu propriul corp
                for (size_t i = 1; i < snake.size(); ++i) {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                        Beep(300, 350);
                        SaveHighScore(score, highScore);
                        LoadHighScore(highScore);
                        score = 0;
                        snake = {{15, 15}, {14, 15}, {13, 15}};
                        dirX = 1; dirY = 0;
                        hasBonus = false;
                        bonusCounter = 0;
                    }
                }

                // Coliziunea cu obstacolele
                for (const auto& obs : obstacles) {
                    if (snake[0].x == obs.x && snake[0].y == obs.y) {
                        Beep(300, 350);
                        SaveHighScore(score, highScore);
                        LoadHighScore(highScore);
                        score = 0;
                        snake = {{15, 15}, {14, 15}, {13, 15}};
                        dirX = 1; dirY = 0;
                        hasBonus = false;
                        bonusCounter = 0;
                    }
                }
            }
        } else {
            clock.restart();
        }

        window.setTitle("Snake SFML | Score: " + to_string(score) + " | High Score: " + to_string(highScore) + (isPaused ? " [PAUSED]" : ""));

        window.clear(sf::Color::Black);

        // Desenare obstacole
        block.setFillColor(sf::Color(0, 200, 200));
        for (const auto& obs : obstacles) {
            block.setPosition(sf::Vector2f(static_cast<float>(obs.x * tileSize), static_cast<float>(obs.y * tileSize)));
            window.draw(block);
        }

        // Desenare fruct normal
        block.setFillColor(sf::Color::Red);
        block.setPosition(sf::Vector2f(static_cast<float>(fruit.x * tileSize), static_cast<float>(fruit.y * tileSize)));
        window.draw(block);

        // Desenare fruct bonus
        if (hasBonus) {
            block.setFillColor(sf::Color::Yellow);
            block.setPosition(sf::Vector2f(static_cast<float>(bonusFruit.x * tileSize), static_cast<float>(bonusFruit.y * tileSize)));
            window.draw(block);
        }

        // Desenare șarpe
        for (size_t i = 0; i < snake.size(); ++i) {
            block.setFillColor(i == 0 ? sf::Color::Green : sf::Color(0, 180, 0));
            block.setPosition(sf::Vector2f(static_cast<float>(snake[i].x * tileSize), static_cast<float>(snake[i].y * tileSize)));
            window.draw(block);
        }

        window.display();
    }

    SaveHighScore(score, highScore);
    return 0;
}