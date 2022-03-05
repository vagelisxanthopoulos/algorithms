#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/* 

Η ιδέα ειναι οτι αποθηκεύουμε όλους τους υποπινακες ανα άθροισμα και με αυξουσα σειρα αρχής-τελους

Μετα ελέγχουμε όλους τους υποπινακες με αθροισμα Κ.

Μετα ελέγχουμε τα βέλτιστα απο αποψη μηκους ζευραγια λ, Κ-λ.

Με ορισμενες βελτιστοποιήσεις και preprocessing αυτη η διαδικασια κοστίζει O(n^2).

Ακολουθεί αναλυτικότερη εξήγηση πριν απο την υλοποίηση κάθε συναρτησης.

*/

struct subarr  //για κάθε subarray αποθηκεύω αρχή, τέλος, άθροισμα στοιχείων
{
	int sum;
    int start;
    int end;
};


struct myinput
{
    int n;  //αριθμός σπιτιών
    int k;  //αριθμός κατοίκων που θέλουμε να εξυπηρετήσουμε
    int* people_per_house;
};


myinput readinput ()
{
    myinput in;
    cin >> in.n;
    cin >> in.k;
    in.people_per_house = new int[in.n]; //διάβασμα γραμμής με κατοίκους ανα σπίτια: O(n)
    for (int i = 0; i < in.n; i++) cin >> in.people_per_house[i];
    return in;
};


//εύρεση prefix sums (θα βοηθήσει για αργότερα): O(n)
int * prefixsums(int * people_per_house, int n) 
{
    int * psums = new int[n];
    psums[0] = people_per_house[0];
    for (int i = 1; i < n; i++) psums[i] = psums[i-1] + people_per_house[i];
    return psums;
};

/* 
Δεν χρησιμοποιούμε vectors γιατί μεγαλώνουν ποιο πολυ από όσο
χρειαζόμαστε και θέλουμε να κάνουμε οικονομία στην μνήμη
*/

/*
Εύρεση για όλους τους υποπίνακες του people_per_house: αρχή, τέλος, άθροισμα.
Φτιάχνουμε πίνακα από πίνακες όπου ο καθε εσωτερικός πίνακας i, περιέχει αυτά 
τα δεδομένα για όλους τους υποπίνακες με αρχή i από τον μικρότερο
προς τον μεγαλύτερο.

Για κάθε υποπίνακα τα βρίσκουμε αυτά που θέλουμς σε σταθερό χρόνο
διότι έχουμε τα prefix sums.
Όλοι οι υποπίνακες αν μετρήσουμε και τα στοιχεία μόνα τους
ειναι ν(ν+1), αρα O(n^2).
*/
void AllSubarrayContructor(int * psums, int n, subarr** res) //παίρνει ως ορισμα τον πινακα που θα φτιάξει
{
    int size;
    for (int i = 0; i < n; i++)
    {
        size = n - i;       //πληθος υποπινάκων με αρχη το i
        res[i] = new subarr[size];
        for (int j = 0; j < size; j++)
        {
            res[i][j].start = i;
            res[i][j].end = i + j;
            if (i > 0) res[i][j].sum = psums[i+j] - psums[i-1];
            else res[i][j].sum = psums[i+j];
        }
    }
};


/*
Στη συνέχεια φτιάχνουμε πίνακα πινάκων hash μεγέθους Κ+1.
Κάθε πίνακας hash[i] περιλαμβάνει τους υποπίνακες του people_per_house
που έχουν αθροισμα i. 

Προφανώς τα αθροίσματα πάνω από Κ δεν μας ενδιαφέρουν.

Έτσι οπως τον φτιαχνουμε κάθε εσωτερικος πινακας του hash περιεχει τους υποπινακες 
με το συγκεκριμενο αθροισμα σε αυξουσα σειρα αρχης (αρα και τελους), διότι τους 
προσθετουμε με την σειρα που τους συνανταμε στην πίνακα με τους original υποπινακες τον οποιον διατρεχουμε
με αυξουσα σειρα αρχης.

Επισης φτιάχνουμε βοηθητικό πινακα που μας λέει πόσοι υποπίνακες αντιστοιχούν σε κάθε άθροισμα.

Διατεχουμε τον αρχικό πίνακα των υποπινάκων 2 φορές άρα O(n^2)
*/

