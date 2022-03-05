#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/*

Παρατηρούμε ότι υπάρχει διάταξη στο πρόβλημα. Δηλαδή αν με καποια πορταλς
δεν ικανοποείται ο στόχος μας τοτε αποκλείεται να ικανοποιείται με λιγότερα.

Η ιδεα λοιπόν ειναι ότι αρχικά σορταρτουμε τα πορταλς σε αύξουσα σειρα -> O(mlogm)

Μετα με binary search διαλεγουμε ποιο θα ειναι το τελευταιο πορταλ. 
Δηλαδή έχουμε στην διαθεσή μας αυτό και όλα τα πλατυτερα (επόμενα).

Μετα ελέγχουμε αν αυτη η επιλογη μας κανει. 
Αν μας κανει παμε δεξιοτερα και δοκιμαζουμε για πλατύρερο κάτω όριο.
Αλλιως παμε αριστεροτερα και δοκιμαζουμε για στενοτερο κατω όριο.

Για τον έλεγχο παίρνουμε λοιπον όλα τα πορταλς
και κανουμε union τα συμπαντα που ενωνουν.

Μετα διατρέχουμε τον πινακα με τους μορτυ και τα συμπαντα που βρίσκονται. 
Για καθε Μορτυ κανουμε find για το τρεχων συμπαν και αλλο ενα find για το επιθυμητο συμπαν.

Αν αυτα τα find επιστρεψουν ιδιο αντιπροσωπο σημαινει πως υπαρχει μονοπατι απο 
το ενα συμπαν προς στο αλλο γιατι ανηκουν στο ιδιο union. 

Ο ελεγχος θεωρείται επιτυχης αν όλα αυτα τα ζευγαρια find επιστρεψουν ιδιο αποτελεσμα μεταξυ τους.

Υλοποιώντας το union-find με βεβαρυμενη ενωση και συμπτυξη μονοπατιων η πολυπλοκοτητα για
καθε union και find θεωρειται πρακτικα σταθερη. 

Οπότε ο έλεγος μας κοστιζει O(n+m) αφου το find γινεται σε σταθερο χρονο για ολους τους μορτυ
και το union γινεται σε σταθερο χρονο για ολα τα πορταλς που εχουμε: O(n) + O(m) = O(n+m)

Οπότε ο έλεγχος επι τις επιλογες δινει (n+m)logm και το σορταρισμα mlogm. Άρα O((n+m)logm)).

*/

struct portal  //για κάθε πορταλ αποθηκεύω τις διαστάσεις που συνδέει και πλάτος
{
	int dim1;
    int dim2;
    int size;
};

struct myinput
{
    int n;  //πλήθος παράλληλων συμπαντων
    int m;  //πλήθος portals
    int* positions; //θεσεις των Μορτυ
    portal* portals; //πίνακας με πορταλς
};

myinput readinput()
{
    myinput in;
    cin >> in.n;
    cin >> in.m;
    in.positions = new int[in.n];
    in.portals = new portal[in.m]; //διάβασμα θέσων μορτυ
    for (int i = 0; i < in.n; i++) cin >> in.positions[i];
    for (int i = 0; i < in.m; i++) //διάβασμα portals
    {
        cin >> in.portals[i].dim1;
        cin >> in.portals[i].dim2;
        cin >> in.portals[i].size;
    }
    for (int i = 0; i < in.n; i++)  //μειώνω τις διαστασεις κατα 1
    {                               //για να συμφωνουν με αριθμηση δεικτων πίνακα
        in.positions[i]--;
    }
    for (int i = 0; i < in.m; i++)
    {
        in.portals[i].dim1--;
        in.portals[i].dim2--;
    }
    return in;
};


/*

υλοποιούμε την find με συμπτυξη μονοπατιων
και με επαναληπτικο τροπο με δικη μας στοιβα (αντι αναδρομικο)
ωστε να μην σκασει το stack σε καμια περίπτωση

*/
int Find(int univ, int* father)  
{
    vector<int> stack;
    int curr = univ;
    int pop;
    while(1)
    {
        if (father[curr] == curr)  //βρήκαμε αντιπροσωπο για το univ
        {
            while(!stack.empty())  //για κάθε στοιχείο που περάσαμε θέτουμε ως πατέρα τον αντιπρόσωπο 
            {   
                pop = stack.back();
                stack.pop_back();
                father[pop] = curr;
            }
            break;
        }
        else 
        {   
            stack.push_back(curr); //αποθήκευση στοιχείου στην στίβα 
            curr = father[curr];  //επικαιροποίηση curr
        }
    }
    return curr;
};

void Union (int univ1, int univ2, int* father, int* monitor) //ο πίνακας monitor περιέχει πόσα στοιχεία έχουν οι αντιπρόσωποι
{                                                            //για τους υπόλοιπους έχει invalid τιμές (αρχικοποιείται με 1 παντου 
    int deleg1 = Find(univ1, father);                        //γιατι όλοι αντιπροσωπεύουν τους εαυτούς τους)
    int deleg2 = Find(univ2, father);
    if (deleg1 == deleg2) return;  //έχουν ιδιο αντιπρόσωπο άρα δεν κάνουμε κάτι
    if (monitor[deleg1] > monitor[deleg2])  //βεβαρυμενη ενωση
    {
        father[deleg2] = deleg1;
        monitor[deleg1] += monitor[deleg2];  //επικαιροποίηση monitor
        monitor[deleg2] = -1;  
    }
    else
    {
        father[deleg1] = deleg2;
        monitor[deleg2] += monitor[deleg1];  //επικαιροποίηση monitor
        monitor[deleg1] = -1;
    }
};

