#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>
#include <Windows.h>

using namespace std;

enum class Move { ROCK, PAPER, SCISSORS };

string moveToString(Move move) {
    switch (move) {
    case Move::ROCK:
        return "Rock";
    case Move::PAPER:
        return "Paper";
    case Move::SCISSORS:
        return "Scissors";
    default:
        return "";
    }
}

Move getWinner(Move playerMove, Move botMove) {
    if (playerMove == botMove)
        return Move::ROCK;

    if ((playerMove == Move::ROCK && botMove == Move::SCISSORS) ||
        (playerMove == Move::PAPER && botMove == Move::ROCK) ||
        (playerMove == Move::SCISSORS && botMove == Move::PAPER)) {
        return playerMove;
    }

    return botMove;
}

Move getBotMove() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 2);

    return static_cast<Move>(dis(gen));
}

void saveGameResult(const string& filename, const string& result) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << result << endl;
        file.close();
    }
    else {
        cout << "Unable to open the file: " << filename << endl;
    }
}

void printGameResults(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open the file: " << filename << endl;
    }
}

string sanitizeName(const string& name) {
    string sanitizedName = name;
    transform(sanitizedName.begin(), sanitizedName.end(), sanitizedName.begin(), ::tolower);
    replace(sanitizedName.begin(), sanitizedName.end(), ' ', '_');
    vector<string> slurs = {
        "nig", "nigger", "nigs", "niggers", "fag", "fags", "faggot", "faggots"
        // Remember to keep the format: "slur1", "slur2", "slur3"
    };

    for (const string& slur : slurs) {
        if (sanitizedName.find(slur) != string::npos) {
            return "";
        }
    }

    return sanitizedName;
}

void savePlayerName(const string& filename, const string& playerName) {
    ofstream file(filename);
    if (file.is_open()) {
        file << playerName;
        file.close();
    }
    else {
        cout << "Unable to open the file: " << filename << endl;
    }
}

string getPlayerName(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string playerName;
        getline(file, playerName);
        file.close();
        return playerName;
    }
    else {
        return "";
    }
}

void changeName(string& playerName) {
    cout << "Enter your new name: ";
    getline(cin, playerName);
    playerName = sanitizeName(playerName);
}

int main() {
    vector<Move> opponentMoves;
    int playerScore = 0;
    int botScore = 0;
    string playerName;
    const string playerNameFile = "player_name.txt";
    const string gameResultsFile = "game_results.txt";
    const string gameLogsFile = "game_logs.txt";
    SetConsoleTitleA("Rock Paper Scissors");

    playerName = getPlayerName(playerNameFile);
    if (playerName.empty()) {
        cout << "Enter your name (without spaces): ";
        getline(cin, playerName);
        playerName = sanitizeName(playerName);

        if (playerName.empty()) {
            cout << "Invalid name. Name cannot contain slurs or spaces.\n";
            playerName = "Player";
        }

        savePlayerName(playerNameFile, playerName);
    }

    cout << "Welcome to Rock-Paper-Scissors, " << playerName << "!\n";
    cout << "Enter 'r' for Rock, 'p' for Paper, or 's' for Scissors. Enter 'q' to quit. Enter 'c' to change name.\n";

    while (true) {
        string input;
        cout << "Enter your move: ";
        getline(cin, input);

        if (input == "q" || input == "Q")
            break;

        if (input == "c" || input == "C") {
            changeName(playerName);
            savePlayerName(playerNameFile, playerName);
            cout << "Name changed successfully. New name: " << playerName << endl;
            continue;
        }

        if (input != "r" && input != "R" && input != "p" && input != "P" && input != "s" && input != "S") {
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        Move playerMove;
        switch (input[0]) {
        case 'r':
        case 'R':
            playerMove = Move::ROCK;
            break;
        case 'p':
        case 'P':
            playerMove = Move::PAPER;
            break;
        case 's':
        case 'S':
            playerMove = Move::SCISSORS;
            break;
        }

        Move botMove = getBotMove();
        opponentMoves.push_back(playerMove);

        Move winner = getWinner(playerMove, botMove);
        if (winner == playerMove) {
            cout << "You win! " << playerName << "'s move: " << moveToString(playerMove) << " | Bot's move: " << moveToString(botMove) << endl;
            playerScore++;
        }
        else if (winner == botMove) {
            cout << "Bot wins! " << playerName << "'s move: " << moveToString(playerMove) << " | Bot's move: " << moveToString(botMove) << endl;
            botScore++;
        }
        else {
            cout << "It's a tie! " << playerName << "'s move: " << moveToString(playerMove) << " | Bot's move: " << moveToString(botMove) << endl;
        }
    }

    saveGameResult(gameResultsFile, playerName + " Score: " + to_string(playerScore));
    saveGameResult(gameResultsFile, "Bot Score: " + to_string(botScore));
    cout << "Thanks for playing. Final scores - " << playerName << ": " << playerScore << " | Bot: " << botScore << endl;
    printGameResults(gameResultsFile);

    ofstream gameLogs(gameLogsFile);
    if (gameLogs.is_open()) {
        gameLogs << "Player Name: " << playerName << endl;
        gameLogs << "Final Scores - " << playerName << ": " << playerScore << " | Bot: " << botScore << endl;
        gameLogs.close();
        cout << "Game logs saved to " << gameLogsFile << endl;
    }
    else {
        cout << "Unable to open the file: " << gameLogsFile << endl;
    }

    return 0;
}
