#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<vector>
#include<iostream>
#include <fstream>
#include <map>
void SelectOption();
using namespace std ;

vector<string>avalue ;
vector<string>cvalue ;

int numberofobject = 0 ;

int findOccurance(string att, string cl)
{
    FILE *fptr;
    char a[100], c[100] ;
    int count = 0 ;
    if ((fptr = fopen("data.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fptr, "%s %s", a, c)!= EOF)
    {
        string s = "" ;
        string s2 = "" ;

        for(int i = 0 ; i < strlen(a) ; i++)
        {
            s += a[i] ;
        }
        for(int i = 0 ; i < strlen(c) ; i++)
        {
            s2 += c[i] ;
        }
        if(s == att && s2 == cl) count++ ;
    }
    fclose(fptr);
    return count ;
}

int findOccuranceOfClass(string animalClass)
{
    FILE *fptr;
    char a[100], c[100];
    int count = 0;
    if ((fptr = fopen("data.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fptr, "%s %s", a, c)!= EOF)
    {
        string s = "" ;
        string s2 = "" ;
        for(int i = 0 ; i < strlen(a) ; i++)
        {
            s += a[i] ;
        }
        for(int i = 0 ; i < strlen(c) ; i++)
        {
            s2 += c[i] ;
        }
        if(s2 == animalClass) count++ ;
    }
    fclose(fptr);
    return count ;
}
int TrainModel()
{
    char a[100], c[100];
    FILE *fptr;
    if ((fptr = fopen("data.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    string s = "" ;
    while (fscanf(fptr, "%s %s", a, c)!= EOF)
    {
        //printf("%s %s", a, c); // show both classes
        //printf("\n") ;
        numberofobject++ ;

        bool flag = false ;
        string s = "" ;
        for(int i = 0 ; i < strlen(a) ; i++)
        {
            s += a[i] ;
        }
        for(int i = 0 ; i < avalue.size() ; i++)
        {
            if(s == avalue[i])
            {
                flag = true ;

            }
        }
        if(!flag)
            avalue.push_back(s) ;


        flag = false ;
        s = "" ;
        for(int i = 0 ; i < strlen(c) ; i++)
        {
            s += c[i] ;
        }
        for(int i = 0 ; i < cvalue.size() ; i++)
        {
            if(s == cvalue[i])
            {
                flag = true ;

            }
        }
        if(!flag)
            cvalue.push_back(s) ;



    }
    printf("number of object :%d\n", numberofobject) ;

    cout << "Classes" << endl;
    for(int i = 0 ; i < avalue.size() ; i++)
    {

        cout<< avalue[i] << endl ;
    }

    cout << "Prior Knowledge" << endl;
    ofstream priorKnowledge("prior.txt");
    float freq;
    for(int i = 0 ; i < cvalue.size() ; i++)
    {
        int occ = findOccuranceOfClass(cvalue[i]);
        freq =  float(occ) / numberofobject;
        priorKnowledge << cvalue[i] << " " << freq << endl;
        cout<< cvalue[i] << " " << freq << endl ;
    }
    priorKnowledge.close();

    cout << "Likelihood" << endl;
    ofstream likelihoodKnowledge("likelihood.txt");
    for(int i = 0 ; i < cvalue.size() ; i++)
    {
        for(int j = 0 ; j < avalue.size() ; j++)
        {
            int n = findOccurance(avalue[j], cvalue[i]) ;
            cout << avalue[j] << "|" << cvalue[i] << " = "<< float(n)/numberofobject << endl ;
            likelihoodKnowledge << avalue[j] << " " << cvalue[i] << " " << float(n) / numberofobject << endl ;
        }
    }
    likelihoodKnowledge.close();
    fclose(fptr);
    SelectOption();
    return 0;
}

float classProbability(string class_name)
{
    string _class; float prior;
    float temp;
    ifstream priork("prior.txt");
    while (priork >> _class >> prior)
    {
        if(class_name == _class  )
        {
            //cout << _class << " " << prior << endl;
            temp = prior;
        }
    }
    priork.close();
    return temp;
}

int TestModel()
{
    string att, _class;
    cout << "Attribute :"; cin >> att;
    //cout << "Animal name :"; cin >> _class;


    string attribute, c_name;
    float probability, res;
    ifstream likelihood("likelihood.txt");
    map<float, string> showRes;
    while (likelihood >> attribute >> c_name >> probability)
    {
        if(att == attribute)
        {
            cout << attribute << " " << c_name << " " << probability << endl;
            res = probability*classProbability(c_name);
            showRes.insert(pair<float, string>(res, c_name));
            cout << "Probability :" << res << endl;
        }

    }
    likelihood.close();

    float temp = 0; string cls_name;
    for (auto itr = showRes.begin(); itr != showRes.end(); ++itr)
    {
        if(itr->first > temp)
        {
            temp = itr->first;
            cls_name = itr->second;
        }

        //cout << itr->first << ' ' << itr->second << '\n';
    }
    cout << endl;
    cout << "The most probable class is " << cls_name << " with probability " << temp;
    cout << endl;
    void SelectOption();
    return 0;
}
void SelectOption()
{
    cout << endl;
    cout << "1. Train Model" << endl;
    cout << "2. Test Model" << endl;
    cout << "3. Exit" << endl;
    int option;
    cin >> option;
    if(option==1)
        TrainModel();
    else if(option==2)
        TestModel();
    else if(option==3)
        exit(0);
    else
    {
        cout << "Wrong input";
        SelectOption();
    }
}
int main()
{
    cout << "\tBayesian Pridtiction" << endl;
    //cout << classProbability("cat");
    SelectOption();

}
