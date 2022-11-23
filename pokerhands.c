

//includes
#include <stdlib.h>
#include <stdio.h>

//defines
#define DECKSIZE 52

//typedefs
typedef struct {
    int pips; 
    char suit; 
} card;

//variables 
card deck[DECKSIZE] = {0,0};
card hand[5] = {0,0};
char suits[] = {'C', 'H', 'S', 'D'};
int numCards = 0;
int numHands = 0;
int numFlush = 0;
int numStraight = 0;
int numStraightFlush = 0;
int numPair = 0;
int numThree = 0;
int numFour = 0; 
int numTwoPair = 0;
int numFullHouse = 0;
int ignore = 0;
int num = 0;
int handsToDeal = 0;
char* crdNames[] = {"ace", "deuce", "three", "four", "five", "six", "seven",
                    "eight", "nine", "ten", "jack", "queen", "king"};


//forward declarations
card* dealHand (card*, int);
card* arrangeHand(card*);
int compare(const void*, const void*);
int isFlush(card*);
int isStraight(card*);
int isStraightFlush(int, int);
int XofaKind(int, int);
int onePair();
int threeOfAK();
int fourOfAK();
int twoPair();
int fullHouse();
void calculateOdds( int, int, int, int, int, int, int, int, int);
void swap (card* , card* );
void initialize(card*);
void shuffle(card*);

/*main method - 
initializes a deck of cards, asks the user how many hands to deal, deals that many hands (shuffling every ten times),
increments instances of poker hands, outputs the odds of getting any given hand*/

int main(void){

  initialize(deck);                                 //set up deck array of 52 cards

   printf("Enter number of hands to deal: ");       //get user input for # of hands to deal
   scanf("%d", &handsToDeal);

  for(int i = 0; i<handsToDeal; i++) {              //deal number of hands in a loop
  
    int reset = numHands%10;                        //reset(shuffle) the deck every ten hands
    if(reset ==0) {
        shuffle(deck);
        numCards = 0;                               //point first card back to beginning of the deck
    }

        dealHand(deck, numCards);                   //deal one hand of five cards
        numCards+=5;                                //increment card counter by five
        numHands++;                                 //increment hand counter by 1
        arrangeHand(hand);                          //sort cards in hand by pips value ascending
        numFlush+= isFlush(hand);                   //count occurrences of each type of hand
        numStraight+= isStraight(hand);
        numStraightFlush+=(isFlush(hand)&&isStraight(hand));
        numPair+=onePair();
        numThree+=threeOfAK();
        numFour+=fourOfAK();
        numTwoPair+= twoPair();
        numFullHouse+= fullHouse();

  }
                                                    //calculate odds and print as output
    calculateOdds(numPair, numTwoPair, numThree, numFour, numFlush, numStraight, numStraightFlush, numFullHouse, numHands);
    
    EXIT_SUCCESS;                                   //exit
  

  
}

/* intialize deck of 52 cards with pip values 1-13 and suit values*/
void initialize(card* deck){
   for (int crdctr = 0; crdctr < DECKSIZE; deck++, crdctr++){
      deck->pips = crdctr%13 + 1;                                      //citation: dave houtman, 2022
      deck->suit = suits[(crdctr)/13];
   }
}

/*shuffle deck of 52 cards by calling swap method*/
void shuffle(card* deck){
   int swapCard;
   for (int crdctr = 0; crdctr <DECKSIZE; crdctr++){                  //citation: dave houtman, 2022
      swapCard = rand()%DECKSIZE;
      swap(deck+crdctr, deck+swapCard);
   }
}

/*swap method to swap what two different cards are pointing to with one another*/
void swap (card* p, card* q){
   card tmp;
   tmp = *p;                                                          //citation: dave houtman, 2022
   *p = *q;
   *q = tmp;
}

/*method that deals a "hand" by assigning five cards in the deck to a smaller hand array of 5*/
card* dealHand(card* deck, int numCards) {
   
    card* p;
    card* inHand = hand;

    for(p = (deck + numCards); p < (deck+numCards+5); p++) {    //point into current location in deck array (so as to not deal the same hand ten times)
        *inHand = *p;                                           //assign value being pointed to by loop variable to next space in hand
        inHand++;                                               //increment card being pointed to in the hand
        }

    return hand;                                    
}

/*comparator to sort cards by pips value*/
int compare(const void* i, const void* j) {       //citation: en.cppreference.com/w/c/algorithm/qsort

    int arg1 = *(const int*)i;
    int arg2 = *(const int*)j;

    return arg1-arg2;                              //compare to see which is higher
}

/*method that calls the qsort method on int compare() to arrange cards in hand by pips value, ascending*/
card* arrangeHand(card* hand) {

    qsort(hand, 5, sizeof(card*), compare);    //sort cards in hand ascending
    return hand;
}

