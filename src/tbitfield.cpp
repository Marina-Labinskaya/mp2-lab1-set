// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("Length must be non-negative");
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(maxLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        res.pMem[i] |= bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    int minMem = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    TBitField res(maxLen);
    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];

    int lastBits = BitLen % (sizeof(TELEM) * 8);
    if (lastBits != 0)
    {
        TELEM mask = (1 << lastBits) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;
    while (istr >> ch)
    {
        if (ch == '0')
            bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else
            break;
        if (i >= bf.BitLen)
            break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