/*

Υλοποίηση merge-sort για τα πορταλς
*/

void merge(portal* portals, int left, int mid, int right)
{
    int tempLeftSize = mid - left + 1;   //δημιουργία και γεμισμα προσωρινων πινάκων
    int tempRightSize = right - mid;
    portal* templeft = new portal[tempLeftSize];  
    portal* tempright = new portal[tempRightSize];
    for (int i = 0; i < tempLeftSize; i++) templeft[i] = portals[left + i];
    for (int i = 0; i < tempRightSize; i++) tempright[i] = portals[mid + 1 + i];

    int i = 0;            //merge
    int j = 0;
    int pos = left;
    while((i<tempLeftSize) && (j<tempRightSize))
    {
        if (templeft[i].size > tempright[j].size)
        {
            portals[pos] = tempright[j];
            pos++;
            j++;
        }
        else
        {
            portals[pos] = templeft[i];
            pos++;
            i++;
        }
    }

    if (j >= tempRightSize)   //ταχτοποιούμε τα υπόλοιπα 
    {
        while (i<tempLeftSize)
        {
            portals[pos] = templeft[i];
            pos++;
            i++;
        }
    }
    else
    {
        while (j<tempRightSize)
        {
            portals[pos] = tempright[j];
            pos++;
            j++;
        }
    }
    delete templeft;   //τα πετάμε αφου δεν τα θελουμε αλλο για οικονομία μνήμης 
    delete tempright;   
}


/* 

επαναληπτική υλοποίηση mergesort για να μην σκάσει το stack

ουσιαστικά κάνουμε merge τους υποπίνακες με 1 στοιχείο, μετα με 2, μετα με 4 κ.ο.κ.

*/
void mergeSort(portal* portals, int start_index, int end_index)
{
   int currLen, left, mid, right; 
  
   for (currLen=1; currLen<=end_index; currLen = 2*currLen) //διπλάσιαζουμε κάθε φορα το τρέχων μηκος
   {
       for (left=0; left<end_index; left += 2*currLen)
       {
           mid = min(left + currLen - 1, end_index);
           right = min(left + 2*currLen - 1, end_index);
           merge(portals, left, mid, right);
       }
   }
}

/*

Συνάρτηση που υλοποιεί τον έλεγχο για το αν μας κάνει συγκεκριμενο κατω οριο στα πορταλς που θα χρησιμοποιηθούν

*/
bool check(portal* portals, int candidate, int n, int m, int* father, int* monitor, int* positions) 
{
    int curr = candidate;     //candidate θεση στενοτερου πορταλ που εχουμε
    int temp1, temp2;
    while(curr < m)          //κανουμε uion στα συμπαντα των πορταλς ξεκινωντας απο το μινιμουμ και πηγαίνοντας δεξιά
    {
        Union(portals[curr].dim1, portals[curr].dim2, father, monitor);
        curr++;
    }
    for (int i = 0; i < n; i++) //κάνουμε find στην τρέχουσα θεση και στην επιθυμητη και αν έστω
    {                           //ενα ζευγαρι δεν εχει ιδιο αποτελεσμα επιστρεφω false
        temp1 = Find(i, father);  
        temp2 = Find(positions[i], father);
        if (temp1 != temp2) return false;
    }
    return true;
}

int main()
{
    myinput input = readinput();
    mergeSort(input.portals, 0, input.m - 1); //σορτάρισμα
    int left = 0;                   //προετοιμασία για binary search
    int right = input.m - 1;
    int mid;
    int* father = new int[input.n];
    int* monitor = new int[input.n];
    while (!(left >= right))     //binary search
    {
        mid = left + (right - left) / 2;
        for (int i=0; i<input.n; i++)    //αρχικοποίηση πινάκων union-find για πέρασμα στην συνάρτηση
        {                                //όλοι ειναι αντιπροσωποι του εαυτου τους και περιεχουν 1 στοιχειο
            father[i] = i;
            monitor[i] = 1;
        }
        if (check(input.portals, mid, input.n, input.m, father, monitor, input.positions)) left = mid + 1;
        else right = mid - 1;
    }
    /*

    Υπάρχει το edge case όταν ενα mid πέτυχε και μετα πηραμε σαν αριστερο άκρο
    το mid + 1 να χάσαμε τo δεξιότερο κάτω φράγμα που πετυχαίνει η επιστροφή των μορτυ.

    Σε αυτην την περίπτωση η binary search μας οδηγεί ακριβως μια θέση μετά
    προσπαθώντας να βρει καλύτερο κατω φραγμα για πορταλ (αλλά μάταια).

    Οπότε κάνουμε εναν τελευταίο έλεγχο για να καλύψουμε αυτην την περίπτωση

    */

    for (int i=0; i<input.n; i++) //τελευταία αρχικοποίηση   
    {                                
            father[i] = i;
            monitor[i] = 1;
    }
    //έλεγχος για αποτέλεσμα BS: αν πετύχει παιρνουμε το αποτελεσμα, αλλιώς το προηγούμενο
    if (check(input.portals, left, input.n, input.m, father, monitor, input.positions)) cout<<input.portals[left].size<<endl;
    else cout<<input.portals[left - 1].size<<endl;
}