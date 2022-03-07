#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct dest
{
    int who;
    int wei;
};

struct input1
{
    int start;
    int end;
    int wei;
};

struct query
{
    int start;
    int end;
};

int main ()
{
    //diavasma input (grammikos xronos ws pros input)

    int v,e,q;

    cin >> v;
    cin >> e;

    dest **graph = new dest*[v]; //pinakas geitniasis
    input1* input = new input1[e]; //pinakas gia na paroume input
    int* participate = new int[v]; //pinakas gia na kseroume pws tha arxikopoiisoume pinaka geitniasis

    memset(participate, 0, v*sizeof(int));

    for (int i=0; i<e; i++)
    {
        cin >> input[i].start;
        cin >> input[i].end;
        cin >> input[i].wei;
        participate[input[i].start-1]++; //update participate
        participate[input[i].end-1]++;
    }

    for (int i=0; i<v; i++) graph[i] = new dest[participate[i]]; //katallili arxikopoiisi

    int* where = new int[v]; //pinakas gia na kseroume posa exoume valei ston pinaka geitniasis gia kathe komvo
    memset(where, 0 , v*sizeof(int));
    int start, end, wei;

    for (int i=0; i<e; i++) //perasma input ston pinaka geitniasis
    {
        start = input[i].start - 1; //i arithmisi ksekinaei apo 1 emeis theloume apo miden
        end = input[i].end - 1;
        wei = input[i].wei;
        graph[start][where[start]].who = end; //undirected graph opote kathe akmi pernaei kai gia tous dyo komvous
        graph[start][where[start]].wei = wei;
        graph[end][where[end]].who = start;
        graph[end][where[end]].wei = wei;
        where[start]++;
        where[end]++;
    }

    delete input;
    delete where;

    cin >> q;

    query *queries = new query[q];

    for (int i=0; i<q; i++)
    {
        cin >> queries[i].start;
        cin >> queries[i].end;
    }

    //ektelesi prim gia kataskeui mst (dialegoume prim dioti akmes para polles se sxesi me komvous)

    priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > metwpo;
    //auto einai priority queue me zeugaria opou to panw panw einai auto me to mikrotero prwto arithmo
    //ara ekei vazoume apostasi kai ston deutero arithmo vazoume ton arithmo tou komvou

    dest *parent = new dest[v]; //edw tha apothikeuthei to mst
    int *dist = new int[v]; //voithitikoi pinakes gia prim
    int *inmst = new int[v];

    for (int i=0; i<v; i++) //arxikopoiisi
    {
        parent[i].who = -1;
        parent[i].wei = -1;
    }
    memset(dist, -1, v*sizeof(int));
    memset(inmst, -1, v*sizeof(int));

    int source = 0;  //thewroume ws prwto komvo ton miden
    dist[source] = 0;
    parent[source].who = source;
    parent[source].wei = 0;
    metwpo.push(make_pair(0, source));

    int node, next;
    while(!metwpo.empty())
    {
        node = metwpo.top().second; //kanoume pop ton komvo me mikroteri apostasi
        metwpo.pop();
        
        if (inmst[node] == 1) continue; //an einai sto mst proxwrame

        inmst[node] = 1; //ton vazoume sto mst

        for (int j=0; j<participate[node]; j++) //an gia kapoio geitona pou den einai sto mst vroume kalyteri apostasi
        {                                       //kanoume update patera kai apostasi kai to vazoume sto metwpo anazitisis
            next = graph[node][j].who;
            wei = graph[node][j].wei;
            if (inmst[next] == 1) continue; //an einai mesa sto mst, proxwrame
            if ((dist[next] != -1) && (dist[next] <= wei)) continue; //an i apostasi einai mikroteri apo varos, proxwrame
            parent[next].who = node; //update parent kai varos
            parent[next].wei = wei;
            dist[next] = wei;  //update apostasi
            metwpo.push(make_pair(dist[next], next)); //vazoume apostasi sto heap
        }
    }

    delete inmst;
    delete dist;
    delete participate;
    for (int i=0; i<v; i++) delete graph[i];
    delete graph;

    /*

    Sto mst pou ftiaksame yparxei to elaxisto bottleneck monopati metaksy dyo opoiwndipote komvwn.

    Auto einai aplo dioti kapoia stigmi stin kataskeui tou mst autoi oi dyo komvoi vriskontan se diaforetikes tomes.

    Autes oi dyo tomes den ginetai pote na enwthikan me kati akrivotero apo tin fthinoteri akmi pou ypirxe, alliws den tha itan mst.

    Opote i apantisi vrisketai sto MST pou exoume.



    To monopati pou syndeei autous tous dyo komvous pernaei anagastika apo lca kai pisw (afou einai dendro den paizei alli periptwsi)

    Opote tha vroume megisto bottleneck apo ton enan komvo ston lca kai megisto bottlenck apo to lca pros ton allon komvo kai
    tha epistrepsoume to megisto apo auta ta dyo.

    Gia lca tha kanoume reduce se rmq me sparse table

    Kai gia megisto bottleneck apo komvo se progono tha kanoume sparse table logiki

    Diladi tha vroume apo kathe komvo megisto edge pros ta panw me ekthetika almata kai meta tha ta syndyazoume gia query se O(log(ypsous))

    */


    //euresi megistwn bottleneck edges pros ta panw me ekthetika almata
    //diladi, kataskeuazoume sparse table

    int size = int(log2(v)) + 1; //kataskeui sparse table (prosthetoume 1 giati mas noiazei kai o ekthetis 0)
    int **ancestor = new int*[v]; //pinakas me progonous se ekthetika almata
    int **bneckup = new int*[v];
    for (int i=0; i<v; i++) 
    {
        bneckup[i] = new int[size];
        ancestor[i] = new int[size];
        memset(bneckup[i], -1, size*sizeof(int));
        memset(ancestor[i], -1, size*sizeof(int));
        if (i!=0) bneckup[i][0] = parent[i].wei;  //i riza den exei oute progono oute monopati pros ta panw
        if (i!=0) ancestor[i][0] = parent[i].who;
    }

    int anc, part1, part2; //voithitikes metavlites
   
    for (int expo=1; expo<size; expo++) //ekthetes
    {
        for (int i=0; i<v; i++) //komvoi
        {
            anc = ancestor[i][expo-1]; //vriskoume progono (p1) proigoumenou ektheti
            if (anc==-1) continue; //an den yparxei progonos proxwrame
            part1 = bneckup[i][expo-1]; //vriskoume bottleneck proigoumenou ektheti (mexri p1)
            part2 = bneckup[anc][expo-1]; //vriskoume bottleneck proigoumenou ektheti apo p1 mexri progono p1 (p2)
            
            if (part1==-1 || part2==-1) continue; //an ta bottlenecks einai -1, diladi den yparxoun, tote proxwrame
            
            bneckup[i][expo] = max(part1, part2); //enwnoume dyo proigoumena kommatia
            ancestor[i][expo] = ancestor[anc][expo-1]; //vazoume ws progono ton p2 pleon
        }
    }

    //metatropi lca se rmq kai kataskeui sparse table

    int **child = new int*[v]; //metatrepoume to parent table se adj list wste na boroume na kanoume dfs
    int *howmany = new int[v]; //pinakas gia na kseroume posa paidia exei o kathenas, wste na arxikopoiisoume swsta adj list
    memset(howmany, 0, v*sizeof(int));

    for (int i=1; i<v; i++) howmany[parent[i].who]++; //den theloume na valoume oti i riza exei paidi ton eauto tis
    for (int i=0; i<v; i++)
    {
        if (howmany[i]==0) continue; //den arxikopoioume pinakes me miden paidia
        child[i] = new int[howmany[i]];
    }

    int *wheree = new int[v]; //voithitikos pinakas gia na kseroume posa exoume valei sto adj list
    memset(wheree, 0, v*sizeof(int));
    int papa;

    for (int i=1; i<v; i++) //pali den vazoume oti riza exei paidi ton eauto tis
    {
        papa = parent[i].who;
        child[papa][wheree[papa]] = i;
        wheree[papa]++;
    }
    delete wheree;

    //dfs (euler walk) gia data gia lca

    int *visit = new int[2*v-1]; //i seira pou vlepoume tous komvous
    int *level = new int[2*v-1]; //ta epipeda tous 
    int *firstmeet = new int[v]; //pote tous petyxainoume prwti fora
    memset(firstmeet, -1, v*sizeof(int));
    vector<int> stack;
    int *childseen = new int[v];
    memset(childseen, 0, v*sizeof(int));

    stack.push_back(0);//vazoume tin riza kai arxikopoioume katallila
    int step = 0;
    int levelnow = 0;
    int curr;

    while(!stack.empty())
    {
        curr = stack.back();
        visit[step] = curr; //kataxwroume pote eidame curr
        level[step] = levelnow; //kataxwroume level
        if (firstmeet[curr] == -1) firstmeet[curr] = step; //kataxwroume firstmeet an xreiazetai
        step++;
        if (childseen[curr] == howmany[curr]) //an exoume dei ola ta paidia meiwnoume level kai pame se epomeno
        {
            levelnow--;
            stack.pop_back();
            continue;
        }
        else //alliws auksanoume to level kai prosthetoume to epomeno kata seira paidi
        {
            levelnow++;
            stack.push_back(child[curr][childseen[curr]]); //ta childseen ksekinane na metrane apo 1 
            childseen[curr]++;                             //ara ama eidame 1 meta vazoume auto me deikti 1 diladi to deutero
        }
    }

    //sparse table gia rmq

    int **rmqsparse = new int*[2*v-1]; //arxikopoiiseis
    int size2 = int(log2(2*v-1))+1;
    for (int i=0; i<2*v-1; i++) 
    {
        rmqsparse[i] = new int[size2];
        memset(rmqsparse[i], -1, size2*sizeof(int));
        rmqsparse[i][0] = i; //rmqsparse epistrefei deikti gia na pame sto visit
    }
    
    int width, index1, index2;
    for (int exp=1; exp<size2; exp++)
    {
        for (int i=0; i<2*v-1; i++)
        {
            width = pow(2, exp);
            if (i+width/2 >= 2*v-1) continue; //an o deiktis einai ektos oriwn proxwrame
            if (rmqsparse[i+width/2][exp-1] == -1) continue; //an to epomeno einai invalid proxwrame
            index1 = rmqsparse[i][exp-1]; //deiktis rmq tou prwtou
            index2 = rmqsparse[i+width/2][exp-1]; //deiktis rmw tou deuterou
            if (level[index1] < level[index2]) rmqsparse[i][exp] = index1; //enwsi
            else rmqsparse[i][exp] = index2;
        }
    }

    //final

    int src, dst, start1, end1, diff, newstart, lca, level1, level2, level3, candi1, candi2;
    for (int i=0; i<q; i++)
    {
        src = queries[i].start-1; //meiwnoume kata ena giati eisodos kseninaei arithmisi apo 1
        dst = queries[i].end-1;
        if (src == dst)
        {
            cout<<"0"<<endl;
            continue;
        }
        start1 = firstmeet[src]; //pou petyxainoume sto euler walk tous dyo komvous
        end1 = firstmeet[dst];
        if (start1 > end1)  //allazoume thesi analoga me to poion petyxainoume prwto
        {
            swap(start1, end1); //start1 antistoixei sto src kai end1 antistoixei sto dst
            swap(src, dst);
        }
        diff = int(log2(end1-start1)); //vriskoume diafora gia na kanoume rmq kai na vroume lca
        newstart = end1 - pow(2,diff);
        index1 = rmqsparse[start1][diff];
        index2 = rmqsparse[newstart][diff];

        if (level[index1] < level[index2]) lca = index1;
        else lca = index2; //pairnoume index (sto level kai visti) gia lca

        level1 = level[start1];
        level2 = level[end1];
        level3 = level[lca];
        candi1 = -1; //ypopsifia apotelesmata
        candi2 = -1;

        while (level3 != level1)
        {
            step = int(log2(level1 - level3)); //kanoume ekthetika almata kai pairnoume megista bottlenecks
            candi1 = max(candi1, bneckup[src][step]); //epikairopoioume ypopsifio
            level1 -= pow(2, step); //epikairopoioume level1 mexri na ftasei level3 tou lca
            src = ancestor[src][step]; //epikairopoioume src
        }

        while (level3 != level2) //kanoume to idio gia deutero ypopsifio
        {
            step = int(log2(level2 - level3));
            candi2 = max(candi2, bneckup[dst][step]);
            level2 -= pow(2, step);
            dst = ancestor[dst][step];
        }

        cout<<max(candi1, candi2)<<endl; //dialegoume to max
    }    
}