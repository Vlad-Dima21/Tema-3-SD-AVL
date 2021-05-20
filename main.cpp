#include <bits/stdc++.h>

using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

struct Nod
{
    Nod *fiuStanga;
    Nod *fiuDreapta;
    int valoare;
    int inaltime;

    Nod(int valoare)
    {
        this -> valoare = valoare;
        this -> inaltime = 1; ///orice frunza are inaltimea 1;
        this -> fiuDreapta = NULL;
        this -> fiuStanga = NULL;
    }
//    ~Nod()
//    {
//        if (this -> fiuStanga != NULL)
//            delete this -> fiuStanga;
//        if (this -> fiuDreapta != NULL)
//            delete this -> fiuDreapta;
//    }
};


int getInaltime(Nod *n) ///functie cu utilitatea ca returneaza inaltimea 0 pentru 'fiii' frunzelor
{
    if (n == NULL)
        return 0;
    return n->inaltime;
}

Nod* RotatieStanga(Nod *n1)
{
//    1                             3
//   / \                           / \
//  2   3           ---->         1   5
//     / \                       / \
//    4   5                     2   4


    Nod *n3 = n1->fiuDreapta;
    Nod *n4 = n3->fiuStanga;

    n3->fiuStanga = n1;
    n1->fiuDreapta = n4;

    n1->inaltime = max(getInaltime(n1->fiuStanga), getInaltime(n1->fiuDreapta)) + 1;
    n3->inaltime = max(getInaltime(n3->fiuStanga), getInaltime(n3->fiuDreapta)) + 1;

    return n3; ///noua radacina a subarborelui

}

Nod* RotatieDreapta(Nod *n1)
{
//            1                          2
//           / \                        / \
//          2   3         ---->        4   1
//         / \                            / \
//        4   5                          5   3

    Nod *n2 = n1->fiuStanga;
    Nod *n5 = n2->fiuDreapta;

    n2->fiuDreapta = n1;
    n1->fiuStanga = n5;

    n1->inaltime = max(getInaltime(n1->fiuStanga), getInaltime(n1->fiuDreapta)) + 1;
    n2->inaltime = max(getInaltime(n2->fiuStanga), getInaltime(n2->fiuDreapta)) + 1;

    return n2;

}

Nod* inserare(Nod *n,int valoare)
{

    if (n == NULL)  ///am ajuns la o frunza
    {
        Nod *nodNou = new Nod(valoare);
        return nodNou;
    }

    if (valoare == n->valoare)
        return n;   ///nu adaugam decat valori distincte

    if (valoare < n->valoare)
        n->fiuStanga =  inserare(n->fiuStanga,valoare);

    if (valoare > n->valoare)
        n->fiuDreapta = inserare(n->fiuDreapta, valoare);

    n->inaltime = max(getInaltime(n->fiuStanga), getInaltime(n->fiuDreapta)) + 1;

    int difInaltimi = getInaltime(n->fiuStanga) - getInaltime(n->fiuDreapta);

    if (difInaltimi > 1 && valoare < n->fiuStanga->valoare)    ///cazul stanga stanga
        n = RotatieDreapta(n);

    else if (difInaltimi > 1 && valoare > n->fiuStanga->valoare)    ///cazul stanga dreapta
    {
        n->fiuStanga = RotatieStanga(n->fiuStanga);
        n = RotatieDreapta(n);
    }
    else if (difInaltimi < -1 && valoare > n->fiuDreapta->valoare)   ///cazul dreapta dreapta
        n = RotatieStanga(n);

    else if (difInaltimi < -1 && valoare < n->fiuDreapta->valoare)   ///cazul dreapta stanga
    {
        n->fiuDreapta = RotatieDreapta(n->fiuDreapta);
        n = RotatieStanga(n);
    }
    return n;
}

