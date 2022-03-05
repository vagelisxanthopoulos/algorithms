#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

/*

Έστω αγοράζουμε κ σετ από τον 1, λ σετ από τον 2 και αναγκαστικά Ν-κ-λ σετ από τον 3. 

Αν βρούμε το ελάχιστο κόστος αυτής της μοιρασιάς και το συγκρίνουμε με τα ελάχιστα κόστη
όλων των αλλων δυνατων μοιρασιών, τότε βρίσκουμε το ελάχιστο κόστος αγοράς Ν σετ.

Αυτές οι μοιρασιές είναι O(N^2). 

Στις μοιρασιές υπάρχουν επικαλύψεις. Π.χ. αν Ν=10 υπάρχουν παραπάνω από μία μοιρασιές όπου αγοράζουμε 2 σετ από τον 1.

Οπότε μας συμφέρει στην αρχη να υπολογίσουμε για κάθε έμπορο ξεχωριστά το ελάχιστο κόστος για σετ από 0 μεχρι Ν. 
Και μετά να γεμίσουμε το πινακάκι με τις μοιρασιές χωρίς να υπολογίζουμε πράγματα δυο φορές.

Για να βρούμε το ελαχιστο κόστος αγοράς δεδομένων σετ από έναν έμπορο, ακολουθούμε τον αλγόριθμο του σακιδίου (τροποποιημενο)
για κάθε κομμάτι του εξοπλισμού (Α,Β,C) ξεχωριστά (αντιστοιχιση: σετς->χωρητικότητα σακιδίου, προσφορες->αντικείμενα, τιμή->κέρδος).

Oπότε έχουμε γεμισμα πίνακα O(N^2) και 9 αλγόριθμους σακιδίου (τροποποιημένοι) O(N*M), άρα πολυπλοκότητα O(N(N+M)).

*/

struct offer
{
    int quantity;
    int price;
};

/*

αυτή η συνάρτηση παίρνει τις προσφορες πχ 1Α και με τροποποιημένο αλγοριθμο knapsak 
βρίσκει τις φθηνότερες επιλογες για 1,2,...,maxsets αντικείμενα

έχουμε state space (o,s) οπου ο είναι οι διαθέσιμες προσφορές, s είναι τα επιθυμητα αντικείμενα 
και για αυτα τα (o,s) έχουμε το ελαχιστο κόστος για τουλάχιστον s αντικείμενα (ή τα περισσότερα δυνατά)

για να βρούμε το ελάχιστο κόστος (ο,s) κάνουμε το εξής: 

1) Αν για (o-1,s) συν τα αντικείμενα της ο-στης προσφοράς προκύπτουν μικροτερα ή ισα με s αντικείμενα, τότε παίρνουμε την ο-οστή προσφορά

2) Αλλιώς κάνουμε σύγκριση και κρατάμε την μικρότερη τιμή από: την τιμή του (o-1,s) και την τιμή του (o-1,s-q)+p
όπου q είναι η ποσότητα της ο-οστής προσφοράς και p η τιμή της. 

Δηλαδή είτε δεν παίρνουμε την νεα προσφορά είτε την παίρνουμε και την προσθέτουμε στην τιμή του μικρότερου δυνατού s, δηλαδή το s-q.

Προσοχή, για να είναι υποψήφιο το (ο-1,s) θα πρέπει προφανώς να έχει τουλάχιστον s αντικείμενα. 

Επίσης στην συγκριση μπορουμε να ειμαστε σιγουροι ότι το (o-1,s-q) έχει τουλάχιστον s-q στοιχεία. Έστω ότι δεν έχει.
Τότε με ο-1 προσφορες έχουμε λιγοτερα απο s-q στοιχεία. Αρα μαζι με την o-οστη έχουμε λιγότερα απο s. Άρα άτοπο
γιατί τότε θα μπαίναμε στην πρωτη περίπτωση.

*/
void cheapestpick(int offersize, offer* offers, int maxsets, int* answer) //maxsets πρεπει να δοθει ως ν+1 ωστε οι δεικτες να φτανουν μεχρι ν
{                                                                         //offersize πρεπει να δοθει επισης ως μ+1 ωστε οι δεικτες να φτανουν μεχρι μ
    int** dp = new int*[offersize];
    int** reachedSets = new int*[offersize]; //κρατάμε το πόσα αντικείμενα έχουμε (για να ελέγχουμε αν είναι μικροτερα του s)
    for (int i=0; i<offersize; i++)
    {
        dp[i] = new int[maxsets];
        reachedSets[i] = new int[maxsets];
    }
    int candidate1, candidate2;

    //εκτελεση dp 

    for (int i=0; i<offersize; i++)
    {
        for (int j=0; j<maxsets; j++)
        {
            if (j==0 || i==0)  //για μηδεν sets ή μηδέν προσφορές η ελάχιστη τιμή είναι 0
            {
                dp[i][j] = 0;
                reachedSets[i][j] = 0; //έχουμε 0 sets
            }

            //ο δεικτης στον offers ειναι πάντα i-1 διότι πχ για i=2 διαθεσιμες προσφορες παιρνουμε, ως καινουρια, την προσφορα στην θεση 1
            
            //με όλες τις προσφορές (μαζι με την καινούρια) φτάνουμε οριακα ή δεν φτάνουμε τα s αντικείμενα
            else if (reachedSets[i-1][j] + offers[i-1].quantity <= j) 
            {
                dp[i][j] = dp[i-1][j] + offers[i-1].price; //παίρνουμε την νέα προσφορά 
                reachedSets[i][j] = reachedSets[i-1][j] + offers[i-1].quantity;
            }
            else //φτάνουν και περισσεύουν οι προσφορες μας για τα επιθυμητα sets
            {
                //εύρεση υποψηφίων σύγκρισης

                if (reachedSets[i-1][j] >= j) candidate1 = dp[i-1][j]; //υποψηφιοτητα μονο αν εχει επαρκη αντικειμενα
                else candidate1 = -1;

                if (j-offers[i-1].quantity < 0) candidate2 = offers[i-1].price;
                else candidate2 = offers[i-1].price + dp[i-1][j-offers[i-1].quantity];

                if (candidate1 <= candidate2 && (candidate1 != -1))
                {
                    dp[i][j] = candidate1;
                    reachedSets[i][j] = reachedSets[i-1][j];
                }
                else
                {
                    dp[i][j] = candidate2;
                    reachedSets[i][j] = reachedSets[i-1][j-offers[i-1].quantity] + offers[i-1].quantity;
                }
            }
        }
    }
    for (int i=0; i<maxsets; i++)
    {
        if (reachedSets[offersize - 1][i] >= i) answer[i] = dp[offersize - 1][i]; //κοιταμε τελευταια σειρα
        else answer[i] = -1; //σημαίνει ότι δεν μπορούμε να πάρουμε i αντικειμενα απο αυτον τον έμπορο
    }
    for (int i=0; i<offersize; i++) //εξοικονομηση μνημης
    {
        delete dp[i]; 
        delete reachedSets[i];
    }
    delete dp;
    delete reachedSets;
};

