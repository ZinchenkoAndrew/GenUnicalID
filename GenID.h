#pragma once
 //Включаем заголовки STL
#include <vector>




class GenID
{
private:
    const size_t nSlots = 10;//Кол-во пар символов в нашем ID
    const std::string_view skipSymbols{ "DGJMQV0" };//Запрещенные для использования в ID символы
    const char delim = '-';//Разделитель пар символов
    std::vector <std::pair<char, char>> id;//Храним наш ID парами символов в массиве vector


    //Проверяем, можно ли использовать данный символ для ID
    inline bool isGoodSymbol(char ch);
    

    //Проверяем, корректна ли пара символов
    bool isCorrectPair(const std::pair<char, char>& section);
    

    //Начальное значение ID
    void inline resetOneSection(int index);
    

    bool getNextPair(std::pair<char, char>& pair);
    
        //Рассчитываем новый ID+=1 с учетом переносов из младших разрядов:
    bool incID();
    

    //Разбиваем строку на подстроки по заданному разделителю:
    std::vector<std::string> split(const std::string& s, char delim);
    

public:
    GenID() :id(nSlots, { 0,0 })//инициализация вектора
    {

    };
    //Метод установки произвольного ID.Новое значение ID принимается
    //в формате "XX-XX-...-XX", где XX - корректные для ID пары символов
    //Проводится удаление лишних пробелов и тщательная проверка ID
    //на корректность,в случае успеха ID обновляется и возвращается true
    bool setNewID(const std::string& new_id_str_const);
    

    //Метод инкремента ID и возврата нового значения в формате 
    //"XX-XX-...-XX", где XX - корректные для ID пары символов
    bool incAndReturnID(std::string& idStr);
    
};