void HasherContructor(subarr** subs, int n, int k, subarr** hash, int* length) //δεχόμαστε ως όρισμα τους πίνακες που θα φτιαξουμε
{
    for (int i = 0; i < k + 1; i++) length[i] = 0;  //αρχικοποίηση πίνακα με μήκη των πινακων που αντιστοιχουν σε καθε αθροισμα
    int temp;
    for (int i = 0; i < n; i++) //βρίσκουμε πόσοι υποπίνακες αντιστοιχούν σε κάθε αθροισμα
    {
        for (int j = 0; j<(n - i); j++)
        {
            temp = subs[i][j].sum;
            if (temp <= k) length[temp]++;
        }
    }
    for (int i = 0; i < k + 1; i++)
    {
        if (length[i] > 0) hash[i] = new subarr[length[i]]; //φτιάχνουμε πίνακες μεγέθους όσα είναι τα αθροίσματα που θα βάλουμε
    }
    int help[k+1]; //πίνακας που θα μας βοηθήσει να γεμίσουμε τον hash ώστε να ξέρουμε σε ποια θεση έχουμε μεινει σε καθε υποπινακα
    for (int i = 0; i < k + 1; i++) help[i] = 0;  //αρχικοποίηση 

    for (int i = 0; i < n; i++) //γεμίζουμε τον hash
    {
        for (int j = 0; j < (n - i); j++)
        {
            temp = subs[i][j].sum;
            if (temp <= k)
            {
                hash[temp][help[temp]] = subs[i][j]; //προσθετουμε τον υποπινακα στον πινακα του αθροισματος
                help[temp]++;                       //που του αντιστοιχει, στην θεση που δειχνει ο help
            }
        }
    }
};

/*

Στη συνέχεια φτιάχνουμε δύο πινακες με πληροφοριες για τον hash. 

O ένας μας λέει για κάθε θέση καθε υποπινακα αθροισματος, ποιο είναι το ελαχιστο μηκος των επόμενων στοιχείων του υποπινακα.

Και ο αλλος, για κάθε θέση καθε υποπινακα αθροισματος, ποιο είναι το ελαχιστο μηκος των προηγούμενων στοιχείων του ύποπίνακα.

Πχ αν ο πρωτος λεγεται Α, τότε Α[i][j] μας δίνει για το αθροισμα i, το ελάχιστο μηκος υποπινακα που συνανταμε από την θεση j και μετά

Διατρέχουμε τον hash δύο φορές άρα O(n^2)

*/

void MinAfterConstructor(subarr** hash, int* length, int n, int k, int** minaf)
{
    int currLen;
    for (int i = 0; i < k + 1; i++)
    {
        for (int j = length[i] - 1; j >= 0; j--)
        {
            currLen = hash[i][j].end - hash[i][j].start + 1;
            if (j == length[i] - 1) minaf[i][j] = currLen;
            else minaf[i][j] = min(currLen, minaf[i][j+1]);
        }
    }
};

void MinBefConstructur(subarr** hash, int* length, int n, int k, int** minbef)
{
    int currLen;
    for (int i = 0; i < k + 1; i++)
    {
        for (int j = 0; j < length[i]; j++)
        {
            currLen = hash[i][j].end - hash[i][j].start + 1;
            if (j == 0) minbef[i][j] = currLen;
            else minbef[i][j] = min(currLen, minbef[i][j-1]);
        }
    }
};


/*

Τώρα απομένει να δούμε όλες τις δυάδες αθροισμάτων (εκτός από το Κ που το κοιταμε μονο του)
Δηλαδή κοιτάμε 1 με Κ-1, 2 με Κ-2 για να βρουμε τον εκαστοτε βελτιστο συνδυασμο (αν υπάρχει)

Εστω κοιταμε 1 με Κ-1. Αρα μας ενδιαφερουν οι υποπινακες 1 και Κ-1 του hash.
Τους διατρεχουμε μια απο την αρχη προς το τελος και μια απο το τελος προς την αρχη.

Ξερουμε ότι στους υποπινακες των αθροισματων συνανταμε τα αθροισματα με αυξουσα αρχη και τελος

Διασχιση απο αρχη προς τα τελος (εδω μας ενδιαφέρουν τα καλυτερα=με_μικροτερο_μηκος αθροισματα που το Κ-1 εμφανιζεται αργοτερα του 1): 

->Για το πρωτο στοιχείο α του υποπινακα με αθροισμα 1 (απο δω και στο εξης θα τον λεμε "1") 
  βρισκουμε το πρωτο στοιχειο β του υποπινακα με αθροισμα Κ-1 (απο δω και στο εξης θα τον λεμε Κ"-1") που ξεκιναει μετα το τελος του α.
->Τότε προσθετουμε μηκος(α)+μινιμουμ_μηκος_μετα(β) διότι ολα τα στοιχεια απο το β και μετα ειναι αποδεκτα για το α αφου ξεκινανε αργοτερα.
->Μετα παλι για το δευτερο στοιχειο του 1 βρισκουμε το πρωτο στοιχειο του Κ-1 που ειναι αποδεκτο και κανουμε παλι την ιδια προσθεση
->Εχουμε δηλαδη δυο μετρητες που τρεχουν. Ο ενας στο 1 και ο αλλος στο Κ-1. 
->Οσο δεν βρισκουμε αποδεκτο προχωραει ο δευτερος, οταν βρουμε προχωραει ο πρωτος.

Για την διασχιση απο τελος προς αρχη βρισκουμε τα αθροισματα οπου το Κ-1 εμφανιζεται πριν το 1:
Οπότε εχουμε δυο μετρητες που ξεκινανε απο το τελος. Για το τελευταιο στοιχειο του 1 βρισκουμε το τελευταιο του Κ-1 που ειναι πριν.
Κανουμε την προσθεση μηκος(α)+μινιμουμ_μηκος_πριν(β).
Οσο βρισκουμε αποδεκτο μειωνεται ο μετρητης του 1, οσο δεν βρισκουμε μειωνεται ο μετρητης του K-1.

Ετσι εχουμε βρει για το καθε αθροισμα λ το βελτιστο Κ-λ.

Αυτο δουλευει λογω της διαταξης η οποια μας λεει οτι αν ο Α βρισκεται πριν απο τον Β τοτε βρισκεται και απο τον Β+1.

Οποτε διατρεχουμε τα 1 και Κ-1 δυο φορες και για τα μινιμουμ_πριν και μινιμουμ_μετα θελουμε O(1) για να τα βρουμε γιατι τα εχουμε ετοιμα.

Οπότε διατρεχουμε όλα τα αθροισματα σε σταθερο χρονο, αρα O(n^2)

*/

