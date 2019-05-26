#pragma once
#include <iostream>
#include <ostream>
#include <cassert>
#include <stack>
#include <algorithm>

using namespace std;

template <typename K, typename V> class Dictionar;

template <class K, class V>
class Node
{
private:
    K key;
    V value;

    Node <K, V>* st;
    Node <K, V>* dr;

    int height;
    void updateHeight();

    friend class Dictionar <K, V>;

public:
    Node ();
    Node (K key, V value);

    unsigned getKey ();

    int retBalans (Node <K, V>* node);

    static int getHeight (Node <K, V>* node);
};

template <class K, class V>
Node <K, V>::Node()
{
    this->key    = 0;
    this->value  = 0;

    this->height = 0;

    this->st   = nullptr;
    this->dr  = nullptr;
}

template <class K, class V>
Node <K, V>::Node (K cheie, V val)
{
    key = cheie;
    value = val;

    st = nullptr;
    dr = nullptr;
}


template <class K, class V>
int Node <K, V>::retBalans (Node <K, V>* node)
{
    if(node == nullptr)
    {
        return 0;
    }

    return getHeight(node->st) - getHeight(node->dr);
}

template <class K, class V>
int Node <K, V>::getHeight (Node <K, V>* node)
{
    if(node == nullptr)
    {
        return -1;
    }
    return node->height;
}

template <class K, class V>
void Node<K, V>::updateHeight()
{
    height = std::max(Node<K, V>::getHeight(st), Node<K, V>::getHeight(dr)) + 1;
}

template <class K, class V>
unsigned Node <K, V>::getKey ()
{
    return this->key;
}
template <class K, class V>
class Dictionar
{
private:
    Node <K,V>* rad;
    unsigned size;

    Node <K, V>* insert (Node <K, V>* a, K cheie, V val);
    Node <K, V>* remove (Node <K, V>* a, V val);
    Node <K, V>* rotireDreapta (Node <K, V>* a);
    Node <K, V>* rotireStanga  (Node <K, V>* a);
    Node <K, V>* valMin (Node <K, V>* a);
    Node <K, V>* stergereNode (Node <K, V>* a, K cheie);
    void prOcheie (Node <K, V>* a, V &val, K cheie);
    void deleteDictionar (Node <K, V>* a);

public:

    Dictionar ();
    Dictionar (const Dictionar&);    ///constructori si destructor
    ~Dictionar ();

    unsigned numarElem();
    void ord (Node <K, V>* a);
                                           ///parcurgeri
    void preOrd (Node <K, V>* a);
    void adaugareDictionar (K, V);
    void afis();
    void stergereCheie (K);
    void stergereTotala ();
    bool cautCheie(K cheie, V &aux);
    bool dictionarGol();

    V operator [] (const K index) ;
    Dictionar <K, V> &operator = (Dictionar<K, V> dtr);

};

template <class K, class V>
Dictionar <K, V>::Dictionar ()
{
    rad = nullptr;
    size = 0;
}

template <class K, class V>
Dictionar <K, V>::Dictionar (const Dictionar &ob)
{
    this->rad = nullptr;
    this->size = 0;

    std::stack <Node <K, V>* > stiva;
    stiva.push(ob.rad);
    while(!stiva.empty())
    {
        Node <K, V>* a = stiva.top();
        stiva.pop();

        if(a->st != nullptr)
        {
            stiva.push(a->st);
        }

        if(a->dr != nullptr)
        {
            stiva.push(a->dr);
        }

        adaugareDictionar(a->key, a->value);
    }
}

template <class K, class V>
Dictionar <K, V>::~Dictionar()
{
    stergereTotala();
}

