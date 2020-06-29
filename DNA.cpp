#include <vector>
#include "DNA.h"
#include "exceptions.h"


static char change_noc(char c)
{
    char res;
    switch(c)
    {
        case 'A': res = 'T'; break;
        case 'T': res = 'A'; break;
        case 'C': res = 'G'; break;
        case 'G': res = 'C'; break;
    }
    return res;
}

void checkValid(const char c)
{
    if (!(c == 'A' || c == 'T' || c == 'G' ||
          c == 'C' || c =='\0'))
    {

        throw ErrorInitDna("check validation failed");
    }
}

size_t Dna::getLength()const
{
    size_t i=0;
    while(m_sequence[i].getNucleotide() != '\0')
    {
        i++;
    }
    return i;
}

char* Dna::convert(Nucleotide* n)const
{
    char *c = new char[getLength()+1];
    size_t i = 0;
    while(n[i].getNucleotide() != '\0')
    {
        c[i] = n[i].getNucleotide();
        i++;
    }
    c[i] = '\0';
    return c;
}

size_t Dna::find(const char *seq_to_find)
{
    std::string in = convert(m_sequence);
    return in.find(seq_to_find);
}

Dna Dna::pair()
{
    size_t size = getLength();
    char res[size];
    for(size_t i=0; i<getLength(); i++)
    {
        res[i] = change_noc(m_sequence[size - i - 1]);
    }
    return Dna(res);
}

Dna Dna::slice(size_t start, size_t end)
{
    char res[end - start + 1];
    for (size_t i = start; i< end; i++)
    {
        res[i-start] = m_sequence[i];
    }
    res[end-start] = '\0';
    return Dna (res);
}

Dna Dna::readFromFile(const char* file_name)
{
    std::string line;
    std::ifstream fs(file_name);
    if(fs.is_open())
    {
        getline(fs,line);
        Dna d(line);
        fs.close();
        return d;

    }
    throw ErrorOpenFile("Can Not Open File");
}

void Dna::writeToFile(const char *file_name)
{
    std::ofstream fs(file_name);
    if(fs.is_open())
    {
        fs << convert(m_sequence);
        fs.close();
        return;
    }
    throw ErrorOpenFile("Can Not Open File");
}

std::vector <size_t> Dna::findAll(char* c_to_find)
{
    std::vector <size_t> result;
    size_t state = 0, index;
    Dna checking = *this;
    while(checking.find(c_to_find)!= -1)
    {
        index = checking.find(c_to_find);
        state += index+strlen(c_to_find) ;
        checking = checking.slice(state,getLength());
        result.push_back(state - strlen(c_to_find));
    }
    std::vector<size_t>::iterator it ;
    return result;
}

std::vector<char*> Dna::findConsensusSequences()
{
    std::vector<size_t> start = findAll("ATG");
    std::vector<size_t> endTAA = findAll("TAA");
    std::vector<size_t> endTGA = findAll("TGA");
    std::vector<size_t> endTAG = findAll("TAG");

    std::vector<char *> result;
    std::vector<size_t>::iterator it_start;
    for (it_start = start.begin(); it_start != start.end(); it_start++)
    {
        for (std::vector<size_t>::iterator it_TAA = endTAA.begin(); it_TAA != endTAA.end(); it_TAA++)
        {
            if((*it_TAA - *it_start) % 3 == 0)
            {
                result.push_back(convert(slice(*it_start,*it_TAA+3).m_sequence));
            }
        }
        for (std::vector<size_t>::iterator it_TAG = endTAG.begin(); it_TAG != endTAG.end(); it_TAG++)
        {
            if((*it_TAG - *it_start) % 3 == 0)
            {
                result.push_back(convert(slice(*it_start,*it_TAG+3).m_sequence));
            }
        }
        for (std::vector<size_t>::iterator it_TGA = endTGA.begin(); it_TGA != endTGA.end(); it_TGA++)
        {
            if((*it_TGA - *it_start) % 3 == 0)
            {
                result.push_back(convert(slice(*it_start,*it_TGA+3).m_sequence));
            }
        }
    }
    return result;
}

size_t Dna::count(char* sub_seq)
{
    return findAll(sub_seq).size();
}