#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

/*

Για εύρεση Μεγιστης Αυξουσας Υπακολουθιας κάνουμε τον αλγοριθμο (nlogn) της περυσινης διαλεξης 16 όπου 
καθως εκτελείται έχουμε για τρέχων i: το ελάχιστο τέλος για όλα τα δυνατα μήκη αύξουσων υπακολουθιων.

Το ιδιο για ευρεση Μεγιστης Φθινουσας Υπακολουθιας, καθώς εκτελείται
έχουμε για τρέχων i: το μέγιστο τέλος για όλα τα δυνατα μήκη φθίνουσων υπακολουθιων.

Γενικά για το πρόβλημα παρατηρουμε δυο πράγματα. Αν το διάστημα "πειραγματος" που ψάχνουμε είναι το [α,β]
τοτε μπορουμε χωρις να επεκτείνουμε το β μεχρι το τελος του πινακα (n) χωρις να αλλαξει κατι. Και αν το κάνουμε
αυτό, τότε μπορουμε να τερματίσουμε την αύξηση στο Κ, πάλι χωρις να αλλαξει κάτι.

Οπότε έτσι απλοποιείται το πρόβλημα και ψάχνουμε την βέλτιστη αρχή του διαστήματος, αφου το τέλος θα είναι
το τέλος του πίνακα και η αύξηση θα είναι Κ.

Οπότε κάνουμε το εξής:

Βρίσκουμε αρχικα την μέγιστη αύξουσα υπακολουθία χωρίς κανένα πείραγμα.

Μετά για κάθε i από n-1 μέχρι 0:
Εκτελουμε τον αλγοριρθμο ΜΑΥ απο την αρχη μέχρι την θέση i και από το τέλος μέχρι την θέση i, εκτελουμε τον αλγοριθμο ΜΦΥ 
με τα στοιχεία όμως να είναι αυξημενα κατα Κ. 

Δηλαδή σε κάθε επανάληψη γίνεται ένα βήμα στον αλγόριθμο ΜΦΥ από τέλος προς την αρχή και ξε-γίνεται ένα βήμα στον ΜΑΥ από την αρχή προς το τέλος.

(το undo στον ΜΑΥ θα το υλοποιήσουμε με καποιο είδος ιστορικού)

Για να βρούμε το μεγιστο δυνατο μήκος που δίνει κάθε επανάληψη, παίρνουμε το καινούριο στοιχείο της ΜΦΥ και
βρίσκουμε το μέγιστο στοιχείο της ΜΑΥ (απο τα ελαχιστα τελη για καθε μηκος) που να ειναι μικροτερο του καινουριου της ΜΦΥ με μια binary search.
Μετα προσθετουμε τα αντιστοιχα μηκη. 

Δεν χρειάζεται να κάνουμε κάτι παραπάνω γιατί για τα προηγούμενα που μπηκαν στην ΜΦΥ ψάξαμε ηδη στην ΜΑΥ 
και μάλιστα σε μια ΜΑΥ που είχε περισσοτερα στοιχεία και άρα περισσοτερες "δυνατοτητες" στα τελη καθε δυνατου μηκους.

Η πολυπλοκοτητα ειναι εκτελεση και ξε-εκτελεση ΜΑΥ, εκτελεση ΜΦΥ και logn για καθε καινουριο στοιχειο της ΜΦΥ. Αρα συνολικα O(nlogn).

*/


struct history
{
    int index;
    int before;
};

//binary search που βρισκει το αμεσως μεγαλυτερο (σε φθινων σορταρισμα)
int binarySearchBigger(int* arr, int l, int r, int x)
{
    while (r >= l) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x)
            return mid - 1;

        if (arr[mid] < x) r = mid - 1;
        else l = mid + 1;
    }
    return r;
};

//binary search που βρισκει το αμεσως μικροτερο
int binarySearchSmaller(int* arr, int l, int r, int x)
{
    while (r >= l) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x)
            return mid - 1;

        if (arr[mid] > x) r = mid - 1;
        else l = mid + 1;
    }
    return r;
};


int main()
{
    int n,k;

    //eisodos

    cin >> n;
    cin >> k;
    int* orig = new int[n];
    for (int i = 0; i < n; i++) cin>>orig[i];

    int* plusk = new int[n]; //pinakas gia lds auksimenos kata k
    for (int i=0; i<n; i++) plusk[i] = orig[i]+k;

    int* inr = new int[n];  //lis
    int* decr = new int[n]; //lds
    int inrLim = 0;        //trexwn teleutaios sibliromenos deiktis lis
    int decrLim = 0;       //trexwn teleutaios sibliromenos deiktis lds
    history* tefteri = new history[n]; //istoriko lis

    //ektelesi LIS
    int place;
    for (int i=0; i<n; i++)
    {
        if (i==0)  //prwto stoixeio bainei pada stin akolouthia mikous 1
        {
            inr[0] = orig[i];
            tefteri[0].index = 0;
            tefteri[0].before = -1;
        }
        else
        {
            place = binarySearchSmaller(inr, 0, inrLim, orig[i]);
            if (place == inrLim) //megalytero tou teleutaiou stoixeiou
            {
                inrLim++;
                inr[place+1] = orig[i];
                tefteri[i].index = place + 1;
                tefteri[i].before = -1;
            }
            else //allou
            {
                tefteri[i].index = place + 1;
                tefteri[i].before = inr[place+1];
                inr[place+1] = orig[i];     //to vazoume sto epomeno apo to akrivws mikrotero
            }
        }
    }

    int maxlen = inrLim + 1;
    int firstlen, total;

    for (int i = n-1; i >= 0; i--)
    {
        //undo lis

        if (tefteri[i].before == -1) inrLim--;  //an adeiasei kapoio tha einai ypoxrewtika to teleutaio
        else inr[tefteri[i].index] = tefteri[i].before;

        //do lds

        if (i==n-1) 
        {
            decr[0] = plusk[i];
            firstlen = 1;
        }
        else
        {
            place = binarySearchBigger(decr, 0, decrLim, plusk[i]);
    
            if (place == decrLim) //mikrotero apo ola
            {
                decrLim++;  //update to orio
                decr[place+1] = plusk[i];
            } 
            else decr[place+1] = plusk[i];
            firstlen = place + 2;  //to mikos einai place+2 giati bainei stin thesi place+1 kai oi deiktes (=miki) ksekinane apo to 0
        }
        //prothetoume firstlen + to veltisto mikos pou vriskoume me BS apo LIS + 1 giati stin LIS oi deiktes ksekinane apo 0
        total = firstlen + 1 + binarySearchSmaller(inr, 0, inrLim, plusk[i]); //vriskoume tin kalyteri lis
        if (maxlen < total) maxlen = total;
    }
    
    cout<<maxlen<<endl;
}