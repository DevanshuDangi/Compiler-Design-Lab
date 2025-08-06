#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

// Character‐classification helpers
bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isRelationalOp(char c) {
    return (c == '<' || c == '>' || c == '=');
}

bool isOtherChar(char c) {
    return (!isLetter(c) && !isDigit(c) && !isRelationalOp(c));
}

// Buffer for the current lexeme being built
string lexemeBuffer;

// Collected tokens: pair< tokenType, tokenText >
vector<pair<string, string>> tokens;

// Recursive DFA‐based lexer; state numbers unchanged
void lex(const string& source, int pos, int state, char prevChar) {
    if (pos >= (int)source.length()) return;
    char currentChar = source[pos];

    switch (state) {
        case 0:
            lexemeBuffer.clear();
            lexemeBuffer += currentChar;
            if      (currentChar == 'i') lex(source, pos+1, 1, currentChar);
            else if (currentChar == 'e') lex(source, pos+1, 14, currentChar);
            else if (currentChar == 't') lex(source, pos+1, 18, currentChar);
            else if (isOtherChar(currentChar)) lex(source, pos+1, 0, currentChar);
            else if (currentChar=='=' || currentChar=='>') lex(source, pos+1, 7, currentChar);
            else if (currentChar=='<') lex(source, pos+1, 10, currentChar);
            else if (isLetter(currentChar)) lex(source, pos+1, 3, currentChar);
            else if (isDigit(currentChar)) lex(source, pos+1, 4, currentChar);
            break;

        case 1:  // saw 'i'
            if (currentChar=='f') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 2, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 2:  // matched "if"
            if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                tokens.emplace_back("if", lexemeBuffer);
                lex(source, pos, 0, prevChar);
            }
            break;

        case 3:  // identifier
            if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                tokens.emplace_back("id", lexemeBuffer);
                lexemeBuffer += currentChar;
                lex(source, pos, 0, prevChar);
            }
            break;

        case 4:  // number
            if (isDigit(currentChar)) {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 4, currentChar);
            }
            else {
                tokens.emplace_back("number", lexemeBuffer);
                lex(source, pos, 0, prevChar);
            }
            break;

        case 7:  // '=' or '>'
            if (currentChar == '=') {
                lexemeBuffer += currentChar;
                tokens.emplace_back("relop", lexemeBuffer);
                lex(source, pos+1, 0, currentChar);
            }
            else {
                tokens.emplace_back("relop", lexemeBuffer);
                lex(source, pos, 0, prevChar);
            }
            break;

        case 10: // '<'
            if (currentChar=='>'||currentChar=='=') {
                lexemeBuffer += currentChar;
                tokens.emplace_back("relop", lexemeBuffer);
                lex(source, pos+1, 0, currentChar);
            }
            else {
                tokens.emplace_back("relop", lexemeBuffer);
                lex(source, pos+1, 0, currentChar);
            }
            break;

        case 14: // saw 'e'
            if (currentChar=='l') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 15, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 15: // saw "el"
            if (currentChar=='s') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 16, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 16: // saw "els"
            if (currentChar=='e') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 17, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 17: // matched "else"
            if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                tokens.emplace_back("else", lexemeBuffer);
                lex(source, pos, 0, prevChar);
            }
            break;

        case 18: // saw 't'
            if (currentChar=='h') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 19, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 19: // saw "th"
            if (currentChar=='e') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 20, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 20: // saw "the"
            if (currentChar=='n') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 21, currentChar);
            }
            else if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                lex(source, pos, 0, prevChar);
            }
            break;

        case 21: // matched "then"
            if (isLetter(currentChar)||isDigit(currentChar)||currentChar=='_') {
                lexemeBuffer += currentChar;
                lex(source, pos+1, 3, currentChar);
            }
            else {
                tokens.emplace_back("then", lexemeBuffer);
                lex(source, pos, 0, prevChar);
            }
            break;

        default:
            break;
    }
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    if (!fin.is_open() || !fout.is_open()) {
        cerr << "Error opening input.txt or output.txt\n";
        return 1;
    }

    // Read entire file into a single string
    string line, sourceCode;
    while (getline(fin, line)) {
        sourceCode += line + ' ';
    }
    fin.close();

    // Tokenize
    lexemeBuffer.clear();
    lex(sourceCode, 0, 0, 0);

    // Write tokens to output.txt
    for (const auto& tk : tokens) {
        fout << "(" << tk.first << ", " << tk.second << ")\n";
    }
    fout.close();
    return 0;
}