/*

αυτή η συνάρτηση παίρνει το αποτέλεσμα της προηγούμενης και βρισκει για έναν έμπορο
τις φθηνότερες τιμες με τις οποίες μπορείς να αγοράσεις 1...Ν sets

*/
void bestPricesMerchant(int offersize1, int offersize2, int offersize3, offer* offers1, offer* offers2, offer* offers3, int sets, int* answer)
{
    int* best1 = new int[sets]; //kalyteres times ana antikeimeno
    int* best2 = new int[sets];
    int* best3 = new int[sets];

    cheapestpick(offersize1, offers1, sets, best1); //gemisma apo proigoumeni sinartisi
    cheapestpick(offersize2, offers2, sets, best2);
    cheapestpick(offersize3, offers3, sets, best3);

    for (int i=0; i<sets; i++)
    {
        if (best1[i]==-1 || best2[i]==-1 || best3[i]==-1) answer[i] = -1;
        else answer[i] = best1[i] + best2[i] + best3[i];
    }

    delete best1;
    delete best2;
    delete best3;
}

int main()
{
    int sets, offersNum;
    offer** offers = new offer*[9];  //φτιαχνουμε 9 πινακες με προσφορες: 1A, 1B, 1C, 2A, 2B, 2C, 3A, 3B, 3C
    int* sizes = new int[9]; //πινακας με μήκη των προηγούμενων 9 πινάκων
    for (int i=0; i<9; i++) sizes[i] = 0; // αρχικοποίηση
    
    //διαβασμα δεδομενων και αποθηκευση τους

    cin >> sets;
    cin >> offersNum;

    for (int i=0; i<9; i++) offers[i] = new offer[offersNum]; //αρχικοποίηση πινάκων

    char tempchar; //βοηθητικες μεταβλητες για ξεχωρισμα προσφορων και αποθήκευση τους στους 9 πινακες
    int tempnum;
    int index; 

    for (int i=0; i<offersNum; i++)
    {
        cin >> tempnum;
        index = 3*(tempnum-1);
        cin >> tempchar;
        if (tempchar == 'A') index += 0;
        else if (tempchar == 'B') index += 1;
        else index += 2; //βρηκαμε σε ποιον απο τους 9 πινακες ανηκει η προσφορα

        offer tostore;  //παιρνουμε ποσοτητα και τιμη
        cin >> tempnum;
        tostore.quantity = tempnum;
        cin >> tempnum;
        tostore.price = tempnum;

        offers[index][sizes[index]] = tostore; //αποθηκευουμε και επικαιροποιουμε size
        sizes[index]++;
    }

    //εύρεση ελάχιστου κόστους αγορας 1..Ν σετ από κάθε έμπορο
    
    int* bestFirst = new int[sets+1];
    bestPricesMerchant(sizes[0]+1, sizes[1]+1, sizes[2]+1, offers[0], offers[1], offers[2], sets+1, bestFirst);

    int* bestSecond = new int[sets+1];
    bestPricesMerchant(sizes[3]+1, sizes[4]+1, sizes[5]+1, offers[3], offers[4], offers[5], sets+1, bestSecond);

    int* bestThird = new int[sets+1];
    bestPricesMerchant(sizes[6]+1, sizes[7]+1, sizes[8]+1, offers[6], offers[7], offers[8], sets+1, bestThird);

    //εύρεση ελάχιστων τιμών όλων των δυνατών συνδυασμων και από αυτές εύρεση απάντησης

    int min = -1;

    int currBestPrice, third;

    bool first = true; //σηματοδοτει πρώτη valid συνολικη τιμη

    for (int fi=0; fi<=sets; fi++)
    {
        for (int sec=0; sec<=sets-fi; sec++)
        {
            third = sets - fi - sec;

            //αν κάποιο ειναι -1 προχωραμε
            if ((bestFirst[fi]==-1) || (bestSecond[sec]==-1) || (bestThird[third]==-1)) continue;

            currBestPrice = bestFirst[fi] + bestSecond[sec] + bestThird[third];

            if (first) min = currBestPrice;    //πρωτη επανάληψη, αρχικοποιούμε min
            else if (currBestPrice < min) min = currBestPrice;

            first = false;
        }
    }

    cout<<min<<endl;
}