template <class K, class V>
Node <K, V>* Dictionar<K, V>::insert(Node <K, V>* a, K cheie, V val)
{
    if(a == nullptr)
    {
        a  = new Node <K, V> (cheie, val);
        return a;
    }
    if(a->key == cheie)
    {
        this->size -= 1;
        a->value = val;
        return a;
    }
    if(cheie > a->key)
    {
        a->dr = insert(a->dr, cheie, val);
    }
    else
    {
        a->st = insert(a->st, cheie, val);
    }

    a->updateHeight();

    int balans = a->retBalans(a);

    if (balans > 1 && cheie < a->st->key)
    {
        return rotireDreapta(a);
    }
    if (balans < -1 && cheie > a->dr->key)
    {
        return rotireStanga(a);
    }
    if (balans > 1 && cheie > a->st->key)
    {
        a->st = rotireStanga(a->st);
        return rotireDreapta(a);
    }
    if (balans < -1 && cheie < a->dr->key)
    {
        a->dr = rotireDreapta(a->dr);
        return rotireStanga(a);
    }

    return a;
}


template <class K, class V>
unsigned Dictionar <K, V>::numarElem()                  ///nr de elem ale dictionarului
{
    return this->size;
}

template <class K, class V>
void Dictionar <K, V>::ord (Node <K, V>* a)
{
    if(a == nullptr)
        return;
    ord(a->st);
    cout << "Cheia: " << a->key;
    cout << " ->>> Valoare: " << a->value << endl << endl;
    ord(a->dr);
}

template <class K, class V>
void Dictionar <K, V>::adaugareDictionar(K cheie, V val)   ///adauare element nou in dictionar
{
    this->size += 1;
    this->rad = insert(this->rad, cheie,  val);
}

template <class K, class V>
void Dictionar <K, V>::afis()
{
    if(rad != nullptr && size > 0)
        ord(rad);
}

template <class K, class V>
void Dictionar <K, V>::preOrd(Node<K, V>* a)
{
    if(a == nullptr)
        return;
    cout << "Cheia: " << a->key;
    cout << "  ->>> Valoare: " << a->value <<endl << endl;

    preOrd(a->st);
    preOrd(a->dr);
}


template <class K, class V>
Node <K, V>* Dictionar<K, V>::rotireStanga(Node <K, V>* a)
{
    Node <K, V>* copilDr = a->dr;
    Node <K, V>* copilSt  = copilDr->st;
    copilDr->st = a;
    a->dr = copilSt;
    a->updateHeight();
    copilDr->updateHeight();

    return copilDr;
}


template <class K, class V>
Node <K, V>* Dictionar<K, V>::rotireDreapta(Node <K, V>* a)
{
    Node <K, V>* copilSt  = a->st;
    Node <K, V>* copilDr = copilSt->dr;
    copilSt->dr = a;
    a->st = copilDr;
    a->updateHeight();
    copilSt->updateHeight();

    return copilSt;
}



template <class K, class V>
void Dictionar <K, V>::prOcheie(Node <K, V>* a, V &val, K cheie)
{
    if(a == nullptr)
        return;

    if(a->key == cheie)
    {
        val = a->value;
        return;
    }

    if(a->key > cheie)
        prOcheie(a->st, val, cheie);
    if(a->key < cheie)
        prOcheie(a->dr, val, cheie);
}

template <class K, class V>
Node <K, V>* Dictionar<K, V>::valMin (Node <K, V>* a)
{
    Node <K, V>* aux = a;
    while(aux->st != nullptr)
        aux = aux->st;
    return aux;
}

template <class K, class V>
bool Dictionar <K, V>::dictionarGol()          ///verifica daca dictionarul este gol
{
    if(numarElem())
        return false;
    return true;
}

template <class K, class V>
bool Dictionar <K, V>::cautCheie(K cheie, V &aux)
{
    prOcheie(rad, aux, cheie);
    if(aux != V())
        return true;
    return false;
}

template <class K, class V>
void Dictionar<K, V>::stergereCheie(K cheie)
{
    this->size -= 1;
    stergereNode(rad, cheie);
    cout<<"Elementul cu cheia "<<cheie<<" a fost sters"<<endl;
}

