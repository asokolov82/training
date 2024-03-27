#include "Trie.h"
#include <algorithm>
using namespace std;

int main() {

    Trie slovar;
    vector<string> words;
    slovar.Insert("hello");
    slovar.Insert("help");
    slovar.Insert("hell");
    slovar.Insert("head");
    slovar.Insert("worm");
    slovar.Insert("window");
    slovar.Insert("winter");
    slovar.Insert("world");
    slovar.Insert("waste");
     
    string prefix(""), phrase(""), message("");
    int selector;
    cout << "Type some text or 'q' for exit. Tips for 'h' and 'w'" << endl;
    while (true) {
        words.clear();
        do {
            getline(cin, phrase);
            auto it = find_if(phrase.begin(), phrase.end(), isdigit);
            if (it == phrase.end()) break; // проверяем что нет цифр в введенной фразе
        } while (true);
        prefix = phrase.substr(phrase.rfind(' ') + 1, phrase.length() - phrase.rfind(' ')); // формируем префикс для поиска в словаре
        if (prefix == "q") break;
        TrieNode* current = slovar.Search(prefix); // ищем префикс
        if (current == NULL or current == &slovar.root) {// если нет вариантов слов 
            cout << "No options" << endl;
            if (phrase != prefix)  message += phrase.substr(0, phrase.rfind(' ')); // если был еще текст кроме префикса, сохраняем его
            cout << message;
        }
        else {
            slovar.search_Words(current, prefix, "", words); // формируем вектор вариантов слов
            cout << "select word options: ";
            for (int i = 0; i != words.size(); i++)
                cout << i + 1 << ": " << words[i] << "    " ;
            do {
                cout << endl << "Please enter a number between 1 and " << words.size() << ": ";
                cin >> selector;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (selector <= words.size() && selector > 0) break;
           } while (true);
            (phrase != prefix) ? message += phrase.substr(0, phrase.rfind(' ')) + " " + words[selector - 1] : message += " " + words[selector - 1]; 
            // если ввели только префикс добавляем к прошлому сообщению только слово, иначе часть фразы вместе со словом
            cout << message;
            cin.get();
        }
    }
     return 0;
}




