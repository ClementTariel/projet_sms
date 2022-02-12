#include <cstdlib>
#include <map>
#include <string>

//on prend que les minuscules
#define ALPHABET_SIZE 26
// on prend que de 2 a 9
#define DIGITS_SIZE 8


class TrieLetter {

private:

    TrieLetter* next_char[ALPHABET_SIZE];
    int freq;
    

public:

    TrieLetter() {
        for (int k = 0; k < ALPHABET_SIZE; k++){
            this->next_char[k] = nullptr;
        }
        freq = 0;
    }

    void insert(const std::string, const int);
    bool del(TrieLetter*, const std::vector<char>::iterator, const std::vector<char>::iterator);
    const int search(const std::string);
    const bool haveChildren(const TrieLetter*);
    std::string num_to_char(char);
    std::vector<std::vector<char>> suggestionsFromNum(TrieLetter*, const std::vector<char>::iterator, const std::vector<char>::iterator, std::vector<char>);

};

std::string TrieLetter::num_to_char(char c) {
    switch(c){
        case '1':
            return ",.!?";
        case '2':
            return "abc";
        case '3':
            return "def";
        case '4':
            return "ghi";
        case '5':
            return "jkl";
        case '6':
            return "mno";
        case '7':
            return "pqrs";
        case '8':
            return "tuv";
        case '9':
            return "wxyz";
        default:
            return "+";
    }
}

void TrieLetter::insert(const std::string word, const int f) {
    TrieLetter* node = this;
    for (int k=0; k<word.size(); k++){
        if (node->next_char[word.at(k)-'a'] == nullptr){
            node->next_char[word.at(k)-'a'] = new TrieLetter();
        }
        node = node->next_char[word.at(k)-'a'];
    }

    node->freq = f;
}

const int TrieLetter::search(const std::string word) {
    //
    //  on verifie que le mot existe ou est un prefixe de mot existant
    //
    if (this == nullptr){
        return -1;
    }
    TrieLetter* node = this;
    for (int k=0; k<word.size(); k++) {
        node = node->next_char[word.at(k)-'a'];
        if (node == nullptr){
            return -1;
        }
    }

    return node->freq;
}

std::vector<std::vector<char>> TrieLetter::suggestionsFromNum(TrieLetter* node, const std::vector<char>::iterator char_in_word, const std::vector<char>::iterator end_of_word, std::vector<char> current_word) {
    std::vector<std::vector<char>> suggestions = std::vector<std::vector<char>>();
    //
    //  on verifie que la node existe
    //
    if (node == nullptr){
        return suggestions;
    }
    if (char_in_word < end_of_word) {
        for (int k=0; k<num_to_char(*char_in_word).size(); k++) {
            int char_num = num_to_char(*char_in_word).at(k)-'a';
            current_word.push_back('a'+char_num);
            std::vector<std::vector<char>> new_suggestions = suggestionsFromNum(node->next_char[char_num],char_in_word+1,end_of_word,current_word);
            current_word.pop_back();
            suggestions.insert(std::end(suggestions), std::begin(new_suggestions), std::end(new_suggestions));
        }
    }else if (node->freq > 0){
        std::vector<char> new_word = std::vector<char>();
        for (int k=0; k<current_word.size(); k++) {
            new_word.push_back(current_word[k]);
            
        }
        suggestions.push_back(new_word);
    }

    return suggestions;
}

const bool TrieLetter::haveChildren(const TrieLetter* node) {
    for (int k=0; k<ALPHABET_SIZE; k++) {
        if (node->next_char[k] != nullptr){
            return true;
        }
    }
    return false;
}


//renvoie true si la node est toujours la et false si word en etait la derniere branche
bool TrieLetter::del(TrieLetter* node, const std::vector<char>::iterator char_in_word, const std::vector<char>::iterator end_of_word) {
    //on verifie quil y a bien quelquechose a supprimer deja
    if (node == nullptr){
        return false;
    }

    // base case
    if (char_in_word == end_of_word) {
        if (haveChildren(node)){
            node->freq = 0;
            return true;
        }else{
            delete node;
            node = nullptr;
            return false;
        }
    }

    char c = *char_in_word;

    if (del(node->next_char[c-'a'],char_in_word+1,end_of_word)){
        //la branche est toujours la donc on la garde
        return true;
    }else if (haveChildren(node) || node->freq>0){
        //on garde la branche car elle a dautres sous-branche/feuille
        return true;
    }else{
        //il ne reste plus rien sur la branche donc on la coupe
        delete node;
        node = nullptr;
        return false;
    }
}

