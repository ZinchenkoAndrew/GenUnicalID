//Включаем заголовки STL

#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <thread>
#include <mutex>


#include "GenID.h"

    //Проверяем, можно ли использовать данный символ для ID
    inline bool GenID::isGoodSymbol(char ch)
    {
        return skipSymbols.find(ch) == skipSymbols.npos;
    }

    //Проверяем, корректна ли пара символов
    bool GenID::isCorrectPair(const std::pair<char, char>& pair)
    {
        auto& [letter, digit] = pair;
        if ((digit < '1') || (digit > '9')) return false;
        if ((letter < 'A') || (letter > 'Z')) return false;
        //лишняя проверка, т.к. мы уже проверили диапазон 1..9, однако может пригодиться при других условиях задания
        //if (!isGoodSymbol(digit)) return false;
        if (!isGoodSymbol(letter)) return false;//дополнительная проверка первого символа пары
        return true;
    };

    //Начальное значение ID
    void inline GenID::resetOneSection(int index)
    {
        id[index] = std::make_pair('A', '1');
    };



    bool GenID::getNextPair(std::pair<char, char>& pair)
    {
        auto& [letter, digit] = pair;
        bool carry = false;
        if (++digit > '9')
        {
            digit = '1';
            do
            {
                if (++letter > 'Z')
                {
                    letter = 'A';
                    //Заглушка на случай,если нет разрешенных "следующих" символов
                    //первый раз мы попадаем сюда с carry = false. Если carry = true, значит мы здесь уже были
                    assert(!carry && "We have forever loop...");
                    carry = true;
                }
            } while (!isGoodSymbol(letter));
        }
        else carry = false;
        return carry;
    }

    //Рассчитываем новый ID+=1 с учетом переносов из младших разрядов:
    bool GenID::incID()
    {
        bool carry = true;
        for (size_t i = 0; i < id.size(); i++)
        {
            //Получаем текущую пару символов:
            auto [letter, digit] = id[i];
            if (carry)
            {
                //Если пара {0,0}(не инициализирована),
                if (letter == 0)
                {
                    // то инициализируем начальным значением {'A','1'}
                    resetOneSection(i);
                    //Так как у нас только начальное значение,то ни о каких переносах не может 
                    //быть и речи:
                    carry = false;
                    break;
                }
                else
                {//иначе получаем следующее значение для пары, 
                 //сохраняем наличие переноса в старший разряд:
                    carry = getNextPair(id[i]);
                }
            }

            if (carry == false) break;
        }
        return carry;
    };


    //Разбиваем строку на подстроки по заданному разделителю:
    std::vector<std::string> GenID::split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }



    //Метод установки произвольного ID.Новое значение ID принимается
    //в формате "XX-XX-...-XX", где XX - корректные для ID пары символов
    //Проводится удаление лишних пробелов и тщательная проверка ID
    //на корректность,в случае успеха ID обновляется и возвращается true
    bool GenID::setNewID(const std::string& new_id_str_const)
    {
        std::string new_id_str{};

        //Удаляем возможные пробелы из входной строки:
        for (auto ch : new_id_str_const)
        {
            if (ch != ' ') new_id_str += ch;
        }

        //Создаем временный ID, так как нет уверенности в корректности пар XX:
        std::vector <std::pair<char, char>> new_id(nSlots, { 0,0 });


        //Разбиваем входную строку по разделителю ('-'), получая вектор из пар XX:
        std::vector<std::string> new_id_sect = split(new_id_str, delim);

        //Если количество пар больше nSlots - выходим, новый ID не установлен:
        if (new_id_sect.size() > nSlots) return false;

        int i = 0;
        //Циклически перебираем все входные пары XX
        for (auto itSectStr = new_id_sect.rbegin(); itSectStr != new_id_sect.rend(); itSectStr++)
        {
            auto sectStr = *itSectStr;
            //Если очередная входная пара XX имеет больше двух символов - выходим, новый ID не установлен:
            if (sectStr.length() > 2) return false;
            std::pair<char, char> section{};
            section.first = sectStr[0];
            section.second = sectStr[1];
            //Если очередная входная пара XX содержит запрещенные символы - выходим, новый ID не установлен:
            if (!isCorrectPair(section)) return false;
            new_id[i++] = section;
        }
        //Если временные ID полностью корректен(что мы проверили выше попарно), то 
        //устанавливаем новый ID:
        id = new_id;
        return true;
    }

    //Метод инкремента ID и возврата нового значения в формате 
    //"XX-XX-...-XX", где XX - корректные для ID пары символов
    bool GenID::incAndReturnID(std::string& idStr)
    {
        //Вызываем incID() и получаем ID+=1. Если инкремент привел к переполнению ID
        //выводим сообщение об ошибке. Переполнение разрядной сетки ID не оговорено в условиях
        //задания,однако должно как-либо отрабатываться. Данная отработка - просто заглушка:
        assert(!incID() && "ID is overflow!!!");

        //Формируем в цикле строку с нашим ID в виде XX-XX... 
        idStr.clear();
        for (auto it = id.rbegin(); it != id.rend(); it++)
        {
            auto [letter, digit] = *it;
            if (letter != 0)
            {
                idStr += letter;
                idStr += digit;
                idStr += delim;
            }
        }
        idStr.resize(idStr.length() - 1);
        //Возвращаем новый ID в idStr по ссылке
        return false;
    }


