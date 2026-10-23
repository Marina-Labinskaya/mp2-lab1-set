// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Конструктор: инициализируем MaxPower и создаём BitField размера mp
TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// Конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// Конструктор преобразования типа (из битового поля в множество)
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

// Оператор приведения к TBitField
TSet::operator TBitField()
{
    return BitField;
}

// Получить макс. к-во эл-тов (мощность Универса)
int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

// Элемент множества? (проверяем, равен ли бит единице)
int TSet::IsMember(const int Elem) const
{
    return BitField.GetBit(Elem);
}

// Включение элемента в множество (ставим бит в 1)
void TSet::InsElem(const int Elem)
{
    BitField.SetBit(Elem);
}

// Исключение элемента из множества (сбрасываем бит в 0)
void TSet::DelElem(const int Elem)
{
    BitField.ClrBit(Elem);
}

// Присваивание
TSet& TSet::operator=(const TSet &s)
{
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

// Сравнение на равенство
int TSet::operator==(const TSet &s) const
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

// Сравнение на неравенство
int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

// Объединение двух множеств
TSet TSet::operator+(const TSet &s)
{
    return TSet(BitField | s.BitField);
}

// Объединение с элементом (добавление элемента)
TSet TSet::operator+(const int Elem)
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

// Разность с элементом (удаление элемента)
TSet TSet::operator-(const int Elem)
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

// Пересечение двух множеств
TSet TSet::operator*(const TSet &s)
{
    return TSet(BitField & s.BitField);
}

// Дополнение (инверсия)
TSet TSet::operator~(void)
{
    return TSet(~BitField);
}

// Ввод множества в формате { e1, e2, ... }
istream &operator>>(istream &istr, TSet &s)
{
    int temp;
    char ch;
    do {
        istr >> ch;
    } while (ch != '{' && !istr.eof());

    do {
        istr >> temp;
        s.InsElem(temp);
        do {
            istr >> ch;
        } while (ch != ',' && ch != '}' && !istr.eof());
    } while (ch != '}' && !istr.eof());

    return istr;
}

// Вывод множества
ostream& operator<<(ostream &ostr, const TSet &s)
{
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}