Nod* stergere(Nod *n,int valoare)
{
    if (n == NULL)
        return n;

    else if (valoare < n->valoare)
        n->fiuStanga = stergere(n->fiuStanga,valoare);
    else if (valoare > n->valoare)
        n -> fiuDreapta = stergere(n->fiuDreapta, valoare);
    else /// if (valoare == n->valoare)
    {

        if (n->fiuStanga == NULL && n->fiuDreapta == NULL)
            n = NULL;

        else if (n->fiuStanga == NULL)
            n = n->fiuDreapta;

        else if (n->fiuDreapta == NULL)
            n = n->fiuStanga;

        else ///are ambii fii, inlocuim radacina acestui subarbore cu cel mai mic nod din subarborele drept
        {
            Nod *succesor = n->fiuDreapta;

            while (succesor->fiuStanga != NULL)
                succesor = succesor->fiuStanga;

            n->valoare = succesor->valoare;
            n ->fiuDreapta = stergere(n->fiuDreapta, succesor->valoare);    ///stergem nodul cu care l-am inlocuit pe cel sters
        }
    }

    if (n == NULL)  ///reechilibram arborele, daca mai are elemente
        return n;

    else
    {

        n->inaltime = max(getInaltime(n->fiuStanga), getInaltime(n->fiuDreapta)) + 1;


        ///la fel ca la inserare
        int difInaltimi = getInaltime(n->fiuStanga) - getInaltime(n->fiuDreapta);

        if (difInaltimi > 1 && valoare < n->fiuStanga->valoare)    ///cazul stanga stanga
            n = RotatieDreapta(n);

        else if (difInaltimi > 1 && valoare > n->fiuStanga->valoare)    ///cazul stanga dreapta
        {
            n->fiuStanga = RotatieStanga(n->fiuStanga);
            n = RotatieDreapta(n);
        }
        else if (difInaltimi < -1 && valoare > n->fiuDreapta->valoare)   ///cazul dreapta dreapta
            n = RotatieStanga(n);

        else if (difInaltimi < -1 && valoare < n->fiuDreapta->valoare)   ///cazul dreapta stanga
        {
            n->fiuDreapta = RotatieDreapta(n->fiuDreapta);
            n = RotatieStanga(n);
        }
    }
    return n;

}

void inordine(Nod *n)
{
    if (n != NULL)
    {
        inordine(n->fiuStanga);
        cout<<n->valoare<<' ';
        inordine(n->fiuDreapta);
    }
}


void AfisareSortate(Nod *n, int minim, int maxim)
{
    if (n != NULL)
    {
        if (n->valoare >= minim && n->valoare <= maxim)
        {
            AfisareSortate(n->fiuStanga, minim, maxim);
            fout<<n->valoare<<' ';
            AfisareSortate(n->fiuDreapta, minim, maxim);
        }
    }
}

bool cautareValoare(Nod *n,int valoare)
{
    bool gasit = 0;
    while (!gasit && n)
    {
        if (n->valoare == valoare)
            gasit = 1;
        else if (n->valoare > valoare)
            n = n->fiuStanga;
        else if (n->valoare < valoare)
            n = n->fiuDreapta;
    }
    return gasit;
}

int Predecesor(Nod *n,int valoare)
{
    if (!n)
        return -1;

    int pred = -1;

    while (n)
    {
        if (valoare > n->valoare)
        {
            pred = n->valoare;
            n = n->fiuDreapta;
        }
        else if (valoare < n->valoare)
            n = n->fiuStanga;

        else if (n -> fiuStanga)     ///am ajuns la nodul pentru care cautam predecesor
        {
            Nod *p = n->fiuStanga;
            while (p->fiuDreapta)
                p = p->fiuDreapta;

            return p->valoare;
        }
        else
            n = NULL;
    }
    return pred;
}

int Succesor(Nod *n, int valoare)
{
    if (!n)
        return -1;

    int succ = -1;

    while (n)
    {
        if (n->valoare > valoare)
        {
            succ = n->valoare;
            n = n->fiuStanga;
        }
        else if (n->valoare < valoare)
            n = n->fiuDreapta;
        else if (n->fiuDreapta)
        {
            Nod *s = n->fiuDreapta;

            while (s->fiuStanga)
                s = s->fiuStanga;

            return s->valoare;
        }
        else
            n = NULL;
    }
    return succ;

}

int main()
{
    Nod *n = NULL;

    int nrOperatii, optiune;
    fin>>nrOperatii;

    while (nrOperatii--)
    {
//        inordine(n);
//        cout<<'\n';
        fin>>optiune;
        switch(optiune)
        {
            case 1:
                {
                    int val;
                    fin>>val;
                    n = inserare(n,val);
                    break;
                }
            case 2:
                {
                    int val;
                    fin>>val;
                    n = stergere(n,val);
                    break;
                }
            case 3:
                {
                    int val;
                    fin>>val;
                    fout<<cautareValoare(n,val)<<'\n';
                    break;
                }
            case 4:
                {
                    int val;
                    fin>>val;
                    fout<<Predecesor(n,val)<<'\n';
                    break;
                }
            case 5:
                {
                    int val;
                    fin>>val;
                    fout<<Succesor(n,val)<<'\n';
                    break;
                }
            case 6:
                {
                    int minim,maxim;
                    fin>>minim>>maxim;
                    AfisareSortate(n,minim,maxim);
                    fout<<'\n';
                    break;
                }
        }
    }

}