/*method that returns 1 (true) if hand is a flush*/
int isFlush(card* hand) {

    card* p = hand;
    int numSuit =0;
    card* next;

    for(p = hand; p<hand+4; p++) {              //loop through cards in hand
        next = p+1;                             //set pointer to next card in hand
        if(p->suit == next->suit) {             //if suit of current card matches the suit of the next card, 
            numSuit++;                          //increment the counter for # of matches
        }
    }

    if(numSuit == 4) return 1;                  //if the suit matches 4 times, isFlush() returns true
    else return 0;

}

/*method that returns 1 (true) if hand is a straight*/
int isStraight(card* hand) {
    int sequence = 0;
    card* p;
    card* next;

for(p = hand; p<hand+5; p++) {                  //loop through cards in hand              
    next = p+1;                                 //set pointer to next card in hand
  if(next->pips == (p->pips+1)) {               //if pips value of next card is equal to current card pips + 1
            sequence++;                         //increment the counter for # of cards in sequence
    }
}

    if(sequence == 4) return 1;                 //if cards are in sequence 4 times, isStraight() returns true
    else return 0;

}

/*method that takes parameters # of cards to match and card value to ignore(if being called twice),
returns either the value of the matching card (if successful) or 0 */
int XofaKind(int num, int ignore) {

    card* p;
    int reference = 0;
    int numMatches = 0;


for(p = hand; p<hand+4; p++) {                          //loop through hand
    card* next = p+1;

    if(p->pips == next->pips&&p->pips!=ignore) {        //if the pips value of the current card matches the pips of the next card
        reference = p->pips;                            //and the pips isnt the value we are ignoring, set the value to match to that pips
        
        for(p = hand; p<hand+5; p++) {                  //loop through hand and count # of cards that match the reference value
            if(p->pips == reference) {
               numMatches++;
        }
    }
    }
}

if(numMatches == num) {                                 //if number of matches = desired amount, return the reference value
    ignore = reference;
 }
 else {
    ignore = 0;                                         //else return 0
 }

 return ignore;

}

/*method that checks if hand has one pair*/
int onePair() {

    if(XofaKind(2, 0)) return 1;                        //call XofaKind once checking for two of the same card
    else return 0;

}

/*method that checks if hand has three of a kind*/
int threeOfAK() {

    if(XofaKind(3, 0)) return 1;                        //call XofaKind once checking for three of the same card
    else return 0;

}

/*method that checks if hand has four of a kind*/
int fourOfAK() {

    if(XofaKind(4, 0)) return 1;                        //call XofaKind once checking for four of the same card
    else return 0;

}

/*method that checks if hand has two pair*/
int twoPair() {

    int ignore = XofaKind(2,0);                        //call XofaKind once checking for two of the same card

    if(XofaKind(2, ignore))  return 1;                 //call XofaKind again, ignoring the value of the first pair to check for second pair
    else return 0;

}

/*method that checks for a full house(three of one kind, two of another)*/
int fullHouse() {
    int fH = 0;
    int true1 = 0;
    int true2 = 0;
    int ignore =0;

    
    ignore=XofaKind(3,0);                           //call XofaKind once, check for three of a kind
    if(ignore!=0&&XofaKind(2, ignore)) true1 = 1;   //call XofaKind again, check for a pair of a different value
                                                    //true if successful 

    ignore = XofaKind(2,0);                         //call XofaKind once, check for a pair
    if(ignore!=0&&XofaKind(3, ignore)) true2 = 1;   //call XofaKind again, check for three of a kind
                                                    //true if successful

    if(true1 || true2) {                            //if either call is true, fullHouse() returns true 
        fH = 1;
    }


return fH;                                          //else return 0
    
}



/*method that calculates the odd of getting any given hand by dividing total number of hands dealt by occurrences of that hand,
and prints the output*/
void calculateOdds(int numPair, int numTwoPair, int numThree, int numFour, int numFlush, int numStraight, int numStraightFlush, int numFullHouse, int numHands) {

    int pairOdds = (int)(numHands/numPair);
    int twoPairOdds = (int)(numHands/numTwoPair);               //calculate odds and cast to int
    int threeOdds = (int)(numHands/numThree);
    int fourOdds = (int)(numHands/numFour);
    int flushOdds = (int)(numHands/numFlush);
    int straightOdds = (int)(numHands/numStraight);
    int fullHouseOdds = (int)(numHands/numFullHouse);
    int straightFlushOdds = (int)(numHands/numStraightFlush);

    //print output

    printf("1P: %d:1\t 2P: %d:1\t 3K: %d:1\t 4K: %d:1\t Full House: %d:1\t Flush: %d:1\t Straight: %d:1\t Straight Flush: %d:1\n", pairOdds, twoPairOdds, threeOdds, fourOdds, fullHouseOdds, flushOdds, straightOdds, straightFlushOdds);

}


