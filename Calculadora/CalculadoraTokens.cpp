//#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <regex>
#include <random>
#include <stdexcept>

using namespace std;

double primary();
double term();
double expression();
inline void error(const string &s);

class Token
{
public:
    char kind;
    double value;
    // Token (char k): kind (k), value (0.0){}
    Token(char k)
    {
        kind = k;
        value = 0.0;
    }
    // Token (char k, double v): kind (k), value (v);
    Token(char k, double v)
    {
        kind = k;
        value = v;
    }
};

class Token_stream
{
public:
    Token get();
    void putback(Token t);

private:
    bool full = {false};
    Token buffer = (0);
};
Token_stream ts;

void Token_stream::putback(Token t)
{
    if (full)
    {
        error("putback() into a full buffer");
    }
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;
    switch (ch)
    {
    case ';':
    case 'q':
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
        return Token(ch);
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token('8', val);
    }
    default:
        error("bad token");
    }
}

inline void error(const string &s)
{
    throw runtime_error(s);
}
/*
Token get_token()
{
    char ch;
    cin >> ch; //

    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
        return Token(ch);
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token('8', val);
    }
    default:
        error("Bad token");

        break;
    }
}*/

double primary()
{
    Token t = ts.get();
    switch (t.kind)
    {
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')')
        {
            error("')' expected");
        }
        return d;
        break;
    }
    case '8':
    {
        return t.value;
    }
    default:
        error("primary expected");
        break;
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();
    while (true)
    {
        switch (t.kind)
        {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0)
            {
                error("Divided by zero");
            }
            left /= primary();
            t = ts.get();
            break;
        }
        /*
        case '%':
            left %= primary(); // borrar este comentario cuando deje de fallar
            t = ts.get();
            break;*/
        default:
            ts.putback(t);
            return left;
            break;
        }
    }
}
// Una expresion debe de ser Term
//

double expression()
{
    double left = term();
    Token t = ts.get();
    while (true)
    {
        switch (t.kind)
        {
        case '+':
            left += term();
            t = ts.get();
            break;
        case '-':
            left -= term();
            t = ts.get();
            break;

        default:
            ts.putback(t);
            return left;
            break;
        }
    }
}

int main()
{
    try
    {
        double val;
        while (cin)
        {
            Token t = ts.get();
            if (t.kind == 'q')
            {
                break;
            }
            if (t.kind == ';')
            {
                cout << "=" << val << endl;
            }
            else
                ts.putback(t);
            val = expression();
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Ni yo se que paso " << endl;
        return 2;
    }
}