#pragma once
 //�������� ��������� STL
#include <vector>




class GenID
{
private:
    const size_t nSlots = 10;//���-�� ��� �������� � ����� ID
    const std::string_view skipSymbols{ "DGJMQV0" };//����������� ��� ������������� � ID �������
    const char delim = '-';//����������� ��� ��������
    std::vector <std::pair<char, char>> id;//������ ��� ID ������ �������� � ������� vector


    //���������, ����� �� ������������ ������ ������ ��� ID
    inline bool isGoodSymbol(char ch);
    

    //���������, ��������� �� ���� ��������
    bool isCorrectPair(const std::pair<char, char>& section);
    

    //��������� �������� ID
    void inline resetOneSection(int index);
    

    bool getNextPair(std::pair<char, char>& pair);
    
        //������������ ����� ID+=1 � ������ ��������� �� ������� ��������:
    bool incID();
    

    //��������� ������ �� ��������� �� ��������� �����������:
    std::vector<std::string> split(const std::string& s, char delim);
    

public:
    GenID() :id(nSlots, { 0,0 })//������������� �������
    {

    };
    //����� ��������� ������������� ID.����� �������� ID �����������
    //� ������� "XX-XX-...-XX", ��� XX - ���������� ��� ID ���� ��������
    //���������� �������� ������ �������� � ���������� �������� ID
    //�� ������������,� ������ ������ ID ����������� � ������������ true
    bool setNewID(const std::string& new_id_str_const);
    

    //����� ���������� ID � �������� ������ �������� � ������� 
    //"XX-XX-...-XX", ��� XX - ���������� ��� ID ���� ��������
    bool incAndReturnID(std::string& idStr);
    
};


