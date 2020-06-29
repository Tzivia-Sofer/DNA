#include <exception>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include "exceptions.h"

class Dna{
private:


public:
    Dna(char*);
    ~Dna();
    Dna(const std::string);
    Dna(const Dna &);
    Dna & operator = (const Dna &);
    Dna & operator = (const std::string);
    Dna & operator = (const char *);
    friend std::ostream& operator << (std::ostream &os,const Dna&);
    bool operator == (const Dna &);
    bool operator != (const Dna &);
    size_t getLength()const;
    Dna readFromFile(const char*);
    void writeToFile(const char*);
    Dna slice(size_t, size_t);
    Dna pair();
    size_t count(char*);
    size_t find(const char*);
    std::vector<char*> findConsensusSequences();
    std::vector <size_t> findAll(char* c_to_find);
    class Nucleotide{                            //inner class to avoid assigning invalid char from operator []
    public:
        Nucleotide(){};
        Nucleotide(const char &);
        Nucleotide & operator = (const char);
        char &getNucleotide() {return m_nucleotide;};
        friend std::ostream& operator << (std::ostream &os,const Nucleotide&);
        operator char(){return m_nucleotide;};

    private:
        char m_nucleotide;

    };
    Nucleotide& operator[] (size_t);
    const Nucleotide& operator[] (size_t) const;
    char* convert(Nucleotide* n)const;

private:
    Nucleotide *m_sequence;
};

void checkValid(const char);

inline std::ostream& operator << (std::ostream &os, const Dna::Nucleotide& dna)
{
    if(dna.m_nucleotide == '\0')
        throw ErrorSequence("NULL object");
    os << dna.m_nucleotide << std::endl;
    return os;
}

inline Dna::Nucleotide& Dna::Nucleotide::operator = (const char c)
{
    checkValid(c);
    m_nucleotide = c;
    return *this;
}

inline Dna::Nucleotide::Nucleotide(const char& c)
{
    checkValid(c);
    m_nucleotide = c;
}

inline Dna::~Dna()
{
    delete this->m_sequence;
}

inline Dna::Nucleotide& Dna::operator[](size_t index)
{
    if(index > strlen(convert(this->m_sequence)))
        throw ErrorIndex("index out of range");
    return m_sequence[index];

}

inline const Dna::Nucleotide& Dna::operator[](size_t index) const
{
    if(index > strlen(convert(this->m_sequence)))
        throw ErrorIndex("index out of range");
    return m_sequence[index];
}

inline bool Dna::operator == (const Dna &dna1)
{
    if (strcmp(convert(this->m_sequence), convert(dna1.m_sequence)) == 0)
        return true;
    return false;
}

inline bool Dna::operator != (const Dna &dna1)
{
    if (strcmp(convert(this->m_sequence), convert(dna1.m_sequence)) != 0)
        return true;
    return false;
}

inline std::ostream& operator << (std::ostream &os, const Dna& dna)
{
    if(dna.m_sequence == NULL)
        throw ErrorSequence("NULL object");
    os << dna.convert(dna.m_sequence) << std::endl;
    return os;
}

inline Dna& Dna::operator = (const Dna &dna2)
{
    if(strcmp(convert(dna2.m_sequence),convert(this->m_sequence) ) != 0)
    {
       return Dna::operator=(convert(dna2.m_sequence));
    }
}

inline Dna& Dna::operator = (const char *sequence)
{
    Nucleotide* tmp = new Nucleotide[strlen(sequence)+1];
    delete this->m_sequence;
    this->m_sequence = tmp;
    for(size_t i=0; i<=getLength(); i++)
    {
        m_sequence[i] = sequence[i];
    }
    return *this;
}

inline Dna& Dna::operator = (const std::string sequence)
{
    return Dna::operator=(sequence.c_str());
}

inline Dna::Dna(const Dna &dna2)
{
    if(dna2.m_sequence==NULL)
        throw ErrorSequence("enter sequence");

    m_sequence = new Nucleotide[(dna2.getLength())+1];
    for (size_t i=0; i<=dna2.getLength(); i++)
    {
        try{
            m_sequence[i] = dna2.m_sequence[i];
        }
        catch(ErrorInitDna & e){
            delete[] m_sequence;
            throw;
        }
    }
}

inline Dna::Dna(char* sequence)
{
    if(sequence==NULL)
        throw ErrorSequence("enter sequence");
    size_t i;
    m_sequence = new Nucleotide[strlen(sequence)+1];
    for (i=0; i<=strlen(sequence); i++)
    {
        try{
            m_sequence[i] = sequence[i];
        }
        catch(ErrorInitDna & e){
            delete[] m_sequence;
            throw;
        }
    }
    m_sequence[i] = '\0';
}

inline Dna::Dna(std::string sequence)
{
    m_sequence = new Nucleotide[sequence.length()+1];
    for (size_t i=0; i<=sequence.length(); i++)
    {
        try{
            m_sequence[i] = sequence[i];
        }
        catch(ErrorInitDna & e){
            delete[] m_sequence;
            throw;
        }
    }
}
