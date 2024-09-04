#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>  // getpid()
#define MAX_LEN 1000
// BLOSUM62 ?? ??
const int BLOSUM62[20][20] = {
    {4, -1, -2 ,-2,  0,-1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0,},
    {-1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3},
    {-2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3},
    {-2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3},
    {0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1},
    {-1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2},
    {-1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2},
    {0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3},
    {-2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3},
    {-1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3},
    {-1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1},
    {-1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2},
    {-1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1},
    {-2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1},
    {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2},
    {1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2},
    {0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0},
    {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3},
    {-2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1},
    {0, -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4}
};

const char AMINO_ACIDS[20] = {'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V'};
const int  aa_frequency[20] = {24, 24,35, 26, 8, 16,34, 44, 14, 21, 49, 25, 15, 34, 32, 43, 22, 14, 17, 40};

int getIndex(char aa) {
    for (int i = 0; i < 20; i++) {
        if (AMINO_ACIDS[i] == aa) {
            return i;
        }
    }
    return -1;
}

char mutate_aa(char aa) {
    int aa_index = getIndex(aa);
    if (aa_index == -1) {
        printf("Invalid amino acid: %c\n", aa);
        exit(EXIT_FAILURE);
    }

    double cum_prob = 0.0;
    double max_cum_prob = 0.0;
    double old_max_cum_prob = 0.0;
    int max_amino_acid = 0;
    for (int i = 0; i < 20; i++) {
        cum_prob = (double) rand() * (BLOSUM62[aa_index][i] + 5);
        if (cum_prob >= old_max_cum_prob) {
	max_amino_acid = i;
	old_max_cum_prob = cum_prob;
	}
        }
   return AMINO_ACIDS[max_amino_acid];
}

void add_aa(int index, int sl, char* f, char* s) {

    double cum_prob = 0.0;
    double max_cum_prob = 0.0;
    int max_amino_acid = 0;

    for (int i = 0; i < 20; i++) {
	srand((unsigned)time(NULL) + i + 23);
        cum_prob = (double) rand()  * (aa_frequency[i]);
        if (cum_prob >= max_cum_prob) {
        max_amino_acid = i;
	max_cum_prob = cum_prob ;
        }

        }

    f [sl] = '\0';
     for (int ii = sl; ii >= index ; ii--)
    {
       f [ii + 1] = f [ii];
       s [ii + 1] = s [ii];
    }
    f [index] = AMINO_ACIDS[max_amino_acid];

}



int main() {
    char fasta[MAX_LEN];
    char ss[MAX_LEN]; 
    int leng_aa;
    int j;
    double rand_prob, rand_prob1, rand_prob2, rand_prob3, rand_prob4;
      srand( (unsigned)time(NULL)+(unsigned)getpid());
    //srand(time(NULL));
        // Read the string from file A.txt
    FILE *file_A = fopen("fasta", "r");
    fscanf(file_A, "%s", fasta);
    fclose(file_A);
    leng_aa = strlen(fasta);

    // Read the string from file B.txt
    FILE *file_B = fopen("ss", "r");
    fscanf(file_B, "%s", ss);
    fclose(file_B);

    double ss_mutation_probability = 0.05;
    double loop_mutation_probability = 0.1;
    double add_probability = 0.08;
    double Nadd_probability = 0.2;
    double Cadd_probability = 0.2;

   // printf("Original sequence: %s\n", fasta);
   rand_prob3 = (double)(rand() % (95 )) / (95 ) ;
   rand_prob4 = (double)(rand() % (11 )) / (11 ) ;
           if (rand_prob3 < Nadd_probability)
		      {   leng_aa++;
		         add_aa (0, leng_aa, fasta, ss); }

	  if (rand_prob4 < Cadd_probability)
	              {   leng_aa++;
	                  add_aa (leng_aa - 1, leng_aa, fasta, ss); }




    for (int i = 0;  i < leng_aa ; i++) {
	srand( (unsigned)time(NULL)+(unsigned)getpid() + i);
        rand_prob = (double)(rand() % (100 + i)) / (100 + i) ;
        rand_prob1 = (double)(rand() % (99 + i)) / (99 + i) ;
        rand_prob2 = (double)(rand() % (97 + i)) / (97 + i) ;
        //printf("rand_prob:  %f, %f, %f\n", rand_prob, rand_prob1, rand_prob2 );

	    

	    if (ss[i] == '-') {

        	if (rand_prob < loop_mutation_probability) {
            	fasta[i] = mutate_aa(fasta[i]); }
		if (rand_prob1 < add_probability) {
	        leng_aa++;
		add_aa (i, leng_aa, fasta, ss); }
	     }
	    else {
                if (rand_prob2 < ss_mutation_probability) {
                fasta[i] = mutate_aa(fasta[i]);
                   }
             }
     }           

   // printf("Mutated sequence:  %s\n", fasta);
   printf("%s\n", fasta);

    return 0;
}

