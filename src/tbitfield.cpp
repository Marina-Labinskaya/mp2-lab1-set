// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <algorithm>

// Конструктор: выделяем память и зануляем её
TBitField::TBitField(int len)
{
    if (len < 0) {
        throw std::invalid_argument("Negative bitfield length");
    }
    BitLen = len;
    const int bitsInElem = sizeof(TELEM) * 8;
    MemLen = (BitLen + bitsInElem - 1) / bitsInElem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

// Конструктор копирования
TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

// Деструктор: освобождаем память
TBitField::~TBitField()
{
    delete[] pMem;
}

// В какой ячейке массива pMem лежит бит номер n
int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return n / (sizeof(TELEM) * 8);
}

// Маска, где на месте бита n стоит 1, а остальные 0
TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return static_cast<TELEM>(1) << (n % (sizeof(TELEM) * 8));
}

// Получить длину (к-во битов)
int TBitField::GetLength(void) const
{
    return BitLen;
}

// Установить бит n в 1
void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

// Очистить бит n (установить в 0)
void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

// Получить значение бита n (0 или 1)
int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0 ? 1 : 0;
}

// Присваивание
TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf) {
        return *this;
    }
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

// Сравнение на равенство
int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

// Сравнение на неравенство
int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

// Операция "ИЛИ" (объединение полей разной или одинаковой длины)
TBitField TBitField::operator|(const TBitField &bf)
{
    int maxLen = std::max(BitLen, bf.BitLen);
    TBitField res(maxLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

// Операция "И" (пересечение полей)
TBitField TBitField::operator&(const TBitField &bf)
{
    int maxLen = std::max(BitLen, bf.BitLen);
    int minMem = std::min(MemLen, bf.MemLen);
    TBitField res(maxLen);
    for (int i = 0; i < minMem; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

// Отрицание (инверсия только значимых BitLen бит!)
TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            res.SetBit(i);
        }
    }
    return res;
}

// Ввод (строка из 0 и 1)
istream &operator>>(istream &istr, TBitField &bf)
{
    char ch;
    int i = 0;
    while (i < bf.BitLen && istr >> ch) {
        if (ch == '1') {
            bf.SetBit(i++);
        } else if (ch == '0') {
            bf.ClrBit(i++);
        } else {
            break;
        }
    }
    return istr;
}

// Вывод
ostream &operator<<(ostream &ostr, const TBitField &bf)
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
