// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>
#include <iostream>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp <= 0) {
        throw std::invalid_argument("Max power must be positive");
    }
}

TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

int TSet::IsMember(const int Elem) const {
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) {
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    BitField.ClrBit(Elem);
}

TSet& TSet::operator=(const TSet &s) {
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const {
    return BitField == s.BitField; // Если мощности разные, BitField сам вернет false (если в tbitfield.cpp есть такая проверка)
}

int TSet::operator!=(const TSet &s) const {
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must belong to the same universe");
    }
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) {
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) {
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must belong to the same universe");
    }
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) {
    return TSet(~BitField);
}

std::istream &operator>>(std::istream &istr, TSet &s) {
    TSet tempSet(s.GetMaxPower());
    char ch;
    while (istr >> ch && ch != '{') {}
    if (istr.fail()) return istr;
    while (istr >> ch) {
        if (ch == '}') break;
        if (ch == ',') continue;
        istr.putback(ch);
        int elem;
        if (istr >> elem) {
            tempSet.InsElem(elem);
        } else {
            break;
        }
    }
    s = tempSet;
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TSet &s) {
    ostr << "{ ";
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) ostr << i << " ";
    }
    ostr << "}";
    return ostr;
}