class Word{
private :
    std::vector<char> word;
    int freq;
public:
    Word() {
        word = std::vector<char>();
        freq = 0;
    }
    Word(const std::vector<char> word_to_copy,int f) {
        word = std::vector<char>();
        for (int k = 0; k < word_to_copy.size(); k++){
            word.push_back(word_to_copy[k]);
        }
        freq = f;
    }
    std::vector<char> getWord(){
        return word;
    }
    int getFreq(){
        return freq;
    }
};

class TrieNumber {

private:

    TrieNumber* next_char[DIGITS_SIZE];
    

public:

    std::vector<Word> words;

    TrieNumber() {
        for (int k = 0; k < DIGITS_SIZE; k++){
            this->next_char[k] = nullptr;
        }
        words = std::vector<Word>();
    }

    void insert(Word);
    bool del(TrieNumber*, const std::vector<char>::iterator, const std::vector<char>::iterator);
    const int search(const std::string);
    const bool haveChildren(const TrieNumber*);
    char char_to_num(char);
    std::vector<std::vector<char>> suggestionsFromNum(TrieNumber*, std::vector<char>);
};

//cest moche mais rapide a implementer
char char_to_num(char l){
    if (l-'0' >= 0 &&l-'0'<=9){
        return l;
    }if (l == 's'){
        return '7';
    }if (l == 'v'){
        return '8';
    }if ((int)l > (int)'v'){
        return '9';
    }
    return (char)('2'+((l-'a')/3));
}

void TrieNumber::insert(Word word) {
    TrieNumber* node = this;
    for (int k=0; k<word.getWord().size(); k++){
        if (node->next_char[word.getWord()[k]-'2'] == nullptr){
            node->next_char[word.getWord()[k]-'2'] = new TrieNumber();
        }
        node = node->next_char[word.getWord()[k]-'2'];
    }
    auto it=node->words.begin();
    while (it != node->words.end() && it->getFreq()>word.getFreq()){
        it++;
    }
    node->words.insert(it,word);
                        
}

const int TrieNumber::search(const std::string word) {
    //
    //  on verifie que le mot existe ou est un prefixe de mot existant
    //
    if (this == nullptr){
        return -1;
    }
    TrieNumber* node = this;
    for (int k=0; k<word.size(); k++) {
        node = node->next_char[word.at(k)-'2'];
        if (node == nullptr){
            return -1;
        }
    }
    int f = 0;
    for (auto it=node->words.begin();it != node->words.end();it++){
        f += it->getFreq();
    }

    return f;
}

std::vector<std::vector<char>> TrieNumber::suggestionsFromNum(TrieNumber* node, std::vector<char> word){
    std::vector<std::vector<char>> suggestions = std::vector<std::vector<char>>();
    //
    //  on verifie que la node existe
    //
    for (int k=0; k<word.size(); k++) {
        node = node->next_char[word[k]-'2'];
        if (node == nullptr){
            return suggestions;
        }
    }
    for (auto it=node->words.begin();it != node->words.end();it++){
        std::vector<char> new_word = it->getWord();
        suggestions.push_back(new_word);
    }

    return suggestions;
}

const bool TrieNumber::haveChildren(const TrieNumber* node) {
    for (int k=0; k<DIGITS_SIZE; k++) {
        if (node->next_char[k] != nullptr){
            return true;
        }
    }
    return false;
}


//renvoie true si la node est toujours la et false si word en etait la derniere branche
bool TrieNumber::del(TrieNumber* node, const std::vector<char>::iterator char_in_word, const std::vector<char>::iterator end_of_word) {
    //on verifie quil y a bien quelquechose a supprimer deja
    if (node == nullptr){
        return false;
    }

    // base case
    if (char_in_word == end_of_word) {
        if (haveChildren(node)){
            while(node->words.size()>0){
                node->words.pop_back();
            }
            return true;
        }else{
            delete node;
            node = nullptr;
            return false;
        }
    }

    char c = *char_in_word;

    if (del(node->next_char[c-'2'],char_in_word+1,end_of_word)){
        //la branche est toujours la donc on la garde
        return true;
    }else if (haveChildren(node) || node->words.size()>0){
        //on garde la branche car elle a dautres sous-branche/feuille
        return true;
    }else{
        //il ne reste plus rien sur la branche donc on la coupe
        delete node;
        node = nullptr;
        return false;
    }
}//*/