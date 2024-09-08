#include <iostream>
#include <conio.h>
#include <vector>
#include <string_view>
#include "Random.h"

namespace Setting
{
    constexpr int guesses{6};
}

namespace Wordlist
{
    std::vector<std::string_view> words{ "Iran" , "sna" , "soroush" , "America" , "family" , "peace" , "honesty" , "strength" };

    std::string_view getrandomword()
    {
        return words[Random::get<std::size_t>(0,words.size()-1)];
    }
}

class Session
{
private:
    std::string_view m_word{Wordlist::getrandomword()};
    std::vector<bool> m_letterisgueesed{std::vector<bool>(26)};
    std::size_t toIndex(char c) const {return static_cast<std::size_t>((c%32)-1);}
    int m_wrongguessesleft{Setting::guesses};

public:
    std::string_view getword() const {return m_word;}
    bool letterisgueesed(char c) const {return m_letterisgueesed[toIndex(c)];}
    void setletterisguessesd (char c) {m_letterisgueesed[toIndex(c)]=true;}
    int wrongguessesleft() const {return m_wrongguessesleft;}
    void removeguess() {--m_wrongguessesleft;}

    bool letterisinword(char c) const
    {
        for (auto i:m_word)
            {if (i==c)
                return true;
            }
                return false;
    }

    bool won()
    {
        for (auto i:m_word)
            if (!letterisgueesed(i))
                {return false;
                }
                return true;
    }
};

void draw(const Session& s)
{
    std::cout << "The word: ";

    for (auto i:s.getword())
    {   if (s.letterisgueesed(i))
           std::cout << i;
        else
           std::cout << "_ ";
    }
    std::cout << "   Wrong guesses: ";
    for (auto i=0 ; i < s.wrongguessesleft() ; ++i)
        std::cout << "+";

    for (char c='a'; c <= 'z' ; ++c)
        if (s.letterisgueesed(c) && !s.letterisinword(c))
        std::cout << c;
    std::cout << '\n';
}

char getguesses(Session& s)
{
    std::cout << "Enter your guess: ";

    while (true)
    {
        char c;
        std::cin >> c;

        if (!std::cin)
        {
            // Fix it
            std::cin.clear();
            std::cout << "That wasn't a valid input.  Try again.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Clear out any extraneous input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // If the user entered an invalid char, try again
        if (c < 'a' || c > 'z')
        {
            std::cout << "That wasn't a valid input.  Try again.\n";
            continue;
        }

        // If the letter was already guessed, try again
        if (s.letterisgueesed(c))
        {
            std::cout << "You already guessed that.  Try again.\n";
            continue;
        }

        // If we got here, this must be a valid guess
        return c;
    }
}

void handle(Session& s , char c)
{
    s.setletterisguessesd(c);

    if (s.letterisinword(c))
       {
            std::cout << "Yes '" << c << "' is in the word!" << '\n';
            return;
       }
        std::cout << "No '" << c << "' is not in the word!" << '\n';

    s.removeguess();
}

main()
{
    std::cout << "Welcome to c++man (a variant of hangman)" << '\n';
    std::cout << "To win: guess the word.   To lose: run out of pluses." << '\n';

    Session s{};

    while (!s.won() && s.wrongguessesleft())
    {
        draw(s);
        char c{getguesses(s)};
        handle(s,c);
    }

    draw(s);
    if (!s.wrongguessesleft())
        std::cout << '\n' << "You lose !" << "The word is: " << s.getword();
    else
        std::cout << '\n' << "You win!";

    getch();
}
