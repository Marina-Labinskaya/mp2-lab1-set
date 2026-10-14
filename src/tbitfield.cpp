// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"




TBitField::TBitField(int len)
{
    if (len < 0)
        throw "negative length";
    BitLen = len;
    MemLen = (len + 31) >> 5;          // сколько unsigned int нужно
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
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

int TBitField::GetMemIndex(const int n) const // индекс в pMem для бита n
{
    return n >> 5;                       // n / 32
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);                // 1 << (n % 32)
}

// доступ к битам

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if ((n < 0) || (n >= BitLen))
        throw "bit out of range";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if ((n < 0) || (n >= BitLen))
        throw "bit out of range";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if ((n < 0) || (n >= BitLen))
        throw "bit out of range";
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this == &bf)
        return *this;
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(len);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        res.pMem[i] |= bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField& bf)
{
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(len);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];
    // обнуляем лишние биты в последнем элементе
    int odd = BitLen & 31;
    if (odd != 0)
        res.pMem[MemLen - 1] &= (1u << odd) - 1;
    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf)
{
    int i = 0;
    char ch;
    while (true)
    {
        istr >> ch;
        if (ch == '0')
            bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else
            break;
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}