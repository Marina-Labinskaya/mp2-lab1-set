// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <cstring>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

// Конструктор: создаёт поле из len битов, инициализированных нулями
TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::out_of_range("TBitField: negative length");

    BitLen = len;
    MemLen = (len + (8 * sizeof(TELEM) - 1)) / (8 * sizeof(TELEM));
    if (MemLen == 0)
        MemLen = 0;

    if (MemLen > 0)
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }
    else
    {
        pMem = nullptr;
    }
}

// Конструктор копирования
TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0)
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    else
    {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

// индекс элемента памяти для бита n
int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField: bit index out of range");
    return n / (8 * sizeof(TELEM));
}

// битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField: bit index out of range");
    return static_cast<TELEM>(1) << (n % (8 * sizeof(TELEM)));
}

// доступ к битам

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField: SetBit index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField: ClrBit index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField: GetBit index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf)
        return *this;

    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        if (MemLen > 0)
            pMem = new TELEM[MemLen];
        else
            pMem = nullptr;
    }
    else
    {
        BitLen = bf.BitLen;
    }

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
    {
        // для последнего элемента сравниваем только значащие биты
        if (i == MemLen - 1)
        {
            int used = BitLen % (8 * sizeof(TELEM));
            TELEM mask = (used == 0) ? static_cast<TELEM>(~static_cast<TELEM>(0))
                                     : ((static_cast<TELEM>(1) << used) - 1);
            if ((pMem[i] & mask) != (bf.pMem[i] & mask))
                return 0;
        }
        else
        {
            if (pMem[i] != bf.pMem[i])
                return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];

    // остаток — копируем из более длинного операнда
    if (MemLen > bf.MemLen)
        for (int i = minMem; i < MemLen; i++)
            res.pMem[i] = pMem[i];
    else
        for (int i = minMem; i < bf.MemLen; i++)
            res.pMem[i] = bf.pMem[i];

    return res;
}

TBitField TBitField::operator&(const TBitField &bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];

    // остаток — нули (обнулён в конструкторе), т.к. операнда нет
    // (в памяти res уже нули)

    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];

    // Обнулить незначащие биты последнего элемента
    if (MemLen > 0)
    {
        int used = BitLen % (8 * sizeof(TELEM));
        if (used != 0)
        {
            TELEM mask = (static_cast<TELEM>(1) << used) - 1;
            res.pMem[MemLen - 1] &= mask;
        }
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf)
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        int bit;
        istr >> bit;
        if (bit)
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf)
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}