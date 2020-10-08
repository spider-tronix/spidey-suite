#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int flagvariable=0; /* Variable that will decide if a linked list already exists or not and if a previous transmission of instructions completed or not */

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

void acknowledge_uart()
{
      while(!(UCSR0A&(1<<UDRE0)));
      UDR0='F' ;
}

uint8_t read_uart()
{
  uint8_t tempvar;
  while(!(UCSR0A & (1<<RXC0)));
  tempvar=UDR0;
  
}
/* Function to execute when message recieved */
ISR(USART_RX_vect)  /*Detect if UDR0 Recieves Data*/
{
  if(flagvariable=0)  /*If this is the first time or if a linked list already exists and is only being updated (that is earlier transmission is not over */                                  
  {
    TCCR0B=0x00;          /* Stop timer till value is being read and acknowledgement is sent*/
    TCNT0=0x00;           /* Refresh Timer to 0*/
    uint8_t buffvar;
    buffvar=read_uart();     /* Read UDR0 and store in buffer variable*/  
    insert_at_end(buffvar);  /*Push to linked list*/
    acknowledge_uart();      /*Acknowledge*/
    TCCR0B=0x01;               /* Start timer to operate in normal mode*/
  }
  else                          /* To enter a new set of instructions . In this else part we delete the old linked list and create a new one */
  {
    TCCR0B=0x00;                /* Stop timer till value is being read and acknowledgement is sent*/
    TCNT0=0x00;                 /* Set counter to 0*/
    flagvariable=0;             /* Update flag variable to indicate that linked list is being updated */
    uint8_t buffvar;
    delete_linked_list();       /* Delete the old linked list with the old instructions*/
    buffvar=read_uart();
    insert_at_end(buffvar);
    acknowledge_uart();
    TCCR0B=0x01;                 /*Start Timer to operate in normal mode*/
  }
}

ISR(TIMER0_OVF_vect)           /*If timer has overflowed that means no new instruction has been sent in the past 256 ticks , hence we can say it is end of transmission*/
{
  TCCR0B=0x00;                 /* Stop Timer*/
  flagvariable=1;              /* Update flag variable to indicate that a linked list already exists and remember that previous transmission is over*/
}


int main(void)
{ 
  sei();                /* Enable global interrupts*/
  TIFR0=0x01;           /* Enable Timer0 interrupt*/
  usart_init();   /*Keeping UART ready always for communication*/

}
