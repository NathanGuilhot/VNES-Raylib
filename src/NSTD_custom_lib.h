// Custom Lib
#include <stdlib.h> //Needed for : rand()
#include <string.h> //Needed for : memcpy()

int c_atoi(char *str)
{ //custom atoi function so I don't need the stdlib
  int res = 0;
  for (int i = 0; str[i] != '\0'; i++)
    res = res * 10 + str[i] - '0';
  return res;
  //Credit : https://www.geeksforgeeks.org/write-your-own-atoi/
}

int max(int pVal1, int pVal2)
{
  if (pVal1>=pVal2){return pVal1;} else {return pVal2;}
}

int min(int pVal1, int pVal2)
{
  if (pVal1<=pVal2){return pVal1;} else {return pVal2;}
}

int clamp(int pVal, int pMarge1, int pMarge2){ return max(pMarge1, min(pVal,pMarge2)); }

int random(int low, int high) {
    int diff, val;
 
    diff = high - low;
    if (diff == 0) {
        return low;
    }
 
    val = rand() % diff;
    return val + low;
}

//Shuffle a array (deck, size, number of passes (increase randomness and process time))
//Source : http://www.rosettacode.org/wiki/Card_shuffles#C
void ShuffleOverhand(int * const mainHand, const int size, int passes) { //Maybe change array type later ?
    int n, cutSize, mp, op, tp, i;
    int *otherHand, *temp;
 
    otherHand = (int *)malloc(size * sizeof(int));
    temp = (int *)malloc(size * sizeof(int));
 
    for (n = 0; n < passes; ++n) {
        mp = 0;
        op = 0;
        tp = 0;
 
        while (mp < size) {
            cutSize = random(0, size / 5) + 1;
 
            /* grab the next cut up to the end of the cards left in the main hand */
            for (i = 0; i < cutSize && mp < size; ++i) {
                temp[tp++] = mainHand[mp++];
            }
 
            /* add them to the cards in the other hand, sometimes to the fron sometimes to the back */
            if (random(0, 10) >= 1) {
                /* front most of the time */
 
                /* move the elements of other hand forward to make room for temp */
                for (i = op - 1; i >= 0; --i) {
                    otherHand[i + tp] = otherHand[i];
                }
 
                /* copy temp to the front of other hand */
                memcpy(otherHand, temp, tp * sizeof(int));
                op += tp;
                tp = 0;
            } else {
                /* end sometimes */
                for (i = 0; i < tp; ++i, ++op) {
                    otherHand[op] = temp[i];
                }
                tp = 0;
            }
        }
 
        /* move the cards back to the main hand */
        memcpy(mainHand, otherHand, size * sizeof(int));
    }
 
    free(otherHand);
    free(temp);
}