#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int endoftrans=0; /* Variable that will decide if a linked list already exists or not and if a previous transmission of instructions completed or not */

/*Create Linked list to hold incoming hex characters*/
/* Linked list required to ensure dynamic nature and avoid array having not enough space to hold all hex characters*/

struct node {
   int info;
   struct node *next;
};
struct node *first = NULL;

/*Function to add elements at end of linked list */

void insert_at_end(uint8_t data)
 {
     struct node *t;
     struct node *temp = (struct node*) malloc(sizeof(struct node));
     temp->info=data;
     temp->next=NULL;
     t=first;
     if(first==NULL)
     {
         first=temp;
     }
     else
     {
         while(t->next!=NULL)
         {
             t=t->next;
         }
         t->next=temp;
    }
 } 

 /* Function to Delete Linked List if a new set of instructions are given after some time */
 
 void delete_linked_list()
 {
    struct node* current = first; 
    struct node* next; 
  
   while (current != NULL)  
   { 
       next = current->next; 
       free(current); 
       current = next; 
   } 
    
   first = NULL;
 }


/* Function to initialise UART */ 

void usart_init()                                
{
  UCSR0B = (1<<RXCIE0)|(1<<TXEN0)| (1<<RXEN0);   /* Enable transmission from USART and enable recieve interrupt */
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);              /* Set size of character to 8-bits*/
  UBRR0L =  0x67;                                /* Set baud rate to 9600*/
}



/* Function to read incoming values from esp32 and write into linked list*/

void acknowledge_uart(char character)
{
      while(!(UCSR0A&(1<<UDRE0)));
      UDR0=character ;
}

uint8_t read_uart()
{
  uint8_t tempvar;
  while(!(UCSR0A & (1<<RXC0)));
  tempvar=UDR0;
  return tempvar;
  
}
/*Function to instructionbytes*/
void getibytes()
{
     while(endoftrans!=1)
    {
      int bytecount=0;
      uint8_t data=0;
      bytecount=read_uart();
      if(bytecount>0)
      {
        acknowledge_uart('S');
        for(int i=0;i<bytecount;i++)
        {
          data=read_uart();
          insert_at_end(data);
          acknowledge_uart('D');
        }
      }
      else
      {
        endoftrans=1;
      } 
    }
}
/*When function to read from UART is called */
void readsignal()
{

    if(endoftrans=0)
    {
      getibytes();
    }
    else
    {
      endoftrans=0;
      delete_linked_list();
      getibytes();
      
    }

}

int main(void)
{ 
   usart_init();   /*Keeping UART ready always for communication*/
   int randomvar;
   while(1)
   {
    randomvar=0;
    randomvar=read_uart();
    if(randomvar=0)
    {readsignal();}
   }

}