template <class K, class V>
Node <K, V>* Dictionar <K, V>::stergereNode(Node <K, V>* rad, K cheie)
{
    if(rad == nullptr)
        return rad;


    if(cheie < rad->key)
        rad->st = stergereNode(rad->st, cheie);
    else if(cheie > rad->key)
        rad->dr = stergereNode(rad->dr, cheie);
    else
    {
        ///0 sau 1 copil
        if(rad->st == nullptr || rad->dr == nullptr)
        {
            Node <K, V>* temp = rad->st ? rad->st : rad->dr;

            ///0 copii
            if(temp == nullptr)
            {
                temp = rad;
                rad = nullptr;
            }
            else                                             ///1 copil
                *rad = *temp;
            delete temp;
        }
        else                                                ///2 copii
        {
            Node <K, V>* temp = valMin(rad->dr);
            rad->key   = temp->key;
            rad->dr = stergereNode(rad->dr, temp->key);
        }
    }


    if(rad == nullptr)                           ///daca arborele avea doar 1 nod
        return rad;

    rad->updateHeight();

    int balans = rad->retBalans(rad);

    if(balans > 1 && rad->st->retBalans(rad->st) >= 0)      ///stanga stanga
        return rotireDreapta(rad);


    if(balans > 1 && rad->st->retBalans(rad->st) < 0)       ///stanga dreapta
    {
        rad->st = rotireStanga(rad->st);
        return rotireDreapta(rad);
    }

    if(balans < -1 && rad->dr->retBalans(rad->dr) <= 0)    ///dreapta dreapta
        return rotireStanga(rad);

    if(balans < -1 && rad->dr->retBalans(rad->dr) > 0)    ///dreapta stanga
    {
        rad->dr = rotireDreapta(rad->dr);
        return rotireStanga(rad);
    }

    return rad;
}

                                                 ///stergere intreg dictionar

template <class K, class V>
void Dictionar <K, V>::deleteDictionar(Node <K, V>* node)
{
    if(node == nullptr)
    {
        return;
    }

    deleteDictionar(node->st);
    deleteDictionar(node->dr);

    delete node;
}

template <class K, class V>
void Dictionar <K, V>::stergereTotala()
{
    deleteDictionar(rad);
    size = 0;
    rad = nullptr;
    cout<<"Dictionarul a fost sters"<<endl;
}

template <class K, class V>
Dictionar<K, V>& Dictionar<K, V>::operator= (Dictionar <K, V> dtr)   ///pentru a se forma un nou dictionar
{
    std::swap(this->rad, dtr.rad);                                   ///pe baza celui deja existent
    std::swap(this->size, dtr.size);


    return *this;
}


template <class K, class V>
V Dictionar<K, V>::operator[] (const K index)     ///supraincarcarea operatorului []
{
    V aux = V();

    prOcheie(rad, aux, index);

    return aux;
}

template <class K, class V>
std::ostream & operator << (std::ostream &out, Dictionar <K, V> &dct)
{
    out <<  "Numar curent de elemente ale dictionarului: " << dct.numarElem() << endl << std::endl; ///pentru afisarea configuratiei existente
    if(dct.numarElem())                                                          ///in dicitonar
    {
        dct.afis();
    }
    return out;
}

void testare()
{
    Dictionar <int, int> D;


    D.adaugareDictionar(1, 143);
    cout << D;                       ///operatorul << supraincarcat

    int valoare;
    assert(D.cautCheie(1, valoare));   ///cautare dupa cheie



    D.adaugareDictionar(44, 213);
    D.adaugareDictionar(87, 412);
    D.adaugareDictionar(6, 5);

    cout << D;                       ///operatorul << supraincarcat

    int x = D[44];
                                    ///operatorul [] supraincarcat
    cout << x << " "<<endl<<endl;

    D.adaugareDictionar(5, 7);
    cout << D;
    D.stergereCheie(5);              ///stergere dupa cheie
    cout << D;
    assert(D.cautCheie(5, valoare));
    D.stergereTotala();              ///stergerea completa a dictionarului
    cout << D;
    D.adaugareDictionar(23, 24);
    Dictionar <int, int> D2;

    D2 = D;                          ///operatorul de atribuire supraincarcat
    cout << D2;
}






