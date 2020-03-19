#include <fstream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

class NFA
{
private:
    int n, m, q0, k, l, stari_finale[100];
    char alfabet[1000];
    vector<int> tranzitie[100][256];

public:
    NFA();
    bool evaluate(char word[]);
};

NFA::NFA()
{
    ifstream f("automat.in");

    int i, x, y;
    char c;
    vector<int> intra_vid[100];
    vector<pair<int, int> > intra_nevid[100];
    queue<int> q;

    f >> n;
    f >> m;
    for(i = 0; i < m; i++)
        f >> alfabet[i];
    f >> q0;
    f >> k;
    for(i = 0; i < k; i++)
        f >> stari_finale[i];
    f >> l;
    for(i = 0; i < l; i++)
    {
        f >> x >> c >> y;
        tranzitie[x][int(c)].push_back(y);
        if(c == '$')
            intra_vid[y].push_back(x);      /// lista cu starile care intra cu '$' in y
        else
            intra_nevid[y].emplace_back(x, int(c));    /// lista cu starile care intra cu litere in y
    }

    for(x = 0; x < n; x++)
        if(!intra_vid[x].empty())   /// daca avem stari care intra cu '$' in y
        {
            while(!q.empty())
                q.pop();
            for(i = 0; i < intra_vid[x].size(); i++)
                q.push(intra_vid[x][i]);    /// avem o coada in care adugam toate starile y, din care putem ajunge la x doar cu '$'
            while(!q.empty())
            {
                y = q.front();
                q.pop();
                for(i = 0; i < intra_nevid[y].size(); i++)
                    tranzitie[intra_nevid[y][i].first][intra_nevid[y][i].second].push_back(x);
                /// daca pentru o stare y din coada avem tranzitia "z c y" si c != '$', atunci adaugam tranzitia "z c x"

                for(i = 0; i < intra_vid[y].size(); i++)
                    q.push(intra_vid[y][i]);
                /// adaugam in coada toate starile care ajung in y cu '$' si continuam procesul
            }
        }

    f.close();
}

bool NFA::evaluate(char word[])
{
    int i, j, p, lg;
    vector<int> noduri[1001];

    lg = strlen(word);
    noduri[0].clear();
    noduri[0].push_back(q0);    ///noduri[i] = lista cu starile in care am ajuns folosind primele i caractere din cuvant
    for(i = 0; i < lg; i++)
    {
        noduri[i + 1].clear();
        for (j = 0; j < noduri[i].size(); j++)
            for (p = 0; p < tranzitie[noduri[i][j]][int(word[i])].size(); p++)
                noduri[i + 1].push_back(tranzitie[noduri[i][j]][int(word[i])][p]);
        ///noduri[i + 1] = lista cu starile in care putem ajunge din starile curente cu caracterul word[i]
    }

    for(i = 0; i < noduri[lg].size(); i++)
        for(j = 0; j < k; j++)
            if(stari_finale[j] == noduri[lg][i])
                return true;    ///  daca vreuna dintre starile in care am ajuns dupa ce am folosit toate caracterele este stare finala,
                                ///cuvantul este acceptat
    return false;
}

int main()
{
    ifstream g("cuvinte.in");
    ofstream h("evaluare.out");

    NFA aut;
    int nrcuv, i;
    char cuv[1000];

    g >> nrcuv;
    g.get();
    for(i = 0; i < nrcuv; i++)
    {
        g.getline(cuv, 1000);
        if(aut.evaluate(cuv))
            h << "Cuvantul '" << cuv << "' este acceptat de automat\n";
        else
            h << "Cuvantul '" << cuv << "' nu este acceptat de automat\n";
    }

    g.close();
    h.close();

    return 0;
}
