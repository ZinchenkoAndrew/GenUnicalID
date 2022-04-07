//�������� ��������� STL

#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <thread>
#include <mutex>


#include "GenID.h"

    //���������, ����� �� ������������ ������ ������ ��� ID
    inline bool GenID::isGoodSymbol(char ch)
    {
        return skipSymbols.find(ch) == skipSymbols.npos;
    }

    //���������, ��������� �� ���� ��������
    bool GenID::isCorrectPair(const std::pair<char, char>& pair)
    {
        auto& [letter, digit] = pair;
        if ((digit < '1') || (digit > '9')) return false;
        if ((letter < 'A') || (letter > 'Z')) return false;
        //������ ��������, �.�. �� ��� ��������� �������� 1..9, ������ ����� ����������� ��� ������ �������� �������
        //if (!isGoodSymbol(digit)) return false;
        if (!isGoodSymbol(letter)) return false;//�������������� �������� ������� ������� ����
        return true;
    };

    //��������� �������� ID
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
                    //�������� �� ������,���� ��� ����������� "���������" ��������
                    //������ ��� �� �������� ���� � carry = false. ���� carry = true, ������ �� ����� ��� ����
                    assert(!carry && "We have forever loop...");
                    carry = true;
                }
            } while (!isGoodSymbol(letter));
        }
        else carry = false;
        return carry;
    }

    //������������ ����� ID+=1 � ������ ��������� �� ������� ��������:
    bool GenID::incID()
    {
        bool carry = true;
        for (size_t i = 0; i < id.size(); i++)
        {
            //�������� ������� ���� ��������:
            auto [letter, digit] = id[i];
            if (carry)
            {
                //���� ���� {0,0}(�� ����������������),
                if (letter == 0)
                {
                    // �� �������������� ��������� ��������� {'A','1'}
                    resetOneSection(i);
                    //��� ��� � ��� ������ ��������� ��������,�� �� � ����� ��������� �� ����� 
                    //���� � ����:
                    carry = false;
                    break;
                }
                else
                {//����� �������� ��������� �������� ��� ����, 
                 //��������� ������� �������� � ������� ������:
                    carry = getNextPair(id[i]);
                }
            }

            if (carry == false) break;
        }
        return carry;
    };


    //��������� ������ �� ��������� �� ��������� �����������:
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



    //����� ��������� ������������� ID.����� �������� ID �����������
    //� ������� "XX-XX-...-XX", ��� XX - ���������� ��� ID ���� ��������
    //���������� �������� ������ �������� � ���������� �������� ID
    //�� ������������,� ������ ������ ID ����������� � ������������ true
    bool GenID::setNewID(const std::string& new_id_str_const)
    {
        std::string new_id_str{};

        //������� ��������� ������� �� ������� ������:
        for (auto ch : new_id_str_const)
        {
            if (ch != ' ') new_id_str += ch;
        }

        //������� ��������� ID, ��� ��� ��� ����������� � ������������ ��� XX:
        std::vector <std::pair<char, char>> new_id(nSlots, { 0,0 });


        //��������� ������� ������ �� ����������� ('-'), ������� ������ �� ��� XX:
        std::vector<std::string> new_id_sect = split(new_id_str, delim);

        //���� ���������� ��� ������ nSlots - �������, ����� ID �� ����������:
        if (new_id_sect.size() > nSlots) return false;

        int i = 0;
        //���������� ���������� ��� ������� ���� XX
        for (auto itSectStr = new_id_sect.rbegin(); itSectStr != new_id_sect.rend(); itSectStr++)
        {
            auto sectStr = *itSectStr;
            //���� ��������� ������� ���� XX ����� ������ ���� �������� - �������, ����� ID �� ����������:
            if (sectStr.length() > 2) return false;
            std::pair<char, char> section{};
            section.first = sectStr[0];
            section.second = sectStr[1];
            //���� ��������� ������� ���� XX �������� ����������� ������� - �������, ����� ID �� ����������:
            if (!isCorrectPair(section)) return false;
            new_id[i++] = section;
        }
        //���� ��������� ID ��������� ���������(��� �� ��������� ���� �������), �� 
        //������������� ����� ID:
        id = new_id;
        return true;
    }

    //����� ���������� ID � �������� ������ �������� � ������� 
    //"XX-XX-...-XX", ��� XX - ���������� ��� ID ���� ��������
    bool GenID::incAndReturnID(std::string& idStr)
    {
        //�������� incID() � �������� ID+=1. ���� ��������� ������ � ������������ ID
        //������� ��������� �� ������. ������������ ��������� ����� ID �� ��������� � ��������
        //�������,������ ������ ���-���� ��������������. ������ ��������� - ������ ��������:
        assert(!incID() && "ID is overflow!!!");

        //��������� � ����� ������ � ����� ID � ���� XX-XX... 
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
        //���������� ����� ID � idStr �� ������
        return false;
    }


