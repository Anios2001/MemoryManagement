
#include <iostream>
#include<sys/mman.h>
#include<vector>
#define MAX_SPACE 4112   // Allow to free the memory per process and collasce them 
using namespace std;
typedef struct Node {
    int size;
    struct Node* next;
} node_t;
typedef struct Header{
    int size;
    int magic_number;
}Alloc_Header;

Alloc_Header* allocateSpace(int size,node_t** head_pointer){
    srand(time(0));
    //calculate sizeof the type to be allocated 
    int size_allocateRegion= sizeof(Alloc_Header);
    //copy the components of head to the space after allocated region
    if((*head_pointer)->size >= (size + sizeof(Alloc_Header))){
    //Some cleanup code must be here to stop the warnings
    void* counter = *head_pointer;
    node_t * newHead= (node_t*) (counter + size +sizeof(Alloc_Header));
    newHead->size= (*head_pointer)->size - (size+ sizeof(Alloc_Header));
    newHead->next= (*head_pointer)->next;
    //allocate new space in place of previous head
    Alloc_Header* new_space= (Alloc_Header*)(*head_pointer);
    *head_pointer= newHead;
    //Initiallise new_space params for MANAGEMENT 
    new_space->size= size;
    new_space->magic_number= rand()%(size);
    //return a pointer to new_space
    
    return new_space;
    }
    else
     return nullptr;
}

void freeSpace(Alloc_Header* memory_reference,node_t ** head_pointer){
    int size= memory_reference->size;
    //check for corruption in Memory
    if(memory_reference->size < sizeof(node_t)){
     cout<<"MEMORY ERROR"<<endl;
     return;
    }
    //Reference to last chunk in memory
    node_t* it= (*head_pointer);
    node_t * memory_reference_tonode= (node_t*)memory_reference;
    while(it->next!=NULL){
    if((it+sizeof(node_t)+it->size)==memory_reference_tonode){
        //Collasce Case
        break;
    }    
     it=it->next;
    }
    if(it!=nullptr){
        it->size+= size + sizeof(Alloc_Header);
    }
    
    //Change its next to memory_reference
   // it->next= (node_t*)memory_reference;
    //Set up newly recovered memory
    //node_t* recoveredMemory= (it)->next;
    //recoveredMemory->size = size + sizeof(Alloc_Header) - sizeof(node_t);
    //recoveredMemory->next=NULL;
    //Console Output
    
   // cout<<"MEMORY RECOVERED "<<recoveredMemory->size<<endl;
    
 }

int SizeofFreeSpace(node_t* head){
    int TotalSize=0,count=0;
    while(head!=NULL){
     TotalSize+=head->size;
     head=head->next;
     count++;
    }
    cout<<"FREE SPACE IN "<<count<<" chunk(s)."<<endl;
    return TotalSize;
}

void collasceFreeSpace(node_t* head){
    while(head!=NULL && head->next != NULL){
      cout<<"HEADS NEXT IS "<<head->next<<" HEAD IS AT "<<head<<endl;
      int COLLASCING_METRIC = (head - head->next)*(sizeof(node_t));
      COLLASCING_METRIC = abs(COLLASCING_METRIC);
      cout<<COLLASCING_METRIC<<","<<(sizeof(node_t) + head->size)<<endl;
    if(COLLASCING_METRIC== (sizeof(node_t) + head->size))
    {
         head->size+= (head->next->size + sizeof(node_t));
         head->next= head->next->next;
         cout<<"collasced"<<endl;
    }
    else{
     head=head->next;
    }
    }
}

int main()
{
    //DISPLAY
    cout<<" *****************DEMONSTRATION OF MEMORY MANAGEMENT *******************"<<endl;
    cout<<" *                                                                     *"<<endl;
      
      //Request Memory from OS to work on
       node_t *head= (node_t*)mmap(NULL,MAX_SPACE,PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,-1,0);
       head->size=MAX_SPACE - sizeof(node_t);
       head->next= NULL;
    
    //DISPLAY   
    cout<<" * Memory Currently contains a single FREE CHUNK OF "<<MAX_SPACE<<" bytes in size *"<<endl ;
    cout<<" * "<<(head->size)<<" bytes of FREE SPACE AVAILABLE...... (HEADER EXCLUDED)         *"<<endl;
    cout<<endl;
    
    //TEST DATA
 vector<int> SIZE_TO_BE_ALLOCATED={20};
    //TEST LOOP
for(auto& x: SIZE_TO_BE_ALLOCATED){
    
    //cout<<"HEAD AT "<<head<<endl;
    Alloc_Header* Memory_ref= allocateSpace(x,&head);
    if(Memory_ref!=nullptr){
    //cout<<"HEAD AT "<<head<<endl;
    cout<<"Memory_ref "<< Memory_ref<<" Allocated Memory Size "<<(Memory_ref->size)<<" MAGIC "<<(Memory_ref->magic_number)<<endl;
     freeSpace(Memory_ref,&head);
    }
    else{
     cout<<"Can not allocate Memory of "<<x<<" byte(s) "<< (x+ sizeof(Alloc_Header) -(head->size))<< " byte(s) of extra space needed"<<endl;
    }
    
}
    //
    cout<<"Free Memory size "<<SizeofFreeSpace(head)<<endl;
    
    //FOOTER
    cout<<"Size of Header of Allocated Space "<<sizeof(Alloc_Header)<<endl;
    cout<<sizeof(node_t)<<endl;
    
    collasceFreeSpace(head);
    cout<<"FREE SPACE SIZE AFTER COLLASCING "<<SizeofFreeSpace(head);
    //node_t* pointer=head + head->size;
    //cout<<head;
    return 0;
}














