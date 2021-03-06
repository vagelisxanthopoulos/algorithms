#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

/*

Η ιδέα είναι η εξής: 

Υποθέτουμε (εκτός από την περίπτωση όπου έχουμε ένα σπίτι με Κ κατοικους) ότι
το αποτέλεσμα αποτελειται από δύο υποπίνακες χωρίς επικάλυψη. 

Αυτό δεν μας περιορίζει κάπως γιατι ακόμα και συνεχόμενος υποπινακας να ειναι 
το αποτέλεσμα, μπορούμε να πούμε οτι αποτελειται απο δύο διπλανους.

Για κάθε πιθανή αρχή, λοιπόν, του δεύτερου υποπίνακα βρίσκουμε, για κάθε πιθανό τέλος,
τον βέλτιστο πρώτο υποπίνακα, ώστε να βγαίνει συνολικό άθροισμα Κ με το ελάχιστο εφικτό μήκος. 

Εάν ο πρώτος καταλήξει να τελείωνει ακριβως πριν την αρχή του δεύτερου δεν μας πειράζει.

Επομένως κάνουμε τα ακόλουθα βήματα:

Δοκιμάζουμε ως αρχές του δεύτερου υποπίνακα όλα τα στοιχεία i με τη σειρά από το πρώτο
μέχρι το τελευταίο.

Για κάθε i δοκιμάζουμε όλα τα πιθανά τέλη. 

Για κάθε πιθανό τέλος του i παίρνουμε το άθροισμα sum από το i μέχρι εκεί.
Και μετά βρίσκουμε στον υποπίνακα [0, i-1] την ελάχιστη σε μήκος εμφάνιση
του αθροίσματος K-sum (αν υπάρχει και αν το sum<K). 

Προσθέτουμε τα μήκη των δυο υποπινάκων που έχουμε και βρίσκουμε υποψήφιο αποτέλεσμα.

Η διάσχιση αυτή (όλες οι πιθανές αρχές-τελη του δεύτερου) ειναι O(n^2).

Το ερώτημα είναι πώς βρίσκουμε κάθε φορα στον υποπίνακα [0, i-1] την βέλτιστη
εμφάνιση του K-sum.

Για να το κάνουμε αυτό έχουμε έναν hash πινακα που κρατά την ελάχιστου μήκους 
εμφάνιση κάθε αθροίσματος από 1 μεχρι Κ-1 στον υποπίνακα [0, i-1].

Σε κάθε αύξηση του i, ο πίνακας επικαιροποιείται με τα νέα αθροίσματα που προκύπτουν
από το νέο στοιχείο που "αφήνεται αριστερα" με την αυξηση του i.

Συνολικά για την διατήρηση αυτού του πίνακα, αφού υπολογίζουμε ουσιαστικα όλα τα 
αθροίσματα ακριβως μία φορά σε Ο(1), χρειαζόμαστε O(n^2) βήματα.

Επίσης η μνήμη που χρειαζόμαστε εκτός από την είσοδο είναι μεγέθους O(Κ).

*/

struct myinput
{
    int n;  //αριθμός σπιτιών
    int k;  //αριθμός κατοίκων που θέλουμε να εξυπηρετήσουμε
    int* people_per_house;
};

//συνάρτηση για διάβασμα input
myinput readinput ()
{
    myinput in;
    cin >> in.n;
    cin >> in.k;
    in.people_per_house = new int[in.n]; //διάβασμα γραμμής με κατοίκους ανα σπίτια: O(n)
    for (int i = 0; i < in.n; i++) cin >> in.people_per_house[i];
    return in;
};


int main()
{
    myinput input = readinput(); //διαβασμα εισοδου

    int answer = -1; //αρχικοποίηση 

    int newsum, currsum;   //βοηθητικες μεταβλητες
    int candidate;  
    int hash[input.k]; //αρχικοποίηση με μηδενικά (invalid τιμες δηλαδη)
    memset(hash, 0, (input.k)*sizeof(int));

    //εναρξη ουσιαστικου κομματιού
    for (int i = 0; i<input.n; i++) //i ειναι η αρχη του δευτερου (δεξιου) υποπινακα που δοκιμαζουμε
    {
        newsum = 0;   
        for (int c=i-1; c>=0; c--)   //επικαιροποιούμε τον hash με όλα τα νέα αθροισματα
        {                            //που μας δίνει το καινουριο στοιχείο που "αφέθηκε αριστερα" με την αυξηση του i
            if (c<i-1) newsum = newsum + input.people_per_house[c]; 
            else newsum = input.people_per_house[i-1];
            if (newsum < input.k)
            {
                if ((hash[newsum] == 0) || ((i-c)<hash[newsum])) hash[newsum] = i - c;
            }
           else break;  //αν είναι ίσο με Κ δεν μας νοιαζει γιατι θα το εχουμε τσεκαρει πριν
        }
        currsum = 0;               //αρχικοποίηση μεταβλητης που κραταει αθροισμα υποψηφιου δεξιου υποπίνακα
        for (int j = i; j<input.n; j++) //j είναι το τελος του δευτερου υποπινακα που δοκιμάζουμε
        {
            currsum+=input.people_per_house[j];
            if (currsum > input.k) break; //αν ξεπεράσαμε το k δεν υπάρχουν άλλες δυνατότητες για αυτήν την αρχη
                                          //δεύτερου υποπίνακα
            if (currsum == input.k) //αν βρήκαμε Κ το συγκρίνουμε με το ηδη βέλτιστο μηκος
            {      
                if (answer == -1) answer = j-i+1;
                else answer = min(answer, (j-i+1)); 
            }
            else if (hash[input.k - currsum]!=0)  //αν υπάρχει το Κ-currsum 
            {                                     //προσθετουμε τα μήκη και συγκρίνουμε τρεχων ελαχιστο μηκος
                candidate = hash[input.k - currsum] + j-i+1;
                if (answer == -1) answer = candidate;
                else answer = min(answer, candidate);
            }
        }
    }
    cout<<answer<<endl;
}