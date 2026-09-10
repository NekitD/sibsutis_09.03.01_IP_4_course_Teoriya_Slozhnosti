#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef struct node {
    int value;
    node* next;
}node;

typedef struct queque {
    node* head;
    node* tail;
    queque* next;
}queque;

typedef struct queque_list {
    queque* head;
    queque* tail;
}queque_list;

void MergeSort(queque *&A);

void PrintQueque(queque *&A){
    node* p = A->head;
    while(p != NULL){
        cout << p->value << " ";
        p = p->next;
    }
}

void CleanQueque(queque *&A){
    node* p = A->head;
    node* p_prev;
    while(p != NULL){
        p_prev = p;
        p = p->next;
        delete p_prev;
    }
    delete A;
    A = NULL;
}

int main()
{
    queque* A = new queque;
    node* p;
    
    srand(time(NULL));
    int k = 1;
    cout << "n = 2^k" << endl;
    cout << "Input k: ";;
    cin >> k;
    cout << endl;
    int n = pow(2, k);

    if (n <= 0){
        return -1;
    }

    p = new node;
    p->value = rand() % n;
    p->next = NULL;
    A->head = p;
    A->tail = p;
    for(int i = 1; i < n; i++){
        p = new node;
        p->value = rand() % n;
        p->next = NULL;
        A->tail->next = p;
        A->tail = p;
    }

    cout << "Default queque: " << endl;;
    PrintQueque(A);
    cout << endl;
    MergeSort(A);
    cout << "Sorted queque: " << endl;
    PrintQueque(A);
    cout << endl;
    CleanQueque(A);
}

queque* Merge(queque* q1, queque* q2){
    node *p1, *p2;
    p1 = q1->head;
    p2 = q2->head;
    queque* res = new queque;
    if(p1->value <= p2->value){
        res->head = p1;
        res->tail = p1;
        p1 = p1->next;   
    } else {
        res->head = p2;
        res->tail = p2;
        p2 = p2->next;
    }
    res->tail->next = NULL;

    while(p1 != NULL && p2 != NULL){
        if(p1->value <= p2->value){
            res->tail->next = p1;
            res->tail = p1;
            p1 = p1->next;
        } else {
            res->tail->next = p2;
            res->tail = p2;
            p2 = p2->next;
        }
        res->tail->next = NULL;
    }
    while(p1 != NULL){
        res->tail->next = p1;
        res->tail = p1;
        p1 = p1->next;
        res->tail->next = NULL;
    }
    while(p2 != NULL){
        res->tail->next = p2;
        res->tail = p2;
        p2 = p2->next;
        res->tail->next = NULL;
    }

    return res;
}


void MergeSort(queque *&A){
    node* p = A->head;
    queque* q;
    queque_list* ques = new queque_list;
    if(p == NULL){
        return;
    }
    cout << "================================================" << endl;
    q = new queque;
    q->head = p;
    p = p->next;
    q->tail = q->head;
    q->head->next = q->tail;
    q->tail->next = NULL;  
    q->next = NULL;
    ques->head = q;
    ques->tail = q;
    PrintQueque(q); 
    cout << " | ";
    while(p != NULL){
        q = new queque;
        q->head = p;
        p = p->next;
        q->tail = q->head;
        q->head->next = q->tail; 
        q->tail->next = NULL; 
        q->next = NULL;
        ques->tail->next = q;
        ques->tail = q;
        PrintQueque(q);
        cout << " | ";
    }
    cout << endl;
    delete A;
    A = NULL;
    cout << "================================================" << endl;
    queque* new_q;
    while(ques->head->next != NULL){
        queque_list* new_ques = new queque_list;
        q = ques->head;
        new_q = Merge(q, q->next);
        new_ques->head = new_q;
        new_ques->tail = new_q;
        new_ques->tail->next = NULL;
        PrintQueque(new_q); 
        cout << " | ";
        q = q->next->next;
        while(q != NULL){
            new_q = Merge(q, q->next);
            new_ques->tail->next = new_q;
            new_ques->tail = new_q;
            new_ques->tail->next = NULL;
            PrintQueque(new_q); 
            cout << " | "; 
            q = q->next->next;
        }
        ques = new_ques;
        new_ques = NULL;
        cout << endl;
        cout << "================================================" << endl;      
    }
    A = ques->head;
}