// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0)
        throw std::out_of_range("TSet: negative max power");
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet: IsMember index out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet: InsElem index out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet: DelElem index out of range");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s)
{
    if (this == &s)
        return *this;
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s)
{
    TSet res(BitField | s.BitField);
    return res;
}

TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet: operator+ element out of range");
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet: operator- element out of range");
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s)
{
    TSet res(BitField & s.BitField);
    return res;
}

TSet TSet::operator~(void)
{
    TSet res(~BitField);
    // Дополнение — в пределах MaxPower
    return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s)
{
    int elem;
    // формат ввода: количество элементов, затем сами элементы
    // (простая реализация — читаем элементы, пока не конец)
    while (istr >> elem)
    {
        if (elem >= 0 && elem < s.MaxPower)
            s.InsElem(elem);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s)
{
    ostr << "{ ";
    for (int i = 0; i < s.MaxPower; i++)
        if (s.IsMember(i))
            ostr << i << " ";
    ostr << "}";
    return ostr;
}