/*
Αυτή η συνάρτηση ελέγχει μια δυαδα αθροισματων (πχ 2 και Κ-2)
*/

int CoupleComparator(subarr* first, subarr* second, int len1, int len2, int* minaf, int* minbef)
{
    int i = 0;
    int j = 0;
    int min = -1; //invalid, γιατι μπορεί να μην βρουμε τιποτα
    int candidate, temp;
    while((i < len1) && (j < len2))  //περιπτωσεις οπου το αθροισμα second βρισκεται μετά το first
    {
        if (first[i].end < second[j].start)
        {
            temp = first[i].end - first[i].start + 1;  //μηκος πρωτου
            candidate = temp + minaf[j];
            if ((min == -1) || (candidate < min)) min = candidate;
            i++;
        }
        else j++;
    }
    i = len1 - 1;
    j = len2 - 1;
    while((j > -1) && (i > -1))  //περιπτωσεις οπου το αθροισμα second βρισκεται πριν το first
    {
        if (first[i].start > second[j].end)
        {
            temp = first[i].end - first[i].start + 1;  //μηκος πρωτου
            candidate = temp + minbef[j];
            if ((min == -1) || (candidate < min)) min = candidate;
            i--;
        }
        else j--;
    }
    return min;
};


int main ()
{
    myinput input = readinput(); //διαβασμα εισοδου
    int * psums = prefixsums(input.people_per_house, input.n); //μερικα αθροισματα
    subarr** subs = new subarr*[input.n];   //αρχικοποιηση πινακα για να τον γεμισει η επομενη συναρητηση
    AllSubarrayContructor(psums, input.n, subs);
    subarr** hash = new subarr*[input.k + 1]; //αρχικοποιηση πινακων για να τους γεμισει η επομενη συναρητηση
    int* length = new int[input.k + 1]; //πίνακας που κρατάει πόσοι υποπίνακες αντιστοιχούν σε κάθε άθροισμα
    HasherContructor(subs, input.n, input.k, hash, length);
    delete subs;   //δεν το χρειαζόμαστε άλλο
    int** minaf = new int*[input.k + 1];  //αρχικοποίηση πινακων για τους γεμισουν οι αντιστοιχες συναρτησεις
    int** minbef = new int*[input.k + 1];
    for (int i = 0; i < input.k + 1; i++) //αρχικοποιουμε και τους υποπινακες συμφωνα με τα αντιστοιχα πληθη των στοιχειων τους
    {
        if (length[i] > 0) 
        {
            minaf[i] = new int[length[i]];
            minbef[i] = new int[length[i]];
        } 
    }
    MinAfterConstructor(hash, length, input.n, input.k, minaf);
    MinBefConstructur(hash, length, input.n, input.k, minbef);
    int min = -1;
    int temp;
    for (int i=0; i<length[input.k]; i++)   //ελέγχουμε τα συνεχομενα διαστήματα με αθροισμα Κ
    {
        temp = hash[input.k][i].end - hash[input.k][i].start + 1; //μηκος
        if ((min == -1) || (temp < min)) min = temp;
    }
    int candidate;
    for (int i = 1; i<=input.k/2; i++) //ελεγχουμε τα μη επικαλυπτομενα διαστηματα με αθροισμα κ για ολους τους πιθανους συνδυασμους
    {                                   //δηλαδη 1,Κ-1  2,Κ-2  κ.ο.κ.
        if ((length[i]>0) && (length[input.k-i]>0))
        {
            candidate = CoupleComparator(hash[i], hash[input.k-i], length[i], length[input.k-i], minaf[input.k-i], minbef[input.k-i]);
            if (min == -1) min = candidate;
            else if ((candidate > 0) && (candidate < min)) min = candidate;
        }
    }
    cout<<min<<endl;
}