#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//merge-sort code taken from geeks for geeks
void merge(int arr[], int start, int mid, int end)
{
    int start2 = mid + 1;
 
    // If the direct merge is already sorted
    if (arr[mid] <= arr[start2]) {
        return;
    }
 
    // Two pointers to maintain start
    // of both arrays to merge
    while (start <= mid && start2 <= end) {
 
        // If element 1 is in right place
        if (arr[start] <= arr[start2]) {
            start++;
        }
        else {
            int value = arr[start2];
            int index = start2;
 
            // Shift all the elements between element 1
            // element 2, right by 1.
            while (index != start) {
                arr[index] = arr[index - 1];
                index--;
            }
            arr[start] = value;
 
            // Update all the pointers
            start++;
            mid++;
            start2++;
        }
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
 
        // Same as (l + r) / 2, but avoids overflow
        // for large l and r
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}

int main ()
{
    std::ios_base::sync_with_stdio (false); //Remove sync with c standards, speeds up cin cout

    // get input
    int n, q;

    cin >> n;
    cin >> q;

    int *allow = new int[n];
    int *want = new int[q];

    for (int i=0; i<n; i++) cin >> allow[i];
    for (int i=0; i<q; i++) cin >> want[i];

    //keep unique allowed edge lengths
    mergeSort(allow, 0, n-1);
    int unsize = 1;

    for(int i=1; i<n; i++)
    {
        if (allow[i] != allow[i-1]) unsize++;
    }

    int *uniqueSteps = new int[unsize];
    int filler = 0;
    uniqueSteps[filler++] = allow[0];

    for (int i=1; i<n; i++)
    {
        if (allow[i] != allow[i-1]) uniqueSteps[filler++] = allow[i];
    }


    //dijkstra 

    /*
    
    estw arithmos k px 5, tote oloi oi arithmoi boroun na graftoun ws 5n i 5n+1 i 5n+2 i 5n+3 i 5n+4 opou n fysikos

    dioti tha exoun ypoloipo 0,1,2,3,4 me tin diairesi me 5

    emeis edw pairnoume to mikrotero vima (gia logous oikonomias), diladi to uniqueSteps[0] kai kanoume pinaka mod
    me theseis osa ta pithana ypoloipa

    apo tous arithmous poy vriskoume me dijkstra san epomenous tou current,
    vazoume sto metwpo mono autous pou pou einai mikroteroi apo ton arithmo pou exei o mod stin antistoixi thesi

    diladi an exoume to 5 tote ton arithmo 12 tha ton valoume sto metwpo mono an einai mikroteros apo ton arithmo
    poy exei o mod stin thesi 2 (->ypoloipo 2)

    episis kanoume update ton mod

    o dijstra teleiwnei otan adeiasei to metwpo, diladi den yparksei oute enas arithmos mikroteros apo auton pou yparxei
    stin antistoixi thesi tou mod

    ara twra o pinakas mod exei ta mikrotera dynata noumera

    gia ton elegxo pairnoume kathe arithmo kai vlepoume an stin thesi mod me to antistoixo ypoloipo yparxei mikroteros i isos arithmos

    an yparxei tote boroume na kataskeuasoume ton arithmo poy theloume apla prosthetontas uniqueSteps[0] i sto paradeigma mas pedaria

    alliws den boroume na ton kataskeuasoume

    den yparxei periptwsi na mas ksefygei kati dioti ston mod exoume tous mikroterous dynatous arithmous

    opote an kapoios epithymitos einai mikroteros tote sigoura den borei na ftiaxtei

    */

    int *mods = new int[uniqueSteps[0]];
    memset(mods, -1, uniqueSteps[0]*sizeof(int));
    priority_queue <int, vector<int>, greater<int> > metwpo;
    int current = -1;
    int candidate, temp;

    metwpo.push(0);


    while(true)
    {
        if (metwpo.empty()) break;
        current = metwpo.top();
        metwpo.pop();

        for (int i=0; i<unsize; i++)
        {
            candidate = current + uniqueSteps[i];
            temp = candidate % uniqueSteps[0];
            if (candidate < mods[temp] || mods[temp] == -1) 
            {
                metwpo.push(candidate);
                mods[temp] = candidate;
            }
        }
    }

    for (int i=0; i<q; i++)
    {
        temp = want[i] % uniqueSteps[0];
        if (want[i] >= mods[temp] && mods[temp] != -1) cout<<"YES"<<endl;
        else cout<<"NO"<<endl;
    }
    
}